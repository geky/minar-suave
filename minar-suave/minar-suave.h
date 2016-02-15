#ifndef MINAR_SUAVE_H
#define MINAR_SUAVE_H

#include "minar/minar.h"
#include <functional>

namespace minar {


// Easy access to cancelling callbacks
using handle_t = callback_handle_t;

void cancel(handle_t handle) {
    Scheduler::cancelCallback(handle);
}


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
handle_t call(Ts... ts) {
    return post(ts...)
        .getHandle();
}

template <typename... Ts>
handle_t call_in(unsigned ms, Ts... ts) {
    return post(ts...)
        .delay(milliseconds(ms))
        .tolerance(0)
        .getHandle();
}

template <typename... Ts>
handle_t call_every(unsigned ms, Ts... ts) {
    return post(ts...)
        .period(milliseconds(ms))
        .tolerance(0)
        .getHandle();
}


}

#endif
