#include <TFMPlus.h>

const int louderPort = 7;   // Digital pin for louder
const int quieterPort = 8;  // Digital pin for quieter
const int maxDistance = 67; // Maximum valid distance
const int minDistance = 15; // Minimum valid distance

TFMPlus tfmP;

int lastValidDistance = 0; // The last valid distance
int lastVolume = 0;        // The last volume

void setup()
{
    pinMode(louderPort, OUTPUT);
    pinMode(quieterPort, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    int distance = tfmP.getDistance(); // Get distance in cm

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

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(" cm, Volume: ");
    Serial.println(volume);

    delay(5); // Pause between measurements
}
