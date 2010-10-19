#ifndef __itkBinaryCloseParabolicImageFilter_txx
#define __itkBinaryCloseParabolicImageFilter_txx

#include "itkProgressAccumulator.h"
#include "itkBinaryCloseParaImageFilter.h"
#include "itkParabolicErodeImageFilter.h"
#include "itkProgressAccumulator.h"
#include "itkCropImageFilter.h"
#include "itkConstantPadImageFilter.h"

namespace itk
{

template <typename TInputImage, typename TOutputImage>
BinaryCloseParaImageFilter<TInputImage, TOutputImage>
::BinaryCloseParaImageFilter()
{
  this->SetNumberOfRequiredOutputs( 1 );
  this->SetNumberOfRequiredInputs( 1 );
  this->m_CircErode = CircErodeType::New();
  this->m_CircDilate = CircDilateType::New();
  this->m_CircCastA = CCastTypeA::New();
  this->m_CircCastB = CCastTypeB::New();

  this->m_RectErode = RectErodeType::New();
  this->m_RectDilate = RectDilateType::New();
  this->m_RectCastA = RCastTypeA::New();
  this->m_RectCastB = RCastTypeB::New();
  this->m_Circular = true;
    // Need to call this after filters are created
  this->SetUseImageSpacing(false);
  this->SetSafeBorder(true);
}


template <typename TInputImage, typename TOutputImage>
void
BinaryCloseParaImageFilter<TInputImage, TOutputImage>
::SetRadius( ScalarRealType radius )
{
  RadiusType s;
  s.Fill(radius);
  this->SetRadius( s );
}

template <typename TInputImage, typename TOutputImage >
void
BinaryCloseParaImageFilter<TInputImage, TOutputImage >
::GenerateData(void)
{
  // Allocate the output
  this->AllocateOutputs();
  // set up the scaling before we pass control over to superclass
  typename TInputImage::SizeType Pad;

  if (this->m_RectErode->GetUseImageSpacing())
    {
    // radius is in mm
    RadiusType R;
    for (unsigned P=0;P<InputImageType::ImageDimension;P++)
      {
      R[P] = 0.5*m_Radius[P] * m_Radius[P];
      Pad[P] = (typename TInputImage::SizeType::SizeValueType) round(m_Radius[P]/this->GetInput()->GetSpacing()[P] + 1);
      //this->SetScale(0.5*m_Radius[P] * m_Radius[P]);
      }
    m_RectErode->SetScale(R);
    m_CircErode->SetScale(R);
    m_RectDilate->SetScale(R);
    m_CircDilate->SetScale(R);

    }
  else
    {
    // radius is in pixels
    RadiusType R;
    // this gives us a little bit of a margin 
    for (unsigned P=0;P<InputImageType::ImageDimension;P++)
      {
      R[P] = (0.5*m_Radius[P] * m_Radius[P]+1);
      Pad[P] = (typename TInputImage::SizeType::SizeValueType)(m_Radius[P]+1);
      }
    //std::cout << "no image spacing " << m_Radius << R << std::endl;
    m_RectErode->SetScale(R);
    m_CircErode->SetScale(R);
    m_RectDilate->SetScale(R);
    m_CircDilate->SetScale(R);

    }

  

  if (m_Circular)
    {
    ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
    progress->SetMiniPipelineFilter(this);
    InputImageConstPointer inputImage;
    inputImage = this->GetInput();

    progress->RegisterInternalFilter(m_CircErode, 0.4f);
    progress->RegisterInternalFilter(m_CircCastA, 0.1f);
    progress->RegisterInternalFilter(m_CircDilate, 0.4f);
    progress->RegisterInternalFilter(m_CircCastB, 0.1f);

    m_CircCastB->SetInput(m_CircDilate->GetOutput());
    m_CircCastB->SetUpperThreshold(0);
    m_CircCastB->SetOutsideValue(1);
    m_CircCastB->SetInsideValue(0);
      

    m_CircErode->SetInput(m_CircCastB->GetOutput());
    m_CircCastA->SetInput(m_CircErode->GetOutput());
    m_CircCastA->SetVal(1.0);

    if (m_SafeBorder) 
      {
      typedef typename itk::ConstantPadImageFilter<InputImageType, InputImageType> PadType;
      typename PadType::Pointer pad = PadType::New();
      pad->SetPadLowerBound( Pad);
      pad->SetPadUpperBound( Pad );
      pad->SetConstant( 0 );
      pad->SetInput( inputImage );

      m_CircDilate->SetInput(pad->GetOutput());


      typedef typename itk::CropImageFilter<TOutputImage, TOutputImage> CropType;
      typename CropType::Pointer crop = CropType::New();
      crop->SetInput( m_CircCastA->GetOutput() );
      crop->SetUpperBoundaryCropSize( Pad );
      crop->SetLowerBoundaryCropSize( Pad );

      crop->GraftOutput( this->GetOutput() );
      crop->Update();
      this->GraftOutput( crop->GetOutput() );
      }
    else
      {
 
      
      m_CircDilate->SetInput(inputImage);
      m_CircCastA->GraftOutput(this->GetOutput());
      m_CircCastA->Update();

      this->GraftOutput(m_CircCastA->GetOutput());
      }
    }
  else
    {
    ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
    progress->SetMiniPipelineFilter(this);
    InputImageConstPointer inputImage;
    inputImage = this->GetInput();

    progress->RegisterInternalFilter(m_RectErode, 0.4f);
    progress->RegisterInternalFilter(m_RectCastA, 0.1f);
    progress->RegisterInternalFilter(m_RectDilate, 0.4f);
    progress->RegisterInternalFilter(m_RectCastB, 0.1f);

    m_RectCastB->SetInput(m_RectDilate->GetOutput());
    m_RectCastB->SetUpperThreshold(0);
    m_RectCastB->SetOutsideValue(1);
    m_RectCastB->SetInsideValue(0);
    
    m_RectErode->SetInput(m_RectCastB->GetOutput());
    m_RectCastA->SetInput(m_RectErode->GetOutput());
    m_RectCastA->SetVal(1);

    if (m_SafeBorder)
      {
      typedef typename itk::ConstantPadImageFilter<InputImageType, InputImageType> PadType;
      typename PadType::Pointer pad = PadType::New();
      pad->SetPadLowerBound( Pad);
      pad->SetPadUpperBound( Pad );
      pad->SetConstant( 0 );
      pad->SetInput( inputImage );

      m_RectDilate->SetInput(pad->GetOutput());

      typedef typename itk::CropImageFilter<TOutputImage, TOutputImage> CropType;
      typename CropType::Pointer crop = CropType::New();
      crop->SetInput( m_RectCastA->GetOutput() );
      crop->SetUpperBoundaryCropSize( Pad );
      crop->SetLowerBoundaryCropSize( Pad );

      crop->GraftOutput( this->GetOutput() );
      crop->Update();
      this->GraftOutput( crop->GetOutput() );

      }
    else
      {
      m_RectDilate->SetInput(inputImage);
      m_RectCastA->GraftOutput(this->GetOutput());
      m_RectCastA->Update();
      this->GraftOutput(m_RectCastA->GetOutput());
      }
    
    }
}


template <typename TInputImage, typename TOutputImage>
void
BinaryCloseParaImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  if (this->m_CircErode->GetUseImageSpacing())
    {
    os << "Radius in world units: " << this->GetRadius() << std::endl;
    }
  else
    {
    os << "Radius in voxels: " << this->GetRadius() << std::endl;
    }
}


} // namespace itk
#endif
