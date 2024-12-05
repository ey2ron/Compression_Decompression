#include "app.h"
#include "mainframe.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(app);

bool app::OnInit() {
	mainframe* mainFrame = new mainframe("HYDRAULIC PRESS PROGRAM");
	mainFrame->SetMinClientSize(wxSize(1280, 720));
	mainFrame->SetMaxClientSize(wxSize(1280, 720));
	mainFrame->Move(130, 80);
	mainFrame->Show();
	return true;
}
