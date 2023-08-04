#include "App.hpp"
#include "Frame.hpp"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
  Frame* frame = new Frame(nullptr, wxID_ANY, "Image Carousel");
  frame->Show(true);

  return true;
}