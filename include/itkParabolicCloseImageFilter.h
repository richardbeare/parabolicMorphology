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
#ifndef itkParabolicCloseImageFilter_h
#define itkParabolicCloseImageFilter_h

#include "itkParabolicOpenCloseSafeBorderImageFilter.h"
#include "itkNumericTraits.h"

namespace itk
{
/**
 * \class ParabolicCloseImageFilter
 * \brief Class for morphological closing
 * operations  with parabolic structuring elements.
 *
 * This filter provides options for padded borders
 *
 * This filter is threaded.
 *
 * Core methods described in the InsightJournal article:
 * "Morphology with parabolic structuring elements"
 *
 * http://hdl.handle.net/1926/1370
 *
 * \sa itkParabolicOpenCloseImageFilter
 *
 *
 * \ingroup ParabolicMorphology
 *
 * \author Richard Beare, Department of Medicine, Monash University,
 * Australia.  <Richard.Beare@monash.edu>
**/

template< typename TInputImage,
          typename TOutputImage = TInputImage >
class ITK_EXPORT ParabolicCloseImageFilter:
  public ParabolicOpenCloseSafeBorderImageFilter< TInputImage, false,
                                                  TOutputImage >
{
public:
  /** Standard class typedefs. */
  typedef ParabolicCloseImageFilter                                                   Self;
  typedef ParabolicOpenCloseSafeBorderImageFilter< TInputImage, false, TOutputImage > Superclass;
  typedef SmartPointer< Self >                                                        Pointer;
  typedef SmartPointer< const Self >                                                  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(ParabolicCloseImageFilter, ParabolicOpenCloseSafeBorderImageFilter);

  /** Pixel Type of the input image */
  typedef TInputImage                                         InputImageType;
  typedef TOutputImage                                        OutputImageType;
  typedef typename TInputImage::PixelType                     PixelType;
  typedef typename NumericTraits< PixelType >::RealType       RealType;
  typedef typename NumericTraits< PixelType >::ScalarRealType ScalarRealType;
  typedef typename TOutputImage::PixelType                    OutputPixelType;

  /** Smart pointer typedef support.  */
  typedef typename TInputImage::Pointer      InputImagePointer;
  typedef typename TInputImage::ConstPointer InputImageConstPointer;

  /** a type to represent the "kernel radius" */
  typedef typename itk::FixedArray< ScalarRealType, TInputImage::ImageDimension > RadiusType;

  /** Image dimension. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  /** Define the image type for internal computations
      RealType is usually 'double' in NumericTraits.
      Here we prefer float in order to save memory.  */
protected:
  ParabolicCloseImageFilter(){}
  virtual ~ParabolicCloseImageFilter() {}
//   void PrintSelf(std::ostream& os, Indent indent) const;
private:
  ParabolicCloseImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);            //purposely not implemented
};
} // end namespace itk

#endif
