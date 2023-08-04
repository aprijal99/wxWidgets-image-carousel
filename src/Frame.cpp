#include "Frame.hpp"

Frame::Frame(wxWindow* parent, wxWindowID id, const wxString& title)
  : wxFrame(parent, id, title)
{
  this->SetMinSize(wxSize(FromDIP(400), FromDIP(250)));
}