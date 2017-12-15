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
#ifndef itkParabolicOpenCloseImageFilter_h
#define itkParabolicOpenCloseImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"

namespace itk
{
/**
 * \class ParabolicOpenCloseImageFilter
 * \brief Parent class for morphological opening and closing
 * operations  with parabolic structuring elements.
 *
 * This filter doesn't use the erode/dilate classes directly so
 * that multiple image copies aren't necessary.
 *
 * This filter is threaded. Threading mechanism derived from
 * SignedMaurerDistanceMap extensions by Gaetan Lehman
 *
 * Core methods described in the InsightJournal article:
 * "Morphology with parabolic structuring elements"
 *
 * http://hdl.handle.net/1926/1370
 *
 * \sa itkParabolicErodeDilateImageFilter
 *
 *
 * \ingroup ParabolicMorphology
 *
 * \author Richard Beare, Department of Medicine, Monash University,
 * Australia.  <Richard.Beare@monash.edu>
 *
**/
template< typename TInputImage,
          bool doOpen,
          typename TOutputImage = TInputImage >
class ITK_EXPORT ParabolicOpenCloseImageFilter:
  public ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard class typedefs. */
  typedef ParabolicOpenCloseImageFilter                   Self;
  typedef ImageToImageFilter< TInputImage, TOutputImage > Superclass;
  typedef SmartPointer< Self >                            Pointer;
  typedef SmartPointer< const Self >                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(ParabolicOpenCloseImageFilter, ImageToImageFilter);

  /** Pixel Type of the input image */
  typedef TInputImage                                         InputImageType;
  typedef TOutputImage                                        OutputImageType;
  typedef typename TInputImage::PixelType                     PixelType;
  typedef typename NumericTraits< PixelType >::RealType       RealType;
  typedef typename NumericTraits< PixelType >::ScalarRealType ScalarRealType;
  typedef typename TOutputImage::PixelType                    OutputPixelType;
  typedef typename OutputImageType::RegionType                OutputImageRegionType;
  typedef typename TInputImage::SizeType                      InputSizeType;
  typedef typename TOutputImage::SizeType                     OutputSizeType;

  /** Smart pointer typedef support.  */
  typedef typename TInputImage::Pointer      InputImagePointer;
  typedef typename TInputImage::ConstPointer InputImageConstPointer;

  typedef typename OutputImageType::IndexType OutputIndexType;

  /** Image related typedefs. */
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** a type to represent the "kernel radius" */
  typedef typename itk::FixedArray< ScalarRealType, TInputImage::ImageDimension > RadiusType;

  /** Define the image type for internal computations
      RealType is usually 'double' in NumericTraits.
      Here we prefer float in order to save memory.  */

  typedef typename NumericTraits< PixelType >::FloatType InternalRealType;
  //typedef typename Image<InternalRealType,
  // itkGetStaticConstMacro(ImageDimension) >   RealImageType;

  // set all of the scales the same
  void SetScale(ScalarRealType scale);

  itkSetMacro(Scale, RadiusType);
  itkGetConstReferenceMacro(Scale, RadiusType);
  /**
   * Set/Get whether the scale refers to pixels or world units -
   * default is false
   */
  itkSetMacro(UseImageSpacing, bool);
  itkGetConstReferenceMacro(UseImageSpacing, bool);
  itkBooleanMacro(UseImageSpacing);

  enum ParabolicAlgorithm {
    NOCHOICE = 0,     // decices based on scale - experimental
    CONTACTPOINT = 1, // sometimes faster at low scale
    INTERSECTION = 2  // default
    };
  /**
   * Set/Get the method used. Choices are contact point or
   * intersection. Intersection is the default. Contact point can be
   * faster at small scales.
   */

  itkSetMacro(ParabolicAlgorithm, int);
  itkGetConstReferenceMacro(ParabolicAlgorithm, int);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro( SameDimension,
                   ( Concept::SameDimension< itkGetStaticConstMacro(InputImageDimension),
                                             itkGetStaticConstMacro(OutputImageDimension) > ) );

  itkConceptMacro( Comparable,
                   ( Concept::Comparable< PixelType > ) );

  /** End concept checking */
#endif
protected:
  ParabolicOpenCloseImageFilter();
  virtual ~ParabolicOpenCloseImageFilter() {}
  void PrintSelf(std::ostream & os, Indent indent) const ITK_OVERRIDE;

  /** Generate Data */
  void GenerateData(void) ITK_OVERRIDE;

  unsigned int SplitRequestedRegion(unsigned int i, unsigned int num,
                                    OutputImageRegionType & splitRegion) ITK_OVERRIDE;

  void ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread, ThreadIdType threadId) ITK_OVERRIDE;

  virtual void GenerateInputRequestedRegion() throw( InvalidRequestedRegionError ) ITK_OVERRIDE;

  // Override since the filter produces the entire dataset.
  void EnlargeOutputRequestedRegion(DataObject *output) ITK_OVERRIDE;

  int m_ParabolicAlgorithm;
private:
  ITK_DISALLOW_COPY_AND_ASSIGN(ParabolicOpenCloseImageFilter);

  RadiusType m_Scale;

  typename TInputImage::PixelType m_Extreme;
  typename TInputImage::PixelType m_Extreme1;
  typename TInputImage::PixelType m_Extreme2;

  int  m_MagnitudeSign;
  int  m_MagnitudeSign1;
  int  m_MagnitudeSign2;
  int  m_CurrentDimension;
  int  m_Stage;
  bool m_UseImageSpacing;
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkParabolicOpenCloseImageFilter.hxx"
#endif

#endif
