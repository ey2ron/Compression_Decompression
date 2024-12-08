#include "mainframe.h"
#include <wx/wx.h>


wxBEGIN_EVENT_TABLE(mainframe, wxFrame)
EVT_BUTTON(wxID_ANY, mainframe::Creatingfile)
EVT_BUTTON(wxID_ANY, mainframe::DoneButtonF)
wxEND_EVENT_TABLE()

wxColour bgcolor(177, 195, 154);
wxColour bgcolorfont(154, 195, 160);
wxColour bgcolorcompress(154, 195, 187);

wxFont buttonFont(14, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
wxFont titlefont(28, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
wxFont AFont(14, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD);
wxFont AFont1(18, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD); 
wxFont emefont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL); 

mainframe::mainframe(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    Fpanel();
}

void mainframe::Fpanel() {
    firstpanel= new wxPanel(this);
    firstpanel->SetBackgroundColour(bgcolorfont);
    wxButton* button = new wxButton(firstpanel, wxID_ANY, "Start Program", wxPoint(378, 318), wxSize(504, 94));
    button->SetBackgroundColour(*wxWHITE);
    button->SetForegroundColour(*wxBLACK);
    button->SetFont(buttonFont);
    button->Bind(wxEVT_ENTER_WINDOW, [button](wxMouseEvent&) {
        button->SetForegroundColour(*wxGREEN);
        button->Refresh();
        });
    button->Bind(wxEVT_LEAVE_WINDOW, [button](wxMouseEvent&) {
        button->SetForegroundColour(*wxBLACK);
        button->Refresh();
        });
    button->Bind(wxEVT_BUTTON, &mainframe::SPanel, this);
}

void mainframe::SPanel(wxCommandEvent& event) {
    firstpanel->Hide();

    secondpanel = new wxPanel(this, wxID_ANY, wxPoint(1, 1), wxSize(1280, 720));
    secondpanel->SetBackgroundColour(bgcolor);

    wxStaticText* drag = new wxStaticText(secondpanel, wxID_ANY, "Drag a .txt file from your device into the box on the left to compress.", wxPoint(365, 600), wxSize(1280, 30));
    drag->SetFont(emefont);
    drag->SetForegroundColour(*wxLIGHT_GREY);
    wxStaticText* drag1 = new wxStaticText(secondpanel, wxID_ANY, "Drag a .bin file from your device into the box on the right to decompress.", wxPoint(355, 630), wxSize(1280, 30));
    drag1->SetFont(emefont);
    drag1->SetForegroundColour(*wxLIGHT_GREY);
    wxStaticText* title = new wxStaticText(secondpanel, wxID_ANY, "FILE COMPRESSOR AND DECOMPRESSOR", wxPoint(310, 70), wxSize(500,40));
    title->SetFont(titlefont);
    CreateFileButton = new wxButton(secondpanel, wxID_ANY, "Create Text File", wxPoint(435, 150), wxSize(400, 70));
    CreateFileButton->SetFont(buttonFont);
    CreateFileButton->Bind(wxEVT_BUTTON, &mainframe::Creatingfile, this);

    // Compress Drop Box
    wxStaticBox* staticBox = new wxStaticBox(secondpanel, wxID_ANY, "", wxPoint(230, 280), wxSize(350, 290));
    staticBox->SetBackgroundColour(bgcolorcompress);
    wxStaticText* compressLabel = new wxStaticText(secondpanel, wxID_ANY, "Drag files here to Compress", wxPoint(270, 300), wxDefaultSize, wxALIGN_LEFT);
    compressLabel->SetFont(AFont); 
    compressLabel->SetBackgroundColour(bgcolorfont); 
    compressBox = new wxListBox(secondpanel, wxID_ANY, wxPoint(255, 340), wxSize(300, 200));
    compressBox->SetDropTarget(new FileDropTarget(compressBox, "Compress"));

    // Decompress Drop Box
    wxStaticBox* staticBox1 = new wxStaticBox(secondpanel, wxID_ANY, "", wxPoint(680, 280), wxSize(350, 290));
    staticBox1->SetBackgroundColour(bgcolorcompress); 
    wxStaticText* decompressLabel = new wxStaticText(secondpanel, wxID_ANY, "Drag files here to Decompress", wxPoint(715, 300), wxDefaultSize, wxALIGN_LEFT);
    decompressLabel->SetFont(AFont);
    decompressLabel->SetBackgroundColour(bgcolorfont);
    decompressBox = new wxListBox(secondpanel, wxID_ANY, wxPoint(705, 340), wxSize(300, 200));
    decompressBox->SetDropTarget(new FileDropTarget(decompressBox, "Decompress"));

    
    wxButton* Ebutton = new wxButton(secondpanel, wxID_ANY, "Exit Program", wxPoint(1100, 660), wxSize(150, 50));
    Ebutton->SetBackgroundColour(*wxWHITE);
    Ebutton->SetForegroundColour(*wxBLACK);
    Ebutton->SetFont(buttonFont);
    Ebutton->Bind(wxEVT_ENTER_WINDOW, [Ebutton](wxMouseEvent&) {
        Ebutton->SetForegroundColour(*wxRED);
        Ebutton->Refresh();
        });
    Ebutton->Bind(wxEVT_LEAVE_WINDOW, [Ebutton](wxMouseEvent&) {
        Ebutton->SetForegroundColour(*wxBLACK);
        Ebutton->Refresh();
        });
    Ebutton->Bind(wxEVT_BUTTON, &mainframe::OnButton3Clicked, this);

   
}
void mainframe::OnButton3Clicked(wxCommandEvent& evt) {
    wxLogMessage("System Closing");
    Close(true); 
} 

void mainframe::Creatingfile(wxCommandEvent& event) {
    secondpanel->Hide();

    creatingtextpanel = new wxPanel(this, wxID_ANY, wxPoint(1,1), wxSize(1280,720));
    wxBoxSizer* textPanelSizer = new wxBoxSizer(wxVERTICAL);
    creatingtextpanel->SetBackgroundColour(bgcolor);


    wxStaticText* textEditorLabel = new wxStaticText(creatingtextpanel, wxID_ANY, "Enter your text below:",wxPoint(160,50), wxDefaultSize, wxALIGN_LEFT);
    textEditorLabel->SetFont(AFont1);

    textcontents = new wxTextCtrl(creatingtextpanel, wxID_ANY, "", wxPoint (160,90), wxSize(960, 500), wxTE_MULTILINE);
    textcontents->SetFont(AFont);  

    DoneButton = new wxButton(creatingtextpanel, wxID_ANY, "Done", wxPoint(1100,660), wxSize(150, 50));
    DoneButton->SetForegroundColour(*wxGREEN);
    DoneButton->Bind(wxEVT_BUTTON, &mainframe::DoneButtonF, this);

    textPanelSizer->Add(textEditorLabel, 0, wxALL | wxLEFT, 10);
    textPanelSizer->Add(textcontents, 1, wxEXPAND | wxALL, 10);
    textPanelSizer->Add(DoneButton, 0, wxALL | wxALIGN_RIGHT, 10);

    wxButton* returnbt = new wxButton(creatingtextpanel, wxID_ANY, "Return", wxPoint(930, 660), wxSize(150, 50)); 
    returnbt->SetForegroundColour(*wxBLACK); 
    returnbt->Bind(wxEVT_BUTTON, &mainframe::returnmain, this);

  
}

void mainframe::returnmain(wxCommandEvent& event) {
    creatingtextpanel->Hide();
    secondpanel->Show();
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
