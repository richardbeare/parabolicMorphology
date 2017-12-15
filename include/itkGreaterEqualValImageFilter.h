/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkGreaterEqualValImageFilter_h
#define itkGreaterEqualValImageFilter_h
#include <itkUnaryFunctorImageFilter.h>
namespace itk
{
/** \class GreaterEqualValImageFilter
 * \brief Computes the absolute difference between an image and a
 * constant. Can be done with ShiftScale and AbsIamgeFilters.
 *
 * \ingroup ParabolicMorphology
 *
 */

namespace Functor
{
template< class TInput, class TOutput >
class GEConst
{
public:
  GEConst() { m_Val = (TInput)0.0; }
  void SetVal(const TInput i) { m_Val = i; }

  ~GEConst() {}
  bool operator!=(const GEConst &) const
  {
    return false;
  }

  bool operator==(const GEConst & other) const
  {
    return !( *this != other );
  }

  inline TOutput operator()(const TInput & A)
  {
    return static_cast< TOutput >( A >= m_Val );
  }

private:
  TInput m_Val;
};
}

template< class TInputImage, class TOutputImage >
class ITK_EXPORT GreaterEqualValImageFilter:
  public
  UnaryFunctorImageFilter< TInputImage, TOutputImage,
                           Functor::GEConst<
                             typename TInputImage::PixelType,
                             typename TOutputImage::PixelType >   >
{
public:
  /** Standard class typedefs. */
  typedef GreaterEqualValImageFilter
                                                                                          Self;
  typedef UnaryFunctorImageFilter< TInputImage, TOutputImage,
                                   Functor::GEConst< typename TInputImage::PixelType,
                                                     typename TOutputImage::PixelType > > Superclass;
  typedef SmartPointer< Self >
                                                                                          Pointer;
  typedef SmartPointer< const Self >
                                                                                          ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  void SetVal(typename TInputImage::PixelType val)
  {
    this->GetFunctor().SetVal(val);
    this->Modified();
  }

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro( InputConvertibleToDoubleCheck,
                   ( Concept::Convertible< typename TInputImage::PixelType, double > ) );
  itkConceptMacro( DoubleConvertibleToOutputCheck,
                   ( Concept::Convertible< double, typename TOutputImage::PixelType > ) );
  /** End concept checking */
#endif
protected:
  GreaterEqualValImageFilter() {}
  virtual ~GreaterEqualValImageFilter() {}
private:
  ITK_DISALLOW_COPY_AND_ASSIGN(GreaterEqualValImageFilter);
};
} // end namespace itk

#endif
