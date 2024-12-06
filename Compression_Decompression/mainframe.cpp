#include "mainframe.h"

wxPanel* secondpanel;
wxPanel* creatingtextpanel;

wxButton* CreateFileButton;
wxButton* DoneButton;

wxTextCtrl* textcontents;
mainframe::mainframe(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    SPanel();
}

void mainframe::SPanel() {
    secondpanel = new wxPanel(this);

    CreateFileButton = new wxButton(secondpanel, wxID_ANY, "Create Text File", wxPoint(30, 30), wxSize(150, 50));
    CreateFileButton->Bind(wxEVT_BUTTON, &mainframe::Creatingfile, this);
}

void mainframe::Creatingfile(wxCommandEvent& event) {
    secondpanel->Hide();

    creatingtextpanel = new wxPanel(this, wxID_ANY, wxPoint(1,1), wxSize(1280, 720));

    textcontents = new wxTextCtrl(creatingtextpanel, wxID_ANY, "", wxPoint(50, 50), wxSize(960, 500), wxTE_MULTILINE);

    DoneButton = new wxButton(creatingtextpanel, wxID_ANY, "Done", wxPoint(1100, 660), wxSize(150, 50));
    DoneButton->Bind(wxEVT_BUTTON, &mainframe::DoneButtonF, this);

}

void mainframe::DoneButtonF(wxCommandEvent& event) {
    creatingtextpanel->Hide();

    secondpanel->Show();

}
