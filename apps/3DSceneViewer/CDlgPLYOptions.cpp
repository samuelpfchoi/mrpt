/* +---------------------------------------------------------------------------+
   |          The Mobile Robot Programming Toolkit (MRPT) C++ library          |
   |                                                                           |
   |                   http://mrpt.sourceforge.net/                            |
   |                                                                           |
   |   Copyright (C) 2005-2011  University of Malaga                           |
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

#include "CDlgPLYOptions.h"

//(*InternalHeaders(CDlgPLYOptions)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(CDlgPLYOptions)
const long CDlgPLYOptions::ID_CHECKBOX1 = wxNewId();
const long CDlgPLYOptions::ID_CHECKBOX2 = wxNewId();
const long CDlgPLYOptions::ID_STATICTEXT1 = wxNewId();
const long CDlgPLYOptions::ID_CHOICE1 = wxNewId();
const long CDlgPLYOptions::ID_STATICTEXT2 = wxNewId();
const long CDlgPLYOptions::ID_TEXTCTRL1 = wxNewId();
const long CDlgPLYOptions::ID_STATICTEXT3 = wxNewId();
const long CDlgPLYOptions::ID_TEXTCTRL2 = wxNewId();
const long CDlgPLYOptions::ID_STATICTEXT4 = wxNewId();
const long CDlgPLYOptions::ID_TEXTCTRL3 = wxNewId();
const long CDlgPLYOptions::ID_RADIOBOX1 = wxNewId();
const long CDlgPLYOptions::ID_BUTTON1 = wxNewId();
const long CDlgPLYOptions::ID_BUTTON2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(CDlgPLYOptions,wxDialog)
	//(*EventTable(CDlgPLYOptions)
	//*)
END_EVENT_TABLE()

CDlgPLYOptions::CDlgPLYOptions(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(CDlgPLYOptions)
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer5;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer6;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxFlexGridSizer* FlexGridSizer1;
	
	Create(parent, id, _("PLY import options"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	FlexGridSizer1 = new wxFlexGridSizer(2, 1, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	FlexGridSizer2 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer4 = new wxFlexGridSizer(4, 1, 0, 0);
	cbXYGrid = new wxCheckBox(this, ID_CHECKBOX1, _("Add XY ground grid"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	cbXYGrid->SetValue(true);
	FlexGridSizer4->Add(cbXYGrid, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	cbXYZ = new wxCheckBox(this, ID_CHECKBOX2, _("Add XYZ corner at origin"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
	cbXYZ->SetValue(false);
	FlexGridSizer4->Add(cbXYZ, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5 = new wxFlexGridSizer(1, 2, 0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Point size:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer5->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	cbPointSize = new wxChoice(this, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
	cbPointSize->SetSelection( cbPointSize->Append(_("1.0")) );
	cbPointSize->Append(_("2.0"));
	cbPointSize->Append(_("3.0"));
	cbPointSize->Append(_("4.0"));
	cbPointSize->Append(_("5.0"));
	cbPointSize->Append(_("6.0"));
	cbPointSize->Append(_("7.0"));
	cbPointSize->Append(_("8.0"));
	cbPointSize->Append(_("9.0"));
	FlexGridSizer5->Add(cbPointSize, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(FlexGridSizer5, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Rotate point cloud"));
	FlexGridSizer6 = new wxFlexGridSizer(3, 2, 0, 0);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Yaw (deg):"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer6->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	edYaw = new wxTextCtrl(this, ID_TEXTCTRL1, _("0.0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	FlexGridSizer6->Add(edYaw, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Pitch (deg):"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer6->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	edPitch = new wxTextCtrl(this, ID_TEXTCTRL2, _("0.0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	FlexGridSizer6->Add(edPitch, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Roll (deg):"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer6->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	edRoll = new wxTextCtrl(this, ID_TEXTCTRL3, _("90.0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	FlexGridSizer6->Add(edRoll, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(FlexGridSizer6, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer4->Add(StaticBoxSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer2->Add(FlexGridSizer4, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 0);
	wxString __wxRadioBoxChoices_1[4] = 
	{
		_("None"),
		_("X"),
		_("Y"),
		_("Z")
	};
	rbIntFromXYZ = new wxRadioBox(this, ID_RADIOBOX1, _(" Intensity from X,Y or Z value "), wxDefaultPosition, wxDefaultSize, 4, __wxRadioBoxChoices_1, 1, 0, wxDefaultValidator, _T("ID_RADIOBOX1"));
	rbIntFromXYZ->SetSelection(2);
	FlexGridSizer2->Add(rbIntFromXYZ, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 2);
	FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer3 = new wxFlexGridSizer(0, 3, 0, 0);
	btnCancel = new wxButton(this, ID_BUTTON1, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	FlexGridSizer3->Add(btnCancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	btnOK = new wxButton(this, ID_BUTTON2, _("Import"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	FlexGridSizer3->Add(btnOK, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(FlexGridSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	Center();
	
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDlgPLYOptions::OnbtnCancelClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CDlgPLYOptions::OnbtnOKClick);
	//*)
}

CDlgPLYOptions::~CDlgPLYOptions()
{
	//(*Destroy(CDlgPLYOptions)
	//*)
}


void CDlgPLYOptions::OnbtnCancelClick(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void CDlgPLYOptions::OnbtnOKClick(wxCommandEvent& event)
{
	EndModal(wxID_OK);
}