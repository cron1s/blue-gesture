#include <TFMPlus.h>
#include <printf.h>

const int louderPort = 6;   // Digital pin for louder  //// Digital pin 2 (PWM) https://www.exp-tech.de/blog/arduino-mega-2560-pinbelegung
const int quieterPort = 7;  // Digital pin for quieter //// Digital pin 3 (PWM) https://www.exp-tech.de/blog/arduino-mega-2560-pinbelegung
const int maxDistance = 67; // Maximum valid distance
const int minDistance = 15; // Minimum valid distance

TFMPlus tfmP;

int lastValidDistance = 0; // The last valid distance
int lastVolume = 0;        // The last volume

void setup()
{
    pinMode(louderPort, OUTPUT);
    pinMode(quieterPort, OUTPUT);
    // Initialisiere den Seriellen Monitor
    Serial.begin(115200);

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

int16_t tfDist = 0;

void loop()
{
    int distance = tfmP.getData(tfDist); // Get distance in cm

    if (distance > maxDistance || distance < minDistance)
    {
        distance = lastValidDistance; // Replace invalid distance with last valid distance
    }
    else
    {
        lastValidDistance = distance; // Store the valid distance
    }

    int volume = map(distance, minDistance, maxDistance, 0, 255); // Map distance to volume
    volume = constrain(volume, 0, 255);                           // Limit volume to range of 0-255

    if (volume != lastVolume) // Volume has changed
    {
        if (volume > lastVolume)
        {                                    // Volume is getting louder
            digitalWrite(louderPort, LOW);   // Invert and pull to ground
            digitalWrite(quieterPort, HIGH); // No inversion and not pulled to ground
        }
        else
        {                                   // Volume is getting quieter
            digitalWrite(louderPort, HIGH); // No inversion and not pulled to ground
            digitalWrite(quieterPort, LOW); // Invert and pull to ground
        }
        lastVolume = volume; // Store the current volume
    }

    printf("Distance: ");
    printf(distance);
    printf(" cm, Volume: ");
    printf(volume);

    delay(5); // Pause between measurements
}
