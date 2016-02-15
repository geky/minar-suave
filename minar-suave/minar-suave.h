#ifndef MINAR_SUAVE_H
#define MINAR_SUAVE_H

#include "minar/minar.h"
#include <functional>

namespace minar {

// Posting from non-irq contexts
// memory allocations!
void _thunk(std::function<void()> f) { f(); }
template <typename F, typename... As>
Scheduler::CallbackAdder post(F f, As... args) {
    return Scheduler::postCallback(
        mbed::util::FunctionPointer1<void, std::function<void()>>(
            _thunk).bind(std::bind(f, args...)));
}

template <typename... Ts>
void call(Ts... ts) {
    post(ts...);
}

template <typename... Ts>
void call_in(unsigned ms, Ts... ts) {
    post(ts...)
        .delay(milliseconds(ms))
        .tolerance(0);
}

template <typename... Ts>
void call_every(unsigned ms, Ts... ts) {
    post(ts...)
        .period(milliseconds(ms))
        .tolerance(0);
}


// Posting from irq contexts
// nonreentrant!
template <typename F, typename... As>
static Scheduler::CallbackAdder post_nonreentrant(F f, As... args) {
    static auto cb = [=]() { std::bind(f, args...)(); };
    return Scheduler::postCallback(
        &cb, (void (decltype(cb)::*)())&decltype(cb)::operator());
}
        
template <typename... Ts>
static void call_nonreentrant(Ts... ts) {
    post(ts...);
}

template <typename... Ts>
static void call_in_nonreentrant(unsigned ms, Ts... ts) {
    post(ts...)
        .delay(milliseconds(ms))
        .tolerance(0);
}

template <typename... Ts>
static void call_every_nonreentrant(unsigned ms, Ts... ts) {
    post(ts...)
        .period(milliseconds(ms))
        .tolerance(0);
}

}

#endif
