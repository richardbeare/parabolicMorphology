#include <iomanip>
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"

#include "plotutils.h"
#include "ioutils.h"

#include "itkBinaryThresholdImageFilter.h"
#include "itkMorphologicalDistanceTransformImageFilter.h"
#include "itkTimeProbe.h"
#include "itkMultiThreader.h"

int
main(int argc, char * argv[])
{
  int iterations = 1;

  if (argc != 6)
  {
    std::cerr << "Usage: " << argv[0] << " inputimage threshold outsideval outim1 outim2" << std::endl;
    return (EXIT_FAILURE);
  }

  iterations = std::stoi(argv[1]);

  itk::MultiThreader::SetGlobalMaximumNumberOfThreads(1);
  constexpr int dim = 2;

  using PType = unsigned char;
  using IType = itk::Image<PType, dim>;
  using FType = itk::Image<float, dim>;

  IType::Pointer input = readIm<IType>(argv[1]);

  // threshold the input to create a mask
  using ThreshType = itk::BinaryThresholdImageFilter<IType, IType>;
  ThreshType::Pointer thresh = ThreshType::New();
  thresh->SetInput(input);

  thresh->SetUpperThreshold(std::stoi(argv[2]));
  thresh->SetInsideValue(0);
  thresh->SetOutsideValue(255);
  writeIm<IType>(thresh->GetOutput(), argv[4]);
  // now to apply the distance transform
  using FilterType = itk::MorphologicalDistanceTransformImageFilter<IType, FType>;

  FilterType::Pointer filter = FilterType::New();

  filter->SetInput(thresh->GetOutput());
  filter->SetOutsideValue(std::stoi(argv[3]));
  filter->Update();

  writeIm<FType>(filter->GetOutput(), argv[5]);
  return EXIT_SUCCESS;
}
