#include <SoftwareSerial.h>
#include <TFMPlus.h>
#include "printf.h"

const int LauterPin = 3;
const int LeiserPin = 4;
SoftwareSerial bluetooth(10, 11); // RX, TX
int letzterGueltigerWert = 0;

void setup()
{
    pinMode(LauterPin, OUTPUT);
    pinMode(LeiserPin, OUTPUT);

    Serial2.begin(115200);
    delay(20);
    tfmP.begin(&Serial2);

    bluetooth.begin(9600);

    // Send some example commands to the TFMini-Plus
    // - - Perform a system reset - - - - - - - - - - -
    printf("Soft reset: ");
    if (tfmP.sendCommand(SOFT_RESET, 0))
    {
        printf("passed.\r\n");
    }
    else
        tfmP.printReply();

    delay(500); // added to allow the System Rest enough time to complete

    // - - Display the firmware version - - - - - - - - -
    printf("Firmware version: ");
    if (tfmP.sendCommand(GET_FIRMWARE_VERSION, 0))
    {
        printf("%1u.", tfmP.version[0]); // print three single numbers
        printf("%1u.", tfmP.version[1]); // each separated by a dot
        printf("%1u\r\n", tfmP.version[2]);
    }
    else
        tfmP.printReply();
    // - - Set the data frame-rate to 20Hz - - - - - - - -
    printf("Data-Frame rate: ");
    if (tfmP.sendCommand(SET_FRAME_RATE, FRAME_20))
    {
        printf("%2uHz.\r\n", FRAME_20);
    }
    else
        tfmP.printReply();
    // - - - - - - - - - - - - - - - - - - - - - - - -
    if (tfmP.sendCommand(GET_FIRMWARE_VERSION, 0))
    {
        printf("%1u.", tfmP.version[0]); // print three single numbers
        printf("%1u.", tfmP.version[1]); // each separated by a dot
        printf("%1u\r\n", tfmP.version[2]);
    }
    else
        tfmP.printReply();
    // - - Set the data frame-rate to 20Hz - - - - - - - -
    printf("Data-Frame rate: ");
    if (tfmP.sendCommand(SET_FRAME_RATE, FRAME_20))
    {
        printf("%2uHz.\r\n", FRAME_20);
    }
    else
        tfmP.printReply();

    delay(500);
}

void loop()
{
    int abstand = tfmP.getData(tfDist);
    int volumen = map(abstand, 15, 67, 0, 255);

    if (abstand > 67)
    {
        volumen = letzterGueltigerWert;
    }
    else
    {
        letzterGueltigerWert = volumen;
    }

    Serial.print("Abstand: ");
    Serial.print(abstand);
    Serial.print("cm, Volumen: ");
    Serial.println(volumen);

    bluetooth.write(volumen); // Senden des Volumens über Bluetooth

    analogWrite(LauterPin, 255 - volumen);
    analogWrite(LeiserPin, volumen);
    // delay(10); // optional: um das Messen zu verlangsamen und Rauschen zu reduzieren
}
