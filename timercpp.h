#ifndef TIMERCPP_H__
#define TIMERCPP_H__

#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

class Timer {
  std::atomic<bool> active{true};

public:
  void setTimeout(std::function<void()> function, int delay);
  void setInterval(std::function<void()> function, int interval);
  void stop();
};

#endif // TIMERCPP_H__

#ifndef TIMERCPP_IMPLEMENTATION_H__
#define TIMERCPP_IMPLEMENTATION_H__

void Timer::setTimeout(std::function<void()> function, int delay) {
  active = true;
  std::thread t([=]() {
    if (!active.load())
      return;
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    if (!active.load())
      return;
    function();
  });
  t.detach();
}

void Timer::setInterval(std::function<void()> function, int interval) {
  active = true;
  std::thread t([=]() {
    while (active.load()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(interval));
      if (!active.load())
        return;
      function();
    }
  });
  t.detach();
}

void Timer::stop() { active = false; }

#endif // TIMERCPP_IMPLEMENTATION_H__
