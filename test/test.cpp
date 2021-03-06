
#include "minar-suave/minar-suave.h"

#include "mbed-drivers/mbed.h"

DigitalOut led1(LED1);
DigitalOut led2(LED_GREEN);
DigitalOut led3(LED_BLUE);

InterruptIn b1(SW2);
InterruptIn b2(SW3);


void test1() {
    led1 = !led1;
}

struct Test2 {
    void test2() {
        led2 = !led2;
    }
};

void test4(int, int, int, int) {
    led1 = !led1;
}

struct Test5 {
    void test5(int, int, int, int) {
        led2 = !led2;
    }
};


void app_start(int, char **) {
    Test2 t2;
    Test5 t5;

    minar::call(test1);
    minar::call(&Test2::test2, &t2);
    minar::call([](){ led3 = !led3; });
    minar::call(test4, 0, 1, 2, 3);
    minar::call(&Test5::test5, &t5, 0, 1, 2, 3);

    minar::call_in( 250, test1);
    minar::call_in( 500, &Test2::test2, &t2);
    minar::call_in( 750, [](){ led3 = !led3; });
    minar::call_in(1000, test4, 0, 1, 2, 3);
    minar::call_in(1250, &Test5::test5, &t5, 0, 1, 2, 3);

    minar::call_every( 250, test1);
    minar::call_every( 500, &Test2::test2, &t2);
    minar::call_every( 750, [](){ led3 = !led3; });
    minar::call_every(1000, test4, 0, 1, 2, 3);
    minar::call_every(1250, &Test5::test5, &t5, 0, 1, 2, 3);

    minar::call_every(1000, printf, "Hello world!\r\n");

    minar::call_on(&b1, &InterruptIn::rise, test1);
    minar::call_on(&b1, &InterruptIn::rise, &Test2::test2, &t2);
    minar::call_on(&b1, &InterruptIn::rise, [](){ led3 = !led3; });
    minar::call_on(&b1, &InterruptIn::rise, test4, 0, 1, 2, 3);
    minar::call_on(&b1, &InterruptIn::rise, &Test5::test5, &t5, 0, 1, 2, 3);

    minar::call_on(&b2, &InterruptIn::fall, printf, "Hi!\r\n");

    minar::handle_t handle1 = minar::call_every(500, printf, "Oops!\r\n");
    minar::cancel(handle1);

    minar::handle_t handle2 = minar::call_on(
            &b2, &InterruptIn::rise, printf, "Oh no!\r\n");
    minar::cancel(handle2);
}

