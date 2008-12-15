
#ifndef __itkImageLinearConstIterator_h
#define __itkImageLinearConstIterator_h

#include "itkImageConstIterator.h"

namespace itk
{

/** \class ImageLinearConstIterator
 * \brief A multi-dimensional image iterator that visits image pixels within a
 * region in a "scan-line" order.
 *
 * ImageLinearConstIterator is templated over image type and is
 * constrained to walk within a specified image region. It is designed for
 * line-by-line processing of images.  This iterator walks a linear path along
 * a selected image direction that is parallel to one of the coordinate axes
 * of the image.  The iterator conceptually breaks the image into a set of
 * parallel lines that span the selected image dimension.
 *
 * ImageLinearConstIterator assumes a particular layout of the image
 * data. The is arranged in a 1D array as if it were [][][][slice][row][col]
 * with Index[0] = col, Index[1] = row, Index[2] = slice, etc.
 *
 * operator++ provides a simple syntax for walking around a region of a
 * multidimensional image. operator++ iterates across a preselected direction
 * constraining the movement to within a region of image. The user can verify
 * when the iterator reaches the boundary of the region along this direction,
 * by calling the IsAtEndOfLine() method. Then it is possible to pass to the
 * next line starting at the first pixel in the row that is part of the region
 * by calling the NextLine() method.
 *
 * This is the typical use of this iterator in a loop:
 *
 * \code
 *  
 * ImageLinearConstIterator<ImageType> it( image, image->GetRequestedRegion() );
 * 
 * it.SetDirection(2);
 * it.GoToBegin();
 * while( !it.IsAtEnd() )
 * {
 *   while( !it.IsAtEndOfLine() )
 *   {
 *      value = it.Get();  // it.Set() doesn't exist in the Const Iterator
 *      ++it;
 *   }
 *   it.NextLine();
 *  } 
 *
 *  \endcode
 *
 * \example  Examples/Iterators/ImageLinearIterator.cxx
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
class ITK_EXPORT ImageLinearConstIterator : public ImageConstIterator<TImage>
{
public:
  /** Standard class typedefs. */
  typedef ImageLinearConstIterator Self;
  typedef ImageConstIterator<TImage>  Superclass;
  itkStaticConstMacro(ImageDimension, unsigned int, TImage::ImageDimension);

  /** Index typedef support. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc.
   * Note that we have to rescope Index back to itk::Index to that is it not
   * confused with ImageIterator::Index. */
  typedef typename TImage::IndexType   IndexType;
  typedef typename TImage::SizeType   SizeType;

  /** Region typedef support. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc.
   * Note that we have to rescope Region back to itk::ImageRegion so that is
   * it not confused with ImageIterator::Index. */
  typedef typename TImage::RegionType RegionType;
  
  /** Image typedef support. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc.
   * Note that we have to rescope Index back to itk::Index to that is it not
   * confused with ImageIterator::Index. */
  typedef TImage ImageType;

  /** PixelContainer typedef support. Used to refer to the container for
   * the pixel data. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc. */
  typedef typename TImage::PixelContainer PixelContainer;
  typedef typename PixelContainer::Pointer PixelContainerPointer;
  
  /** Default constructor. Needed since we provide a cast constructor. */
  ImageLinearConstIterator() : ImageConstIterator<TImage>() {}
  
  /** Constructor establishes an iterator to walk a particular image and a
   * particular region of that image. */
  ImageLinearConstIterator(const ImageType *ptr, const RegionType& region);

  /** Constructor that can be used to cast from an ImageIterator to an
   * ImageLinearConstIterator. Many routines return an ImageIterator but for a
   * particular task, you may want an ImageLinearConstIterator.  Rather than
   * provide overloaded APIs that return different types of Iterators, itk
   * returns ImageIterators and uses constructors to cast from an
   * ImageIterator to a ImageLinearConstIterator. */
  ImageLinearConstIterator( const ImageConstIterator<TImage> &it)
    { this->ImageConstIterator<TImage>::operator=(it); }

  void GoToBegin()
  {
    Superclass::GoToBegin();
    this->m_BeginLine = this->m_Offset;
    this->m_EndLine = this->m_BeginLine + this->m_Region.GetSize()[this->m_Direction] * this->m_Jump;
    
  };

  void GoToEnd()
  {
    Superclass::GoToEnd();
    this->m_EndLine = this->m_EndOffset;
    this->m_BeginLine = this->m_EndLine - this->m_Region.GetSize()[this->m_Direction] * this->m_Jump;
  }


  /** Go to the next line.
   * \sa operator++  \sa operator-- \sa IsAtEndOfLine \sa PreviousLine \sa End */
  inline void NextLine(void);

  /** Go to the previous line.
   * \sa operator++ \sa operator-- \sa IsAtEndOfLine \sa NextLine \sa End */
  inline void PreviousLine(void);

  /** Go to the beginning pixel of the current line.
   * \sa GoToReverseBeginOfLine \sa operator++ \sa operator-- \sa NextLine \sa IsAtEndOfLine */
  void GoToBeginOfLine(void);

  /** Go to the beginning pixel of the current line.
   * \sa GoToBeginOfLine \sa operator++ \sa operator-- \sa NextLine \sa IsAtEndOfLine */
  void GoToReverseBeginOfLine(void);

  /** Go to the past end pixel of the current line.
   * \sa GoToBeginOfLine \sa operator++ \sa operator-- \sa NextLine \sa IsAtEndOfLine */
  void GoToEndOfLine(void);

  /** Move an iterator to the End of the region. */
  void GoToReverseBegin(void)
  {
    this->m_Offset = this->m_EndOffset;
  }

  bool IsAtReverseEnd(void) const
  {
    return (this->m_Offset >= this->m_BeginOffset);
  }


  /** Test if the index is at the end of line */
  inline bool IsAtEndOfLine(void)
  {
    return this->m_Offset >= this->m_EndLine;
  }

  /** Test if the index is at the begin of line */
  inline bool IsAtReverseEndOfLine(void)
  {
    return this->m_Offset < this->m_BeginLine;
  }

  /** Set the direction of movement */
  inline void SetDirection(unsigned int direction)
  {
    if( direction >= TImage::ImageDimension )
      {
      itkGenericExceptionMacro(<<"In image of dimension " << TImage::ImageDimension <<" Direction " << direction << " sas selected");
      }
    m_Direction = direction;
    m_Jump = this->m_OffsetTable[ m_Direction ];
    // lines are being swept in only 2 possible ways - if the line is
    // in direction 0, then sweeping will be in direction 1. Otherwise
    // sweeping will be in direction 0.
    m_LineLength = this->m_Region.GetSize()[m_Direction];
    m_LineJump = m_Jump * m_LineLength;
    this->m_EndLine = this->m_BeginLine + this->m_LineJump;
  }

  bool IsAtEnd(void)
  {
    return !m_Remaining;
  }

  /** Increment (prefix) the selected dimension.
   * No bounds checking is performed. \sa GetIndex \sa operator-- */
  inline Self & operator++()
  {
    this->m_Offset += m_Jump;
    return *this;
  }

  /** Decrement (prefix) the selected dimension.
   * No bounds checking is performed.  \sa GetIndex \sa operator++ */
  inline Self & operator--()
  {
    this->m_Offset -= m_Jump;
    return *this;
  }

protected:
  // I think we need this - normally only the WithIndex iterators have
  // it.
  unsigned long          m_OffsetTable[ ImageDimension + 1 ];
  // keep track of the beginning and end of line - m_BeginOffset and
  // m_EndOffset are defined in parent classes
  unsigned long m_BeginLine, m_EndLine;
  unsigned long m_LineJump;
  unsigned long m_LineLength;
private:
  unsigned long  m_Jump;
  unsigned int   m_Direction, m_LineMove;
  IndexType m_startIndex, m_EndIndex;
  bool m_Remaining;
};

  //----------------------------------------------------------------------
  //  Go to next line
  //----------------------------------------------------------------------
  template<class TImage>
  inline
  void 
  ImageLinearConstIterator<TImage>
  ::NextLine(void)
  {
    // this is a modified version of the line iterator with index
    // move to the start of the line
    this->m_Offset = this->m_BeginLine;
    IndexType ind = this->m_Image->ComputeIndex( this->m_Offset );
    for( unsigned int n=0; n<TImage::ImageDimension; n++ )
      {

      this->m_Remaining = false;
    
      if( n == m_Direction ) 
        {
        continue;
        }
    
      ind[ n  ]++;
      if( ind[n] <  this->m_EndIndex[n] )
        {
        this->m_Offset += this->m_OffsetTable[ n ];
        this->m_Remaining = true;
        break;
        }
      else 
        {
        this->m_Offset -= this->m_OffsetTable[ n ] * ( this->m_Region.GetSize()[n]-1 );
        }
      }
    this->m_BeginLine = this->m_Offset;
    this->m_EndLine = this->m_BeginLine + this->m_LineJump;
  }



  //----------------------------------------------------------------------
  //  Pass to the last pixel on the previous line
  //----------------------------------------------------------------------
  template<class TImage>
  inline
  void 
  ImageLinearConstIterator<TImage>
  ::PreviousLine(void)
  {

    this->m_Offset = this->m_BeginLine;
  IndexType ind = this->m_Image->ComputeIndex( this->m_Offset );
  for( unsigned int n=0; n<TImage::ImageDimension; n++ )
    {
    
    this->m_Remaining = false;
    
    if( n == m_Direction ) 
      {
      continue;
      }
    
    ind[ n  ]--;
    if( ind[n] <  this->m_EndIndex[n] )
      {
      this->m_Offset -= this->m_OffsetTable[ n ];
      this->m_Remaining = true;
      break;
      }
    else 
      {
      this->m_Offset += this->m_OffsetTable[ n ] * ( this->m_Region.GetSize()[n]-1 );
      }
    }
  this->m_BeginLine = this->m_Offset;
  this->m_EndLine = this->m_BeginLine + this->m_LineJump;

  }
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageLinearConstIterator.txx"
#endif

#endif 
