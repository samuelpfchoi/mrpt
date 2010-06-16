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
#ifndef opengl_CAngularObservationMesh_H
#define opengl_CAngularObservationMesh_H
#include <mrpt/opengl/CRenderizable.h>
#include <mrpt/opengl/CSetOfTriangles.h>
#include <mrpt/math/CMatrixTemplate.h>
#include <mrpt/math/CMatrixB.h>
#include <mrpt/utils/stl_extensions.h>
#include <mrpt/slam/CObservation2DRangeScan.h>
#include <mrpt/slam/CPointsMap.h>
#include <mrpt/opengl/CSetOfLines.h>

#include <mrpt/math/geometry.h>

namespace mrpt	{
namespace opengl	{
	using namespace mrpt::utils;
	using namespace mrpt::slam;
	using namespace mrpt::poses;

	DEFINE_SERIALIZABLE_PRE_CUSTOM_BASE_LINKAGE(CAngularObservationMesh,CRenderizable, MAPS_IMPEXP)

	/**
	  * A mesh built from a set of 2D laser scan observations.
	  * Each element of this set is a single scan through the yaw, given a specific pitch.
	  * Each scan has a CPose3D identifying the origin of the scan, which ideally is the
	  * same for every one of them.
	  */
	class MAPS_IMPEXP CAngularObservationMesh:public CRenderizable	{
		DEFINE_SERIALIZABLE(CAngularObservationMesh)
	public:
		/**
		  * Range specification type, with several uses.
		  */
		struct MAPS_IMPEXP TDoubleRange	{
		private:
			/**
			  * Range type. 
			  * If 0, it's specified by an initial and a final value, and an increment.
			  * If 1, it's specified by an initial and a final value, and a fixed size of samples.
			  * If 2, it's specified by an aperture, a fixed size of samples and a boolean variable controlling direction. This type is always zero-centered.
			  */
			char rangeType;
			/**
			  * Union type with the actual data.
			  * \sa rangeType
			  */
			union rd	{
				struct	{
					double initial;
					double final;
					double increment;
				}	mode0;
				struct	{
					double initial;
					double final;
					size_t amount;
				}	mode1;
				struct	{
					double aperture;
					size_t amount;
					bool negToPos;
				}	mode2;
			}	rangeData;
			/**
			  * Constructor from initial value, final value and range.
			  */
			TDoubleRange(double a,double b,double c):rangeType(0)	{
				rangeData.mode0.initial=a;
				rangeData.mode0.final=b;
				rangeData.mode0.increment=c;
			}
			/**
			  * Constructor from initial value, final value and amount of samples.
			  */
			TDoubleRange(double a,double b,size_t c):rangeType(1)	{
				rangeData.mode1.initial=a;
				rangeData.mode1.final=b;
				rangeData.mode1.amount=c;
			}
			/**
			  * Constructor from aperture, amount of samples and scan direction.
			  */
			TDoubleRange(double a,size_t b,bool c):rangeType(2)	{
				rangeData.mode2.aperture=a;
				rangeData.mode2.amount=b;
				rangeData.mode2.negToPos=c;
			}
		public:
			/**
			  * Creates a range of values from the initial value, the final value and the increment.
			  * \throw std::logic_error if the increment is zero.
			  */
			inline static TDoubleRange CreateFromIncrement(double initial,double final,double increment)	{
				if (increment==0) throw std::logic_error("Invalid increment value.");
				return TDoubleRange(initial,final,increment);
			}
			/**
			  * Creates a range of values from the initial value, the final value and a desired amount of samples.
			  */
			inline static TDoubleRange CreateFromAmount(double initial,double final,size_t amount)	{
				return TDoubleRange(initial,final,amount);
			}
			/**
			  * Creates a zero-centered range of values from an aperture, an amount of samples and a direction.
			  */
			inline static TDoubleRange CreateFromAperture(double aperture,size_t amount,bool negToPos=true)	{
				return TDoubleRange(aperture,amount,negToPos);
			}
			/**
			  * Returns the total aperture of the range.
			  * \throw std::logic_error on invalid range type.
			  */
			inline double aperture() const	{
				switch (rangeType)	{
					case 0:return (sign(rangeData.mode0.increment)==sign(rangeData.mode0.final-rangeData.mode0.initial))?fabs(rangeData.mode0.final-rangeData.mode0.initial):0;
					case 1:return rangeData.mode1.final-rangeData.mode1.initial;
					case 2:return rangeData.mode2.aperture;
					default:throw std::logic_error("Unknown range type.");
				}
			}
			/**
			  * Returns the first value of the range.
			  * \throw std::logic_error on invalid range type.
			  */
			inline double initialValue() const	{
				switch (rangeType)	{
					case 0:
					case 1:return rangeData.mode0.initial;
					case 2:return rangeData.mode2.negToPos?-rangeData.mode2.aperture/2:rangeData.mode2.aperture/2;
					default:throw std::logic_error("Unknown range type.");
				}
			}
			/**
			  * Returns the last value of the range.
			  * \throw std::logic_error on invalid range type.
			  */
			inline double finalValue() const	{
				switch (rangeType)	{
					case 0:return (sign(rangeData.mode0.increment)==sign(rangeData.mode0.final-rangeData.mode0.initial))?rangeData.mode0.final:rangeData.mode0.initial;
					case 1:return rangeData.mode1.final;
					case 2:return rangeData.mode2.negToPos?rangeData.mode2.aperture/2:-rangeData.mode2.aperture/2;
					default:throw std::logic_error("Unknown range type.");
				}
			}
			/**
			  * Returns the increment between two consecutive values of the range.
			  * \throw std::logic_error on invalid range type.
			  */
			inline double increment() const	{
				switch (rangeType)	{
					case 0:return rangeData.mode0.increment;
					case 1:return (rangeData.mode1.final-rangeData.mode1.initial)/static_cast<double>(rangeData.mode1.amount-1);
					case 2:return rangeData.mode2.negToPos?rangeData.mode2.aperture/static_cast<double>(rangeData.mode2.amount-1):-rangeData.mode2.aperture/static_cast<double>(rangeData.mode2.amount-1);
					default:throw std::logic_error("Unknown range type.");
				}
			}
			/**
			  * Returns the total amount of values in this range.
			  * \throw std::logic_error on invalid range type.
			  */
			inline size_t amount() const	{
				switch (rangeType)	{
					case 0:return (sign(rangeData.mode0.increment)==sign(rangeData.mode0.final-rangeData.mode0.initial))?1+static_cast<size_t>(ceil((rangeData.mode0.final-rangeData.mode0.initial)/rangeData.mode0.increment)):1;
					case 1:return rangeData.mode1.amount;
					case 2:return rangeData.mode2.amount;
					default:throw std::logic_error("Unknown range type.");
				}
			}
			/**
			  * Gets a vector with every value in the range.
			  * \throw std::logic_error on invalid range type.
			  */
			void values(vector_double &vals) const;
			/**
			  * Returns the direction of the scan. True if the increment is positive, false otherwise.
			  * \throw std::logic_error on invalid range type.
			  */
			inline bool negToPos() const	{
				switch (rangeType)	{
					case 0:return sign(rangeData.mode0.increment)>0;
					case 1:return sign(rangeData.mode1.final-rangeData.mode1.initial)>0;
					case 2:return rangeData.mode2.negToPos;
					default:throw std::logic_error("Unknown range type.");
				}
			}
		};
	protected:
		/**
		  * Updates the mesh, if needed. It's a const method, but modifies mutable content.
		  */
		void updateMesh() const;
		/**
		  * Empty destructor.
		  */
		virtual ~CAngularObservationMesh()	{}
		/**
		  * Actual set of triangles to be displayed.
		  */
		mutable std::vector<CSetOfTriangles::TTriangle> triangles;
		/**
		  * Internal method to add a triangle to the mutable mesh.
		  */
		void addTriangle(const TPoint3D &p1,const TPoint3D &p2,const TPoint3D &p3) const;
		/**
		  * Whether the mesh will be displayed wireframe or solid.
		  */
		bool mWireframe;
		/**
		  * Mutable variable which controls if the object has suffered any change since last time the mesh was updated.
		  */
		mutable bool meshUpToDate;
		/**
		  * Whether the object may present transparencies or not.
		  */
		bool mEnableTransparency;
		/**
		  * Mutable object with the mesh's points.
		  */
		mutable mrpt::math::CMatrixTemplate<TPoint3D> actualMesh;
		/**
		  * Scan validity matrix.
		  */
		mutable mrpt::math::CMatrixB validityMatrix;
		/**
		  * Observation pitch range. When containing exactly two elements, they represent the bounds.
		  */
		vector<double> pitchBounds;
		/**
		  * Actual scan set which is used to generate the mesh.
		  */
		vector<CObservation2DRangeScan> scanSet;
		/**
		  * Basic constructor.
		  */
		CAngularObservationMesh():mWireframe(true),meshUpToDate(false),mEnableTransparency(true),actualMesh(0,0),validityMatrix(0,0),pitchBounds(),scanSet()	{}
	public:
		/**
		  * Returns whether the object is configured as wireframe or solid.
		  */
		inline bool isWireframe() const	{
			return mWireframe;
		}
		/**
		  * Sets the display mode for the object. True=wireframe, False=solid.
		  */
		inline void setWireframe(bool enabled=true)	{
			mWireframe=enabled;
		}
		/**
		  * Returns whether the object may be transparent or not.
		  */
		inline bool isTransparencyEnabled() const	{
			return mEnableTransparency;
		}
		/**
		  * Enables or disables transparencies.
		  */
		inline void enableTransparency(bool enabled=true)	{
			mEnableTransparency=enabled;
		}
		/**
		  * Renderizes the object.
		  * \sa mrpt::opengl::CRenderizable
		  */
		virtual void render() const;
		/**
		  * Traces a ray to the object, returning the distance to a given pose through its X axis.
		  * \sa mrpt::opengl::CRenderizable,trace2DSetOfRays,trace1DSetOfRays
		  */
		virtual bool traceRay(const mrpt::poses::CPose3D &o,double &dist) const;
		/**
		  * Sets the pitch bounds for this range.
		  */
		void setPitchBounds(const double initial,const double final);
		/**
		  * Sets the pitch bounds for this range.
		  */
		void setPitchBounds(const std::vector<double> bounds);
		/**
		  * Gets the initial and final pitch bounds for this range.
		  */
		void getPitchBounds(double &initial,double &final) const;
		/**
		  * Gets the pitch bounds for this range.
		  */
		void getPitchBounds(std::vector<double> &bounds) const;
		/**
		  * Gets the scan set.
		  */
		void getScanSet(std::vector<CObservation2DRangeScan> &scans) const;
		/**
		  * Sets the scan set.
		  */
		bool setScanSet(const std::vector<CObservation2DRangeScan> &scans);
		/**
		  * Gets the mesh as a set of triangles, for displaying them.
		  * \sa generateSetOfTriangles(std::vector<TPolygon3D> &),mrpt::opengl::CSetOfTriangles,mrpt::opengl::CSetOfTriangles::TTriangle
		  */
		void generateSetOfTriangles(CSetOfTrianglesPtr &res) const;
		/**
		  * Returns the scanned points as a 3D point cloud. The target pointmap must be passed as a pointer to allow the use of any derived class.
		  */
		void generatePointCloud(CPointsMap *out_map) const;
		/**
		  * Gets a set of lines containing the traced rays, for displaying them.
		  * \sa getUntracedRays,mrpt::opengl::CSetOfLines
		  */
		void getTracedRays(CSetOfLinesPtr &res) const;
		/**
		  * Gets a set of lines containing the untraced rays, up to a specified distance, for displaying them.
		  * \sa getTracedRays,mrpt::opengl::CSetOfLines
		  */
		void getUntracedRays(CSetOfLinesPtr &res,double dist) const;
		/**
		  * Gets the mesh as a set of polygons, to work with them.
		  * \sa generateSetOfTriangles(mrpt::opengl::CSetOfTriangles &)
		  */
		void generateSetOfTriangles(std::vector<TPolygon3D> &res) const;
		/**
		  * Retrieves the full mesh, along with the validity matrix.
		  */
		void getActualMesh(mrpt::math::CMatrixTemplate<mrpt::math::TPoint3D> &pts,mrpt::math::CMatrixBool &validity) const	{
			if (!meshUpToDate) updateMesh();
			pts=actualMesh;
			validity=validityMatrix;
		}
	private:
		/**
		  * Internal functor class to trace a ray.
		  */
		template<class T> class FTrace1D	{
		protected:
			const CPose3D &initial;
			const T &e;
			vector<double> &values;
			std::vector<char> &valid;
		public:
			FTrace1D(const T &s,const CPose3D &p,vector_double &v,std::vector<char> &v2):initial(p),e(s),values(v),valid(v2)	{}
			void operator()(double yaw)	{
				double dist;
				const CPose3D pNew=initial+CPose3D(0.0,0.0,0.0,yaw,0.0,0.0);
				if (e->traceRay(pNew,dist))	{
					values.push_back(dist);
					valid.push_back(1);
				}	else	{
					values.push_back(0);
					valid.push_back(0);
				}
			}
		};
		/**
		  * Internal functor class to trace a set of rays.
		  */
		template<class T> class FTrace2D	{
		protected:
			const T &e;
			const CPose3D &initial;
			CAngularObservationMeshPtr &caom;
			const CAngularObservationMesh::TDoubleRange &yaws;
			std::vector<CObservation2DRangeScan> &vObs;
			const CPose3D &pBase;
		public:
			FTrace2D(const T &s,const CPose3D &p,CAngularObservationMeshPtr &om,const CAngularObservationMesh::TDoubleRange &y,std::vector<CObservation2DRangeScan> &obs,const CPose3D &b):e(s),initial(p),caom(om),yaws(y),vObs(obs),pBase(b)	{}
			void operator()(double pitch)	{
				vector_double yValues;
				yaws.values(yValues);
				CObservation2DRangeScan o=CObservation2DRangeScan();
				const CPose3D pNew=initial+CPose3D(0,0,0,0,pitch,0);
				vector_double values;
				std::vector<char> valid;
				size_t nY=yValues.size();
				values.reserve(nY);
				valid.reserve(nY);
				for_each(yValues.begin(),yValues.end(),FTrace1D<T>(e,pNew,values,valid));
				o.aperture=yaws.aperture();
				o.rightToLeft=yaws.negToPos();
				o.maxRange=10000;
				o.sensorPose=pNew;
				o.deltaPitch=0;
				mrpt::utils::copy_container_typecasting(values,o.scan);
				o.validRange=valid;
				vObs.push_back(o);
			}
		};
	public:
		/**
		  * 2D ray tracing (will generate a 3D mesh). Given an object and two ranges, realizes a scan from the initial pose and stores it in a CAngularObservationMesh object.
		  * The objective may be a COpenGLScene, a CRenderizable or any children of its.
		  * \sa mrpt::opengl::CRenderizable,mrpt::opengl::COpenGLScene.
		  */
		template<class T> static void trace2DSetOfRays(const T &e,const CPose3D &initial,CAngularObservationMeshPtr &caom,const TDoubleRange &pitchs,const TDoubleRange &yaws)	{
			vector_double pValues;
			pitchs.values(pValues);
			std::vector<CObservation2DRangeScan> vObs;
			vObs.reserve(pValues.size());
			for_each(pValues.begin(),pValues.end(),FTrace2D<T>(e,initial,caom,yaws,vObs,initial));
			caom->mWireframe=false;
			caom->mEnableTransparency=false;
			caom->setPitchBounds(pValues);
			caom->setScanSet(vObs);
		}
		/**
		  * 2D ray tracing (will generate a vectorial mesh inside a plane). Given an object and a range, realizes a scan from the initial pose and stores it in a CObservation2DRangeScan object.
		  * The objective may be a COpenGLScene, a CRenderizable or any children of its.
		  * \sa mrpt::opengl::CRenderizable,mrpt::opengl::COpenGLScene.
		  */
		template<class T> static void trace1DSetOfRays(const T &e,const CPose3D &initial,CObservation2DRangeScan &obs,const TDoubleRange &yaws)	{
			vector_double yValues;
			yaws.values(yValues);
			vector_double scanValues;
			std::vector<char> valid;
			size_t nV=yaws.amount();
			scanValues.reserve(nV);
			valid.reserve(nV);
			for_each(yValues.begin(),yValues.end(),FTrace1D<T>(e,initial,scanValues,valid));
			obs.aperture=yaws.aperture();
			obs.rightToLeft=yaws.negToPos();
			obs.maxRange=10000;
			obs.sensorPose=initial;
			obs.deltaPitch=0;
			obs.scan=scanValues;
			obs.validRange=valid;
		}
	};
}
}
#endif