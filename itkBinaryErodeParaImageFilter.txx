#ifndef __itkBinaryErodeParabolicImageFilter_txx
#define __itkBinaryErodeParabolicImageFilter_txx

#include "itkProgressAccumulator.h"
#include "itkBinaryErodeParaImageFilter.h"
#include "itkParabolicErodeImageFilter.h"
#include "itkProgressAccumulator.h"

namespace itk
{

template <typename TInputImage, typename TOutputImage>
BinaryErodeParaImageFilter<TInputImage, TOutputImage>
::BinaryErodeParaImageFilter()
{
  this->SetNumberOfRequiredOutputs( 1 );
  this->SetNumberOfRequiredInputs( 1 );
  this->m_Para = ParabolicType::New();
  this->m_Cast = CastType::New();
  // Need to call this after filters are created
  this->SetUseImageSpacing(false);
}


template <typename TInputImage, typename TOutputImage>
void
BinaryErodeParaImageFilter<TInputImage, TOutputImage>
::SetRadius( ScalarRealType radius )
{
  RadiusType s;
  s.Fill(radius);
  this->SetRadius( s );
}

template <typename TInputImage, typename TOutputImage >
void
BinaryErodeParaImageFilter<TInputImage, TOutputImage >
::GenerateData(void)
{
  // Allocate the output
  this->AllocateOutputs();
  // set up the scaling before we pass control over to superclass
  if (this->m_Para->GetUseImageSpacing())
    {
    // radius is in mm
    RadiusType R;
    for (unsigned P=0;P<InputImageType::ImageDimension;P++)
      {
      R[P] = 0.5*m_Radius[P] * m_Radius[P];
      //this->SetScale(0.5*m_Radius[P] * m_Radius[P]);
      }
    m_Para->SetScale(R);

    }
  else
    {
    std::cout << "no image spacing " << m_Radius << std::endl;
    // radius is in pixels
    RadiusType R;
    for (unsigned P=0;P<InputImageType::ImageDimension;P++)
      {
      R[P] = 0.5*m_Radius[P] * m_Radius[P] + 1;
      }
    m_Para->SetScale(R);
    }

  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);
  InputImageConstPointer inputImage;
  inputImage = this->GetInput();

  progress->RegisterInternalFilter(m_Para, 0.8f);
  progress->RegisterInternalFilter(m_Cast, 0.2f);

  m_Para->SetInput(inputImage);
  m_Cast->SetInput(m_Para->GetOutput());
  m_Cast->GraftOutput(this->GetOutput());
  m_Cast->Update();
  this->GraftOutput(m_Cast->GetOutput());
  
}


template <typename TInputImage, typename TOutputImage>
void
BinaryErodeParaImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  if (this->m_Para->GetUseImageSpacing())
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
