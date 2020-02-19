#include <iomanip>
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"
#include "ioutils.h"

#include <itkBinaryThresholdImageFilter.h>

#include "itkBinaryOpenParaImageFilter.h"

#include "itkTimeProbe.h"
#include "itkMultiThreader.h"

int
main(int argc, char * argv[])
{
  if (argc != 5)
  {
    std::cerr << "Usage: " << argv[0] << " inputim radius outimpref thresh" << std::endl;
    return (EXIT_FAILURE);
  }

  itk::MultiThreader::SetGlobalMaximumNumberOfThreads(1);
  constexpr int dim = 2;

  using PType = unsigned char;
  using IType = itk::Image<PType, dim>;
  using FType = itk::Image<float, dim>;

  IType::Pointer input = readIm<IType>(argv[1]);

  using ThreshType = itk::BinaryThresholdImageFilter<IType, IType>;
  ThreshType::Pointer thresh = ThreshType::New();
  thresh->SetInput(input);
  thresh->SetInsideValue(0);
  thresh->SetOutsideValue(1);
  thresh->SetUpperThreshold(std::stoi(argv[4]));

  using FilterType = itk::BinaryOpenParaImageFilter<IType, IType>;

  FilterType::Pointer filter = FilterType::New();
  int                 testrad = std::stoi(argv[2]);
  filter->SetInput(thresh->GetOutput());
  filter->SetUseImageSpacing(true);
  filter->SetCircular(true);
  filter->SetRadius(testrad);

  writeIm<IType>(filter->GetOutput(), std::string(argv[3]) + "_" + argv[2] + ".png");
  writeIm<IType>(thresh->GetOutput(), std::string(argv[3]) + "_thresh.png");

  filter->Print(std::cout, itk::Indent(0));

  return EXIT_SUCCESS;
}
