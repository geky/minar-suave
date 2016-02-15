
#include "minar-suave/minar-suave.h"

#include "mbed-drivers/mbed.h"

DigitalOut led1(LED_RED);
DigitalOut led2(LED_GREEN);
DigitalOut led3(LED_BLUE);

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

    minar::call_nonreentrant(test1);
    minar::call_nonreentrant(&Test2::test2, &t2);
    minar::call_nonreentrant([](){ led3 = !led3; });
    minar::call_nonreentrant(test4, 0, 1, 2, 3);
    minar::call_nonreentrant(&Test5::test5, &t5, 0, 1, 2, 3);

    minar::call_in_nonreentrant(2250, test1);
    minar::call_in_nonreentrant(2500, &Test2::test2, &t2);
    minar::call_in_nonreentrant(2750, [](){ led3 = !led3; });
    minar::call_in_nonreentrant(3000, test4, 0, 1, 2, 3);
    minar::call_in_nonreentrant(3250, &Test5::test5, &t5, 0, 1, 2, 3);

    minar::call_every_nonreentrant(2250, test1);
    minar::call_every_nonreentrant(2500, &Test2::test2, &t2);
    minar::call_every_nonreentrant(2750, [](){ led3 = !led3; });
    minar::call_every_nonreentrant(3000, test4, 0, 1, 2, 3);
    minar::call_every_nonreentrant(3250, &Test5::test5, &t5, 0, 1, 2, 3);
}

