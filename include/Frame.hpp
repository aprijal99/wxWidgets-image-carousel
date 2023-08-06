#pragma once

#include <wx/wx.h>
#include "BitmapGallery.hpp"

class Frame: public wxFrame
{
public:
  Frame(wxWindow* parent, wxWindowID id, const wxString& title);
//   ~Frame()
//   {
//     wxLog::SetActiveTarget(nullptr);
//     delete logger;
//   }

// private:
//   wxLog* logger;
};