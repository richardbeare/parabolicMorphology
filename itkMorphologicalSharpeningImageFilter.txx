#ifndef __itkMorphologicalSharpeningImageFilter_txx
#define __itkMorphologicalSharpeningImageFilter_txx

#include "itkMorphologicalSharpeningImageFilter.h"
#include "itkProgressAccumulator.h"

namespace itk 

{
template <typename TInputImage, typename TOutputImage> 
MorphologicalSharpeningImageFilter<TInputImage, TOutputImage>
::MorphologicalSharpeningImageFilter()
{
  this->SetNumberOfRequiredOutputs( 1 );
  this->SetNumberOfRequiredInputs( 1 );

  m_Erode = ErodeType::New();
  m_Dilate = DilateType::New();
  m_Cast = CastType::New();
  m_Iterations = 1;
  this->SetScale(1);
  this->SetUseImageSpacing(false)
}


template <typename TInputImage, typename TOutputImage> 
MorphologicalSharpeningImageFilter<TInputImage, TOutputImage>
::GenerateData(void)
{

  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  // Allocate the output
  this->AllocateOutputs();

  InputImageConstPointer inputImage = this->GetInput();
  m_Cast->SetInput(inputImage);

  m_Erode->SetInput(m_Cast->GetOutput());
  m_Dilate->SetInput(m_Cast->GetOutput()));



  
}


} // end namespace itk



#endif
