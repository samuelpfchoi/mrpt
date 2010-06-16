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
#ifndef CICP_H
#define CICP_H

#include <mrpt/slam/CMetricMapsAlignmentAlgorithm.h>
#include <mrpt/utils/CLoadableOptions.h>

namespace mrpt
{
	namespace poses
	{
		class	CPosePDFParticles;
	}

	namespace slam
	{
		using namespace poses;

		/** The ICP algorithm selection, used in mrpt::slam::CICP::options.
		  *  For details on the algorithms refer to http://www.mrpt.org/Scan_Matching_Algorithms
		  */
		enum TICPAlgorithm
		{
			icpClassic = 0,
			icpLevenbergMarquardt,
			icpIKF
		};

		/** Several implementations of ICP (Iterative closest point) algorithms for aligning two point maps.
		 *
		 *   See CICP::AlignPDF for the entry point of the algorithm, and CICP::TConfigParams for all the parameters to the method.
		 *  The algorithm has been exteneded with multihypotheses-support for the correspondences, which generates a Sum-of-Gaussians (SOG)
		 *    PDF as output. See scan_matching::robustRigidTransformation.
		 *
		 * For further details on the method, check the wiki:
		 *   http://www.mrpt.org/Scan_Matching_Algorithms
		 *
		 * \sa CMetricMapsAlignmentAlgorithm
		 */
		class SLAM_IMPEXP  CICP : public CMetricMapsAlignmentAlgorithm
		{
		public:
			/** The ICP algorithm configuration data
			 */
			class SLAM_IMPEXP TConfigParams : public utils::CLoadableOptions
			{
			public:
				/** Initializer for default values:
				  */
				TConfigParams();

				/** See utils::CLoadableOptions
				  */
				void  loadFromConfigFile(
					const mrpt::utils::CConfigFileBase  &source,
					const std::string &section);

				/** See utils::CLoadableOptions
				  */
				void  dumpToTextStream(CStream	&out) const;


				/** The algorithm to use (default: icpClassic)
				  *  See http://www.mrpt.org/Scan_Matching_Algorithms for details.
				  */
				TICPAlgorithm	ICP_algorithm;

				/** The usual approach: to consider only the closest correspondence for each local point (Default to true)
				  */
				bool	onlyClosestCorrespondences;

				/** Apart of "onlyClosestCorrespondences=true", if this option is enabled only the closest correspondence for each reference point will be kept (default=false).
				  */
				bool 	onlyUniqueRobust;

				/** Maximum number of iterations to run.
				  */
				unsigned int	maxIterations;

				/** Initial threshold distance for two points to become a correspondence.
				  */
				float	thresholdDist,thresholdAng;

				/**	The scale factor for threshold everytime convergence is achieved.
				 */
				float	ALFA;

				/** The size for threshold such that iterations will stop, since it is considered precise enough.
				 */
				float	smallestThresholdDist;

				/** This is the normalization constant \f$ \sigma^2_p \f$ that is used to scale the whole 3x3 covariance.
				  *  This has a default value of \f$ (0.02)^2 \f$, that is, a 2cm sigma.
				  *  See the paper: ....
				  */
				float	covariance_varPoints;

				/** Perform a RANSAC step after the ICP convergence, to obtain a better estimation of the pose PDF.
				  */
				bool	doRANSAC;

				/** RANSAC-step options:
				  * \sa CICP::robustRigidTransformation
				  */
				unsigned int	ransac_minSetSize,ransac_maxSetSize,ransac_nSimulations;

				/** RANSAC-step options:
				  * \sa CICP::robustRigidTransformation
				  */
				float			ransac_mahalanobisDistanceThreshold;

				/** RANSAC-step option: The standard deviation in X,Y of landmarks/points which are being matched (used to compute covariances in the SoG)
				  * \sa CICP::robustRigidTransformation
				  */
				float			normalizationStd;

				/** RANSAC-step options:
				  * \sa CICP::robustRigidTransformation
				  */
				bool			ransac_fuseByCorrsMatch;

				/** RANSAC-step options:
				  * \sa CICP::robustRigidTransformation
				  */
				float			ransac_fuseMaxDiffXY, ransac_fuseMaxDiffPhi;

				/** Cauchy kernel rho, for estimating the optimal transformation covariance, in meters (default = 0.07m).
				  */
				float			kernel_rho;

				/** Whether to use kernel_rho to smooth distances, or use distances directly (default=true)
				  */
				bool			use_kernel;

				/** The size of the perturbance in x & y used to estimate the Jacobians of the square error (in LM & IKF methods, default=0.05).
				  */
				float			Axy_aprox_derivatives;

				/** The initial value of the lambda parameter in the LM method (default=1e-4).
				  */
				float			LM_initial_lambda;

			};

			TConfigParams  options; //!< The options employed by the ICP align.


			/** Constructor with the default options */
			CICP() : options() { }
			/** Constructor that directly set the ICP params from a given struct \sa options */
			CICP(const TConfigParams &icpParams) : options(icpParams) { }

			virtual ~CICP() { }	//!< Destructor


			/** The ICP algorithm return information.
			 */
			struct SLAM_IMPEXP TReturnInfo
			{
				TReturnInfo() :
					cbSize(sizeof(TReturnInfo)),
					nIterations(0),
					goodness(0),
					quality(0)
				{
				}

				/** Size in bytes of this struct: Must be set correctly before using it */
				unsigned int	cbSize;

				/** The number of executed iterations until convergence.
				 */
				unsigned short	nIterations;

				/** A goodness measure for the alignment, it is a [0,1] range indicator of percentage of correspondences.
				 */
				float			goodness;

				/** A measure of the 'quality' of the local minimum of the sqr. error found by the method.
				  * Higher values are better. Low values will be found in ill-conditioned situations (e.g. a corridor).
				  */
				float			quality;
			};

			/** An implementation of CMetricMapsAlignmentAlgorithm for the case of a point maps and a occupancy grid/point map.
			 *
			 *  This method computes the PDF of the displacement (relative pose) between
			 *   two maps: <b>the relative pose of m2 with respect to m1</b>. This pose
			 *   is returned as a PDF rather than a single value.
			 *
			 *  \note This method can be configurated with "CICP::options"
			 *  \note The output PDF is a CPosePDFGaussian if "doRANSAC=false", or a CPosePDFSOG otherwise.
			 *
			 * \param m1			[IN] The first map (CAN BE A mrpt::poses::CPointsMap derived class or a mrpt::slam::COccupancyGrid2D class)
			 * \param m2			[IN] The second map. (MUST BE A mrpt::poses::CPointsMap derived class)The pose of this map respect to m1 is to be estimated.
			 * \param initialEstimationPDF	[IN] An initial gross estimation for the displacement.
			 * \param runningTime	[OUT] A pointer to a container for obtaining the algorithm running time in seconds, or NULL if you don't need it.
			 * \param info			[OUT] A pointer to a CICP::TReturnInfo, or NULL if it isn't needed.
			 *
			 * \return A smart pointer to the output estimated pose PDF.
			 *
			 * \sa CMetricMapsAlignmentAlgorithm, CICP::options, CICP::TReturnInfo
			 */
			CPosePDFPtr AlignPDF(
					const CMetricMap		*m1,
					const CMetricMap		*m2,
					const CPosePDFGaussian	&initialEstimationPDF,
					float					*runningTime = NULL,
					void					*info = NULL );

			/** Align a pair of metric maps, aligning the full 6D pose.
			 *   The meaning of some parameters are implementation dependant,
			 *    so look at the derived classes for more details.
			 *  The goal is to find a PDF for the pose displacement between
			 *   maps, that is, <b>the pose of m2 relative to m1</b>. This pose
			 *   is returned as a PDF rather than a single value.
			 *
			 *  \note This method can be configurated with a "options" structure in the implementation classes.
			 *
			 * \param m1			[IN] The first map (MUST BE A COccupancyGridMap2D  derived class)
			 * \param m2			[IN] The second map. (MUST BE A CPointsMap derived class) The pose of this map respect to m1 is to be estimated.
			 * \param initialEstimationPDF	[IN] An initial gross estimation for the displacement.
			 * \param runningTime	[OUT] A pointer to a container for obtaining the algorithm running time in seconds, or NULL if you don't need it.
			 * \param info			[OUT] See derived classes for details, or NULL if it isn't needed.
			 *
			 * \return A smart pointer to the output estimated pose PDF.
			 * \sa CICP
			 */
			CPose3DPDFPtr Align3DPDF(
					const CMetricMap		*m1,
					const CMetricMap		*m2,
					const CPose3DPDFGaussian	&initialEstimationPDF,
					float					*runningTime = NULL,
					void					*info = NULL );


		protected:
			/** Computes:
			  *  \f[ K(x^2) = \frac{x^2}{x^2+\rho^2}  \f]
			  *  or just return the input if options.useKernel = false.
			  */
			float kernel(const float &x2, const float &rho2);

			/** The internal method implementing CICP::AlignPDF when options.ICP_algorithm is icpClassic.
			  */
			CPosePDFPtr ICP_Method_Classic(
					const CMetricMap		*m1,
					const CMetricMap		*m2,
					const CPosePDFGaussian	&initialEstimationPDF,
					TReturnInfo				&outInfo );

			/** The internal method implementing CICP::AlignPDF when options.ICP_algorithm is icpLevenbergMarquardt.
			  */
			CPosePDFPtr ICP_Method_LM(
					const CMetricMap		*m1,
					const CMetricMap		*m2,
					const CPosePDFGaussian	&initialEstimationPDF,
					TReturnInfo				&outInfo );

			/** The internal method implementing CICP::AlignPDF when options.ICP_algorithm is icpIKF.
			  */
			CPosePDFPtr ICP_Method_IKF(
					const CMetricMap		*m1,
					const CMetricMap		*m2,
					const CPosePDFGaussian	&initialEstimationPDF,
					TReturnInfo				&outInfo );

			/** The internal method implementing CICP::Align3DPDF when options.ICP_algorithm is icpClassic.
			  */
			CPose3DPDFPtr ICP3D_Method_Classic(
					const CMetricMap		*m1,
					const CMetricMap		*m2,
					const CPose3DPDFGaussian &initialEstimationPDF,
					TReturnInfo				&outInfo );


		};

	} // End of namespace
} // End of namespace

#endif