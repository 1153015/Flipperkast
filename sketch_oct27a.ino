//Knoppen 
#define knopPinLinks 2
#define knopPinRechts 13

//Servo's
#include <Servo.h>
Servo servoLinks; 
Servo servoRechts;

//Sensor waarden
#define irSensorPin 7
int score = 0;
int huidigeStatus = LOW;
int vorigeStatus = LOW; 

//Eerste 7-segment display
int dataPin = 11;
int clockPin = 12;
int latchPin = 8;

//Tweede 7-segment display
int dataPin2 = 10;
int clockPin2 = 6;
int latchPin2 = 4;

//LED's tijdvariabelen
unsigned long huidigeTijd = 0;
bool stopTijd = false; //geeft aan of het spel afgelopen is of niet

//Netwerk score
String naam = "Selena";
String machine = "Selena's Flipperkast";

//Segmentdata voor cijfers 0–9 (a–g, dp)
byte digits[10] = 
{
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

void setup() 
{
  Serial.begin(9600);

  //IR-sensor
  pinMode(irSensorPin, INPUT);

  //Eerste display
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  //Tweede display
  pinMode(dataPin2, OUTPUT);
  pinMode(clockPin2, OUTPUT);
  pinMode(latchPin2, OUTPUT);

  //Knoppen
  pinMode(knopPinLinks, INPUT_PULLUP); 
  pinMode(knopPinRechts, INPUT_PULLUP);

  //Servo's
  servoLinks.attach(9); 
  servoRechts.attach(5);

  //LED's
  pinMode(A5, OUTPUT); //groene led
  pinMode(A4, OUTPUT); //gele led
  pinMode(A3, OUTPUT); //rode led

  //Displays laten starten op 00
  for (int i = 0; i < 3; i++){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digits[0]);
  digitalWrite(latchPin, HIGH);

  digitalWrite(latchPin2, LOW);
  shiftOut(dataPin2, clockPin2, MSBFIRST, digits[0]);
  digitalWrite(latchPin2, HIGH);

  delay(200);
  }
}

void loop() 
{
  //Als de tijd om is, doet de code niks meer
  if (stopTijd == true) {
    return;
  }

  //Score telling
  huidigeStatus = digitalRead(irSensorPin); //leest de huidige status van de sensor
  if (huidigeStatus != vorigeStatus) { 
    if (huidigeStatus == HIGH) { 
      score++;

    if (score >= 100) {
    score = 0;
    }

    int tientallen = score / 10;
    int eenheden = score % 10;

    //Eerste display
    digitalWrite(latchPin, LOW); //latch laag
    shiftOut(dataPin, clockPin, MSBFIRST, digits[tientallen]); //stuurt 8 bits een voor een naar de shift register
    digitalWrite(latchPin, HIGH); //latch hoog

    //Tweede display
    digitalWrite(latchPin2, LOW);
    shiftOut(dataPin2, clockPin2, MSBFIRST, digits[eenheden]); 
    digitalWrite(latchPin2, HIGH);
    }
  }

  vorigeStatus = huidigeStatus; //bewaart de oude status

  //Knop code
  if (digitalRead(knopPinLinks) == LOW) {
    servoLinks.write(80); }

  else {
    (digitalRead(knopPinLinks) == HIGH);
    servoLinks.write(0); }

  if (digitalRead(knopPinRechts) == LOW) {
    servoRechts.write(100); }

  else {
    (digitalRead(knopPinRechts) == HIGH);
    servoRechts.write(180); }

//LED's
  huidigeTijd = millis();

  if (huidigeTijd < 150000) {
    //2,5 minuut: groene led
    digitalWrite(A5, HIGH);
    digitalWrite(A4, LOW);
    digitalWrite(A3, LOW);
  }

  else if (huidigeTijd < 240000) {
    //1,5 minuut: gele led
    digitalWrite(A5, LOW);
    digitalWrite(A4, HIGH);
    digitalWrite(A3, LOW);
  }

  else {
    //Tijd om: rode led
    digitalWrite(A5, LOW);
    digitalWrite(A4, LOW);
    digitalWrite(A3, HIGH);

    //Printen van score naar serial
    Serial.print(naam);
    Serial.print(", ");
    Serial.print(score);
    Serial.print(", ");
    Serial.println(machine);

    //Spel stoppen
    stopTijd = true;
  }  

 }