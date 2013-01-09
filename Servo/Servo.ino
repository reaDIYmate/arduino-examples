/*
 * This program works exactly like the "Sweep" example of the official Arduino
 * Servo library.
 */
#include <Servo.h>
/*
 * First we create a Servo object.
 */
Servo servo;

void setup() {
    /*
     * On the reaDIYmate board, the servo is usually on pin 4.
     */
    servo.attach(4);
}

void loop() {
    /*
     * Each time we move the servo by one degree, we wait for 50 milliseconds in
     * order to achieve a smooth motion.
     */
    for (int i = 10; i < 150; i++) {
        servo.write(i);
        delay(50);
    }
    for (int i = 150; i > 10; i--) {
        servo.write(i);
        delay(50);
    }
}
