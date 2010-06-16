/* +---------------------------------------------------------------------------+
   |          The Mobile Robot Programming Toolkit (MRPT) C++ library          |
   |                                                                           |
   |                   http://mrpt.sourceforge.net/                            |
   |                                                                           |
   |   Copyright (C) 2005-2010  University of Malaga                           |
   |                                                                           |
   |    This software was written by the Machine Perception and Intelligent    |
   |      Robotics Lab, University of Malaga (Spain).                          |
   |    Contact: Jose-Luis Blanco  <jlblanco@ctima.uma.es>                     |
   |                                                                           |
   |  This file is part of the MRPT project.                                   |
   |                                                                           |
   |     MRPT is free software: you can redistribute it and/or modify          |
   |     it under the terms of the GNU General Public License as published by  |
   |     the Free Software Foundation, either version 3 of the License, or     |
   |     (at your option) any later version.                                   |
   |                                                                           |
   |   MRPT is distributed in the hope that it will be useful,                 |
   |     but WITHOUT ANY WARRANTY; without even the implied warranty of        |
   |     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         |
   |     GNU General Public License for more details.                          |
   |                                                                           |
   |     You should have received a copy of the GNU General Public License     |
   |     along with MRPT.  If not, see <http://www.gnu.org/licenses/>.         |
   |                                                                           |
   +---------------------------------------------------------------------------+ */

#include <mrpt/base.h>  // Precompiled headers


#include <mrpt/poses/CPoint2DPDFGaussian.h>
#include <mrpt/poses/CPose3D.h>
#include <mrpt/poses/CPoint3D.h>
#include <mrpt/math/CMatrixD.h>
#include <mrpt/random/RandomGenerators.h>

using namespace mrpt::poses;
using namespace mrpt::utils;
using namespace mrpt::random;

IMPLEMENTS_SERIALIZABLE( CPoint2DPDFGaussian, CPoint2DPDF, mrpt::poses )


/*---------------------------------------------------------------
	Constructor
  ---------------------------------------------------------------*/
CPoint2DPDFGaussian::CPoint2DPDFGaussian() : mean(0,0), cov()
{
}

/*---------------------------------------------------------------
	Constructor
  ---------------------------------------------------------------*/
CPoint2DPDFGaussian::CPoint2DPDFGaussian(
	const CPoint2D	&init_Mean,
	const CMatrixDouble22 &init_Cov ) : mean(init_Mean), cov(init_Cov)
{
}

/*---------------------------------------------------------------
	Constructor
  ---------------------------------------------------------------*/
CPoint2DPDFGaussian::CPoint2DPDFGaussian(
	const CPoint2D	&init_Mean ) : mean(init_Mean), cov()
{
}


/*---------------------------------------------------------------
						writeToStream
  ---------------------------------------------------------------*/
void  CPoint2DPDFGaussian::writeToStream(CStream &out,int *version) const
{
	if (version)
		*version = 0;
	else
	{
		out << CPoint2D(mean) << cov;
	}
}

/*---------------------------------------------------------------
						readFromStream
  ---------------------------------------------------------------*/
void  CPoint2DPDFGaussian::readFromStream(CStream &in,int version)
{
	switch(version)
	{
	case 0:
		{
			in >> mean >> cov;
		} break;
	default:
		MRPT_THROW_UNKNOWN_SERIALIZATION_VERSION(version)

	};
}


/*---------------------------------------------------------------
						operator =
  ---------------------------------------------------------------*/
void  CPoint2DPDFGaussian::copyFrom(const CPoint2DPDF &o)
{
	if (this == &o) return;		// It may be used sometimes

	// Convert to gaussian pdf:
	o.getCovarianceAndMean(cov,mean);
}

/*---------------------------------------------------------------

  ---------------------------------------------------------------*/
void  CPoint2DPDFGaussian::saveToTextFile(const std::string &file) const
{
	MRPT_START;

	FILE	*f=os::fopen(file.c_str(),"wt");
	if (!f) return;

	os::fprintf(f,"%f %f %f\n", mean.x(), mean.y(), mean.z() );

	os::fprintf(f,"%f %f %f\n", cov(0,0),cov(0,1),cov(0,2) );
	os::fprintf(f,"%f %f %f\n", cov(1,0),cov(1,1),cov(1,2) );
	os::fprintf(f,"%f %f %f\n", cov(2,0),cov(2,1),cov(2,2) );


	os::fclose(f);

	MRPT_END;
}

/*---------------------------------------------------------------
						changeCoordinatesReference
 ---------------------------------------------------------------*/
void  CPoint2DPDFGaussian::changeCoordinatesReference(const CPose3D &newReferenceBase )
{
	// Clip the 4x4 matrix
	CMatrixDouble22 M = CMatrixDouble22( newReferenceBase.getHomogeneousMatrixVal() );

	// The mean:
	mean = CPoint2D( newReferenceBase +mean );

	// The covariance:
	M.multiply_HCHt(CMatrixDouble22(cov), cov); // save in cov
}

/*---------------------------------------------------------------
					bayesianFusion
 ---------------------------------------------------------------*/
void  CPoint2DPDFGaussian::bayesianFusion( const CPoint2DPDFGaussian &p1, const CPoint2DPDFGaussian &p2 )
{
	MRPT_START;

	CMatrixDouble22	C1_inv;
	p1.cov.inv(C1_inv);

	CMatrixDouble22	C2_inv;
	p2.cov.inv(C2_inv);

	CMatrixDouble22	 L = C1_inv;
	L+= C2_inv;

	L.inv(cov); // The new cov.

	CMatrixDouble21	x1 = CMatrixDouble21(p1.mean);
	CMatrixDouble21	x2 = CMatrixDouble21(p2.mean);
	CMatrixDouble21 x = cov * ( C1_inv*x1 + C2_inv*x2 );

	mean.x( x.get_unsafe(0,0) );
	mean.y( x.get_unsafe(1,0) );

	std::cout << "IN1: " << p1.mean << "\n" << p1.cov << "\n";
	std::cout << "IN2: " << p2.mean << "\n" << p2.cov << "\n";
	std::cout << "OUT: " << mean << "\n" << cov << "\n";

	MRPT_END;
}

/*---------------------------------------------------------------
					productIntegralWith
 ---------------------------------------------------------------*/
double  CPoint2DPDFGaussian::productIntegralWith( const CPoint2DPDFGaussian &p) const
{
	MRPT_START
	// --------------------------------------------------------------
	// 12/APR/2009 - Jose Luis Blanco:
	//  The integral over all the variable space of the product of two
	//   Gaussians variables amounts to simply the evaluation of
	//   a normal PDF at (0,0), with mean=M1-M2 and COV=COV1+COV2
	// ---------------------------------------------------------------
	CMatrixDouble22 C = cov + p.cov;	// Sum of covs:

	CMatrixDouble22 C_inv;
	C.inv(C_inv);

	CMatrixDouble21	MU(UNINITIALIZED_MATRIX); // Diff. of means
	MU.get_unsafe(0,0) = mean.x() - p.mean.x();
	MU.get_unsafe(1,0) = mean.y() - p.mean.y();

	return std::pow( M_2PI, -0.5*state_length )
		* (1.0/std::sqrt( C.det() ))
		* exp( -0.5* MU.multiply_HCHt_scalar(C_inv) );

	MRPT_END
}


/*---------------------------------------------------------------
					productIntegralNormalizedWith
 ---------------------------------------------------------------*/
double  CPoint2DPDFGaussian::productIntegralNormalizedWith( const CPoint2DPDFGaussian &p) const
{
	return std::exp( -0.5*square(mahalanobisDistanceTo(p)) );
}

/*---------------------------------------------------------------
					drawSingleSample
 ---------------------------------------------------------------*/
void CPoint2DPDFGaussian::drawSingleSample(CPoint2D &outSample) const
{
	MRPT_START

	vector_double vec;
	randomGenerator.drawGaussianMultivariate(vec,cov);

	ASSERT_(vec.size()==2);
	outSample.x( mean.x() + vec[0] );
	outSample.y( mean.y() + vec[1] );

	MRPT_END
}

/*---------------------------------------------------------------
					bayesianFusion
 ---------------------------------------------------------------*/
void  CPoint2DPDFGaussian::bayesianFusion( const CPoint2DPDF &p1_, const CPoint2DPDF &p2_,const double &minMahalanobisDistToDrop )
{
	MRPT_START;

	// p1: CPoint2DPDFGaussian, p2: CPosePDFGaussian:
	ASSERT_( p1_.GetRuntimeClass() == CLASS_ID(CPoint2DPDFGaussian) );
	ASSERT_( p2_.GetRuntimeClass() == CLASS_ID(CPoint2DPDFGaussian) );

	THROW_EXCEPTION("TODO!!!");

	MRPT_END
}

/*---------------------------------------------------------------
					mahalanobisDistanceTo
 ---------------------------------------------------------------*/
double CPoint2DPDFGaussian::mahalanobisDistanceTo( const CPoint2DPDFGaussian & other ) const
{
	// The difference in means:
	CMatrixDouble12 deltaX;
	deltaX.get_unsafe(0,0) = other.mean.x() - mean.x();
	deltaX.get_unsafe(0,1) = other.mean.y() - mean.y();

	// The inverse of the combined covs:
	CMatrixDouble22 COV = other.cov;
	COV += this->cov;

	CMatrixDouble22 COV_inv;
	COV.inv(COV_inv);
	return sqrt( deltaX.multiply_HCHt_scalar(COV_inv) );
}