#pragma once

#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include <vector>

enum class BitmapScalling: int
{
  Center = 0, Fit, FillWidth, FillHeight
};

class BitmapGallery: public wxWindow
{
public:
  BitmapGallery(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0);
  
  void DrawBitmaps(wxGraphicsContext* gc, const wxSize& drawSize);
  void DrawNavigationRect(wxGraphicsContext* gc, const wxRect& rect);
  void DrawArrow(wxGraphicsContext* gc, const wxRect& reactToCenterIn, double lineLength, double lineWidth, double rotationAngle);
  void DrawDots(wxGraphicsContext* gc, const wxSize& drawSize, int dotCount, int dotRadius, int dotSpacing);

  void AnimateToPrevious();
  void AnimateToNext();
  void StartAnimation(double offsetStart, double offsetTarget, int indexTarget);

  std::vector<wxBitmap> bitmaps;
  BitmapScalling scalling = BitmapScalling::Center;
private:
  bool shouldShowLeftArrow = false, shouldShowRightArrow = false;
  int selectedIndex;

  // Animator animator;
  double animationOffsetNormalized = 0;

  void OnPaint(wxPaintEvent& event);
  void OnKeyDown(wxKeyEvent& event);
  void OnLeftDown(wxMouseEvent& event);
  void OnMouseMove(wxMouseEvent& event);
  void OnMouseLeave(wxMouseEvent& event);

  wxSize NavigationRectSize();
  wxRect NavigationRectLeft();
  wxRect NavigationRectRight();
};