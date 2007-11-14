#ifndef __itkParabolicErodeDilateImageFilter_txx
#define __itkParabolicErodeDilateImageFilter_txx

#include "itkParabolicErodeDilateImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
//#define NOINDEX
#ifndef NOINDEX
#include "itkImageLinearIteratorWithIndex.h"
#include "itkImageLinearConstIteratorWithIndex.h"
#else
#include "itkImageLinearIterator.h"
#include "itkImageLinearConstIterator.h"
#endif
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
  RadiusType s;
  s.Fill(scale);
  this->SetScale( s );
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
#ifndef NOINDEX
  typedef ImageLinearConstIteratorWithIndex< TInputImage  >  InputConstIteratorType;
  typedef ImageLinearIteratorWithIndex< TOutputImage >  OutputIteratorType;

  // for stages after the first
  typedef ImageLinearConstIteratorWithIndex< TOutputImage  >  OutputConstIteratorType;
#else
  typedef ImageLinearConstIterator< TInputImage  >  InputConstIteratorType;
  typedef ImageLinearIterator< TOutputImage >  OutputIteratorType;

  // for stages after the first
  typedef ImageLinearConstIterator< TOutputImage  >  OutputConstIteratorType;
#endif

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
  if (m_Scale[0] > 0)
    {
    // Perform as normal
    //RealType magnitude = 1.0/(2.0 * m_Scale[0]);
    unsigned long LineLength = region.GetSize()[0];
    RealType image_scale = this->GetInput()->GetSpacing()[0];

    doOneDimension<InputConstIteratorType,OutputIteratorType, 
      RealType, OutputPixelType, doDilate>(inputIterator, outputIterator, 
					   progress, LineLength, 0, 
					   this->m_MagnitudeSign, 
					   this->m_UseImageSpacing,
					   this->m_Extreme,
					   image_scale, 
					   this->m_Scale[0]);
    }
  else 
    {
    // copy to output
    typedef ImageRegionConstIterator<TInputImage> InItType;
    typedef ImageRegionIterator<TOutputImage> OutItType;

    InItType InIt(inputImage, region);
    OutItType OutIt(outputImage, region);
    while (!InIt.IsAtEnd())
      {
      OutIt.Set(static_cast<OutputPixelType>(InIt.Get()));
      ++InIt;
      ++OutIt;
      }
    }
  // now deal with the other dimensions
  for (unsigned dd = 1; dd < imageDimension; dd++)
    {
    if (m_Scale[dd] > 0)
      {
      // create a vector to buffer lines
      unsigned long LineLength = region.GetSize()[dd];
      //RealType magnitude = 1.0/(2.0 * m_Scale[dd]);
      RealType image_scale = this->GetInput()->GetSpacing()[dd];

      doOneDimension<OutputConstIteratorType,OutputIteratorType,
	RealType, OutputPixelType, doDilate>(inputIteratorStage2, outputIterator, 
					     progress, LineLength, dd,
					     this->m_MagnitudeSign, 
					     this->m_UseImageSpacing,
					     this->m_Extreme,
					     image_scale,
					     this->m_Scale[dd]);
      }
    }
}

template <typename TInputImage, bool doDilate, typename TOutputImage>
void
ParabolicErodeDilateImageFilter<TInputImage, doDilate, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  if (m_UseImageSpacing)
    {
    os << "Scale in world units: " << m_Scale << std::endl;
    }
  else
    {
    os << "Scale in voxels: " << m_Scale << std::endl;
    }
}


} // namespace itk
#endif
