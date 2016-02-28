#include "minar-suave/minar-suave.h"
#include <set>

namespace minar {


std::set<std::function<void()>*> handles;

handle_t handle(std::function<void()> dtor) {
    std::function<void()> *ptr = new std::function<void()>(dtor);
    handles.insert(ptr);
    return ptr;
}

bool cancel(handle_t handle) {
    if (Scheduler::cancelCallback(handle)) {
        return true;
    } 

    std::function<void()> *ptr = (std::function<void()>*)handle;

    if (handles.erase(ptr)) {
        (*ptr)();
        delete ptr;
        return true;
    }

    return false;
}


}
