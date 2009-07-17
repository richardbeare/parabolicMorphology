#ifndef __itkBinaryErodeParaImageFilter_h
#define __itkBinaryErodeParaImageFilter_h

#include "itkParabolicErodeImageFilter.h"
#include "itkCastImageFilter.h"

namespace itk
{

/**
 * \class BinaryErodeParaImageFilter
 * \brief Class for binary morphological erosion operation.
 *
 * This class uses the parabolic morphology operations to do very
 * efficient erosions by circles/spheres. The operations are efficient
 * because the underlying parabolic operations are separable and
 * the operations are implicitly short circuited in comparison to a
 * full distance transform approach.
 *
 * The basic idea is that a binary erosion or dilation by a circle/sphere
 * can be carried out by thresholding a distance transform. By using
 * the parabolic filters we can avoid computing the entire distance
 * transform and instead only compute the subset we are interested
 * in. In addition in the special case of erosion, the threshold
 * operation can be replaced by a floating point to integer cast.
 *
 * The output type must be integer (probably char) and the foreground
 * value must be 1. Float types are used internally.
 *
 * This filter was developed as a result of discussions with
 * M.Starring on the ITK mailing list.
 * 
 * \sa itkParabolicErodeImageFilter
 *
 * \author Richard Beare, Department of Medicine, Monash University,
 * Australia.  <Richard.Beare@med.monash.edu.au>
**/


template <typename TInputImage,
          typename TOutputImage= TInputImage >
class ITK_EXPORT BinaryErodeParaImageFilter:
    public ImageToImageFilter<TInputImage,TOutputImage>

{

public:
  /** Standard class typedefs. */
  typedef BinaryErodeParaImageFilter  Self;
  typedef ImageToImageFilter<TInputImage,TOutputImage> Superclass;
  typedef SmartPointer<Self>                   Pointer;
  typedef SmartPointer<const Self>        ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(BinaryErodeParaImageFilter, ParabolicErodeImageFilter);


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

  typedef typename NumericTraits< PixelType >::FloatType   InternalRealType;

  /** Image dimension. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  typedef typename itk::FixedArray<ScalarRealType, TInputImage::ImageDimension> RadiusType;

  void SetRadius(ScalarRealType radius);
  itkSetMacro(Radius, RadiusType);
  itkGetConstReferenceMacro(Radius, RadiusType);

  void SetUseImageSpacing(bool g)
  {
    m_Para->SetUseImageSpacing(true);
  }

  /** Image related typedefs. */

  /* add in the traits here */

protected:
  void GenerateData( void );

  BinaryErodeParaImageFilter();
  virtual ~BinaryErodeParaImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  typedef typename itk::Image<InternalRealType, InputImageType::ImageDimension> InternalImageType;
  typedef typename itk::ParabolicErodeImageFilter<TInputImage, InternalImageType> ParabolicType;
  typedef typename itk::CastImageFilter<InternalImageType, OutputImageType> CastType;
private:
  BinaryErodeParaImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  RadiusType m_Radius;

  typename ParabolicType::Pointer m_Para;
  typename CastType::Pointer m_Cast;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBinaryErodeParaImageFilter.txx"
#endif


#endif //__itkBinaryErodeParaImageFilter_h
