#include <iomanip>
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"
#include "itkChangeInformationImageFilter.h"
#include "ioutils.h"

#include "itkTimeProbe.h"
#include "itkMultiThreader.h"

#include <itkImageRegionConstIterator.h>
#include <itkConstShapedNeighborhoodIterator.h>
#include "itkConstantBoundaryCondition.h"
#include "itkConnectedComponentAlgorithm.h"

#include <set>
#include <vector>

// testing ideas for rotation of rle direction


template <class IndexType>
void
genRLE(std::vector<IndexType> fgSet, std::vector<IndexType> bgSet, int axis)
{
  // fgSet must be sorted correctly

  using IndVecType = typename std::vector<IndexType>;
  using ItType = typename IndVecType::const_iterator;

  IndVecType startpoints, endpoints;

  // fgSet contains surface points. We therefore may have either a
  // series of consecutive points, or two some distance apart. The
  // first situation occurs at a straight edge parallel to image
  // axes. The second occurs across the inside of an object.
  // Actually we can have both, because a run may start as an edge and
  // then go inside the object.

  for (ItType it = fgSet.begin(); it != fgSet.end(); it++)
  {
  }
}

template <class PixType, int dim>
void
doAllRLE(std::string input, std::string output)
{
  using ImType = typename itk::Image<PixType, dim>;
  using PImType = typename ImType::Pointer;
  using IndexType = typename ImType::IndexType;

  // probably a long int
  using SingleIndexType = typename ImType::OffsetValueType;

  PImType inIm = readIm<ImType>(input);

  using IndexSetType = typename std::vector<IndexType>;

  using SISetType = typename std::set<SingleIndexType>;

  // fgSet could be a vector because we definitely only insert once
  // bgSet needs to be a set
  IndexSetType fgSet;
  IndexSetType bgSet;

  SISetType bgSISet;

  using itRegType = typename itk::ImageRegionConstIterator<ImType>;
  using itShapedType = typename itk::ConstShapedNeighborhoodIterator<ImType>;

  itk::Size<dim> radius;
  radius.Fill(1);

  itRegType    itReg(inIm, inIm->GetLargestPossibleRegion());
  itShapedType itShaped(radius, inIm, inIm->GetLargestPossibleRegion());

  // set up the neighborhood
  setConnectivity(&itShaped, true);
  // set up boundary condition
  itk::ConstantBoundaryCondition<ImType> lcbc;
  lcbc.SetConstant(0);
  itShaped.OverrideBoundaryCondition(&lcbc);

  for (itReg.GoToBegin(); !itReg.IsAtEnd(); ++itReg)
  {
    if (itReg.Get())
    {
      // found an on pixel, so check the neighborhood.
      IndexType here = itReg.GetIndex();
      itShaped += here - itShaped.GetIndex();
      // iterate over the neighborhood
      bool surface = false;
      for (typename itShapedType::ConstIterator nIt = itShaped.Begin(); nIt != itShaped.End(); nIt++)
      {
        if (!nIt.Get())
        {
          // this is a surface pixel
          surface = true;
          IndexType                          N = here + nIt.GetNeighborhoodOffset();
          SingleIndexType                    I = inIm->ComputeOffset(N);
          typename SISetType::const_iterator loc = bgSISet.find(I);
          if (loc != bgSISet.end())
          {
            bgSISet.insert(I);
            bgSet.push_back(N);
          }
        }
      }
      if (surface)
      {
        fgSet.push_back(here);
      }
    }
  }

  genRLE<IndexType>(fgSet, bgSet, 0);
}

int
main(int argc, char * argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " inputim outputim " << std::endl;
    return (EXIT_FAILURE);
  }

  doAllRLE<unsigned char, 2>(std::string(argv[1]), std::string(argv[2]));

  return EXIT_SUCCESS;
}
