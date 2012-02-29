#include <iomanip>
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkChangeInformationImageFilter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"

#include "itkParabolicErodeImageFilter.h"
#include "itkTimeProbe.h"
#include "itkMultiThreader.h"

// sanity check of the image spacing option 

int main(int, char * argv[])
{
  //itk::MultiThreader::SetGlobalMaximumNumberOfThreads(1);
  const int dim = 2;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;


  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();
  std::cout << reader->GetOutput()->GetSpacing() << std::endl;

  typedef itk::ParabolicErodeImageFilter< IType,IType > FilterType;

  FilterType::Pointer filter = FilterType::New();
  
  filter->SetInput( reader->GetOutput() );

  filter->SetScale(1.0);
  filter->SetUseImageSpacing(true);
  filter->Update();

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();
  
  return EXIT_SUCCESS;
}

