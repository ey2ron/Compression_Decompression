#pragma once
#include <wx/wx.h>
class mainframe : public wxFrame
{
public:
	mainframe(const wxString& title);
private:
	void SPanel();
	void Creatingfile(wxCommandEvent& evt);
	void DoneButtonF(wxCommandEvent& evt);
};

