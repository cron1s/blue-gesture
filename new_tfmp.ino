#include <TFMPlus.h>
#include <printf.h>

const int louderPort = 6;   // Digitaler Pin für lauter
const int quieterPort = 7;  // Digitaler Pin für leiser
const int maxDistance = 67; // Maximale gültige Entfernung
const int minDistance = 15; // Minimale gültige Entfernung

TFMPlus tfmP;

int lastValidDistance = 0; // Die zuletzt gültige Entfernung
int lastVolume = 0;        // Die zuletzt eingestellte Lautstärke

void setup()
{
    pinMode(louderPort, OUTPUT);
    pinMode(quieterPort, OUTPUT);
    Serial.begin(115200); // Initialisiere den Seriellen Monitor

    delay(20);                                             // Gib dem Port Zeit zur Initialisierung
    printf_begin();                                        // Initialisiere printf.
    printf("\r\nTFMPlus Library Example - 10SEP2021\r\n"); // Sage 'Hallo'

    Serial2.begin(115200); // Initialisiere die serielle Schnittstelle des TFMPlus Geräts.
    delay(20);             // Gib dem Port Zeit zur Initialisierung
    tfmP.begin(&Serial2);  // Initialisiere das Gerätebibliotheksobjekt und...
                           // übergebe die serielle Schnittstelle des Geräts an das Objekt.

    // Sende einige Beispielsbefehle an den TFMini-Plus
    // - - Führe einen Systemreset durch - - - - - - - - - - -
    printf("Soft reset: ");
    if (tfmP.sendCommand(SOFT_RESET, 0))
    {
        printf("erfolgreich.\r\n");
    }
    else
        tfmP.printReply();

    delay(500); // Wartezeit, um dem Systemreset genügend Zeit zum Abschluss zu geben

    // - - Zeige die Firmware-Version an - - - - - - - - -
    printf("Firmware-Version: ");
    if (tfmP.sendCommand(GET_FIRMWARE_VERSION, 0))
    {
        printf("%1u.", tfmP.version[0]); // Gib drei einzelne Zahlen aus
        printf("%1u.", tfmP.version[1]); // jeweils durch einen Punkt getrennt
        printf("%1u\r\n", tfmP.version[2]);
    }
    else
        tfmP.printReply();
    // - - Setze die Daten-Framerate auf 20Hz - - - - - - - -
    printf("Daten-Framerate: ");
    if (tfmP.sendCommand(SET_FRAME_RATE, FRAME_20))
    {
        printf("%2uHz.\r\n", FRAME_20);
    }
    else
        tfmP.printReply();

    delay(500); // Wartezeit von einer halben Sekunde.
}

int16_t tfDist = 0; // Entfernungswert vom TFMPlus Sensor

void changevolume(int prozent, bool quiet)
{
    if (quiet)
    {
        pinMode(quieterPort, OUTPUT); // Setze den Pin für leiser als OUTPUT
        pinMode(louderPort, INPUT);   // Setze den Pin für lauter als INPUT
    }
    else
    {
        pinMode(louderPort, OUTPUT); // Setze den Pin für lauter als OUTPUT
        pinMode(quieterPort, INPUT); // Setze den Pin für leiser als INPUT
    }
    delay(350); // Pause von 350ms (relativ)

    pinMode(louderPort, INPUT);  // Setze den Pin für lauter zurück auf INPUT
    pinMode(quieterPort, INPUT); // Setze den Pin für leiser zurück auf INPUT
}

void loop()
{
    tfmP.getData(tfDist);  // Lese Entfernungswert in cm vom TFMPlus Sensor
    int distance = tfDist; // Speichere den Entfernungswert in einer lokalen Variable

    if (distance > maxDistance || distance < minDistance)
    {
        distance = lastValidDistance; // Ersetze ungültigen Entfernungswert mit dem letzten gültigen Entfernungswert
    }
    else
    {
        lastValidDistance = distance; // Speichere den gültigen Entfernungswert als letzten gültigen Entfernungswert
    }

    int volume = map(distance, minDistance, maxDistance, 0, 255); // Mappe den Entfernungswert auf den Volumenwert
    volume = constrain(volume, 0, 255);                           // Begrenze den Volumenwert auf den Bereich von 0-255

    if (volume != lastVolume) // Wenn sich das Volumen geändert hat
    {
        int diff = (lastVolume - volume);
        if (diff < 0)
            diff *= -1;
        if (volume > lastVolume)
        {
            changevolume(diff, true); // Wenn das Volumen erhöht wurde, rufe die Funktion changevolume mit "true" auf
        }
        else
        {
            changevolume(diff, false); // Wenn das Volumen verringert wurde, rufe die Funktion changevolume mit "false" auf
        }
        lastVolume = volume; // Speichere das aktuelle Volumen als letztes Volumen
    }

    printf("Entfernung: %d cm, Volumen: %d\n", distance, volume, tfDist);

    delay(5); // Pause zwischen den Messungen
}
