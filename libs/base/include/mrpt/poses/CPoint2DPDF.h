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
#ifndef CPoint2DPDF_H
#define CPoint2DPDF_H

#include <mrpt/utils/CSerializable.h>
#include <mrpt/utils/CProbabilityDensityFunction.h>
#include <mrpt/poses/CPoint2D.h>

namespace mrpt
{
namespace poses
{
	using namespace mrpt::math;

	DEFINE_SERIALIZABLE_PRE_CUSTOM_BASE( CPoint2DPDF, mrpt::utils::CSerializable )

	/** Declares a class that represents a Probability Distribution function (PDF) of a 2D point (x,y).
	 *   This class is just the base class for unifying many diferent
	 *    ways this PDF can be implemented.
	 *
	 *  For convenience, a pose composition is also defined for any
	 *    PDF derived class, changeCoordinatesReference, in the form of a method rather than an operator.
     *
	 *  For a similar class for 6D poses (a 3D point with attitude), see CPose3DPDF
	 *
	 *  See also the tutorial on <a href="http://www.mrpt.org/Probability_Density_Distributions_Over_Spatial_Representations">probabilistic spatial representations in the MRPT</a>.
	 *
	 * \sa CPoint2D, CPointPDF
	 */
	class BASE_IMPEXP CPoint2DPDF : public mrpt::utils::CSerializable, public mrpt::utils::CProbabilityDensityFunction<CPoint2D,2>
	{
		DEFINE_VIRTUAL_SERIALIZABLE( CPoint2DPDF )

	 public:
		 /** Returns an estimate of the pose, (the mean, or mathematical expectation of the PDF)
		  */
		MRPT_DECLARE_DEPRECATED_FUNCTION("**deprecated** Use getMean instead", 
		CPoint2D  getEstimatedPoint() const )
		{
			CPoint2D p;
			this->getMean(p);
			return p;
		}

		/** Copy operator, translating if necesary (for example, between particles and gaussian representations)
		  */
		virtual void  copyFrom(const CPoint2DPDF &o) = 0;

		/** Bayesian fusion of two point distributions (product of two distributions->new distribution), then save the result in this object (WARNING: See implementing classes to see classes that can and cannot be mixtured!)
		  * \param p1 The first distribution to fuse
		  * \param p2 The second distribution to fuse
		  * \param minMahalanobisDistToDrop If set to different of 0, the result of very separate Gaussian modes (that will result in negligible components) in SOGs will be dropped to reduce the number of modes in the output.
		  */
		virtual void  bayesianFusion( const CPoint2DPDF &p1, const CPoint2DPDF &p2, const double &minMahalanobisDistToDrop = 0)  = 0 ;

	}; // End of class def.


	} // End of namespace
} // End of namespace

#endif