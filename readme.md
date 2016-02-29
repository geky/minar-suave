
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

minar-suave simply adds several functions to the minar namespace:
``` cpp
// Execute the callback as soon as possible
minar::call(func);

// Execute the callback after a specified time in milliseconds
minar::call_in(1000, func);

// Execute the callback periodically in milliseconds
minar::call_every(1000, func);

// Execute the callback on an external event
minar::call_on(&button, &InterruptIn::rise, func);
```

minar-suave can handle any arguments accepted by [std::bind](http://en.cppreference.com/w/cpp/utility/functional/bind). This provides many possibilities for callbacks:
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

minar-suave also supports canceling callbacks:
``` cpp
minar::handle_t handle = minar::call_in(10000, printf, "oops");
minar::cancel(handle);
```

## Issues

- If non-trivial bind arguments are used, minar-suave may perform memory
  allocations at the bind site. This can be problematic in interrupt contexts.

- minar-suave uses several features of C++11 including std::function and
  variadic templates. This can prevent compilation on non-compliant compilers.

If these issues are problematic, you should probably just be using 
[minar](https://github.com/ARMmbed/minar) directly.
