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

#include <mrpt/vision.h>  // Precompiled headers

#include <mrpt/vision/CVideoFileWriter.h>

#if MRPT_HAS_OPENCV
	// Include the OPENCV libraries:
	#define CV_NO_CVV_IMAGE   // Avoid CImage name crash

#	if MRPT_OPENCV_VERSION_NUM>=0x211
#		include <opencv2/core/core.hpp>
#		include <opencv2/highgui/highgui.hpp>
#		include <opencv2/imgproc/imgproc.hpp>
#		include <opencv2/imgproc/imgproc_c.h>
#	else
#		include <cv.h>
#		include <highgui.h>
#	endif

	#ifdef CImage	// For old OpenCV versions (<=1.0.0)
	#undef CImage
	#endif
#endif

#define M_WRITER (const_cast<CvVideoWriter*>( static_cast<const CvVideoWriter*>(m_video.get())) )
#define M_WRITER_PTR (reinterpret_cast<CvVideoWriter**>(m_video.getPtrToPtr()))


using namespace mrpt;
using namespace mrpt::vision;
using namespace mrpt::system;
using namespace mrpt::utils;

/* ----------------------------------------------------------
						Ctor
   ---------------------------------------------------------- */
CVideoFileWriter::CVideoFileWriter() : m_video(), m_img_size(0,0)
{
}

/* ----------------------------------------------------------
						Dtor
   ---------------------------------------------------------- */
CVideoFileWriter::~CVideoFileWriter()
{
	close();
}

/* ----------------------------------------------------------
						open
   ---------------------------------------------------------- */
bool CVideoFileWriter::open(
	const std::string &out_file,
	double fps,
	const mrpt::vision::TImageSize & frameSize,
	const std::string &fourcc,
	bool isColor )
{
#if MRPT_HAS_OPENCV
	close();

	int cc;

	if (fourcc.empty())
	{
#if MRPT_OPENCV_VERSION_NUM<=0x100
		cc = 0; // Default
#else
	#ifdef MRPT_OS_WINDOWS
		cc = CV_FOURCC_DEFAULT; // Default CV_FOURCC_PROMPT;
	#else
		cc = CV_FOURCC_DEFAULT; // Default
	#endif
#endif
	}
	else
	if (fourcc.size()==4)
	{
		cc = CV_FOURCC( fourcc[0],fourcc[1],fourcc[2],fourcc[3] );
	}
	else
	{
		std::cerr << "[CVideoFileWriter::open] fourcc string must be four character length or empty for default." << std::endl;
		return false;
	}

	m_video = cvCreateVideoWriter(out_file.c_str(),cc,fps,cvSize(frameSize.x,frameSize.y),isColor ? 1:0);

	m_img_size = frameSize;

	return m_video.get() != NULL;
#else
	std::cerr << "[CVideoFileWriter::open] ERROR: MRPT was compiled without OpenCV support "  << std::endl;
	return false;
#endif
}


/* ----------------------------------------------------------
						close
   ---------------------------------------------------------- */
void CVideoFileWriter::close()
{
#if MRPT_HAS_OPENCV
	if (!M_WRITER) return;
	cvReleaseVideoWriter( M_WRITER_PTR );
	*M_WRITER_PTR = NULL;
#endif
}


/* ----------------------------------------------------------
						operator <<
   ---------------------------------------------------------- */
const CVideoFileWriter& CVideoFileWriter::operator << (const mrpt::utils::CImage& img) const
{
	if (!m_video.get())
		THROW_EXCEPTION("Call open first")

	if ((size_t)m_img_size.x!=img.getWidth() || (size_t)m_img_size.y!=img.getHeight())
		THROW_EXCEPTION(format("Video frame size is %ix%i but image is %ux%u", m_img_size.x,m_img_size.y,(unsigned)img.getWidth(),(unsigned)img.getHeight() ));

#if MRPT_HAS_OPENCV
	if (! cvWriteFrame( M_WRITER, (IplImage*)img.getAsIplImage() ) )
		THROW_EXCEPTION("Error writing image frame to video file")
#endif
	return *this;
}

/* ----------------------------------------------------------
						writeImage
   ---------------------------------------------------------- */
bool CVideoFileWriter::writeImage(const mrpt::utils::CImage& img) const
{
	if (!m_video.get())
		return false;

	if ((size_t)m_img_size.x!=img.getWidth() || (size_t)m_img_size.y!=img.getHeight())
	{
		std::cout << format("[CVideoFileWriter::writeImage] Error: video frame size is %ix%i but image is %ux%u", m_img_size.x,m_img_size.y,(unsigned)img.getWidth(),(unsigned)img.getHeight() ) << std::endl;
		return false;
	}

#if MRPT_HAS_OPENCV
	return 0!= cvWriteFrame( M_WRITER, (IplImage*)img.getAsIplImage() );
#else
	return false;
#endif
}
