#ifndef __itkParabolicErodeDilateImageFilter_txx
#define __itkParabolicErodeDilateImageFilter_txx

#include "itkParabolicErodeDilateImageFilter.h"
#include "itkImageLinearIteratorWithIndex.h"
#include "itkImageLinearConstIteratorWithIndex.h"
#include "itkParabolicMorphUtils.h"

namespace itk
{

template <typename TInputImage, bool doDilate, typename TOutputImage>
ParabolicErodeDilateImageFilter<TInputImage, doDilate, TOutputImage>
::ParabolicErodeDilateImageFilter()
{
  this->SetNumberOfRequiredOutputs( 1 );
  this->SetNumberOfRequiredInputs( 1 );
// needs to be selected according to erosion/dilation

  if (doDilate)
    {
    m_Extreme = NumericTraits<PixelType>::min();
    m_MagnitudeSign = 1;
    }
  else
    {
    m_Extreme = NumericTraits<PixelType>::max();
    m_MagnitudeSign = -1;
    }
  m_UseImageSpacing = false;
}

template <typename TInputImage, bool doDilate, typename TOutputImage>
void
ParabolicErodeDilateImageFilter<TInputImage, doDilate, TOutputImage>
::SetScale( ScalarRealType scale )
{

  this->m_Sigma.Fill(scale);
  this->Modified();
}

template <typename TInputImage, bool doDilate, typename TOutputImage>
void
ParabolicErodeDilateImageFilter<TInputImage, doDilate, TOutputImage>
::SetScale( RadiusType scale)
{

  for (unsigned i = 0; i < TInputImage::ImageDimension; i++)
    {
    this->m_Sigma[i] = scale[i];
    }
  this->Modified();
}

template <typename TInputImage, bool doDilate, typename TOutputImage>
void
ParabolicErodeDilateImageFilter<TInputImage, doDilate, TOutputImage>
::GenerateInputRequestedRegion() throw(InvalidRequestedRegionError)
{
  // call the superclass' implementation of this method. this should
  // copy the output requested region to the input requested region
  Superclass::GenerateInputRequestedRegion();

  // This filter needs all of the input
  InputImagePointer image = const_cast<InputImageType *>( this->GetInput() );
  if( image )
    {
    image->SetRequestedRegion( this->GetInput()->GetLargestPossibleRegion() );
    }
}

template <typename TInputImage, bool doDilate, typename TOutputImage>
void
ParabolicErodeDilateImageFilter<TInputImage,doDilate, TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *output)
{
  TOutputImage *out = dynamic_cast<TOutputImage*>(output);

  if (out)
    {
    out->SetRequestedRegion( out->GetLargestPossibleRegion() );
    }
}


template <typename TInputImage, bool doDilate, typename TOutputImage >
void
ParabolicErodeDilateImageFilter<TInputImage, doDilate, TOutputImage >
::GenerateData(void)
{

  typedef ImageLinearConstIteratorWithIndex< TInputImage  >  InputConstIteratorType;
  typedef ImageLinearIteratorWithIndex< TOutputImage >  OutputIteratorType;

  // for stages after the first
  typedef ImageLinearConstIteratorWithIndex< TOutputImage  >  OutputConstIteratorType;

  typedef ImageRegion< TInputImage::ImageDimension > RegionType;

  typename TInputImage::ConstPointer   inputImage(    this->GetInput ()   );
  typename TOutputImage::Pointer       outputImage(   this->GetOutput()        );

  const unsigned int imageDimension = inputImage->GetImageDimension();

  outputImage->SetBufferedRegion( outputImage->GetRequestedRegion() );
  outputImage->Allocate();
  RegionType region = inputImage->GetRequestedRegion();

  InputConstIteratorType  inputIterator(  inputImage,  region );
  OutputIteratorType      outputIterator( outputImage, region );
  OutputConstIteratorType inputIteratorStage2( outputImage, region );

  // setup the progress reporting 
  unsigned int numberOfLinesToProcess = 0;
  for (unsigned  dd = 0; dd < imageDimension; dd++)
    {
    numberOfLinesToProcess += region.GetSize()[dd];
    }

  ProgressReporter progress(this,0, numberOfLinesToProcess);

  // deal with the first dimension - this should be copied to the
  // output if the scale is 0
  if (m_Sigma[0] > 0)
    {
    // Perform as normal
    RealType magnitude = 1.0/(2.0 * m_Sigma[0]);
    unsigned long LineLength = region.GetSize()[0];
    RealType image_scale = this->GetInput()->GetSpacing()[0];

//     doOneDimension<InputConstIteratorType,OutputIteratorType>(inputIterator, outputIterator, 
// 							      progress, LineLength, 0);
    doOneDimension<InputConstIteratorType,OutputIteratorType, 
      RealType, OutputPixelType, doDilate>(inputIterator, outputIterator, 
					   progress, LineLength, 0, 
					   this->m_MagnitudeSign, 
					   this->m_UseImageSpacing,
					   this->m_Extreme,
					   image_scale, 
					   this->m_Sigma[0]);
    }
  else 
    {
    // copy to output
    }
  // now deal with the other dimensions
  for (unsigned dd = 1; dd < imageDimension; dd++)
    {
    if (m_Sigma[dd] > 0)
      {
      // create a vector to buffer lines
      unsigned long LineLength = region.GetSize()[dd];
      RealType magnitude = 1.0/(2.0 * m_Sigma[dd]);
      RealType image_scale = this->GetInput()->GetSpacing()[0];

//       doOneDimension<InputConstIteratorType,OutputIteratorType>(inputIteratorStage2, outputIterator, 
// 								progress, LineLength, dd);
      doOneDimension<InputConstIteratorType,OutputIteratorType,
	RealType, OutputPixelType, doDilate>(inputIteratorStage2, outputIterator, 
					     progress, LineLength, dd,
					     this->m_MagnitudeSign, 
					     this->m_UseImageSpacing,
					     this->m_Extreme,
					     image_scale,
					     this->m_Sigma[dd]);
      }
    }
}

#if 0
template <typename TInputImage, bool doDilate, typename TOutputImage>
void
ParabolicErodeDilateImageFilter<TInputImage, doDilate, TOutputImage>
::DoLine(LineBufferType &LineBuf, LineBufferType &tmpLineBuf, 
	 const RealType magnitude)
{
  // contact point algorithm
  long koffset = 0, newcontact=0;  // how far away the search starts.

  const long LineLength = LineBuf.size();
  // negative half of the parabola
  for (long pos = 0; pos < LineLength; pos++)
    {
    RealType BaseVal = (RealType)m_Extreme; // the base value for comparison
    for (long krange = koffset; krange <= 0; krange++)
      {
      // difference needs to be paramaterised
      RealType T = LineBuf[pos + krange] - magnitude * krange * krange;
      // switch on template parameter - hopefully gets optimized away.
      if (doDilate ? (T >= BaseVal) : (T <= BaseVal) )
	{
	BaseVal = T;
	newcontact = krange;
	}
      }
    tmpLineBuf[pos] = BaseVal;
    koffset = newcontact - 1;
    }
  // positive half of parabola
  koffset = newcontact = 0;
  for (long pos = LineLength - 1; pos >= 0; pos--)
    {
    RealType BaseVal = (RealType)m_Extreme; // the base value for comparison
    for (long krange = koffset; krange >= 0; krange--)
      {
      RealType T = tmpLineBuf[pos + krange] - magnitude * krange * krange;
      if (doDilate ? (T >= BaseVal) : (T <= BaseVal))
	{
	BaseVal = T;
	newcontact = krange;
	}
      }
    LineBuf[pos] = BaseVal;
    koffset = newcontact + 1;
    }
}
#endif

template <typename TInputImage, bool doDilate, typename TOutputImage>
void
ParabolicErodeDilateImageFilter<TInputImage, doDilate, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  if (m_UseImageSpacing)
    {
    os << "Scale in world units: " << m_Sigma << std::endl;
    }
  else
    {
    os << "Scale in voxels: " << m_Sigma << std::endl;
    }
}


} // namespace itk
#endif
