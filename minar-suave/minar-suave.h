#ifndef MINAR_SUAVE_H
#define MINAR_SUAVE_H

#include "minar/minar.h"
#include "mbed-drivers/CThunk.h"
#include <functional>

namespace minar {


// Easy access to cancelling callbacks
using handle_t = callback_handle_t;

void cancel(handle_t handle) {
    Scheduler::cancelCallback(handle);
}


// Posting from non-irq contexts
// memory allocations!
template <typename F, typename... As>
Scheduler::CallbackAdder post(F f, As... args) {
    auto bound = std::bind(f, args...);
    return Scheduler::postCallback(
        mbed::util::FunctionPointer1<void, decltype(bound)>(
            [](decltype(bound) f){ f(); }).bind(bound));
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

template <typename E, typename F, typename... As>
handle_t call_on(E event, F f, As... args) {
    auto bound = [=](){ std::bind(f, args...)(); };
    decltype(bound) *stored = new decltype(bound)(bound);
    CThunk<decltype(bound)> *thunk = new CThunk<decltype(bound)>(stored, 
            (void (decltype(bound)::*)())&decltype(bound)::operator());

    event((CThunkEntry)thunk->entry());

    return 0; // TODO
}

template <typename E, typename F, typename... As>
handle_t call_on(E *obj, void (E::*event)(void(*)()), F f, As... args) {
    return call_on([&](void (*f)()) { (obj->*event)(f); }, f, args...);
}


}

#endif
