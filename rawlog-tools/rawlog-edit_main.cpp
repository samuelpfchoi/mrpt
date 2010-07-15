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

// ===========================================================================
//  Program: rawlog-edit
//
//  Intention: A generic rawlog (dataset) files manipulation program, 
//   much like the GUI program RawlogViewer but allowing command-line 
//   operations. 
//  See the "--help" output for list of supported operations and further 
//   instructions.
//
//  About integration with bash/.BAT scripts: The program will return 0 upon
//   successful execution, without dumping any information to stdout (unless
//   --verbose is used). Upon error, it will return -1.
// 
//  Started: JLBC @ Jul-2010
// ===========================================================================

#include <mrpt/obs.h>

#include "CRawlogProcessor.h"


using namespace mrpt;
using namespace mrpt::utils;
using namespace mrpt::slam;
using namespace mrpt::system;
using namespace std;

// Declarations:
#define VERBOSE_COUT	if (verbose) cout << "[rawlog-edit:verbose] "
typedef void (*TOperationFunctor)(CFileGZInputStream &in_rawlog, TCLAP::CmdLine &cmdline, bool verbose);


// Frwd. decl:
void op_externalize(CFileGZInputStream &in_rawlog, TCLAP::CmdLine &cmdline, bool verbose);
void op_info(CFileGZInputStream &in_rawlog, TCLAP::CmdLine &cmdline, bool verbose);
void op_remove_label(CFileGZInputStream &in_rawlog, TCLAP::CmdLine &cmdline, bool verbose);

// Declare the supported command line switches ===========
TCLAP::CmdLine cmd("rawlog-edit", ' ', MRPT_getVersion().c_str());

TCLAP::ValueArg<std::string> arg_input_file ("i","input","Input dataset (required) (*.rawlog)",true,"","dataset.rawlog",cmd);
TCLAP::ValueArg<std::string> arg_output_file("o","output","Output dataset (*.rawlog)",false,"","dataset_out.rawlog",cmd);

//TCLAP::ValueArg<double> arg_Ax("X","Ax","In detect-test mode, displacement in X (m)",false,4,"X",cmd);

TCLAP::SwitchArg arg_overwrite("w","overwrite","Force overwrite target file without prompting.",cmd, false);

TCLAP::SwitchArg arg_verbose("v","verbose","Verbose output",cmd, false);


// ======================================================================
//     main() of rawlog-edit
// ======================================================================
int main(int argc, char **argv)
{
	vector<TCLAP::Arg*> arg_ops;  // to be destroyed on exit.
	int ret_val = 0;

	try
	{
		// List of possible operations ---------------
		map<string,TOperationFunctor>  ops_functors;

		arg_ops.push_back(new TCLAP::SwitchArg("x","externalize","Op: convert to external storage.",cmd, false) );
		ops_functors["externalize"] = &op_externalize;

		arg_ops.push_back(new TCLAP::SwitchArg("f","info","Op: parse input file and dump information and statistics.",cmd, false) );
		ops_functors["info"] = &op_info;

		arg_ops.push_back(new TCLAP::ValueArg<std::string>("r","remove-label","Op: Remove all observation matching the given sensor label.",false,"","",cmd) );
		ops_functors["remove-label"] = &op_remove_label;
		// End of list of possible operations ---------------

		// Parse arguments:
		// -------------------------------------
		if (!cmd.parse( argc, argv ))
			throw std::runtime_error(""); // should exit.

		string input_rawlog  = arg_input_file.getValue();
		const bool verbose = arg_verbose.getValue();

		// Check the selected operation:
		//  Only one of the ops should be selected:
		string selected_op;
		for (size_t i=0;i<arg_ops.size();i++)
			if (arg_ops[i]->isSet())
			{
				if (selected_op.empty())
						selected_op = arg_ops[i]->getName();
				else	throw std::runtime_error(
					"Exactly one operation must be indicated on command line.\n"
					"Use --help to see the list of possible operations.");
			}

		if (selected_op.empty())
		{
			throw std::runtime_error(
				"Don't know what to do: No operation was indicated.\n"
				"Use --help to see the list of possible operations.");
		}

		VERBOSE_COUT << "Operation to perform: " << selected_op << endl;

		// This will be done for any operation: open the input rawlog
		// ------------------------------------------------------------
		if (!mrpt::system::fileExists(input_rawlog))
			throw runtime_error(format("Input file doesn't exist: '%s'",input_rawlog.c_str()));

		// Open input rawlog:
		CFileGZInputStream  fil_input;
		VERBOSE_COUT << "Opening '" << input_rawlog << "'...\n";
		fil_input.open(input_rawlog);
		VERBOSE_COUT << "Open OK.\n";

		// External storage directory?
		CImage::IMAGES_PATH_BASE = CRawlog::detectImagesDirectory(input_rawlog);
		if (mrpt::system::directoryExists(CImage::IMAGES_PATH_BASE)) {
			VERBOSE_COUT << "Found external storage directory: " << CImage::IMAGES_PATH_BASE << "\n";
		}
		else {
			VERBOSE_COUT << "Warning: No external storage directory was found (not an issue if the rawlog does not contain delayed-load images).\n";
		}


		// ------------------------------------
		//  EXECUTE THE REQUESTED OPERATION
		// ------------------------------------
		ASSERTMSG_(ops_functors.find(selected_op)!=ops_functors.end(), "Internal error: Unknown operation functor!")
		
		// Call the selected functor:
		ops_functors[selected_op](fil_input,cmd,verbose);

		// successful end of program.
		ret_val = 0; 
	}
	catch(std::exception &e)
	{
		if (strlen(e.what())) std::cerr << e.what() << std::endl;
		ret_val = -1;
	}

	// Free mem:
	for (size_t i=0;i<arg_ops.size();i++)
		delete arg_ops[i];

	// end:
	return ret_val;
}


// ======================================================================
//		op_externalize
// ======================================================================
void op_externalize(CFileGZInputStream &in_rawlog, TCLAP::CmdLine &cmdline,bool verbose)
{
	if (!arg_output_file.isSet())
		throw runtime_error("This operation requires an output file. Use '-o file' or '--output file'.");

	const string output_rawlog = arg_output_file.getValue();

	if (fileExists(output_rawlog) && !arg_overwrite.getValue() )
		throw runtime_error(string("*ABORTING*: Output file already exists: ") + output_rawlog + string("\n. Select a different output path, remove the file or force overwrite with '-w' or '--overwrite'.") );
	
	// Create the default "/Images" directory.
	string 	outDir = extractFileDirectory(output_rawlog) + string("/") + extractFileName(output_rawlog) + string("_Images");
	if (directoryExists(outDir))
		throw runtime_error(string("*ABORTING*: Output directory for images already exists: ") + outDir + string("\n. Select a different output path or remove the directory.") );

	VERBOSE_COUT << "Creating directory: " << outDir << endl;

	mrpt::system::createDirectory( outDir );
	if (!fileExists(outDir))
		throw runtime_error(string("*ABORTING*: Couldn't create directory: ") + outDir );

	// Add the final /
	outDir+="/";

	// Let the user choose the image format:
	string imgFileExtension = "jpg";


	// A class to do this operation:
	class CRawlogProcessor_Externalize : public CRawlogProcessorOnEachObservation
	{
	public:
		CRawlogProcessor_Externalize(CFileGZInputStream &in_rawlog, TCLAP::CmdLine &cmdline, bool verbose) : CRawlogProcessorOnEachObservation(in_rawlog,cmdline,verbose)
		{ }

		bool processOneObservation(CObservationPtr  &obs)
		{

			return true;
		}
	};

	// Process
	// ---------------------------------
	CRawlogProcessor_Externalize proc(in_rawlog,cmdline,verbose);
	proc.doProcessRawlog();

	// Dump statistics:
	// ---------------------------------
	cout << "Time to process file (sec)        : " << proc.m_timToParse << "\n";

}


struct TInfoPerSensorLabel
{
	TInfoPerSensorLabel() : occurrences(0), tim_first(INVALID_TIMESTAMP),tim_last(INVALID_TIMESTAMP) {}

	string		className;
	size_t		occurrences;
	TTimeStamp  tim_first, tim_last;
};


// ======================================================================
//		op_info
// ======================================================================
void op_info(CFileGZInputStream &in_rawlog, TCLAP::CmdLine &cmdline,bool verbose)
{
	// A class to do this operation:
	class CRawlogProcessor_Info : public CRawlogProcessor
	{
	public:
		// Stats to gather:
		bool  has_actSF_format;
		bool  has_obs_format;
		size_t  nActions;
		size_t  nSFs;
		map<string,TInfoPerSensorLabel>   infoPerSensorLabel;

		CRawlogProcessor_Info(CFileGZInputStream &in_rawlog, TCLAP::CmdLine &cmdline, bool verbose) : CRawlogProcessor(in_rawlog,cmdline,verbose)
		{
			has_actSF_format = false;
			has_obs_format   = false;
			nActions = 0;
			nSFs     = 0;
		}

		virtual bool processOneEntry(
			CActionCollectionPtr &actions,
			CSensoryFramePtr     &SF,
			CObservationPtr      &obs)
		{
			// Rawlog format: Normally only one of both should exist simultaneously!
			if (actions || SF) has_actSF_format = true;
			if (obs) has_obs_format = true;
			if (actions) nActions++;
			if (SF) nSFs++;

			// Process each observation individually, either from "obs" or each within a "SF":
			for (size_t idxObs=0; true; idxObs++)
			{
				CObservationPtr  obs_indiv;
				if (obs)
				{
					if (idxObs>0)  break;
					obs_indiv = obs;
				}
				else if (SF)
				{
					if (idxObs>=SF->size()) break;
					obs_indiv = SF->getObservationByIndex(idxObs);
				}
				else break; // shouldn't...

				// Process "obs_indiv":
				ASSERT_(obs_indiv)
				if (!obs_indiv->sensorLabel.empty())
				{
					TInfoPerSensorLabel &d = infoPerSensorLabel[obs_indiv->sensorLabel];
					
					d.className = obs_indiv->GetRuntimeClass()->className;
					d.occurrences++;
					if (d.tim_first==INVALID_TIMESTAMP)  
						d.tim_first = obs_indiv->timestamp;
					d.tim_last = obs_indiv->timestamp;
				}
			}

			// Clear read objects:
			actions.clear_unique();
			SF.clear_unique();
			obs.clear_unique();

			return true; // No error.
		}

	}; // end CRawlogProcessor_Info 

	// Process
	// ---------------------------------
	CRawlogProcessor_Info proc(in_rawlog,cmdline,verbose);
	proc.doProcessRawlog();


	// Dump statistics:
	// ---------------------------------
	cout << "Time to parse file (sec)          : " << proc.m_timToParse << "\n";
	cout << "Physical file size                : " << mrpt::system::unitsFormat(proc.m_filSize) << "B\n";
	cout << "Uncompressed file size            : " << mrpt::system::unitsFormat(in_rawlog.getPosition()) << "B\n";
	cout << "Compression ratio                 : " << format("%.02f%%\n", 100.0*double(proc.m_filSize)/double(in_rawlog.getPosition()));
	cout << "Overall number of objects         : " << proc.m_rawlogEntry << "\n";
	cout << "Actions/SensoryFrame format       : " << (proc.has_actSF_format ? "Yes":"No") << "\n";
	cout << "Observations format               : " << (proc.has_obs_format ? "Yes":"No") << "\n";

	// By sensor labels:
	cout << "All sensor labels                 : ";
	for (map<string,TInfoPerSensorLabel>::const_iterator it=proc.infoPerSensorLabel.begin();it!=proc.infoPerSensorLabel.end();++it)
	{
		if (it!=proc.infoPerSensorLabel.begin()) cout << ", "; 
		cout << it->first;
	}
	cout << "\n";

	for (map<std::string,TInfoPerSensorLabel>::const_iterator it=proc.infoPerSensorLabel.begin();it!=proc.infoPerSensorLabel.end();++it)
	{
		const TTimeStamp	tf = it->second.tim_first;
		const TTimeStamp	tl = it->second.tim_last;
		double Hz = 0, dur = 0;
		if (tf!=INVALID_TIMESTAMP && tl!=INVALID_TIMESTAMP)
		{
			dur = mrpt::system::timeDifference(tf,tl);
			Hz = double(it->second.occurrences>1 ? it->second.occurrences-1 : 1)/dur;
		}
		cout << "Sensor (Label/Occurs/Rate/Durat.) : " << 
			format("%15s /%7u /%5.03f /%.03f\n",
				it->first.c_str(), 
				(unsigned)it->second.occurrences,
				Hz, 
				dur);
	}

}

// ======================================================================
//		op_remove_label
// ======================================================================
void op_remove_label(CFileGZInputStream &in_rawlog, TCLAP::CmdLine &cmdline,bool verbose)
{
}


