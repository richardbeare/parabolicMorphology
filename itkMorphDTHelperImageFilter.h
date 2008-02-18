#ifndef __itkMorphDTHelperImageFilter_h
#define __itkMorphDTHelperImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"

namespace itk
{
  
/** \class MorphDTHelperImageFilter
 * \brief Implements a pixel-wise operator to form a signed distance transform.
 *
 * Numeric conversions (castings) are done by the C++ defaults.
 * 
 * \ingroup IntensityImageFilters  Multithreaded
 */
namespace Function {  

template< class TInput1, class TOutput=TInput1>
class MorphDTHelper
{
public:
  MorphDTHelper() {}
  ~MorphDTHelper() {}
  void SetWeight(double i) { m_Weight = i; }
  bool operator!=( const MorphDTHelper & ) const
  {
    return false;
  }
  bool operator==( const MorphDTHelper & other ) const
  {
    return !(*this != other);
  }
  inline TOutput operator()( const TInput1 & A)
  { 
    return static_cast<TOutput>(m_Weight * vcl_sqrt((double)A));
  }
private:
  double m_Weight;

};
}

template <class TInputImage1, class TOutputImage=TInputImage1>
class ITK_EXPORT MorphDTHelperImageFilter :
    public
UnaryFunctorImageFilter<TInputImage1,TOutputImage, 
                         Function::MorphDTHelper< 
  typename TInputImage1::PixelType, 
  typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef MorphDTHelperImageFilter  Self;
  typedef UnaryFunctorImageFilter<TInputImage1,TOutputImage, 
                                   Function::MorphDTHelper< 
    typename TInputImage1::PixelType, 
    typename TOutputImage::PixelType>   
  > Superclass;
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(MorphDTHelperImageFilter, 
               UnaryFunctorImageFilter);

  void SetWeight(double val)
  {
    this->GetFunctor().SetWeight(val);
    this->Modified();
  }

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(Input1ConvertibleToOutputCheck,
    (Concept::Convertible<typename TInputImage1::PixelType,
                          typename TOutputImage::PixelType>));
  /** End concept checking */
#endif

protected:
  MorphDTHelperImageFilter() {}
  virtual ~MorphDTHelperImageFilter() {}

private:
  MorphDTHelperImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk


#endif
