#ifndef _itkImageLinearConstIterator_txx
#define _itkImageLinearConstIterator_txx

#include "itkImageLinearConstIterator.h"


namespace itk
{



//----------------------------------------------------------------------
//  Constructor
//----------------------------------------------------------------------
template<class TImage>
ImageLinearConstIterator<TImage>
::ImageLinearConstIterator(
                          const ImageType *ptr,
                          const RegionType& region )
    : ImageConstIterator<TImage>( ptr, region ) 
{
  memcpy(m_OffsetTable, ptr->GetOffsetTable(),
         (ImageDimension+1)*sizeof(unsigned long));
  this->m_startIndex = this->m_Region.GetIndex();

  for (unsigned int i=0; i < ImageDimension; ++i)
    {
    unsigned long size = region.GetSize()[i];
    if( size > 0 )
      {
      m_Remaining = true;
      }
    m_EndIndex[i] = m_startIndex[i] + static_cast<long>(size);
    }
  this->SetDirection( 0 );
}


//----------------------------------------------------------------------
//  Go to the last pixel of the current line
//----------------------------------------------------------------------
template<class TImage>
void 
ImageLinearConstIterator<TImage>
::GoToReverseBeginOfLine(void)
{
  this->m_Offset = this->m_EndLine - this->m_Jump;
}



//----------------------------------------------------------------------
//  Go to the first pixel of the current line
//----------------------------------------------------------------------
template<class TImage>
void 
ImageLinearConstIterator<TImage>
::GoToBeginOfLine(void)
{
  this->m_Offset = this->m_BeginLine;
}




//----------------------------------------------------------------------
//  Pass to the past last pixel of the current line
//----------------------------------------------------------------------
template<class TImage>
void 
ImageLinearConstIterator<TImage>
::GoToEndOfLine(void)
{
  this->m_Offset = this->m_EndLine;
}


} // end namespace itk

#endif
