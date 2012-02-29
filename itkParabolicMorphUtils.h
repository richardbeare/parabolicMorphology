#ifndef __itkParabolicUtils_h
#define __itkParabolicUtils_h

#include <itkArray.h>

#include "itkProgressReporter.h"
namespace itk {

// contact point algorithm
template <class LineBufferType, class RealType, bool doDilate>
void DoLineCP(LineBufferType &LineBuf, LineBufferType &tmpLineBuf, 
	      const RealType magnitude, const RealType m_Extreme)
{
  // contact point algorithm
  long koffset = 0, newcontact=0;  // how far away the search starts.

  const long LineLength = LineBuf.size();
  // negative half of the parabola
  for (long pos = 0; pos < LineLength; pos++)
    {
    RealType BaseVal = (RealType)m_Extreme; // the base value for
					    // comparison
    for (long krange = koffset; krange <= 0; krange++)
      {
      // difference needs to be paramaterised
      RealType T = LineBuf[pos + krange] - magnitude * krange * krange;
      // switch on template parameter - hopefully gets optimized away.
      if (doDilate ? (T >= BaseVal) : (T <= BaseVal) )
	{
	BaseVal = T;
	newcontact = krange;
	}
      }
    tmpLineBuf[pos] = BaseVal;
    koffset = newcontact - 1;
    }
  // positive half of parabola
  koffset = newcontact = 0;
  for (long pos = LineLength - 1; pos >= 0; pos--)
    {
    RealType BaseVal = (RealType)m_Extreme; // the base value for comparison
    for (long krange = koffset; krange >= 0; krange--)
      {
      RealType T = tmpLineBuf[pos + krange] - magnitude * krange * krange;
      if (doDilate ? (T >= BaseVal) : (T <= BaseVal))
	{
	BaseVal = T;
	newcontact = krange;
	}
      }
    LineBuf[pos] = BaseVal;
    koffset = newcontact + 1;
    }
}

// intersection algorithm
// This algorithm has been described a couple of times. First by van
// den Bookgaard and more recently by Felzenszwalb and Huttenlocher,
// in the context of generalized distance transform
template <class LineBufferType, class IndexBufferType,
	  class EnvBufferType, class RealType, bool doDilate>
void DoLineIntAlg(LineBufferType &LineBuf, EnvBufferType &F, 
		  IndexBufferType &v, IndexBufferType &z,
		  const RealType magnitude)
{
  int     k;    /* Index of rightmost parabola in lower envelope */
  /* Locations of parabolas in lower envelope */
  /* these need to be int, rather than unsigned, to make boundary
  conditions easy to test */
//  typedef typename itk::Array<int> IndexBufferType;

  // remember to pass these buffers as argmuments - task for optimizing
//  IndexBufferType  v(LineBuf.size());
  /* Locations of boundaries between parabolas */
//  IndexBufferType  z(LineBuf.size());
  int     q;
  double  s;
  /* holds precomputed scale*f(q) + q^2 for speedup */
//  LineBufferType F(LineBuf.size());

  // initialize
  k = 0;
  v[0] = 0;
  z[0] = NumericTraits<int>::NonpositiveMin();
  z[1] = NumericTraits<int>::max();
  F[0] = LineBuf[0];
  const size_t N(LineBuf.size());

  for (q=1; q<N; q++)   /* main loop */
    {
    if (doDilate)
      {
      F[q] = (LineBuf[q]/magnitude) - (q*q);  /* precompute f(q) + q^2 for speedup */
      k++;
      do 
	{
	k--; /* remove last parabola from surface */
	s = (F[q] - F[v[k]]) / (2 * (v[k] - q)); /* compute intersection */
	} 
      while (s <= z[k]);
      k++; /* bump k to add new parabola */      
      }
    else
      {
      F[q] = (LineBuf[q]/magnitude) + (q*q);  /* precompute f(q) + q^2 for speedup */
      k++;
      do 
	{
	k--; /* remove last parabola from surface */
	s = (F[q] - F[v[k]]) / (2 * (q - v[k])); /* compute intersection */
	} 
      while (s <= z[k]);
      k++; /* bump k to add new parabola */
      }
    v[k] = q;
    z[k] = s;
    assert((k+1) <= N);
    z[k+1] = NumericTraits<int>::max();
    } /* for q */
  /* now reconstruct output */
  k = 0;
  if (doDilate)
    {
    for (q=0; q<N; q++) 
      {
      while (z[k+1] < q) k++;
      assert(v[k] < N);
      assert(v[k] >= 0);
      LineBuf[q] = ((F[v[k]]-(q * (q - 2*v[k]))) * magnitude);
      }
    }
  else
    {
    for (q=0; q<N; q++) 
      {
      while (z[k+1] < q) k++;
      assert(v[k] < N);
      assert(v[k] >= 0);
      LineBuf[q] = ((q * (q - 2*v[k]) + F[v[k]]) * magnitude);
      }
    }

}

template <class TInIter, class TOutIter, class RealType, 
	  class OutputPixelType, bool doDilate>
void doOneDimension(TInIter &inputIterator, TOutIter &outputIterator, 
		    ProgressReporter &progress,
		    const long LineLength, 
		    const unsigned direction,
		    const int m_MagnitudeSign,
		    const bool m_UseImageSpacing,
		    const RealType m_Extreme,
		    const RealType image_scale,
		    const RealType Sigma, 
		    int ParabolicAlgorithmChoice)
{
  enum {
    NOCHOICE = 0,         // decices based on scale - experimental
    CONTACTPOINT = 1, // sometimes faster at low scale
    INTERSECTION = 2  // default
  } ParabolicAlgorithm;

//  typedef typename std::vector<RealType> LineBufferType;

  // message from M.Starring suggested performance gain using Array
  // instead of std::vector.
  typedef typename itk::Array<RealType> LineBufferType;
  RealType iscale = 1.0;
  if (m_UseImageSpacing)
    {
    iscale = image_scale;
    }
  if (ParabolicAlgorithmChoice == NOCHOICE)
    {
    // both set to true or false - use scale to figure it out
    if ((2.0 * Sigma) < 0.2)
      {
      ParabolicAlgorithmChoice = CONTACTPOINT;
      }
    else
      {
      ParabolicAlgorithmChoice = INTERSECTION;
      }
    }


  if (ParabolicAlgorithmChoice == CONTACTPOINT)
    {
    // using the contact point algorithm

//  const RealType magnitude = m_MagnitudeSign * 1.0/(2.0 *
//  Sigma/(iscale*iscale));
    // restructure equation to reduce numerical error
    const RealType magnitudeCP = (m_MagnitudeSign * iscale * iscale)/(2.0 * Sigma);
    
    LineBufferType LineBuf(LineLength);
    LineBufferType tmpLineBuf(LineLength);
    inputIterator.SetDirection(direction);
    outputIterator.SetDirection(direction);
    inputIterator.GoToBegin();
    outputIterator.GoToBegin();
  
    while( !inputIterator.IsAtEnd() && !outputIterator.IsAtEnd() )
      {
      // process this direction
      // fetch the line into the buffer - this methodology is like
      // the gaussian filters
      unsigned int i=0;
      while( !inputIterator.IsAtEndOfLine() )
	{
	LineBuf[i++]      = static_cast<RealType>(inputIterator.Get());
	++inputIterator;
	}
      
      DoLineCP<LineBufferType,  RealType, doDilate>(LineBuf, tmpLineBuf, magnitudeCP, m_Extreme);
      // copy the line back
      unsigned int j=0;
      while( !outputIterator.IsAtEndOfLine() )
	{
	outputIterator.Set( static_cast<OutputPixelType>( LineBuf[j++] ) );
	++outputIterator;
	}
      
      // now onto the next line
      inputIterator.NextLine();
      outputIterator.NextLine();
      progress.CompletedPixel();
      }
    }
  else
    {
    // using the Intersection algorithm
    typedef typename itk::Array<int> IndexBufferType;

    const RealType magnitudeInt = (iscale * iscale)/(2.0 * Sigma);
    LineBufferType LineBuf(LineLength);
    LineBufferType Fbuf(LineLength);
    IndexBufferType Vbuf(LineLength);
    IndexBufferType Zbuf(LineLength+1);
    
    inputIterator.SetDirection(direction);
    outputIterator.SetDirection(direction);
    inputIterator.GoToBegin();
    outputIterator.GoToBegin();
  
    unsigned count = 0;
    while( !inputIterator.IsAtEnd() && !outputIterator.IsAtEnd() )
      {
      // process this direction
      // fetch the line into the buffer - this methodology is like
      // the gaussian filters
      unsigned int i=0;
      while( !inputIterator.IsAtEndOfLine() )
	{
	LineBuf[i++]      = static_cast<RealType>(inputIterator.Get());
	++inputIterator;
	}
      DoLineIntAlg<LineBufferType, IndexBufferType, 
		   LineBufferType, RealType, doDilate>(LineBuf, Fbuf, Vbuf, Zbuf, magnitudeInt);
      ++count;
      // copy the line back
      unsigned int j=0;
      while( !outputIterator.IsAtEndOfLine() )
	{
	outputIterator.Set( static_cast<OutputPixelType>( LineBuf[j++] ) );
	++outputIterator;
	}
      
      // now onto the next line
      inputIterator.NextLine();
      outputIterator.NextLine();
      progress.CompletedPixel();
      }    
    }


}


}
#endif
