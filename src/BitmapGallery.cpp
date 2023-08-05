#include "BitmapGallery.hpp"

BitmapGallery::BitmapGallery(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
  : wxWindow(parent, id, pos, size, wxFULL_REPAINT_ON_RESIZE | style)
{
  this->SetBackgroundStyle(wxBG_STYLE_PAINT);
  this->Bind(wxEVT_PAINT, &BitmapGallery::OnPaint, this);
}

void BitmapGallery::OnPaint(wxPaintEvent& event)
{
  wxAutoBufferedPaintDC dc(this);
  dc.Clear();

  wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
  if (gc && bitmaps.size() > 0)
  {

  }
}