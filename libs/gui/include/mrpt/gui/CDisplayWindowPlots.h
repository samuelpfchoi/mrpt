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
#ifndef  CDisplayWindowPlots_H
#define  CDisplayWindowPlots_H

#include <mrpt/gui/CBaseGUIWindow.h>
#include <mrpt/math/CMatrixTemplateNumeric.h>
#include <mrpt/math/lightweight_geom_data.h>
#include <mrpt/utils/CImage.h>

/*---------------------------------------------------------------
	Class
  ---------------------------------------------------------------*/
namespace mrpt
{
	namespace gui
	{
		using namespace mrpt::utils;
		using namespace mrpt::math;

		class CWindowDialogPlots;

		DEFINE_MRPT_OBJECT_PRE_CUSTOM_BASE_LINKAGE(CDisplayWindowPlots,  mrpt::gui::CBaseGUIWindow, GUI_IMPEXP)

		/** Create a GUI window and display plots with MATLAB-like interfaces and commands.
		 *
		 *  For a list of supported events with the observer/observable pattern, see the discussion in mrpt::gui::CBaseGUIWindow.
		 *
		 *   See CDisplayWindowPlots::plot
		 */
		class GUI_IMPEXP CDisplayWindowPlots : public mrpt::gui::CBaseGUIWindow
		{
			// This must be added to any CObject derived class:
			DEFINE_MRPT_OBJECT( CDisplayWindowPlots )

		public:
			typedef void (* TCallbackMenu) (int menuID,float cursor_x, float cursor_y, void* userParam); //!< Type for the callback function used in setMenuCallback

		protected:
			friend class CWindowDialogPlots;

			bool		m_holdon;   	//!< Whether hold_on is enabled
			bool		m_holdon_just_disabled;
			uint32_t  	m_holdon_cnt;	//!< Counter for hold_on
			TCallbackMenu m_callback;
			void		*m_callback_param;

		public:

			/** Constructor
			 */
			CDisplayWindowPlots(
				const std::string &windowCaption = std::string(),
				unsigned int initialWidth = 350,
				unsigned int initialHeight = 300 );

			/** Class factory returning a smart pointer */
			static CDisplayWindowPlotsPtr Create(
				const std::string	&windowCaption = std::string(),
				unsigned int		initialWindowWidth = 400,
				unsigned int		initialWindowHeight = 300 )
			{
				return CDisplayWindowPlotsPtr(new CDisplayWindowPlots(windowCaption,initialWindowWidth,initialWindowHeight));
			}

			/** Destructor
			 */
			virtual ~CDisplayWindowPlots();

			/** Resizes the window, stretching the image to fit into the display area.
			 */
			void  resize( unsigned int width, unsigned int height );

			/** Changes the position of the window on the screen.
			 */
			void  setPos( int x, int y );

			/** Changes the window title text.
			  */
			void  setWindowTitle( const std::string &str );

			/** Enable/disable the feature of pan/zoom with the mouse (default=enabled)
			*/
			void  enableMousePanZoom( bool enabled );

			/** Adds a new layer with a 2D plot based on two vectors of X and Y points, using a MATLAB-like syntax.
			  *  Each call to this function creates a new plot, unless the plot name coincides with an already existing plot: in this case the X & Y points are used to update this existing layer (this also applies to using the default plot name).
			  *  If "hold_on" is enabled, then every call will always create a new plot, even if no "plotName" is provided.
			  *
			  *  The lineFormat string is a combination of the following characters:
			  * - Line styles:
			  *		- '.': One point for each data point
			  *		- '-': A continuous line
			  *		- ':': A dashed line
			  * - Colors:
			  *		- k: black
			  *		- r: red
			  *		- g: green
			  *		- b: blue
			  *		- m: magenta
			  *		- c: cyan
			  * - Line width:
			  *		- '1' to '9': The line width (default=1)
			  *
			  *  Examples:
			  *   - 'r.' -> red points.
			  *   - 'k3' or 'k-3' -> A black line with a line width of 3 pixels.
			  * \note The vectors x & y can be of types: float or double.
			  * \sa axis, axis_equal, axis_fit, clear, hold_on, hold_off
			  */
			template <typename T>
			void GUI_IMPEXP plot(
				const std::vector<T> &x,
				const std::vector<T> &y,
				const std::string  &lineFormat = std::string("b-"),
				const std::string  &plotName = std::string("plotXY") );

			/** Adds a new layer with a 2D plot based on the vector Y, using a MATLAB-like syntax.
			  *  Each call to this function creates a new plot, unless the plot name coincides with an already existing plot: in this case the X & Y points are used to update this existing layer (this also applies to using the default plot name).
			  *  If "hold_on" is enabled, then every call will always create a new plot, even if no "plotName" is provided.
			  *
			  *  The lineFormat string is a combination of the following characters:
			  * - Line styles:
			  *		- '.': One point for each data point
			  *		- '-': A continuous line
			  *		- ':': A dashed line
			  * - Colors:
			  *		- k: black
			  *		- r: red
			  *		- g: green
			  *		- b: blue
			  *		- m: magenta
			  *		- c: cyan
			  * - Line width:
			  *		- '1' to '9': The line width (default=1)
			  *
			  *  Examples:
			  *   - 'r.' -> red points.
			  *   - 'k3' or 'k-3' -> A black line with a line width of 3 pixels.
			  * \note The method can be called with vectors of types: float, double.
			  * \sa axis, axis_equal, axis_fit, clear, hold_on, hold_off
			  */
			template <typename T>
			void GUI_IMPEXP plot(
				const std::vector<T> &y,
				const std::string  &lineFormat = std::string("b-"),
				const std::string  &plotName = std::string("plotXY") );

			/** Set the view area according to the passed coordinated.
			  */
			void axis( float x_min, float x_max, float y_min, float y_max, bool aspectRatioFix = false );

			/** Enable/disable the fixed X/Y aspect ratio fix feature (default=disabled).
			  */
			void axis_equal(bool enable=true);

			/** Fix automatically the view area according to existing graphs.
			  */
			void axis_fit(bool aspectRatioFix=false);

			/** Plots a 2D ellipse given its mean, covariance matrix, and
			  *  Each call to this function creates a new plot, unless the plot name coincides with an already existing plot: in this case the new values are used to update this existing layer (this also applies to using the default plot name).
			  *  If "hold_on" is enabled, then every call will always create a new plot, even if no "plotName" is provided.
			  *
			  *  For a description of lineFormat see CDisplayWindowPlots::plot.
			  *  The "quantiles" value determines the confidence interval for the ellipse:
			  *     - 1 : 68.27% confidence interval
			  *     - 2 : 95.45%
			  *     - 3 : 99.73%
			  *     - 4 : 99.994%
			  * \note This method can be called with 2x2 fixed-sized or dynamic-size matrices of types: float or double.
			  * \sa axis, axis_equal, axis_fit, hold_on, hold_off
			  */
			template <typename T>
			void GUI_IMPEXP plotEllipse(
				const T mean_x,
				const T mean_y,
				const CMatrixTemplateNumeric<T> &cov22,
				const float quantiles,
				const std::string  &lineFormat = std::string("b-"),
				const std::string  &plotName = std::string("plotEllipse"),
				bool showName = false);

			/** Plots a 2D ellipse given its mean, covariance matrix, and
			  *  Each call to this function creates a new plot, unless the plot name coincides with an already existing plot: in this case the new values are used to update this existing layer (this also applies to using the default plot name).
			  *  If "hold_on" is enabled, then every call will always create a new plot, even if no "plotName" is provided.
			  *
			  *  For a description of lineFormat see CDisplayWindowPlots::plot.
			  *  The "quantiles" value determines the confidence interval for the ellipse:
			  *     - 1 : 68.27% confidence interval
			  *     - 2 : 95.45%
			  *     - 3 : 99.73%
			  *     - 4 : 99.994%
			  * \note This method can be called with 2x2 fixed-sized or dynamic-size matrices of types: float or double.
			  * \sa axis, axis_equal, axis_fit, hold_on, hold_off
			  */
			template <typename T>
			void GUI_IMPEXP plotEllipse(
				const T mean_x,
				const T mean_y,
				const CMatrixFixedNumeric<T,2,2> &cov22,
				const float quantiles,
				const std::string  &lineFormat = std::string("b-"),
				const std::string  &plotName = std::string("plotEllipse"),
				bool showName = false);

			/** Adds a bitmap image layer.
			  *  Each call to this function creates a new layer, unless the plot name coincides with an already existing plot: in this case the new values are used to update this existing layer (this also applies to using the default plot name).
			  *
			  * \sa axis, axis_equal, axis_fit, hold_on, hold_off
			  */
			void image(
				const utils::CImage &img,
				const float &x_left,
				const float &y_bottom,
				const float &x_width,
				const float &y_height,
				const std::string  &plotName = std::string("image") );


			/** Remove all plot objects in the display.
			  * \sa plot
			  */
			void clear();

			/** Remove all plot objects in the display (clear and clf do exactly the same).
			  * \sa plot, hold_on, hold_off
			  */
			inline void clf() {
				clear();
			}

			/** Enables keeping all the graphs, instead of overwritting them.
			  * \sa hold_off, plot
			  */
			void hold_on();

			/** Disables keeping all the graphs (this is the default behavior).
			  * \sa hold_on, plot
			  */
			void hold_off();

			/** Disables keeping all the graphs (this is the default behavior).
			  * \param label The text that appears in the new popup menu item.
			  * \param menuID Any positive number (0,1,..). Used to tell which menu was selected in the user callback.
			  * \sa setMenuCallback
			  */
			void addPopupMenuEntry( const std::string &label, int menuID );


			/** Must be called to have a callback when the user selects one of the user-defined entries in the popup menu.
			  * \sa addPopupMenuEntry
			  */
			void setMenuCallback(TCallbackMenu userFunction, void* userParam = NULL );


		}; // End of class def.
	}

} // End of namespace

#endif