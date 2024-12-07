#include "mainframe.h"

wxBEGIN_EVENT_TABLE(mainframe, wxFrame)
EVT_BUTTON(wxID_ANY, mainframe::Creatingfile)
EVT_BUTTON(wxID_ANY, mainframe::DoneButtonF)
wxEND_EVENT_TABLE()

mainframe::mainframe(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    SPanel();
}

void mainframe::SPanel() {
    secondpanel = new wxPanel(this);

    wxStaticText* panelLabel = new wxStaticText(secondpanel, wxID_ANY, "File Management Panel",
        wxPoint(30, 10), wxDefaultSize, wxALIGN_LEFT);

    CreateFileButton = new wxButton(secondpanel, wxID_ANY, "Create Text File", wxPoint(30, 50), wxSize(150, 50));
    CreateFileButton->Bind(wxEVT_BUTTON, &mainframe::Creatingfile, this);

    // Compress Drop Box
    wxStaticText* compressLabel = new wxStaticText(secondpanel, wxID_ANY, "Drag files here to Compress:", wxPoint(200, 120), wxDefaultSize, wxALIGN_LEFT);
    compressBox = new wxListBox(secondpanel, wxID_ANY, wxPoint(200, 140), wxSize(300, 200));
    compressBox->SetDropTarget(new FileDropTarget(compressBox, "Compress"));

    // Decompress Drop Box
    wxStaticText* decompressLabel = new wxStaticText(secondpanel, wxID_ANY, "Drag files here to Decompress:", wxPoint(550, 120), wxDefaultSize, wxALIGN_LEFT);
    decompressBox = new wxListBox(secondpanel, wxID_ANY, wxPoint(550, 140), wxSize(300, 200));
    decompressBox->SetDropTarget(new FileDropTarget(decompressBox, "Decompress"));

   
}

void mainframe::Creatingfile(wxCommandEvent& event) {
    secondpanel->Hide();

    creatingtextpanel = new wxPanel(this, wxID_ANY, wxPoint(1,1), wxSize(1280,720));
    wxBoxSizer* textPanelSizer = new wxBoxSizer(wxVERTICAL);


    wxStaticText* textEditorLabel = new wxStaticText(creatingtextpanel, wxID_ANY, "Enter your text below:",
        wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

    textcontents = new wxTextCtrl(creatingtextpanel, wxID_ANY, "", wxPoint (50,50), wxSize(960, 500), wxTE_MULTILINE);
    DoneButton = new wxButton(creatingtextpanel, wxID_ANY, "Done", wxPoint(1100,660), wxSize(150, 50));
    DoneButton->Bind(wxEVT_BUTTON, &mainframe::DoneButtonF, this);

    textPanelSizer->Add(textEditorLabel, 0, wxALL | wxLEFT, 10);
    textPanelSizer->Add(textcontents, 1, wxEXPAND | wxALL, 10);
    textPanelSizer->Add(DoneButton, 0, wxALL | wxALIGN_RIGHT, 10);

    creatingtextpanel->SetSizer(textPanelSizer);
    creatingtextpanel->Show();
    Layout();
}

void mainframe::DoneButtonF(wxCommandEvent& event) {
    creatingtextpanel->Hide();
    wxFileDialog saveFileDialog(this, _("Save Text File"), "", "",
        "Text files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL) {
        return; 
    }

    std::ofstream outFile(saveFileDialog.GetPath().ToStdString());
    if (outFile.is_open()) {
        outFile << textcontents->GetValue().ToStdString();
        outFile.close();
    }

    secondpanel->Show();
    Layout();
}
