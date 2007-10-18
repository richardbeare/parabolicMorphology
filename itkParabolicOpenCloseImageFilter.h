#ifndef __itkParabolicOpenCloseImageFilter_h
#define __itkParabolicOpenCloseImageFilter_h

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
 * This filter is doesn't use the erode/dilate classes directly so
 * that multiple image copies aren't necessary.
 *
 * This filter isn't threaded.
 *
 * \sa itkParabolicErodeDilateImageFilter
 *
 * \author Richard Beare, Department of Medicine, Monash University,
 * Australia.  <Richard.Beare@med.monash.edu.au>
 *
**/
template <typename TInputImage,
	  bool doOpen,
          typename TOutputImage= TInputImage >
class ITK_EXPORT ParabolicOpenCloseImageFilter:
    public ImageToImageFilter<TInputImage,TOutputImage>
{

public:
  /** Standard class typedefs. */
  typedef ParabolicOpenCloseImageFilter  Self;
  typedef ImageToImageFilter<TInputImage,TOutputImage> Superclass;
  typedef SmartPointer<Self>                   Pointer;
  typedef SmartPointer<const Self>        ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


  /** Pixel Type of the input image */
  typedef TInputImage                                    InputImageType;
  typedef TOutputImage                                   OutputImageType;
  typedef typename TInputImage::PixelType                PixelType;
  typedef typename NumericTraits<PixelType>::RealType    RealType;
  typedef typename NumericTraits<PixelType>::ScalarRealType ScalarRealType;
  typedef typename TOutputImage::PixelType  OutputPixelType;

  /** Smart pointer typedef support.  */
  typedef typename TInputImage::Pointer  InputImagePointer;
  typedef typename TInputImage::ConstPointer  InputImageConstPointer;

  /** a type to represent the "kernel radius" */
  typedef typename itk::FixedArray<ScalarRealType, TInputImage::ImageDimension> RadiusType;

  /** Image dimension. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  /** Define the image type for internal computations
      RealType is usually 'double' in NumericTraits.
      Here we prefer float in order to save memory.  */

  typedef typename NumericTraits< PixelType >::FloatType   InternalRealType;
  //typedef typename Image<InternalRealType, itkGetStaticConstMacro(ImageDimension) >   RealImageType;

  // set all of the scales the same
  void SetScale(ScalarRealType scale);
  // different scale for each direction
  void SetScale(RadiusType scale);

  itkGetConstReferenceMacro(Sigma, RadiusType);
  /**
   * Set/Get whether the scale refers to pixels or world units -
   * default is false
   */
  itkSetMacro(UseImageSpacing, bool);
  itkGetConstReferenceMacro(UseImageSpacing, bool);

protected:
  ParabolicOpenCloseImageFilter();
  virtual ~ParabolicOpenCloseImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;
  
  /** Generate Data */
  void GenerateData( void );
  virtual void GenerateInputRequestedRegion() throw(InvalidRequestedRegionError);
  // Override since the filter produces the entire dataset.
  void EnlargeOutputRequestedRegion(DataObject *output);
  
  typedef typename std::vector<ScalarRealType> LineBufferType;
  
private:
  ParabolicOpenCloseImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  RadiusType m_Sigma;
  typename TInputImage::PixelType m_Extreme, m_Extreme1, m_Extreme2;

  int m_MagnitudeSign, m_MagnitudeSign1, m_MagnitudeSign2;

  bool m_UseImageSpacing;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkParabolicOpenCloseImageFilter.txx"
#endif


#endif
