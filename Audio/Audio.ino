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
 * The Audio class allows us to play MP3 files. It can read data from a file on
 * the SD card, and send it to the MP3 chip on the reaDIYmate board. The MP3
 * chip will automatically decode and play incoming data through the speaker.
 *
 * First we create an SdFat object, which is used to access the file system on
 * the SD card.
 */
SdFat sd;
/*
 * We have to pass the Audio constructor the pin numbers used to communicate
 * with the MP3 chip (4 pins) and to control the SD card (1 pin).
 * On the reaDIYmate board the pinout is as follows:
 * - DREQ on pin 70
 * - XCS on pin 71
 * - XDCS on pin 72
 * - XRESET on pin 73
 * - SD card chip select on pin 77
 */
Audio audio(70, 73, 72, 71, sd, 77);

void setup() {
    /*
     * Now we simply call the play() method of our Audio object and provide the
     * name of an MP3 file.
     *
     * The file name must follow the so-called "8.3" convention: at most eight
     * characters, optionally followed by a period "." and a filename extension
     * of at most three characters; all characters are uppercase.
     */
    audio.play("THE_FILE.MP3");
}

void loop() {
}
