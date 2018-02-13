#include <iomanip>
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkChangeInformationImageFilter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"

#include "itkMorphologicalSignedDistanceTransformImageFilter.h"
//#include "itkMorphologicalDistanceTransformImageFilter.h"
#include "itkSignedMaurerDistanceMapImageFilter.h"
#include "itkSubtractImageFilter.h"

#include "itkTimeProbe.h"
#include "itkMultiThreader.h"

// sanity check of the image spacing option

int main(int argc, char *argv[])
{
  //itk::MultiThreader::SetGlobalMaximumNumberOfThreads(1);
  const int dim = 3;

  using PType = unsigned char;
  using IType = itk::Image< PType, dim >;

  using FType = itk::Image< float, dim >;

  using ReaderType = itk::ImageFileReader< IType >;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  using FilterType = itk::MorphologicalSignedDistanceTransformImageFilter< IType, FType >;

  FilterType::Pointer filter = FilterType::New();

  filter->SetInput( reader->GetOutput() );
  filter->SetOutsideValue( atoi(argv[3]) );
  filter->SetUseImageSpacing(true);

  using WriterType = itk::ImageFileWriter< FType >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName(argv[2]);
  writer->Update();

  using MaurerType = itk::SignedMaurerDistanceMapImageFilter< IType, FType >;
  MaurerType::Pointer maurer = MaurerType::New();
  maurer->SetInput( reader->GetOutput() );
  maurer->SetUseImageSpacing(true);
  maurer->SetSquaredDistance(false);

  writer->SetInput( maurer->GetOutput() );
  writer->SetFileName(argv[3]);
  writer->Update();

  using SubType = itk::SubtractImageFilter< FType, FType, FType >;
  SubType::Pointer sub = SubType::New();
  sub->SetInput( filter->GetOutput() );
  sub->SetInput2( maurer->GetOutput() );

  writer->SetInput( sub->GetOutput() );
  writer->SetFileName(argv[4]);
  writer->Update();
  return EXIT_SUCCESS;
}
