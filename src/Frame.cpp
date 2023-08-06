#include "Frame.hpp"

Frame::Frame(wxWindow* parent, wxWindowID id, const wxString& title)
  : wxFrame(parent, id, title)
{
  // logger = new wxLogStderr();
  // wxLog::SetActiveTarget(logger);

  wxInitAllImageHandlers();

  BitmapGallery* gallery = new BitmapGallery(this);
  gallery->scaling = BitmapScaling::FillWidth;

  wxMenu* menuFile = new wxMenu;
  menuFile->Append(wxID_OPEN, "&Open...\tCtrl-O", "Open a file");

  wxMenuBar* menuBar = new wxMenuBar;
  menuBar->Append(menuFile, "&File");

  this->SetMenuBar(menuBar);

  this->Bind(
    wxEVT_MENU,
    [=](wxCommandEvent& event) {
      wxFileDialog openFileDialog(this, "Open image", "", "", "Image files (*.png;*.jpeg;*.jpg)|*.png;*.jpeg;*.jpg", wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
      if (openFileDialog.ShowModal() == wxID_CANCEL) return;

      wxArrayString paths;
      openFileDialog.GetPaths(paths);

      std::vector<wxBitmap> bitmaps;
      for (wxString& path: paths)
      {
        bitmaps.push_back({wxImage(path)});
      }

      gallery->bitmaps = bitmaps;
      gallery->Refresh();
    },
    wxID_OPEN
  );

  this->SetSize(wxSize(FromDIP(800), FromDIP(700)));
  this->Centre();

  // wxLogMessage("App run successfully"); // The log message is shown in DEBUG CONSOLE (run as debugging)
}