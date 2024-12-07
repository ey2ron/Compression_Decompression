    #pragma once
    #include <wx/wx.h>
    #include "wx/dnd.h"
    #include "wx/filedlg.h"
    #include "wx/listbox.h"
    #include <fstream>
    #include <string>
    #include "compressor.h"
    #include <wx/filename.h>
    
    class FileDropTarget : public wxFileDropTarget {
    public:
        huffmanAlg huff;
        FileDropTarget(wxListBox* listBox, const std::string& operationType)
            : m_listBox(listBox), m_operationType(operationType) {}

        virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) override {
            for (const auto& file : filenames) {
                m_listBox->Append(file);
                if (m_operationType == "Compress") {
                    CompressFile(file.ToStdString());
                }
                else if (m_operationType == "Decompress") {
                    DecompressFile(file.ToStdString());
                }

                int index = m_listBox->FindString(file);
                if (index != wxNOT_FOUND) {
                    m_listBox->Delete(index);
                }
            }
            return true;
        }

    private:
        wxListBox* m_listBox;
        std::string m_operationType;

        void CompressFile(const std::string& filePath) {
            // Placeholder logic for file compression
            huff.compressFile(filePath);
            wxLogMessage("Compressed: %s", filePath);

        }

        void DecompressFile(const std::string& filePath) {
          
            huff.decompressFile(filePath);
            wxLogMessage("Decompressed: %s", filePath);
        }

    };

    class mainframe : public wxFrame {
    public:
        mainframe(const wxString& title);
        huffmanAlg huff;
    private:
        void SPanel();
        void Creatingfile(wxCommandEvent& evt);
        void DoneButtonF(wxCommandEvent& evt);

        wxPanel* secondpanel;
        wxPanel* creatingtextpanel;
        wxButton* CreateFileButton;
        wxButton* DoneButton;
        wxTextCtrl* textcontents;
    
        wxListBox* compressBox;
        wxListBox* decompressBox;

        wxDECLARE_EVENT_TABLE();
    };
