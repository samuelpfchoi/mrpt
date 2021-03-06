/* +---------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)               |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2014, Individual contributors, see AUTHORS file        |
   | See: http://www.mrpt.org/Authors - All rights reserved.                   |
   | Released under BSD License. See details in http://www.mrpt.org/License    |
   +---------------------------------------------------------------------------+ */

#include <mrpt/base.h>  // Precompiled headers 


#include <mrpt/math/CMatrix.h>

using namespace mrpt;
using namespace mrpt::math;
using namespace mrpt::utils;


// This must be added to any CSerializable class implementation file.
IMPLEMENTS_SERIALIZABLE(CMatrix, CSerializable, mrpt::math)

/*---------------------------------------------------------------
						writeToStream
 ---------------------------------------------------------------*/
void  CMatrix::writeToStream(CStream &out, int *out_Version) const
{
	if (out_Version)
		*out_Version = 0;
	else
	{
		// First, write the number of rows and columns:
		out << (uint32_t)rows() << (uint32_t)cols();

		if (rows()>0 && cols()>0)
			for (Index i=0;i<rows();i++)
				out.WriteBufferFixEndianness<Scalar>(&coeff(i,0),cols());
	}

}

/*---------------------------------------------------------------
						readFromStream
 ---------------------------------------------------------------*/
void  CMatrix::readFromStream(CStream &in, int version)
{
	switch(version)
	{
	case 0:
		{
			uint32_t nRows,nCols;

			// First, write the number of rows and columns:
			in >> nRows >> nCols;

			setSize(nRows,nCols);

			if (nRows>0 && nCols>0)
				for (Index i=0;i<rows();i++)
					in.ReadBufferFixEndianness<Scalar>(&coeffRef(i,0),nCols);
		} break;
	default:
		MRPT_THROW_UNKNOWN_SERIALIZATION_VERSION(version)

	};
}

