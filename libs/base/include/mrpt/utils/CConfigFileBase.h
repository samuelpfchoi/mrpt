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
#ifndef  CConfigFileBase_H
#define  CConfigFileBase_H

#include <mrpt/utils/utils_defs.h>

#include <mrpt/math/CMatrixTemplate.h>

/*---------------------------------------------------------------
	Class
  ---------------------------------------------------------------*/
namespace mrpt
{
namespace utils
{
	/** This class allows loading and storing values and vectors of different types from a configuration text, which can be implemented as a ".ini" file, a memory-stored string, etc...
	  *   This is a virtual class, use only as a pointer to an implementation of one of the derived classes.
	  */
	class BASE_IMPEXP CConfigFileBase
	{
	protected:
		/** A virtual method to write a generic string.
		  */
		virtual void  writeString(const std::string &section,const std::string &name, const std::string &str) = 0;

		/** A virtual method to read a generic string.
         * \exception std::exception If the key name is not found and "failIfNotFound" is true. Otherwise the "defaultValue" is returned.
		 */
		virtual std::string  readString(
            const std::string &section,
            const std::string &name,
            const std::string &defaultStr,
            bool failIfNotFound = false) const = 0;

	public:
        /** Virtual destructor...
         */
         virtual ~CConfigFileBase()
         {
         }

		/** Returns a list with all the section names.
		  */
		virtual void getAllSections( vector_string	&sections ) const = 0 ;
		
		/** Returs a list with all the keys into a section.
		  */
		virtual void getAllKeys( const std::string section, vector_string	&keys ) const = 0;

		/** Checks if a given section exists (name is case insensitive) */
		bool sectionExists( const std::string &section_name) const;

		/** Save a configuration parameter of type "double"
		 */
		void  write(const std::string &section, const std::string &name, double value);

		/** Save a configuration parameter of type "float"
		 */
		void  write(const std::string &section, const std::string &name, float value);

		/** Save a configuration parameter of type "int"
		 */
		void  write(const std::string &section, const std::string &name, int value);

		/** Save a configuration parameter of type "unsigned int"
		 */
		void  write(const std::string &section, const std::string &name, unsigned int value);

#if MRPT_WORD_SIZE>32
		/** Save a configuration parameter of type "size_t"
		 */
		void  write(const std::string &section, const std::string &name, size_t value);
#endif		

		/** Save a configuration parameter of type "string"
		 */
		void  write(const std::string &section, const std::string &name, const std::string &value);

		/** Save a configuration parameter of type "std::vector<int>"
		 */
		void  write(const std::string &section, const std::string &name, const std::vector<int> &value);

		/** Save a configuration parameter of type "std::vector<unsigned int>"
		 */
		void  write(const std::string &section, const std::string &name, const std::vector<unsigned int> &value);

		/** Save a configuration parameter of type "std::vector<float>"
		 */
		void  write(const std::string &section, const std::string &name, const std::vector<float> &value);

		/** Save a configuration parameter of type "std::vector<double>"
		 */
		void  write(const std::string &section, const std::string &name, const std::vector<double> &value);

		/** Save a configuration parameter of type "std::vector<bool>"
		 */
		void  write(const std::string &section, const std::string &name, const std::vector<bool> &value);

		/** Reads a configuration parameter of type "double"
         * \exception std::exception If the key name is not found and "failIfNotFound" is true. Otherwise the "defaultValue" is returned.
		 */
		double  read_double(const std::string &section, const std::string &name, double defaultValue, bool failIfNotFound = false) const;

		/** Reads a configuration parameter of type "float"
         * \exception std::exception If the key name is not found and "failIfNotFound" is true. Otherwise the "defaultValue" is returned.
		 */
		float  read_float(const std::string &section, const std::string &name, float defaultValue, bool failIfNotFound = false) const;

		/** Reads a configuration parameter of type "bool", codified as "1"/"0" or "true"/"false" or "yes"/"no" for true/false, repectively.
         * \exception std::exception If the key name is not found and "failIfNotFound" is true. Otherwise the "defaultValue" is returned.
		 */
		bool  read_bool(const std::string &section, const std::string &name, bool defaultValue, bool failIfNotFound = false) const;

		/** Reads a configuration parameter of type "int"
         * \exception std::exception If the key name is not found and "failIfNotFound" is true. Otherwise the "defaultValue" is returned.
		 */
		int  read_int(const std::string &section, const std::string &name, int defaultValue, bool failIfNotFound = false) const;

		/** Reads a configuration parameter of type "uint64_t": As in all other methods, the numeric value can be in decimal or hexadecimal with the prefix "0x"
         * \exception std::exception If the key name is not found and "failIfNotFound" is true. Otherwise the "defaultValue" is returned.
		 */
		uint64_t read_uint64_t(const std::string &section, const std::string &name, uint64_t defaultValue, bool failIfNotFound = false ) const;

		/** Reads a configuration parameter of type "string"
         * \exception std::exception If the key name is not found and "failIfNotFound" is true. Otherwise the "defaultValue" is returned.
		 */
		std::string  read_string(const std::string &section, const std::string &name, const std::string &defaultValue, bool failIfNotFound = false) const;

		/** Reads a configuration parameter of type "string", and keeps only the first word (this can be used to eliminate possible comments at the end of the line)
         * \exception std::exception If the key name is not found and "failIfNotFound" is true. Otherwise the "defaultValue" is returned.
		 */
		std::string  read_string_first_word(const std::string &section, const std::string &name, const std::string &defaultValue, bool failIfNotFound = false) const;

		/** Reads a configuration parameter of type "std::vector<unsigned int>"
         * \exception std::exception If the key name is not found and "failIfNotFound" is true. Otherwise the "defaultValue" is returned.
		 */
		void  read_vector(
			const std::string					&section,
			const std::string					&name,
			const std::vector<uint32_t>		&defaultValue,
			std::vector<uint32_t>			&outValues,
			bool failIfNotFound = false) const;

		/** Reads a configuration parameter of type "std::vector<int>"
         * \exception std::exception If the key name is not found and "failIfNotFound" is true. Otherwise the "defaultValue" is returned.
		 */
		void  read_vector(
			const std::string			&section,
			const std::string			&name,
			const std::vector<int32_t>		&defaultValue,
			std::vector<int32_t>			&outValues,
			bool failIfNotFound = false) const;

		/** Reads a configuration parameter of type "std::vector<unsigned int>"
         * \exception std::exception If the key name is not found and "failIfNotFound" is true. Otherwise the "defaultValue" is returned.
		 */
		void  read_vector(
			const std::string					&section,
			const std::string					&name,
			const std::vector<uint64_t>		&defaultValue,
			std::vector<uint64_t>			&outValues,
			bool failIfNotFound = false) const;

		/** Reads a configuration parameter of type "std::vector<int>"
         * \exception std::exception If the key name is not found and "failIfNotFound" is true. Otherwise the "defaultValue" is returned.
		 */
		void  read_vector(
			const std::string			&section,
			const std::string			&name,
			const std::vector<int64_t>		&defaultValue,
			std::vector<int64_t>			&outValues,
			bool failIfNotFound = false) const;


		/** Reads a configuration parameter of type "std::vector<float>"
         * \exception std::exception If the key name is not found and "failIfNotFound" is true. Otherwise the "defaultValue" is returned.
		 */
		void  read_vector(
			const std::string			&section,
			const std::string			&name,
			const std::vector<float>	&defaultValue,
			std::vector<float>			&outValues,
			bool failIfNotFound = false) const;

		/** Reads a configuration parameter of type "std::vector<double>"
         * \exception std::exception If the key name is not found and "failIfNotFound" is true. Otherwise the "defaultValue" is returned.
		 */
		void  read_vector(
			const std::string			&section,
			const std::string			&name,
			const std::vector<double>	&defaultValue,
			std::vector<double>			&outValues,
			bool failIfNotFound = false ) const;

		/** Reads a configuration parameter of type "std::vector<bool>"
         * \exception std::exception If the key name is not found and "failIfNotFound" is true. Otherwise the "defaultValue" is returned.
		 */
		void  read_vector(
			const std::string			&section,
			const std::string			&name,
			const std::vector<bool>		&defaultValue,
			std::vector<bool>			&outValues,
			bool failIfNotFound = false ) const;

		/** Reads a configuration parameter as a matrix written in a matlab-like format - for example: "[2 3 4 ; 7 8 9]"
		 *  This template method can be instantiated for matrices of the types: int, long, unsinged int, unsigned long, float, double, long double
         * \exception std::exception If the key name is not found and "failIfNotFound" is true. Otherwise the "defaultValue" is returned.
		 */
		 template <class T>
		 void read_matrix(
			const std::string			&section,
			const std::string			&name,
			mrpt::math::CMatrixTemplate<T>	&outMatrix,
			const mrpt::math::CMatrixTemplate<T> &defaultMatrix = mrpt::math::CMatrixTemplate<T>(0,0),
			bool failIfNotFound = false ) const;


	}; // End of class def.

	/** An useful macro for loading variables stored in a INI-like file under a key with the same name that the variable, and assigning the variable the current value if not found in the config file.
	  *  The variableType must be the suffix of "read_XXX" functions, i.e. int, bool,...
	  */
#define MRPT_LOAD_CONFIG_VAR(variableName,variableType,configFileObject,sectionNameStr) \
	{ variableName = configFileObject.read_##variableType(sectionNameStr,#variableName,variableName); }

	/** Loads a float variable, stored as radians but entered in the INI-file as degrees:
	  */
#define MRPT_LOAD_CONFIG_VAR_DEGREES(variableName,configFileObject,sectionNameStr) \
	{ variableName = DEG2RAD( configFileObject.read_float(sectionNameStr,#variableName, RAD2DEG(variableName)) ); }

#define MRPT_LOAD_CONFIG_VAR_CAST(variableName,variableType,variableTypeCast,configFileObject,sectionNameStr) \
	{ variableName = static_cast<variableTypeCast>(configFileObject.read_##variableType(sectionNameStr,#variableName,variableName)); }


#define MRPT_LOAD_HERE_CONFIG_VAR(variableName,variableType,targetVariable,configFileObject,sectionNameStr) \
		targetVariable = configFileObject.read_##variableType(sectionNameStr,#variableName,targetVariable,false);

#define MRPT_LOAD_HERE_CONFIG_VAR_NO_DEFAULT(variableName,variableType,targetVariable,configFileObject,sectionNameStr) \
	{ try { \
		targetVariable = configFileObject.read_##variableType(sectionNameStr,#variableName,targetVariable,true); \
    } catch (std::exception &) \
    { \
		THROW_EXCEPTION( format( "Value for '%s' not found in config file", static_cast<const char*>(#variableName ) )); \
	} }\


#define MRPT_LOAD_CONFIG_VAR_NO_DEFAULT(variableName,variableType,configFileObject,sectionNameStr) \
	{ try { \
		variableName = configFileObject.read_##variableType(sectionNameStr,#variableName,variableName,true); \
    } catch (std::exception &) \
    { \
		THROW_EXCEPTION( format( "Value for '%s' not found in config file", static_cast<const char*>(#variableName ) )); \
	} }\

#define MRPT_LOAD_CONFIG_VAR_CAST_NO_DEFAULT(variableName,variableType,variableTypeCast,configFileObject,sectionNameStr) \
	{ try { \
		variableName = static_cast<variableTypeCast>(configFileObject.read_##variableType(sectionNameStr,#variableName,variableName,true)); \
    } catch (std::exception &) \
    { \
		THROW_EXCEPTION( format( "Value for '%s' not found in config file", static_cast<const char*>(#variableName ) )); \
	} }\


#define MRPT_LOAD_HERE_CONFIG_VAR_CAST(variableName,variableType,variableTypeCast,targetVariable,configFileObject,sectionNameStr) \
		targetVariable = static_cast<variableTypeCast>(configFileObject.read_##variableType(sectionNameStr,#variableName,targetVariable));

#define MRPT_LOAD_HERE_CONFIG_VAR_CAST_NO_DEFAULT(variableName,variableType,variableTypeCast,targetVariable,configFileObject,sectionNameStr) \
	{ try { \
		targetVariable = static_cast<variableTypeCast>(configFileObject.read_##variableType(sectionNameStr,#variableName,targetVariable,true)); \
    } catch (std::exception &) \
    { \
		THROW_EXCEPTION( format( "Value for '%s' not found in config file", static_cast<const char*>(#variableName ) )); \
	} }\


#define MRPT_SAVE_CONFIG_VAR(variableName,configFileObject,sectionNameStr) \
	{ configFileObject.write(sectionNameStr,#variableName,variableName); }

#define MRPT_SAVE_CONFIG_VAR_DEGREES(variableName,configFileObject,sectionNameStr) \
	{ configFileObject.write(sectionNameStr,#variableName, RAD2DEG(variableName)); }


	} // End of namespace
} // end of namespace
#endif