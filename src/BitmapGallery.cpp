#include "BitmapGallery.hpp"

BitmapGallery::BitmapGallery(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
  : wxWindow(parent, id, pos, size, wxFULL_REPAINT_ON_RESIZE)
{
  logger = new wxLogStderr();
  wxLog::SetActiveTarget(logger);

  this->SetBackgroundStyle(wxBG_STYLE_PAINT);
  this->Bind(wxEVT_PAINT, &BitmapGallery::OnPaint, this);
  
  this->Bind(wxEVT_KEY_DOWN, &BitmapGallery::OnKeyDown, this);
  this->Bind(wxEVT_LEFT_DOWN, &BitmapGallery::OnLeftDown, this);
  this->Bind(wxEVT_LEFT_DCLICK, &BitmapGallery::OnLeftDown, this);
  this->Bind(wxEVT_MOTION, &BitmapGallery::OnMouseMove, this);
  this->Bind(wxEVT_LEAVE_WINDOW, &BitmapGallery::OnMouseLeave, this);
}

BitmapGallery::~BitmapGallery()
{
  wxLog::SetActiveTarget(nullptr);
  delete logger;
}

void BitmapGallery::OnPaint(wxPaintEvent& event)
{
  wxAutoBufferedPaintDC dc(this);
  dc.SetBackground(wxBrush(wxColour("#2c2828")));
  dc.Clear();

  wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
  if (gc)
  {
    const wxSize drawSize = GetClientSize();
    DrawBitmaps(gc, drawSize);

    double arrowLineLength = NavigationRectSize().GetWidth() * 2 / 3;
    double arrowLineWidth = FromDIP(5);

    if (shouldShowLeftArrow)
    {
      DrawNavigationRect(gc, NavigationRectLeft());
      DrawArrow(gc, NavigationRectLeft(), arrowLineLength, arrowLineWidth, 0);
    }

    if (shouldShowRightArrow)
    {
      DrawNavigationRect(gc, NavigationRectRight());
      DrawArrow(gc, NavigationRectRight(), arrowLineLength, arrowLineWidth, M_PI);
    }

    const int dotRadius = FromDIP(4);
    const int dotSpacing = FromDIP(5);
    const int dotCount = bitmaps.size();

    if (dotCount > 1) DrawDots(gc, drawSize, dotCount, dotRadius, dotSpacing);
  }

  if (gc) delete gc;
}

void BitmapGallery::DrawBitmaps(wxGraphicsContext* gc, const wxSize& drawSize)
{
  const wxGraphicsMatrix currentTransform = gc->GetTransform();
  const wxSize dipDrawSize = ToDIP(drawSize);

  gc->Translate(-FromDIP(dipDrawSize.GetWidth()) * selectedIndex, 0);

  if (animator.IsRunning())
  {
    gc->Translate(-FromDIP(dipDrawSize.GetWidth()) * animationOffsetNormalized, 0);
  }

  for (const wxBitmap& bitmap: bitmaps)
  {
    const wxSize bmpSize = bitmap.GetSize();
    double imageW = bmpSize.GetWidth();
    double imageH = bmpSize.GetHeight();

    if (scaling == BitmapScaling::Fit)
    {
      double scaleX = dipDrawSize.GetWidth() / imageW;
      double scaleY = dipDrawSize.GetHeight() / imageH;

      double scale = std::min(scaleX, scaleY);

      imageW *= scale;
      imageH *= scale;
    }
    else if (scaling == BitmapScaling::FillWidth)
    {
      double scaleX = dipDrawSize.GetWidth() / imageW;

      imageW *= scaleX;
      imageH *= scaleX;
    }
    else if (scaling == BitmapScaling::FillHeight)
    {
      double scaleY = dipDrawSize.GetHeight() / imageH;

      imageW *= scaleY;
      imageH *= scaleY;
    }

    double cellCenterX = dipDrawSize.GetWidth() / 2;
    double imageCenterX = imageW / 2;

    double cellCenterY = dipDrawSize.GetHeight() / 2;
    double imageCenterY = imageH / 2;

    double bitmapX = cellCenterX - imageCenterX;
    double bitmapY = cellCenterY - imageCenterY;

    gc->Clip(0, 0, FromDIP(dipDrawSize.GetWidth()), FromDIP(dipDrawSize.GetHeight()));
    gc->DrawBitmap(bitmap, FromDIP(bitmapX), FromDIP(bitmapY), FromDIP(imageW), FromDIP(imageH));

    gc->ResetClip();

    gc->Translate(FromDIP(dipDrawSize.GetWidth()), 0);
  }

  gc->SetTransform(currentTransform);
}

void BitmapGallery::DrawNavigationRect(wxGraphicsContext* gc, const wxRect& rect)
{
  gc->SetPen(*wxTRANSPARENT_PEN);
  gc->SetBrush(wxBrush(wxColour(255, 255, 255, 64)));

  gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());
}

void BitmapGallery::DrawArrow(wxGraphicsContext* gc, const wxRect& rectToCenterIn, double lineLength, double lineWidth, double rotationAngle)
{
  const wxGraphicsMatrix currentTransform = gc->GetTransform();
  const wxPoint rectCenter = rectToCenterIn.GetPosition() + rectToCenterIn.GetSize() / 2;

  gc->SetPen(wxPen(wxColour(255, 255, 255, 255), lineWidth));

  gc->Translate(rectCenter.x, rectCenter.y);
  gc->Rotate(-M_PI / 4);
  gc->Rotate(rotationAngle);
  gc->Translate(-lineLength / 4, -lineLength / 4);

  gc->StrokeLine(0, 0, lineLength, 0);
  gc->StrokeLine(0, 0, 0, lineLength);

  gc->SetTransform(currentTransform);
}

wxSize BitmapGallery::NavigationRectSize()
{
  return {FromDIP(30), GetClientSize().GetHeight()};
}

wxRect BitmapGallery::NavigationRectLeft()
{
  return {0, 0, NavigationRectSize().GetWidth(), NavigationRectSize().GetHeight()};
}

wxRect BitmapGallery::NavigationRectRight()
{
  return {GetClientSize().GetWidth() - NavigationRectSize().GetWidth(), 0, NavigationRectSize().GetWidth(), NavigationRectSize().GetHeight()};
}

void BitmapGallery::DrawDots(wxGraphicsContext* gc, const wxSize& drawSize, int dotCount, int dotRadius, int dotSpacing)
{
  const wxGraphicsMatrix currentTransform = gc->GetTransform();
  const int dotsWidth = dotCount * dotRadius * 2 + (dotCount - 1) * dotSpacing;

  gc->Translate(-dotsWidth / 2, -dotRadius);
  gc->Translate(drawSize.GetWidth() / 2, drawSize.GetHeight() - dotRadius * 4);

  for (int i = 0; i < dotCount; i++)
  {
    gc->SetPen(*wxTRANSPARENT_PEN);

    if (i == selectedIndex) gc->SetBrush(wxBrush(wxColour(255, 255, 255, 255)));
    else gc->SetBrush(wxBrush(wxColour(255, 255, 255, 64)));

    gc->DrawEllipse(0, 0, dotRadius * 2, dotRadius * 2);
    gc->Translate(dotRadius * 2 + dotSpacing, 0);
  }

  gc->SetTransform(currentTransform);
}

void BitmapGallery::OnKeyDown(wxKeyEvent& event)
{
  if (event.GetKeyCode() == WXK_LEFT) AnimateToPrevious();
  else if (event.GetKeyCode() == WXK_RIGHT) AnimateToNext();
  else event.Skip();
}

void BitmapGallery::OnLeftDown(wxMouseEvent& event)
{
  if (shouldShowLeftArrow && NavigationRectLeft().Contains(event.GetPosition())) AnimateToPrevious();
  else if (shouldShowRightArrow && NavigationRectRight().Contains(event.GetPosition())) AnimateToNext();
  else event.Skip();
}

void BitmapGallery::OnMouseMove(wxMouseEvent& event)
{
  if (bitmaps.size() == 0) return;
  else if (NavigationRectLeft().Contains(event.GetPosition()) && selectedIndex != 0)
  {
    shouldShowLeftArrow = true;
    Refresh();
  }
  else if (NavigationRectRight().Contains(event.GetPosition()) && selectedIndex != bitmaps.size() - 1)
  {
    shouldShowRightArrow = true;
    Refresh();
  }
  else
  {
    shouldShowLeftArrow = false;
    shouldShowRightArrow = false;
    Refresh();
  }
}

void BitmapGallery::OnMouseLeave(wxMouseEvent& event)
{
  shouldShowLeftArrow = false;
  shouldShowRightArrow = false;
  Refresh();
}

void BitmapGallery::AnimateToPrevious()
{
  if (animator.IsRunning()) return;
  if (selectedIndex <= 0) return;

  StartAnimation(0.0, -1.0, selectedIndex - 1);
}

void BitmapGallery::AnimateToNext()
{
  if (animator.IsRunning()) return;
  if (selectedIndex >= bitmaps.size() - 1) return;

  StartAnimation(0.0, 1.0, selectedIndex + 1);
}

void BitmapGallery::StartAnimation(double offsetStart, double offsetTarget, int indexTarget)
{
  AnimatedValue xOffset = {
    offsetStart, offsetTarget,
    [this](double value) { animationOffsetNormalized = value; },
    AnimatedValue::EaseInOutCubic
  };

  animator.SetAnimatedValues({ xOffset });
  animator.SetOnIter([this]() { Refresh(); });
  animator.SetOnStop([this, indexTarget]() {
    if (indexTarget == 0) shouldShowLeftArrow = false;
    if (indexTarget == bitmaps.size() - 1) shouldShowRightArrow = false;

    selectedIndex = indexTarget;
    animationOffsetNormalized = 0;
    Refresh();
  });

  animator.Start(200);
}