
#ifndef _itkImageLinearIterator_txx
#define _itkImageLinearIterator_txx

#include "itkImageLinearIterator.h"

namespace itk
{



template< typename TImage >
ImageLinearIterator<TImage>
::ImageLinearIterator()
  : ImageLinearConstIterator<TImage>() 
{


}



template< typename TImage >
ImageLinearIterator<TImage>
::ImageLinearIterator(ImageType *ptr, const RegionType& region) :
  ImageLinearConstIterator<TImage>(   ptr, region ) 
{


}


 
template< typename TImage >
ImageLinearIterator<TImage>
::ImageLinearIterator( const ImageIterator<TImage> &it):
  ImageLinearConstIterator<TImage>(it)
{ 
}

 
template< typename TImage >
ImageLinearIterator<TImage>
::ImageLinearIterator( const ImageLinearConstIterator<TImage> &it):
  ImageLinearConstIterator<TImage>(it)
{ 
}

 
template< typename TImage >
ImageLinearIterator<TImage> &
ImageLinearIterator<TImage>
::operator=( const ImageLinearConstIterator<TImage> &it)
{ 
  this->ImageLinearConstIterator<TImage>::operator=(it);
  return *this;
}



} // end namespace itk

#endif
