#include <iomanip>
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"
#include "itkChangeInformationImageFilter.h"
#include "ioutils.h"
#include <itkBinaryThresholdImageFilter.h>
#include "itkBinaryDilateParaImageFilter.h"

#include "itkTimeProbe.h"
#include "itkMultiThreader.h"

int
main(int argc, char * argv[])
{
  if (argc != 6)
  {
    std::cerr << "Usage: " << argv[0] << " inputimage threshold size outim outim2" << std::endl;
    return (EXIT_FAILURE);
  }

  itk::MultiThreader::SetGlobalMaximumNumberOfThreads(1);
  constexpr int dim = 3;

  using PType = unsigned char;
  using IType = itk::Image<PType, dim>;
  using FType = itk::Image<float, dim>;

  IType::Pointer inputOrig = readIm<IType>(argv[1]);

  // threshold the input to create a mask
  using ThreshType = itk::BinaryThresholdImageFilter<IType, IType>;
  ThreshType::Pointer thresh = ThreshType::New();
  thresh->SetInput(inputOrig);

  thresh->SetUpperThreshold(std::stoi(argv[2]));
  thresh->SetInsideValue(0);
  thresh->SetOutsideValue(1);
  writeIm<IType>(thresh->GetOutput(), argv[5]);
  // now to apply the erosion
  using FilterType = itk::BinaryDilateParaImageFilter<IType, IType>;

  FilterType::Pointer filter = FilterType::New();

  filter->SetInput(thresh->GetOutput());
  filter->SetUseImageSpacing(true);
  filter->SetRadius(std::stod(argv[3]));

  writeIm<IType>(filter->GetOutput(), argv[4]);

  return EXIT_SUCCESS;
}
