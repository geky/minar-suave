#ifndef MINAR_SUAVE_H
#define MINAR_SUAVE_H

#include "minar/minar.h"
#include "mbed-drivers/CThunk.h"
#include <functional>

namespace minar {


// Access to callback lifetimes
typedef void *handle_t;

handle_t handle(std::function<void()> dtor);
bool cancel(handle_t handle);


// Posting from non-irq contexts
// memory allocations!
template <typename F, typename... As>
Scheduler::CallbackAdder post(F f, As... args) {
    return Scheduler::postCallback(
        mbed::util::FunctionPointer1<void, std::function<void()>>(
            [](std::function<void()> f){ f(); }).bind(
                std::bind(f, args...)));
}

template <typename F, typename... As>
handle_t call(F f, As... args) {
    return post(f, args...)
        .getHandle();
}

template <typename F, typename... As>
handle_t call_in(unsigned ms, F f, As... args) {
    return post(f, args...)
        .delay(milliseconds(ms))
        .tolerance(0)
        .getHandle();
}

template <typename F, typename... As>
handle_t call_every(unsigned ms, F f, As... args) {
    return post(f, args...)
        .period(milliseconds(ms))
        .tolerance(0)
        .getHandle();
}

template <typename E, typename F, typename... As>
handle_t call_on(E event, F f, As... args) {
    std::function<void()> *stored = new std::function<void()>(
            [=]() { std::bind(f, args...)(); });
    CThunk<std::function<void()>> *thunk = new CThunk<std::function<void()>>(
        stored, (void (std::function<void()>::*)())
            &std::function<void()>::operator());

    event((CThunkEntry)thunk->entry());

    return handle([=]() {
        event(0);
        delete thunk;
        delete stored;
    });
}

template <typename E, typename F, typename... As>
handle_t call_on(E *obj, void (E::*event)(void(*)()), F f, As... args) {
    return call_on([=](void (*f)()) { (obj->*event)(f); }, f, args...);
}


}

#endif
