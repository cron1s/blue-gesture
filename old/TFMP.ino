#include <TFMPlus.h>
#include "printf.h"

// Definiere die Pins für die Lautstärkeregelung
const int leiserPin = 3;
const int lauterPin = 5;

// Definiere die Zeitdauer, die eine Messung stabil sein muss, um weitergeleitet zu werden
const unsigned long stabileMesszeit = 5000; // 5 Sekunden

// Definiere den Spielraum, in dem Messungen als gleichwertig betrachtet werden
const float stabilitaet = 1.5; // cm

// Definiere die maximal und minimal zulässigen Entfernungen
const float minEntfernung = 20; // cm
const float maxEntfernung = 67; // cm

// Erstelle ein Objekt des Lidar Sensors
TFMPlus tfmP;

// Variablen zur Speicherung der letzten Messwerte und Messzeiten
float letzteEntfernung;
unsigned long letzteMesszeit;

void setup()
{
    // Initialisiere den Seriellen Monitor
    Serial.begin(115200);

    // Initialisiere die Pins für die Lautstärkeregelung
    pinMode(leiserPin, OUTPUT);
    pinMode(lauterPin, OUTPUT);

    delay(20);                                             // Give port time to initalize
    printf_begin();                                        // Initialize printf.
    printf("\r\nTFMPlus Library Example - 10SEP2021\r\n"); // say 'hello'

    Serial2.begin(115200); // Initialize TFMPLus device serial port.
    delay(20);             // Give port time to initalize
    tfmP.begin(&Serial2);  // Initialize device library object and...
                           // pass device serial port to the object.

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

    /*  // - - - - - - - - - - - - - - - - - - - - - - - -
        // The next two commands may be used to switch the device
        // into I2C mode.  This sketch will no longer receive UART
        // (serial) data.  The 'TFMPI2C_example' sketch in the
        // TFMPI2C Library can be used to switch the device back
        // to UART mode.
        // Don't forget to switch the cables, too.
        // - - - - - - - - - - - - - - - - - - - - - - - -
        printf( "Set I2C Mode: ");
        if( tfmP.sendCommand( SET_I2C_MODE, 0))
        {
            printf( "mode set.\r\n");
        }
        else tfmP.printReply();
        printf( "Save settings: ");
        if( tfmP.sendCommand( SAVE_SETTINGS, 0))
        {
            printf( "saved.\r\n");
        }
        else tfmP.printReply();
        // - - - - - - - - - - - - - - - - - - - - - - - -
    */

    delay(500); // And wait for half a second.
}

// Initialize variables
int16_t tfDist = 0; // Distance to object in centimeters

void loop()
{
    // Führe eine Messung durch
    float entfernung = tfmP.getData(tfDist);
    printf("Dist:%04icm ", tfDist);
    printf("\r\n");

    // Überprüfe, ob die Entfernung innerhalb der zulässigen Grenzen liegt
    if (entfernung >= minEntfernung && entfernung <= maxEntfernung)
    {

        // Überprüfe, ob die Messung stabil ist und ob sich der Wert geändert hat
        if (abs(entfernung - letzteEntfernung) <= stabilitaet && millis() - letzteMesszeit >= stabileMesszeit)
        {

            // Sende den Messwert an den Seriellen Monitor
            Serial.println(entfernung);

            // Leite den Messwert an die entsprechenden Pins weiter
            if (entfernung <= 30)
            {
                digitalWrite(leiserPin, HIGH);
                digitalWrite(lauterPin, LOW);
            }
            else if (entfernung >= 60)
            {
                digitalWrite(leiserPin, LOW);
                digitalWrite(lauterPin, HIGH);
            }
            else
            {
                digitalWrite(leiserPin, LOW);
                digitalWrite(lauterPin, LOW);
            }

            // Speichere den aktuellen Messwert und die Messzeit
            letzteEntfernung = entfernung;
            letzteMesszeit = millis();
        }
    }
}