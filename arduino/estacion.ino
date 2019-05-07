//librerias
#include <Wire.h> //I2C
#include <Adafruit_AM2315.h> //sensor de temperatura y humedad ambiental
#include "SparkFunMPL3115A2.h" //sensor de presión del shell

//Pins
#define WSPEED 3
#define RAIN 2
#define WDIR A0

//Variables
Adafruit_AM2315 am2315;
MPL3115A2 myPressure; //instancia de sensor de presión
volatile long lastWindISR = 0;
volatile byte windClicks = 0;
volatile float dailyrainin = 0;
volatile unsigned long raintime, raininterval, rainlast;
long lastWindCheck = 0;


//activación del anemómetro cuando se activa el magneto
void wspeedISR(){
  if (millis() - lastWindISR > 10){ 
    lastWindISR = millis();
    windClicks++;
  }
}

void rainISR(){
  raintime = millis();
  raininterval = raintime - rainlast;

  if (raininterval > 10)
  {
    dailyrainin += 0.2794; //Each dump is 0.011" of water
    rainlast = raintime; // set up for next event
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(WSPEED, INPUT_PULLUP); //input anemómetro

  //sensor de presión
  myPressure.begin();
  myPressure.setModeBarometer();
  myPressure.setOversampleRate(7);
  //myPressure.enableEventFlags();

  //interrupciones
  attachInterrupt(1, wspeedISR, FALLING);
  attachInterrupt(0, rainISR, FALLING);
  interrupts();
}

float getWindSpeed(){
  float deltaTime = (millis() - lastWindCheck)/1000.0;
  float windSpeed = (float)windClicks/deltaTime;
  windSpeed *= 2.4;
  windClicks = 0;
  lastWindCheck = millis();
  return(windSpeed);
}

int getWindDirection(){
  unsigned int adc;
  adc = analogRead(WDIR);
  if (adc < 380) return (113);
  if (adc < 393) return (68);
  if (adc < 414) return (90);
  if (adc < 456) return (158);
  if (adc < 508) return (135);
  if (adc < 551) return (203);
  if (adc < 615) return (180);
  if (adc < 680) return (23);
  if (adc < 746) return (45);
  if (adc < 801) return (248);
  if (adc < 833) return (225);
  if (adc < 878) return (338);
  if (adc < 913) return (0);
  if (adc < 940) return (293);
  if (adc < 967) return (315);
  if (adc < 990) return (270);
}

void loop() {
  int wDir = getWindDirection();
  float wSpeed = getWindSpeed(); 
  float pressure = myPressure.readPressure();
  float temperature = am2315.readTemperature();
  float humidity = am2315.readHumidity();

  Serial.print("{");
  Serial.print("\"WindDirection\":");
  Serial.print(wDir);
  Serial.print(",\"WindSpeed\":");
  Serial.print(wSpeed);
  Serial.print(",\"Pressure\":");
  Serial.print(pressure);
  Serial.print(",\"Humidity\":");
  Serial.print(humidity);
  Serial.print(",\"Temperature\":");
  Serial.print(temperature);
  Serial.println("}");

  delay(60000);

}
