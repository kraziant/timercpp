#ifndef TIMERCPP_H__
#define TIMERCPP_H__

#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

namespace timer {
    class set_timeout {

        public:

            set_timeout(const std::function<void()>     &func,
                        const std::chrono::milliseconds &ms)
                : _active(true)
                , _delay(ms)
            {

#ifdef TIMER_DEBUG
                _created = std::chrono::system_clock::now();
#endif
                _thread = std::thread([=]() {
                    if (!_active.load())
                        return;
#ifdef TIMER_DEBUG
                    _prerun = std::chrono::system_clock::now();
#endif
                    std::this_thread::sleep_for(delay());
                    if (!_active.load())
                        return;
#ifdef TIMER_DEBUG
                    _runned = std::chrono::system_clock::now();
#endif
                    func();
                });
            }

            set_timeout(const set_timeout &st) = delete;
            set_timeout(set_timeout &&st) = default;

            set_timeout &operator=(const set_timeout &st) = delete;
            set_timeout &operator=(set_timeout &&st) = default;

            ~set_timeout()
            {
                _active = false;
                if (_thread.joinable()) {
                    _thread.join();
                }
#ifdef TIMER_DEBUG
                std::cout
                    << "real delays(us): "
                    << (std::chrono::duration_cast<std::chrono::microseconds>(
                            _prerun - _created)
                            .count() /
                        1000.0)
                    << " "

                    << (std::chrono::duration_cast<std::chrono::microseconds>(
                            _runned - _created)
                            .count() /
                        1000.0)
                    << "\n";
#endif
            }

        protected:

            inline std::chrono::milliseconds delay() { return _delay; }

        private:

            std::atomic<bool>         _active;
            std::thread               _thread;
            std::chrono::milliseconds _delay;
#ifdef TIMER_DEBUG
            std::chrono::time_point<std::chrono::system_clock> _created;
            std::chrono::time_point<std::chrono::system_clock> _prerun;
            std::chrono::time_point<std::chrono::system_clock> _runned;
#endif
    };
    class interval {

        public:

            interval(const std::function<void()>     &func,
                     const std::chrono::milliseconds &ms)
                : _active(true)
                , _delay(ms)
            {

#ifdef TIMER_DEBUG
                _created = std::chrono::system_clock::now();
#endif
                _thread = std::thread([=]() {
                    while (_active.load()) {
                        std::this_thread::sleep_for(delay());
                        if (!_active.load())
                            return;
                        func();
                    }
                });
            }

            interval(const interval &st) = delete;
            interval(interval &&st) = default;

            interval &operator=(const interval &st) = delete;
            interval &operator=(interval &&st) = default;

            ~interval()
            {
                _active = false;
                if (_thread.joinable()) {
                    _thread.join();
                }
#ifdef TIMER_DEBUG
                std::cout
                    << "real delays(us): "
                    << (std::chrono::duration_cast<std::chrono::microseconds>(
                            _prerun - _created)
                            .count() /
                        1000.0)
                    << " "

                    << (std::chrono::duration_cast<std::chrono::microseconds>(
                            _runned - _created)
                            .count() /
                        1000.0)
                    << "\n";
#endif
            }

        protected:

            inline std::chrono::milliseconds delay() { return _delay; }

        private:

            std::atomic<bool>         _active;
            std::thread               _thread;
            std::chrono::milliseconds _delay;
#ifdef TIMER_DEBUG
            std::chrono::time_point<std::chrono::system_clock> _created;
            std::chrono::time_point<std::chrono::system_clock> _prerun;
            std::chrono::time_point<std::chrono::system_clock> _runned;
#endif
    };

} // namespace timer

#endif
