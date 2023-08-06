#include "Frame.hpp"

Frame::Frame(wxWindow* parent, wxWindowID id, const wxString& title)
  : wxFrame(parent, id, title)
{
  // logger = new wxLogStderr();
  // wxLog::SetActiveTarget(logger);

  this->SetMinSize(wxSize(FromDIP(400), FromDIP(250)));
  this->Centre();

  // wxLogMessage("%d, %d", GetClientSize().GetHeight(), GetClientSize().GetWidth()); // The log message is shown in DEBUG CONSOLE (run as debugging)
}