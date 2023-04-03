#include <TFMPlus.h>

const int LauterPin = 9;         // Pin, um den Lauter-Kanal des Bluetooth-Moduls zu steuern
const int LeiserPin = 10;        // Pin, um den Leiser-Kanal des Bluetooth-Moduls zu steuern
const int MinDistance = 15;      // Mindestabstand in Zentimeter, um die Lautstärke auf 0 zu setzen
const int MaxDistance = 67;      // Maximalabstand in Zentimeter, um die Lautstärke auf 255 zu setzen
const int DefaultVolume = 128;   // Standardlautstärke, falls der Lidar-Sensor keine gültigen Daten liefert
const int InvalidDistance = 100; // Ungültiger Abstandswert, wenn der Lidar-Sensor kein Signal liefert

TFMPlus tfmP;

void setup()
{
    pinMode(LauterPin, OUTPUT);
    pinMode(LeiserPin, OUTPUT);
    digitalWrite(LauterPin, HIGH); // Invertieren des Outputs, um das Bluetooth-Modul anzusteuern
    digitalWrite(LeiserPin, HIGH); // Invertieren des Outputs, um das Bluetooth-Modul anzusteuern

    tfmP.begin();
}

void loop()
{
    int distance = tfmP.getDistance(LidarPin);

    if (distance > 0 && distance < InvalidDistance)
    {
        int volume = map(distance, MinDistance, MaxDistance, 0, 255);
        volume = constrain(volume, 0, 255);

        analogWrite(LauterPin, 255 - volume); // Invertieren des Outputs, um das Bluetooth-Modul anzusteuern
        analogWrite(LeiserPin, 255 - volume); // Invertieren des Outputs, um das Bluetooth-Modul anzusteuern
    }
    delay(100); // Kurze Verzögerung, um den Lidar-Sensor nicht zu überfordern
}
