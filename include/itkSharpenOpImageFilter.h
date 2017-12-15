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
#ifndef itkSharpenOpImageFilter_h
#define itkSharpenOpImageFilter_h

#include "itkTernaryFunctorImageFilter.h"

namespace itk
{
/** \class SharpenOpImageFilter
 * \brief Implements the sharpening operation. The inputs are the
 * dilated, eroded and original images.
 *
 * This class is parametrized over the types of the three
 * input images and the type of the output image.
 * Numeric conversions (castings) are done by the C++ defaults.
 *
 * In reality the input and output types of this filter are expected
 * to be the same.
 *
 * Core methods described in the InsightJournal article:
 * "Morphology with parabolic structuring elements"
 *
 * http://hdl.handle.net/1926/1370
 * \ingroup ParabolicMorphology
 *
 * \author Richard Beare, Department of Medicine, Monash University,
 * Australia.  <Richard.Beare@monash.edu>
 *
 */
namespace Function
{
template< class TInput1, class TInput2, class TInput3, class TOutput >
class SharpM
{
public:
  SharpM() {}
  ~SharpM() {}
  bool operator!=(const SharpM &) const
  {
    return false;
  }

  bool operator==(const SharpM & other) const
  {
    return !( *this != other );
  }

  inline TOutput operator()(const TInput1 & A,
                            const TInput2 & B,
                            const TInput3 & C)
  {
    // the sharpening operator. A is the dilation, B the original, C
    // the erosion
    TInput2 diff1 = A - B;
    TInput2 diff2 = B - C;

    if ( diff1 < diff2 ) { return (TOutput)A; }
    if ( diff2 < diff1 ) { return (TOutput)C; }
    return ( (TOutput)B );
  }
};
}

template< class TInputImage1, class TInputImage2,
          class TInputImage3, class TOutputImage >
class ITK_EXPORT SharpenOpImageFilter:
  public
  TernaryFunctorImageFilter< TInputImage1, TInputImage2,
                             TInputImage3, TOutputImage,
                             Function::SharpM< typename TInputImage1::PixelType,
                                               typename TInputImage2::PixelType,
                                               typename TInputImage3::PixelType,
                                               typename TOutputImage::PixelType >   >
{
public:
  /** Standard class typedefs. */
  typedef SharpenOpImageFilter Self;
  typedef TernaryFunctorImageFilter< TInputImage1, TInputImage2,
                                     TInputImage3, TOutputImage,
                                     Function::SharpM< typename TInputImage1::PixelType,
                                                       typename TInputImage2::PixelType,
                                                       typename TInputImage3::PixelType,
                                                       typename TOutputImage::PixelType >   >  Superclass;
  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(SharpenOpImageFilter,
               TernaryFunctorImageFilter);
protected:
  SharpenOpImageFilter() {}
  virtual ~SharpenOpImageFilter() {}
private:
  ITK_DISALLOW_COPY_AND_ASSIGN(SharpenOpImageFilter);
};
} // end namespace itk

#endif
