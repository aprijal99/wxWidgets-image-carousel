#pragma once

#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include <vector>
#include "Animator.hpp"
#include "AnimatedValue.hpp"

enum class BitmapScaling: int
{
  Center = 0, Fit, FillWidth, FillHeight
};

class BitmapGallery: public wxWindow
{
public:
  BitmapGallery(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
  ~BitmapGallery();
  
  void DrawBitmaps(wxGraphicsContext* gc, const wxSize& drawSize);
  void DrawNavigationRect(wxGraphicsContext* gc, const wxRect& rect);
  void DrawArrow(wxGraphicsContext* gc, const wxRect& rectToCenterIn, double lineLength, double lineWidth, double rotationAngle);
  void DrawDots(wxGraphicsContext* gc, const wxSize& drawSize, int dotCount, int dotRadius, int dotSpacing);

  void AnimateToPrevious();
  void AnimateToNext();
  void StartAnimation(double offsetStart, double offsetTarget, int indexTarget);

  std::vector<wxBitmap> bitmaps;
  BitmapScaling scaling = BitmapScaling::Center;
private:
  wxLog* logger;

  bool shouldShowLeftArrow = false, shouldShowRightArrow = false;
  int selectedIndex = 0;

  Animator animator;
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