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

#ifndef opengl_CText_H
#define opengl_CText_H

#include <mrpt/opengl/CRenderizable.h>

namespace mrpt
{
	namespace opengl
	{
		class OPENGL_IMPEXP CText;

		// This must be added to any CSerializable derived class:
		DEFINE_SERIALIZABLE_PRE_CUSTOM_BASE_LINKAGE( CText, CRenderizable, OPENGL_IMPEXP )

		/** A 2D text (bitmap rendering): it always "faces the observer" despite it's at some 3D location.
		  *  Use setString and setFont to change the text displayed by this object.
		  *
		  *  \note All texts appear with the font GLUT_BITMAP_TIMES_ROMAN_10 for now (i.e. setFont is ignored)
		  *  \sa opengl::COpenGLScene
		  */
		class OPENGL_IMPEXP CText : public CRenderizable
		{
			DEFINE_SERIALIZABLE( CText )
		protected:
			std::string		m_str;
            std::string		m_fontName;
            int				m_fontHeight, m_fontWidth;

		public:
			void setString( const std::string &s ) { m_str=s; } //!< Sets the text to display
			std::string getString() const { return m_str; }  //!< Return the current text associated to this label

			void setFont(const std::string &s, int height ) { m_fontName=s; m_fontHeight=height; } //!< Sets the font (It has no effect yet!)
			std::string getFont() const { return m_fontName; }

			/** Render
			  */
			void  render() const;

		private:

			/** Constructor
			  */
			CText( const std::string &str = std::string("") );

			/** Private, virtual destructor: only can be deleted from smart pointers */
			virtual ~CText();
		};

	} // end namespace

} // End of namespace


#endif