#pragma once

#include <wx/wx.h>
#include <vector>
#include <chrono>
#include <stdexcept>
#include <functional>
#include "AnimatedValue.hpp"

class Animator: public wxEvtHandler
{
public:
  Animator();
  void SetAnimatedValue(const std::vector<AnimatedValue>& values);
  const std::vector<AnimatedValue>& GetAnimatedValues() const;
  void SetOnIter(const std::function<void()>& onIter);
  void SetOnStop(const std::function<void()>& onStop);
  void Start(double durationMs);
  void Stop();
  void Reset();
  bool IsRunning() const;

private:
  wxTimer timer;
  double animationDurationMs;
  std::vector<AnimatedValue> animatedValues;
  std::chrono::steady_clock::time_point startTime;

  std::function<void()> onIter;
  std::function<void()> onStop;

  void OnTimer(wxTimerEvent& event);
};