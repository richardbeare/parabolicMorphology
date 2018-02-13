#include <iomanip>
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"

#include "itkParabolicDilateImageFilter.h"
#include "itkParabolicErodeImageFilter.h"
#include "itkTimeProbe.h"
#include "itkMultiThreader.h"

int main(int, char *argv[])
{
  //itk::MultiThreader::SetGlobalMaximumNumberOfThreads(1);
  const int dim = 2;

  using PType = unsigned char;
  using IType = itk::Image< PType, dim >;

  using ReaderType = itk::ImageFileReader< IType >;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  using FilterType = itk::ParabolicErodeImageFilter< IType, IType >;

  FilterType::Pointer filter = FilterType::New();

  filter->SetInput( reader->GetOutput() );

  FilterType::RadiusType scale;
  scale[0] = 1;
  scale[1] = 0.5;

//  filter->SetParabolicAlgorithm(FilterType::CONTACTPOINT);
  filter->SetScale(scale);
  itk::TimeProbe NewTime;
  filter->SetUseImageSpacing(true);
  for ( unsigned i = 0; i < 100; i++ )
    {
    filter->Modified();
    NewTime.Start();
    filter->Update();
    NewTime.Stop();
    }

  using WriterType = itk::ImageFileWriter< IType >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName(argv[2]);
  writer->Update();
  std::cout << std::setprecision(3)
            << NewTime.GetMean() << std::endl;

  return 0;
}
