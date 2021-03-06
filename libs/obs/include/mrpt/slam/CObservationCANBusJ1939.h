/* +---------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)               |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2014, Individual contributors, see AUTHORS file        |
   | See: http://www.mrpt.org/Authors - All rights reserved.                   |
   | Released under BSD License. See details in http://www.mrpt.org/License    |
   +---------------------------------------------------------------------------+ */
#ifndef CObservationCANBusJ1939_H
#define CObservationCANBusJ1939_H

#include <mrpt/utils/CSerializable.h>
#include <mrpt/slam/CObservation.h>

namespace mrpt
{
namespace slam
{
	DEFINE_SERIALIZABLE_PRE_CUSTOM_BASE_LINKAGE( CObservationCANBusJ1939 , CObservation, OBS_IMPEXP)

	/** This class stores a message from a CAN BUS with the protocol J1939
	 *
	 * \sa CObservation
	 * \ingroup mrpt_obs_grp
	 */
	class OBS_IMPEXP CObservationCANBusJ1939 : public CObservation
	{
		// This must be added to any CSerializable derived class:
		DEFINE_SERIALIZABLE( CObservationCANBusJ1939 )

	 public:
		/** Constructor.
		*/
		CObservationCANBusJ1939(  ) :
			m_pgn(0), m_src_address(0), m_priority(0), m_pdu_format(0), m_pdu_spec(0), m_data_length(0)
		{ }

		/** Destructor
		*/
		virtual ~CObservationCANBusJ1939()
		{ }

		/** The Parameter Group Number within this frame */
		uint16_t m_pgn;

		/** The address of the source node within this frame */
		uint8_t m_src_address;

		/** The priority */
		uint8_t m_priority;

		/** PDU Format */
		uint8_t m_pdu_format;

		/** PDU Specific */
		uint8_t m_pdu_spec;

		/** Data length  */
		uint8_t m_data_length;

		/** The data within this frame (0-8 bytes) */
		std::vector<uint8_t> m_data;

		/** The ASCII frame */
		std::vector<char> m_raw_frame;

		/** Not used */
		void getSensorPose( CPose3D &out_sensorPose ) const { }
		void setSensorPose( const CPose3D & ) { }

	}; // End of class def.

	} // End of namespace
} // End of namespace

#endif
