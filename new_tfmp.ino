#include <TFMPlus.h>
#include <printf.h>

const int louderPort = 6;   // Digitaler Pin für lauter //// Digitaler Pin 2 (PWM) https://www.exp-tech.de/blog/arduino-mega-2560-pinbelegung
const int quieterPort = 7;  // Digitaler Pin für leiser //// Digitaler Pin 3 (PWM) https://www.exp-tech.de/blog/arduino-mega-2560-pinbelegung
const int maxDistance = 67; // Maximale gültige Entfernung
const int minDistance = 15; // Minimale gültige Entfernung
TFMPlus tfmP;
int lastValidDistance = 0; // Die letzte gültige Entfernung
int lastVolume = 0;        // Die letzte Lautstärke

void setup()
{
    pinMode(louderPort, OUTPUT);
    pinMode(quieterPort, OUTPUT);
    // Initialisiere den Seriellen Monitor
    Serial.begin(115200);
    delay(20);                                              // Gib dem Port Zeit zur Initialisierung
    printf_begin();                                         // Initialisiere printf.
    printf("\r\nTFMPlus Library Beispiel - 10SEP2021\r\n"); // Sende 'Hallo'
    Serial2.begin(115200);                                  // Initialisiere die serielle Verbindung zum TFMPlus Gerät.
    delay(20);                                              // Gib dem Port Zeit zur Initialisierung
    tfmP.begin(&Serial2);                                   // Initialisiere das Geräte-Library-Objekt und...
                                                            // übergebe die serielle Verbindung des Geräts an das Objekt.
    // Sende einige Beispielbefehle an den TFMini-Plus
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
        printf("%1u.", tfmP.version[0]); // Drucke drei einzelne Zahlen
        printf("%1u.", tfmP.version[1]); // getrennt durch Punkte
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
int16_t tfDist = 0;
void changevolume(int prozent, bool quiet)
{
    if (quiet)
    {
        pinMode(quieterPort, OUTPUT); // Setze quieterPort als Ausgang
        pinMode(louderPort, INPUT);   // Setze louderPort als Eingang
    }
    else
    {
        pinMode(louderPort, OUTPUT); // Setze louderPort als Ausgang
        pinMode(quieterPort, INPUT); // Setze quieterPort als Eingang
    }
    delay(quiet * 70); // Verzögerung basierend auf dem Wert von quiet
}

void loop()
{
    tfmP.getData(tfDist); // Hole die Entfernung in cm
    int distance = tfDist;
    if (distance > maxDistance || distance < minDistance)
    {
        distance = lastValidDistance; // Ersetze ungültige Entfernung mit der letzten gültigen Entfernung
    }
    else
    {
        lastValidDistance = distance; // Speichere die gültige Entfernung
    }
    int volume = map(distance, minDistance, maxDistance, 0, 255); // Mappe die Entfernung auf die Lautstärke
    volume = constrain(volume, 0, 255);                           // Begrenze die Lautstärke auf den Bereich von 0-255
    if (volume != lastVolume)                                     // Lautstärke hat sich geändert
    {
        int diff = (lastVolume - volume); // Berechnet die Differenz zwischen den lastVolume und Volume
        if (diff < 0)
            diff *= -1;
        if (volume > lastVolume)
        {
            changevolume(diff, true); // Ändere die Lautstärke leiser
        }
        else
        {
            changevolume(diff, false); // Ändere die Lautstärke lauter
        }
        lastVolume = volume; // Speichere die aktuelle Lautstärke
    }
    printf("Entfernung: %d cm, Lautstärke: %d\n", distance, volume, tfDist);

    delay(5); // Pause zwischen den Messungen
}
