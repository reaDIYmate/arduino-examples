#include <Api.h>
#include <Audio.h>
#include <BufferedStream.h>
#include <digitalWriteFast.h>
#include <Download.h>
#include <ExtendedStream.h>
#include <HttpClient.h>
#include <JsonStream.h>
#include <PusherClient.h>
#include <SerialStream.h>
#include <Sha256.h>
#include <StatusLed.h>
#include <Vs1011.h>
#include <Wifly.h>
#include "SdFat.h"
/*
 * Interaction with the Pusher service is managed by the PusherClient class.
 * To construct a PusherClient object, you need a buffer (to store the data you
 * will retrieve from Pusher), and a Wifly object (to manage WLAN connectivity).
 *
 * The buffer is just a fixed-size, statically allocated char array.
 */
char buffer[2048];
/*
 * To create a Wifly object, you need to give the constructor the HardwareSerial
 * (i.e the UART) connected to the RN171 module on the board, as well as the pin
 * numbers used to control the Wifly.
 *
 * If you're using the reaDIYmate board, this line of code is always going to be
 * the same.
 */
Wifly wifly = Wifly(Serial1, 49, 74, 75, 76);
/*
 * Now that we have declared both the buffer and the Wifly object, we can create
 * a PusherClient object:
 */
PusherClient pusher(wifly, buffer, 2048);

void setup() {
    /*
     * Since we're going to use the serial port to print some messages we have
     * to initialize the Serial object.
     */
    Serial.begin(115200);
    /*
     * We'll also use the green LED. On the reaDIYmate board the corresponding
     * pin is number 78.
     */
     pinMode(78, OUTPUT);
    /*
     * The connection is handled here in the setup() function.
     * First, we'll initialize the communication with the Wifi module:
     */
    wifly.initialize();
    /*
     * Then we'll setup the PusherClient object. In order to connect to a public
     * channel, we only need to provide two things:
     * - the name of the channel we want our program to connect to
     * - the public API key of our Pusher application
     */
    pusher.setChannel("theChannel");
    pusher.setKey("37b2a3d0703782f40a52");
    /*
     * Now we simply call the connect() method to open a websocket and suscribe
     * to the channel we just set up:
     */
    pusher.connect();
    pusher.subscribe();
}

void loop() {
    /*
     * Here we call the hasNextEvent() method on the PusherClient object to
     * check for Pusher messages.
     */
    if (pusher.hasNextEvent()) {
        /*
         * When we receive a message, we look for a value named "value".
         */
        int value = pusher.getIntegerByName("value");
        if (value >= 0) {
            /* If the value in the message is greater than 0 we:
             * - send a message through the serial port
             * - turn the LED on for 1 second.
             */
            Serial.println("Message sent!");
            digitalWrite(78, HIGH);
            delay(1000);
            digitalWrite(78, LOW);
        }
    }
}
