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



#include <mrpt/math/CPolygon.h>
#include <mrpt/math/utils.h>

using namespace mrpt;
using namespace mrpt::math;
using namespace mrpt::utils;
using namespace std;

// This must be added to any CSerializable class implementation file.
IMPLEMENTS_SERIALIZABLE(CPolygon, CSerializable, mrpt::math)


/*---------------------------------------------------------------
	Implements the writing to a CStream capability of
		CSerializable objects
  ---------------------------------------------------------------*/
void  CPolygon::writeToStream(CStream &out, int *version) const
{
	if (version)
		*version = 2;
	else
	{
		// The number of vertexs:
		const uint32_t n = (uint32_t)TPolygon2D::size();

		// Size:
		out << n;

		// Vertices:
		if (n)
			out.WriteBuffer( &TPolygon2D::operator[](0), n*sizeof(TPolygon2D::operator[](0)));
	}

}

/*---------------------------------------------------------------
	Implements the reading from a CStream capability of
		CSerializable objects
  ---------------------------------------------------------------*/
void  CPolygon::readFromStream(CStream &in, int version)
{
	switch(version)
	{
	case 0:  // floats
		{
			// The number of vertexs:
			uint32_t	i,n;
			float		f;

			// All elemental typed variables:
			in >> n;
			in >> f; //max_x=f;
			in >> f; //max_y=f;
			in >> f; //min_x=f;
			in >> f; //min_y=f;
			in >> f; //cx=f;
			in >> f; //cy=f;

			TPolygon2D::resize(n);

			// The vertexs arrays:
			for (i=0;i<n;i++) { in >> f; TPolygon2D::operator[](i).x = f;	}
			for (i=0;i<n;i++) { in >> f; TPolygon2D::operator[](i).y = f;	}
		} break;

	case 1:
		{
			// The number of vertexs:
			uint32_t	n;
			double dumm;

			// All elemental typed variables:
			in >> n >> dumm >> dumm >> dumm >> dumm >> dumm >> dumm;
			        //max_x >> max_y >> min_x >> min_y >> cx >> cy;

			TPolygon2D::resize(n);

			// The vertexs arrays:
			for (size_t i=0;i<n;i++) { in >> dumm; TPolygon2D::operator[](i).x = dumm;	}
			for (size_t i=0;i<n;i++) { in >> dumm; TPolygon2D::operator[](i).y = dumm;	}
		} break;
	case 2:
		{
			// The number of vertexs:
			uint32_t	n;

			// All elemental typed variables:
			in >> n;

			TPolygon2D::resize(n);

			// The vertexs arrays:
			if (n>0)
				in.ReadBuffer( (void*)&TPolygon2D::operator[](0), n*sizeof(TPolygon2D::operator[](0)));
		} break;
	default:
		MRPT_THROW_UNKNOWN_SERIALIZATION_VERSION(version)

	};

}

/*---------------------------------------------------------------
	Set all vertices at once, not to be used normally.
  ---------------------------------------------------------------*/
void  CPolygon::setAllVertices( const std::vector<double> &x, const std::vector<double> &y )
{
	ASSERT_(x.size()==y.size() && !x.empty());
	setAllVertices( x.size() ,&x[0],&y[0] );
}

/*---------------------------------------------------------------
	Set all vertices at once, not to be used normally.
  ---------------------------------------------------------------*/
void  CPolygon::setAllVertices( size_t nVertices, const double *xs, const double *ys )
{
	// Resize:
	TPolygon2D::resize(nVertices);
	for (size_t i=0;i<nVertices;i++)
	{
		TPolygon2D::operator[](i).x = xs[i];
		TPolygon2D::operator[](i).y = ys[i];
	}
}


/*---------------------------------------------------------------
	Set all vertices at once, not to be used normally.
  ---------------------------------------------------------------*/
void  CPolygon::setAllVertices( size_t nVertices, const float *xs, const float *ys )
{
	// Resize:
	TPolygon2D::resize(nVertices);
	for (size_t i=0;i<nVertices;i++)
	{
		TPolygon2D::operator[](i).x = xs[i];
		TPolygon2D::operator[](i).y = ys[i];
	}
}

void  CPolygon::getAllVertices( std::vector<double> &x, std::vector<double> &y ) const
{
	// Resize:
	const size_t n = TPolygon2D::size();
	x.resize(n);
	y.resize(n);
	for (size_t i=0;i<n;i++)
	{
		x[i] = TPolygon2D::operator[](i).x;
		y[i] = TPolygon2D::operator[](i).y;
	}
}