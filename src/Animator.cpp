#include "Animator.hpp"

Animator::Animator()
{
  timer.SetOwner(this);
  this->Bind(wxEVT_TIMER, &Animator::OnTimer, this);
}

void Animator::OnTimer(wxTimerEvent& event)
{
  auto now = std::chrono::steady_clock::now();
  auto elapsedMs = std::chrono::duration_cast<std::chrono::microseconds>(now - startTime).count();

  if (elapsedMs >= animationDurationMs)
  {
    Stop();
    return;
  }

  double tNorm = elapsedMs / animationDurationMs;

  for (AnimatedValue& value: animatedValues)
  {
    double callbackValue = value.easingFunction(value.startValue, value.endValue, tNorm);
    value.onValueChanged(callbackValue);
  }

  onIter();
}

void Animator::SetAnimatedValue(const std::vector<AnimatedValue>& values)
{
  animatedValues = values;
}

const std::vector<AnimatedValue>& Animator::GetAnimatedValues() const
{
  return animatedValues;
}

void Animator::SetOnIter(const std::function<void()>& onIter)
{
  this->onIter = onIter;
}

void Animator::SetOnStop(const std::function<void()>& onStop)
{
  this->onStop = onStop;
}

void Animator::Start(double durationMs)
{
  if (animatedValues.empty()) throw std::runtime_error("No animated values");
  if (durationMs <= 0) throw std::runtime_error("Duration must be positive");

  animationDurationMs = durationMs;
  startTime = std::chrono::steady_clock::now();

  timer.Start(10);
}

void Animator::Stop()
{
  timer.Stop();
  onStop();
}

void Animator::Reset()
{
  for (AnimatedValue& value: animatedValues)
  {
    value.onValueChanged(value.startValue);
  }
}

bool Animator::IsRunning() const
{
  return timer.IsRunning();
}