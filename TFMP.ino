#include <TFMPlus.h> // Include TFMini Plus Library v1.5.0
TFMPlus tfmP;        // Create a TFMini Plus object

#include "printf.h" // Modified to support Intel based Arduino

void setup()
{
  Serial.begin(115200);                                  // Intialize terminal serial port
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
int16_t tfFlux = 0; // Strength or quality of return signal
int16_t tfTemp = 0; // Internal temperature of Lidar sensor chip

// Use the 'getData' function to pass back device data.
void loop()
{
  delay(50); // Loop delay to match the 20Hz data frame rate

  if (tfmP.getData(tfDist, tfFlux, tfTemp)) // Get data from the device.
  {
    if (tfDist <= 20) // Measure and set volume ref. to distance
    {
      // setvolume = 10;       // Set Volume ref. to distance --> Working on
      printf("Volume set 10"); // Print out distance to monitor for testing
      printf("\r\n");
    }
    else if (tfDist <= 23)
    {
      // setvolume = 16;
      printf("Volume set 16");
      printf("\r\n");
    }
    else if (tfDist <= 26)
    {
      // setvolume = 22;
      printf("Volume set 22");
      printf("\r\n");
    }
    else if (tfDist <= 29)
    {
      // setvolume = 28;
      printf("Volume set 28");
      printf("\r\n");
    }
    else if (tfDist <= 32)
    {
      // setvolume = 34;
      printf("Volume set 34");
      printf("\r\n");
    }
    else if (tfDist <= 35)
    {
      // setvolume = 40;
      printf("Volume set 40");
      printf("\r\n");
    }
    else if (tfDist <= 38)
    {
      // setvolume = 46;
      printf("Volume set 46");
      printf("\r\n");
    }
    else if (tfDist <= 41)
    {
      // setvolume = 52;
      printf("Volume set 52");
      printf("\r\n");
    }
    else if (tfDist <= 44)
    {
      // setvolume = 58;
      printf("Volume set 58");
      printf("\r\n");
    }
    else if (tfDist <= 47)
    {
      // setvolume = 64;
      printf("Volume set 64");
      printf("\r\n");
    }
    else if (tfDist <= 50)
    {
      // setvolume = 70;
      printf("Volume set 70");
      printf("\r\n");
    }
    else if (tfDist <= 50)
    {
      // setvolume = 70;
      printf("Volume set 70");
      printf("\r\n");
    }
    else if (tfDist <= 53)
    {
      // setvolume = 70;
      printf("Volume set 70");
      printf("\r\n");
    }
    else if (tfDist <= 56)
    {
      // setvolume = 82;
      printf("Volume set 82");
      printf("\r\n");
    }
    else if (tfDist <= 59)
    {
      // setvolume = 88;
      printf("Volume set 88");
      printf("\r\n");
    }
    else if (tfDist <= 62)
    {
      // setvolume = 94;
      printf("Volume set 94");
      printf("\r\n");
    }
    else if (tfDist <= 67)
    {
      // setvolume = 100;
      printf("Volume set 100");
      printf("\r\n");
    }
    /*
    printf("Dist:%04icm ", tfDist);    // display distance,
    printf("Flux:%05i ", tfFlux);      // display signal strength/quality,
    printf("Temp:%2i%s", tfTemp, "C"); // display temperature,
    printf("\r\n");                    // end-of-line.
    */
  }
  else // If the command fails...
  {
    tfmP.printFrame(); // display the error and HEX dataa
  }
}
