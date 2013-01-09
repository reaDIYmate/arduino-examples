/*
 * In this program we use an Api object to send a request, receive the response
 * and extract useful information from it.
 *
 * The action to perform will be simple: connect to Facebook and retrieve the
 * number of Likes for the reaDIYmate page.
 */
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
 * Making calls to the API is managed by the Api class.
 * To construct an Api object, we need a Wifly object (to manage WLAN
 * connectivity).
 *
 * To create a Wifly object, you need to give the constructor the HardwareSerial
 * (i.e the UART) connected to the RN171 module on the board, as well as the pin
 * numbers used to control the Wifly.
 *
 * If you're using the reaDIYmate board, this line of code is always going to be
 * the same.
 */
Wifly wifly = Wifly(Serial1, 49, 74, 75, 76);
/*
 * The Api constructor also needs a buffer.
 *
 * The buffer is just a fixed-size, statically allocated char array. It needs to
 * be large enough to hold the expected response, but it still needs to fit in
 * the small amount of memory available on the device. When we fetch the number
 * of likes on a Facebook page, the request will be 200-300 bytes long while the
 * body of the response will probably remain well under 200 bytes, so 2048
 * should be plenty.
 */
char buffer[2048];
/*
 * Finally, the Api constructor requires two additional parameters: the host
 * name and the base URL (the "entry point" of the API). In order to minimize
 * the RAM footprint, Api objects use "PROGMEM" (i.e Flash-based) strings for
 * these parameters.
 */
const char API_HOST[] PROGMEM = "graph.facebook.com";
const char API_PATH[] PROGMEM = "/";
/*
 * Now that we a Wifly object, a buffer, a host and a path we can create our Api
 * object.
 */
Api api(wifly, buffer, 2048, API_HOST, API_PATH);

void setup() {
    /*
     * First, we'll initialize the communication with the Wifi module.
     */
    wifly.initialize();
    /*
     * Then we open the serial port for debug messages.
     */
    Serial.begin(9600);
    /*
     * Before we can send requests, we need to connect to the Facebook
     * webservice. Note that unlike HttpClient, the Api class doesn't require
     * you to repeat the host, since it stores it internally.
     */
     api.connect();
     /*
      * Sending a request and receiving a response is simpler aswell, although
      * the PSTR() macros make it look a bit messy - but hey, they mean more RAM
      * for the rest of the program!
      */
     int nBytes = api.call(PSTR("readiymate"), PSTR("field"), "likes");
     /*
      * Now we can print the response we received to the Serial Monitor. Before
      * that, we make sure we've actually received a response!
      */
     if (nBytes > 0) {
         Serial.print(F("Number of likes:"));
         Serial.println(api.getIntegerByName("likes"));
     }
     else {
         Serial.println(F("The response seems to be empty. Maybe an error occured?"));
     }
}

void loop() {
}
