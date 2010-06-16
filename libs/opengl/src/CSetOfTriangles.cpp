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

#include <mrpt/opengl.h>  // Precompiled header


#include <mrpt/opengl/CSetOfTriangles.h>
#include "opengl_internals.h"
#include <mrpt/math/CMatrixTemplateNumeric.h>
#include <mrpt/poses/CPose3D.h>
#include <mrpt/poses/CPoint3D.h>

using namespace mrpt;
using namespace mrpt::opengl;
using namespace mrpt::poses;
using namespace mrpt::utils;
using namespace mrpt::math;
using namespace std;

IMPLEMENTS_SERIALIZABLE( CSetOfTriangles, CRenderizable, mrpt::opengl )


/*---------------------------------------------------------------
							render
  ---------------------------------------------------------------*/
void   CSetOfTriangles::render() const
{
#if MRPT_HAS_OPENGL_GLUT

	if (m_enableTransparency)
	{
		//glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
    else
    {
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
    }

	/*glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);*/
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);

	vector<TTriangle>::const_iterator	it;

	glBegin(GL_TRIANGLES);

	for (it=m_triangles.begin();it!=m_triangles.end();it++)
	{
        // Compute the normal vector:
        // ---------------------------------
        float	ax= it->x[1] - it->x[0];
        float	ay= it->y[1] - it->y[0];
        float	az= it->z[1] - it->z[0];

        float	bx= it->x[2] - it->x[0];
        float	by= it->y[2] - it->y[0];
        float	bz= it->z[2] - it->z[0];

        glNormal3f(ay*bz-az*by,-ax*bz+az*bx,ax*by-ay*bx);

		glColor4f( it->r[0],it->g[0],it->b[0],it->a[0] );
		glVertex3f(it->x[0],it->y[0],it->z[0]);

		glColor4f( it->r[1],it->g[1],it->b[1],it->a[1] );
		glVertex3f(it->x[1],it->y[1],it->z[1]);

		glColor4f( it->r[2],it->g[2],it->b[2],it->a[2] );
		glVertex3f(it->x[2],it->y[2],it->z[2]);
	}

	glEnd();

	glDisable(GL_BLEND);
//	glDisable(GL_LIGHTING);
#endif
}

/*---------------------------------------------------------------
   Implements the writing to a CStream capability of
     CSerializable objects
  ---------------------------------------------------------------*/
void  CSetOfTriangles::writeToStream(CStream &out,int *version) const
{

	if (version)
		*version = 1;
	else
	{
		writeToStreamRender(out);
		uint32_t	n = (uint32_t)m_triangles.size();
		out << n;
		if (n)
			out.WriteBuffer( &m_triangles[0], n*sizeof(TTriangle) );

		// Version 1:
		out << m_enableTransparency;
	}
}

/*---------------------------------------------------------------
	Implements the reading from a CStream capability of
		CSerializable objects
  ---------------------------------------------------------------*/
void  CSetOfTriangles::readFromStream(CStream &in,int version)
{
	switch(version)
	{
	case 0:
	case 1:
		{
			readFromStreamRender(in);
			uint32_t	n;
			in >> n;
			m_triangles.assign(n,TTriangle());
			if (n)
				in.ReadBuffer( &m_triangles[0], n*sizeof(TTriangle) );

			if (version>=1)
					in >> m_enableTransparency;
			else	m_enableTransparency = true;
		} break;
	default:
		MRPT_THROW_UNKNOWN_SERIALIZATION_VERSION(version)

	};
	polygonsUpToDate=false;
}

bool CSetOfTriangles::traceRay(const mrpt::poses::CPose3D &o,double &dist) const	{
	if (!polygonsUpToDate) updatePolygons();
	return mrpt::math::traceRay(tmpPolygons,o-CPose3D(m_x,m_y,m_z,DEG2RAD(m_yaw),DEG2RAD(m_pitch),DEG2RAD(m_roll)),dist);
}

//Helper function. Given two 2D points (y1,z1) and (y2,z2), returns three coefficients A, B and C so that both points
//verify Ay+Bz+C=0
//returns true if the coefficients have actually been calculated
/*
inline bool lineCoefs(const float &y1,const float &z1,const float &y2,const float &z2,float coefs[3])	{
	if ((y1==y2)&(z1==z2)) return false;	//Both points are the same
	if (y1==y2)	{
		//Equation is y-y1=0
		coefs[0]=1;
		coefs[1]=0;
		coefs[2]=-y1;
		return true;
	}	else	{
		//Equation is:
		// z1 - z2        /z2 - z1       \ .
		// -------y + z + |-------y1 - z1| = 0
		// y2 - y1        \y2 - y1       /
		coefs[0]=(z1-z2)/(y2-y1);
		coefs[1]=1;
		coefs[2]=((z2-z1)/(y2-y1))*y1-z1;
		return true;
	}
}
*/
/*
bool CSetOfTriangles::traceRayTriangle(const mrpt::poses::CPose3D &transf,double &dist,const float xb[3],const float yb[3],const float zb[3])	{
	//Computation of the actual coordinates in the beam's system.
	float x[3];
	float y[3];
	float z[3];
	for (int i=0;i<3;i++) transf.composePoint(xb[i],yb[i],zb[i],x[i],y[i],z[i]);

	//If the triangle is parallel to the beam, no collision is posible.
	//The triangle is parallel to the beam if the projection of the triangle to the YZ plane results in a line.
	float lCoefs[3];
	if (!lineCoefs(y[0],z[0],y[1],z[1],lCoefs)) return false;
	else if (lCoefs[0]*y[2]+lCoefs[1]*z[2]+lCoefs[2]==0) return false;
	//Basic sign check
	if (x[0]<0&&x[1]<0&&x[2]<0) return false;
	if (y[0]<0&&y[1]<0&&y[2]<0) return false;
	if (z[0]<0&&z[1]<0&&z[2]<0) return false;
	if (y[0]>0&&y[1]>0&&y[2]>0) return false;
	if (z[0]>0&&z[1]>0&&z[2]>0) return false;
	//Let M be the following matrix:
	//  /p1\ /x1 y1 z1\ .
	//M=|p2|=|x2 y2 z2|
	//  \p3/ \x3 y3 z3/
	//If M has rank 3, then (p1,p2,p3) conform a plane which does not contain the origin (0,0,0).
	//If M has rank 2, then (p1,p2,p3) may conform either a line or a plane which contains the origin.
	//If M has a lesser rank, then (p1,p2,p3) do not conform a plane.
	//Let N be the following matrix:
	//N=/p2-p1\ = /x1 y1 z1\ .
	//  \p3-p1/   \x3 y3 z3/
	//Given that the rank of M is 2, if the rank of N is still 2 then (p1,p2,p3) conform a plane; either, a line.
	float mat[9];
	for (int i=0;i<3;i++)	{
		mat[3*i]=x[i];
		mat[3*i+1]=y[i];
		mat[3*i+2]=z[i];
	}
	CMatrixTemplateNumeric<float> M=CMatrixTemplateNumeric<float>(3,3,mat);
	float d2=0;
	float mat2[6];
	switch (M.rank())	{
		case 3:
			//M's rank is 3, so the triangle is inside a plane which doesn't pass through (0,0,0).
			//This plane's equation is Ax+By+Cz+1=0. Since the point we're searching for verifies y=0 and z=0, we
			//only need to compute A (x=-1/A). We do this using Cramer's method.
			for (int i=0;i<9;i+=3) mat[i]=1;
			d2=(CMatrixTemplateNumeric<float>(3,3,mat)).det();
			if (d2==0) return false;
			else dist=M.det()/d2;
			break;
		case 2:
			//if N's rank is 2, the triangle is inside a plane containing (0,0,0).
			//Otherwise, (p1,p2,p3) don't conform a plane.
			for (int i=0;i<2;i++)	{
				mat2[3*i]=x[i+1]-x[0];
				mat2[3*i+1]=y[i+1]-y[0];
				mat2[3*i+2]=z[i+1]-z[0];
			}
			if (CMatrixTemplateNumeric<float>(2,3,mat2).rank()==2) dist=0;
			else return false;
			break;
		default:
			return false;
	}
	if (dist<0) return false;
	//We've already determined the collision point between the beam and the plane, but we need to check if this
	//point is actually inside the triangle. We do this by projecting the scene into a <x=constant> plane, so
	//that the triangle is defined by three 2D lines and the beam is the point (y,z)=(0,0).

	//For each pair of points, we compute the line that they conform, and then we check if the other point's
	//sign in that line's equation equals that of the origin (that is, both points are on the same side of the line).
	//If this holds for each one of the three possible combinations, then the point is inside the triangle.
	//Furthermore, if any of the three equations verify f(0,0)=0, then the point is in the verge of the line, which
	//is considered as being inside. Note, whichever is the case, that f(0,0)=lCoefs[2].

	//lineCoefs already contains the coefficients for the first line.
	if (lCoefs[2]==0) return true;
	else if (((lCoefs[0]*y[2]+lCoefs[1]*z[2]+lCoefs[2])>0)!=(lCoefs[2]>0)) return false;
	lineCoefs(y[0],z[0],y[2],z[2],lCoefs);
	if (lCoefs[2]==0) return true;
	else if (((lCoefs[0]*y[1]+lCoefs[1]*z[1]+lCoefs[2])>0)!=(lCoefs[2]>0)) return false;
	lineCoefs(y[1],z[1],y[2],z[2],lCoefs);
	if (lCoefs[2]==0) return true;
	else return ((lCoefs[0]*y[0]+lCoefs[1]*z[0]+lCoefs[2])>0)==(lCoefs[2]>0);
}
*/

void CSetOfTriangles::setColor(double R,double G,double B,double A)	{
	m_color_R=R;
	m_color_G=G;
	m_color_B=B;
	m_color_A=A;
	for (std::vector<TTriangle>::iterator it=m_triangles.begin();it!=m_triangles.end();++it) for (size_t i=0;i<3;i++)	{
		it->r[i]=R;
		it->g[i]=G;
		it->b[i]=B;
		it->a[i]=A;
	}
}

void CSetOfTriangles::setColor(const mrpt::utils::TColorf &c)	{
	m_color_R=c.R;
	m_color_G=c.G;
	m_color_B=c.B;
	m_color_A=c.A;
	for (std::vector<TTriangle>::iterator it=m_triangles.begin();it!=m_triangles.end();++it) for (size_t i=0;i<3;i++)	{
		it->r[i]=c.R;
		it->g[i]=c.G;
		it->b[i]=c.B;
		it->a[i]=c.A;
	}
}

void CSetOfTriangles::setColorR(const double r)	{
	m_color_R=r;
	for (std::vector<TTriangle>::iterator it=m_triangles.begin();it!=m_triangles.end();++it) for (size_t i=0;i<3;i++) it->r[i]=r;
}

void CSetOfTriangles::setColorG(const double g)	{
	m_color_G=g;
	for (std::vector<TTriangle>::iterator it=m_triangles.begin();it!=m_triangles.end();++it) for (size_t i=0;i<3;i++) it->g[i]=g;
}

void CSetOfTriangles::setColorB(const double b)	{
	m_color_B=b;
	for (std::vector<TTriangle>::iterator it=m_triangles.begin();it!=m_triangles.end();++it) for (size_t i=0;i<3;i++) it->b[i]=b;
}

void CSetOfTriangles::setColorA(const double a)	{
	m_color_A=a;
	for (std::vector<TTriangle>::iterator it=m_triangles.begin();it!=m_triangles.end();++it) for (size_t i=0;i<3;i++) it->a[i]=a;
}

void CSetOfTriangles::getPolygons(std::vector<mrpt::math::TPolygon3D> &polys) const	{
	if (!polygonsUpToDate) updatePolygons();
	size_t N=tmpPolygons.size();
	for (size_t i=0;i<N;i++) polys[i]=tmpPolygons[i].poly;
}

void CSetOfTriangles::updatePolygons() const	{
	TPolygon3D tmp(3);
	size_t N=m_triangles.size();
	tmpPolygons.resize(N);
	for (size_t i=0;i<N;i++) for (size_t j=0;j<3;j++)	{
		const TTriangle &t=m_triangles[i];
		tmp[j].x=t.x[j];
		tmp[j].y=t.y[j];
		tmp[j].z=t.z[j];
		tmpPolygons[i]=tmp;
	}
	polygonsUpToDate=true;
}