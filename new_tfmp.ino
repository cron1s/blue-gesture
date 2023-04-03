#include <TFMPlus.h>

const int LauterPin = 3;
const int LeiserPin = 4;

int letzterGueltigerWert = 0;

void setup()
{
    pinMode(LauterPin, OUTPUT);
    pinMode(LeiserPin, OUTPUT);
    Serial.begin(9600);
    tfmP.begin(TFMPLUS_115200);
}

void loop()
{
    int abstand = tfmP.readDistance();
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

    // Senden des Volumens über Bluetooth
    // Hier muss der entsprechende Code für das verwendete Bluetooth-Modul eingefügt werden
    // Zum Beispiel: Serial.write(volumen);

    analogWrite(LauterPin, 255 - volumen);
    analogWrite(LeiserPin, volumen);
    delay(10); // optional: um das Messen zu verlangsamen und Rauschen zu reduzieren
}
