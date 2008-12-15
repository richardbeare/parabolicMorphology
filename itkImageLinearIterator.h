
#ifndef __itkImageLinearIterator_h
#define __itkImageLinearIterator_h

#include "itkImageLinearConstIterator.h"
#include "itkImageIterator.h"

namespace itk
{

/** \class ImageLinearIterator
 * \brief A multi-dimensional image iterator that visits image pixels within a
 * region in a "scan-line" order.
 *
 *  This iterator is a subclass of itk::ImageLinearConstIterator that
 *  adds write-access functionality.  Please see
 *  itk::ImageLinearConstIterator for more information.
 *
 * \par MORE INFORMATION
 * For a complete description of the ITK Image Iterators and their API, please
 * see the Iterators chapter in the ITK Software Guide.  The ITK Software Guide
 * is available in print and as a free .pdf download from http://www.itk.org.
 *
 * \ingroup ImageIterators
 *
 * \sa ImageConstIterator \sa ConditionalConstIterator
 * \sa ConstNeighborhoodIterator \sa ConstShapedNeighborhoodIterator
 * \sa ConstSliceIterator  \sa CorrespondenceDataStructureIterator 
 * \sa FloodFilledFunctionConditionalConstIterator 
 * \sa FloodFilledImageFunctionConditionalConstIterator 
 * \sa FloodFilledImageFunctionConditionalIterator 
 * \sa FloodFilledSpatialFunctionConditionalConstIterator 
 * \sa FloodFilledSpatialFunctionConditionalIterator 
 * \sa ImageConstIterator \sa ImageConstIterator 
 * \sa ImageIterator \sa ImageIterator
 * \sa ImageLinearConstIterator  \sa ImageLinearIterator 
 * \sa ImageRandomConstIterator  \sa ImageRandomIterator 
 * \sa ImageRegionConstIterator \sa ImageRegionConstIterator 
 * \sa ImageRegionExclusionConstIterator 
 * \sa ImageRegionExclusionIterator 
 * \sa ImageRegionIterator  \sa ImageRegionIterator 
 * \sa ImageRegionReverseConstIterator  \sa ImageRegionReverseIterator 
 * \sa ImageReverseConstIterator  \sa ImageReverseIterator 
 * \sa ImageSliceConstIterator  \sa ImageSliceIterator 
 * \sa NeighborhoodIterator \sa PathConstIterator  \sa PathIterator 
 * \sa ShapedNeighborhoodIterator  \sa SliceIterator 
 * \sa ImageConstIterator
 *
 */
template<typename TImage>
class ITK_EXPORT ImageLinearIterator : public ImageLinearConstIterator<TImage>
{
public:
  /** Standard class typedefs. */
  typedef ImageLinearIterator Self;
  typedef ImageLinearConstIterator<TImage>  Superclass;
  
   /** Types inherited from the Superclass */
  typedef typename Superclass::IndexType              IndexType;
  typedef typename Superclass::IndexValueType         IndexValueType;
  typedef typename Superclass::SizeType               SizeType;
  typedef typename Superclass::SizeValueType          SizeValueType;
  typedef typename Superclass::OffsetType             OffsetType;
  typedef typename Superclass::OffsetValueType        OffsetValueType;
  typedef typename Superclass::RegionType             RegionType;
  typedef typename Superclass::ImageType              ImageType;
  typedef typename Superclass::PixelContainer         PixelContainer;
  typedef typename Superclass::PixelContainerPointer  PixelContainerPointer;
  typedef typename Superclass::InternalPixelType      InternalPixelType;
  typedef typename Superclass::PixelType              PixelType;
  typedef typename Superclass::AccessorType           AccessorType;


  /** Default constructor. Needed since we provide a cast constructor. */
  ImageLinearIterator();
  
  /** Constructor establishes an iterator to walk a particular image and a
   * particular region of that image. */
  ImageLinearIterator(ImageType *ptr, const RegionType& region);

  /** Constructor that can be used to cast from an ImageIterator to an
   * ImageLinearIterator. Many routines return an ImageIterator but for a
   * particular task, you may want an ImageLinearIterator.  Rather than
   * provide overloaded APIs that return different types of Iterators, itk
   * returns ImageIterators and uses constructors to cast from an
   * ImageIterator to a ImageLinearIterator. */
  ImageLinearIterator( const ImageIterator<TImage> &it);
  
  /** Set the pixel value */
  void Set( const PixelType & value) const  
    { this->m_PixelAccessorFunctor.Set(*(const_cast<InternalPixelType *>(this->m_Buffer + this->m_Offset)),value); }

  /** Return a reference to the pixel 
   * This method will provide the fastest access to pixel
   * data, but it will NOT support ImageAdaptors. */
  PixelType & Value(void) 
    { return *(const_cast<InternalPixelType *>(this->m_Position)); }
 
protected:
  /** the construction from a const iterator is declared protected
      in order to enforce const correctness. */
  ImageLinearIterator( const ImageLinearConstIterator<TImage> &it);
  Self & operator=(const ImageLinearConstIterator<TImage> & it);
 

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageLinearIterator.txx"
#endif

#endif 



