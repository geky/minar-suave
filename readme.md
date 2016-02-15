
# MINAR _suave_

A [minar](https://github.com/ARMmbed/minar) for a simpler sort of programmer.

This is not intended to replace the existing interface or even match all of the minar's features. Instead, minar-suave attempts to provide a more palpable interface for the unsophisticated programmer.

## Usage

``` cpp
#include "minar-suave/minar-suave.h"
DigitalOut led1(LED1);

void app_start(int, char **) {
    minar::call_every(250, []() {
        led1 = !led1;
    });
}
```

minar-suave simply adds several functions to the minar namespace
- `call(function)` - Execute the callback as soon as possible.
- `call_in(milliseconds, function)` - Execute the callback after the specified time
- `call_every(milliseconds, function)` - Execute the callback periodically

minar-suave can handle any arguments accepted by [std::bind](http://en.cppreference.com/w/cpp/utility/functional/bind). This provides many possibilities for callbacks.
``` cpp
// Basic functions
void func() {
    printf("Hi!\n");
}

minar::call(func);

// Methods
struct Class {
    const char *data;
    void method() {
        printf(data);
    }
};

Class instance{"Hi!\n"};
minar::call(&Class::method, &instance);

// Lambdas
const char *data = "Hi!\n";
minar::call([=]() {
    printf(data);
}

// Functions with arguments
minar::call(printf, "Hi!\n");

```

minar-suave also supports canceling callbacks.
```
minar::handle_t handle = minar::call_in(10000, printf, "oops");
minar::cancel(handle);
```

## IRQ issues

Unfortunately, the flexibility of minar-suave comes at the cost of memory allocations at the bind site. This makes the call functions unsafe in interrupt contexts.

If you need to post to minar in an interrupt context, you should use [minar](https://github.com/ARMmbed/minar) directly.
