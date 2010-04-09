// testing ideas for rotation of rle direction

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


template <class PixType, int dim>
void doRLE(std::string input, std::string output)
{
    typedef typename itk::Image<PixType, dim> ImType;
    typedef typename ImType::Pointer PImType;
    typedef typename ImType::IndexType IndexType;
    
    PImType inIm = readIm<ImType>(input);
    
    typedef typename std::set<IndexType> IndexSetType;
    
    // fgSet could be a vector because we definitely only insert once
    // bgSet needs to be a set
    IndexSetType fgSet;
    IndexSetType bgSet;
    
    typedef typename itk::ImageRegionConstIterator<ImType> itRegType;
    typedef typename itk::ConstShapedNeighborhoodIterator<ImType> itShapedType;
    
    itk::Size< dim > radius;
    radius.Fill(1);

    itRegType itReg(input, input->GetLargestPossibleRegion());
    itShapedType itShaped(radius, input, input->GetLargestPossibleRegion());
    
    // set up the neighborhood
    setConnectivity(&itShaped, true);
    // set up boundary condition
    ConstantBoundaryCondition<LabelImageType> lcbc;
    lcbc.SetConstant( 0 );
    itShaped.OverrideBoundaryCondition(&lcbc)
    
    for (itReg.GoToBegin(); !itReg.IsAtEnd(); ++itReg)
    {
        if (itReg.Get())
        {
         // found an on pixel, so check the neighborhood.
        IndexType here = itReg.GetIndex();
        itShaped += here - itShaped.GetIndex();
        // iterate over the neighborhood
            bool surface = false;
        for (typename itShapedType::ConstIterator nIt = itShaped.Begin();
            nIt != itShaped.End(); nIt++)
        {
        if (!nIt.Get())
        {
            // this is a surface pixel
            bgSet.insert(here + nIt.GetNeighborhoodOffset());
            surface = true;
        }
        if (surface)
        {
        fgSet.insert(here);
        }
        }
            
            
        }
    }
    // two surface sets at this point
    // represent a rle as a pair of vectors, each containing end points
    genRLE<IndexSetType>(fgSet, bgSet
    
}

int main(int argc, char * argv[])
{


  if (argc != 3)
    {
    std::cerr << "Usage: " << argv[0] << " inputim outputim " << std::endl;
    return (EXIT_FAILURE);
    }

    
    doRLE<unsigned char, 2>(std::string(argv[1]), std::string(argv[2]));

    return EXIT_SUCCESS;
}

