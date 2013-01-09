/*
 * In this program we use an HttpClient object to send a request and store the
 * response, then we use a JsonStream object to extract useful information from
 * the response.
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
 * Generating requests and reading responses is handled by the HttpClient class.
 * To construct an HttpClient object, you need a Wifly object (to manage WLAN
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
 * Now that we a Wifly object, we can create an HttpClient object.
 */
HttpClient http(wifly);
/*
 * All the methods of HttpClient that involve sending or receiving data need a
 * buffer as an argument. Therefore we will create one before we start using our
 * HttpClient object.
 *
 * The buffer is just a fixed-size, statically allocated char array. It needs to
 * be large enough to hold the expected response, but it still needs to fit in
 * the small amount of memory available on the device. When we fetch the number
 * of likes on a Facebook page, the request will be 200-300 bytes long while the
 * body of the response will probably remain well under 200 bytes, so 2048
 * should be plenty.
 */
char buffer[2048];

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
     * webservice.
     */
     http.connect("graph.facebook.com");
     /*
      * To issue a GET request, we simply call the get() method of our
      * HttpClient object. We pass four arguments to this method:
      * - the address of the buffer so that it knows where to write its data
      * - the size of the buffer so that it knows how much it can write
      * - the host name
      * - the URL we wish to access on the server.
      *
      * The get() method will send the request and wait for the response. If it
      * receives a response, it will read its body into the buffer we provided
      * and return its length in bytes.
      */
     int nBytes = http.get(buffer, 2048, "graph.facebook.com", "/readiymate?fields=likes");
     /*
      * Now we can print the response we received to the Serial Monitor. Before
      * that, we make sure that we've actually received a response!
      */
     if (nBytes <= 0) {
         Serial.println(F("The response seems to be empty. Maybe an error occured?"));
     }
     else {
         Serial.println(buffer);
         /*
          * If everything is fine, the buffer contains something like this:
          *
          * {
          *     "likes": 214,
          *     "id": "233156223364511"
          * }
          *
          * We're interested in the value associated with the key "likes".
          * Fortunately, the JsonStream class is here to help us. We simply
          * create a JsonStream object
          * by passing the size and location of our buffer to the constructor.
          */
          JsonStream json(buffer, 2048);
          /*
           * The JsonStream class has several methods that are useful when
           * receiving data from an API that uses JSON. Here we only need to
           * call the getIntegerByName() method and pass it the appropriate key
           * as an argument.
           */
           int nLikes = json.getIntegerByName("likes");
           Serial.print(F("Number of likes: "));
           Serial.println(nLikes);
           /*
            * NOTE: If you wish to use the reaDIYmate board to access web APIs,
            * have a look at the example called "API", which demonstrates how to
            * use the API class to perform the same actions in a more concise
            * way.
            */
     }
}

void loop() {
}
