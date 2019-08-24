//librerias
#include <Wire.h> //I2C para AM2315 (A4 SDA, A5 SCL) 
#include <Adafruit_AM2315.h> //sensor de temperatura y humedad ambiental
#include "SparkFunMPL3115A2.h" //sensor de presión del shell
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>

//Pins
#define WSPEED 3
#define RAIN 2
#define WDIR A0
#define DHTTYPE DHT22 //sensor DHT22
#define DHTPIN 9
#define ONE_WIRE_BUS 12 //sensor DS18B20

DHT dht(DHTPIN, DHTTYPE);

OneWire oneWire(ONE_WIRE_BUS); //sensor DS18B20
DallasTemperature sensors(&oneWire); //sensor DS18B20

const byte rxPin = 10;
const byte txPin = 11;
SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

//Variables
Adafruit_AM2315 am2315;
MPL3115A2 myPressure; //instancia de sensor de presión
volatile long lastWindISR = 0;
volatile byte windClicks = 0;
volatile float rainin = 0;
volatile float rainHour[60];
volatile unsigned long raintime, raininterval, rainlast;
long lastWindCheck = 0;
int valueS1;
int valueS2;
int valueS3;

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
    rainin += 0.2794; //Each dump is 0.011" of water
    rainlast = raintime; // set up for next event
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial){
    delay(10);
  }
  
  pinMode(WSPEED, INPUT_PULLUP); //input anemómetro

  //tarjeta gases
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(38400);

  //iniciar sensores
  dht.begin();
  sensors.begin();
  am2315.begin();
  
  //sensor de presión
  myPressure.begin();
  myPressure.setModeBarometer();
  myPressure.setOversampleRate(7);
  //myPressure.enableEventFlags();

  //interrupciones
  attachInterrupt(1, wspeedISR, FALLING);
  attachInterrupt(0, rainISR, FALLING);
  interrupts();

  delay(5000);
}

float getRain(){
  float deltaTime = (millis() - rainlast)/1000.0;
  float rain = (float)rainin/deltaTime;
  rainin = 0;
  return(rain);
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
  sensors.requestTemperatures();
  int wDir = getWindDirection();
  float wSpeed = getWindSpeed();
  float myRain = getRain(); 
  float pressure = myPressure.readPressure();
  float tempAM2315 = am2315.readTemperature();
  float humAM2315 = am2315.readHumidity();
  float humDHT22 = dht.readHumidity();
  float tempDHT22 = dht.readTemperature();
  float tempDS18B20 = sensors.getTempCByIndex(0);

  if (mySerial.available() > 0){
    String valor = mySerial.readString();
    valueS1 = valor.substring(3,9).toInt();
    valueS2 = valor.substring(13,19).toInt();
    valueS3 = valor.substring(23,29).toInt();
  }

  Serial.print("{");
  Serial.print("\"DateTime\":");
  Serial.print(0);
  Serial.print(",\"WindDirection\":");
  Serial.print(wDir);
  Serial.print(",\"WindSpeed\":");
  Serial.print(wSpeed);
  Serial.print(",\"Rain\":");
  Serial.print(myRain);
  Serial.print(",\"Pressure\":");
  Serial.print(pressure/100000);
  Serial.print(",\"HumAM2315\":");
  Serial.print(humAM2315);
  Serial.print(",\"TempAM2315\":");
  Serial.print(tempAM2315);
  Serial.print(",\"HumDHT22\":");
  Serial.print(humDHT22);
  Serial.print(",\"TempDHT22\":");
  Serial.print(tempDHT22);
  Serial.print(",\"TempDS18B20\":");
  Serial.print(tempDS18B20);
  Serial.print(",\"CO\":");
  Serial.print(valueS1);
  Serial.print(",\"SO2\":");
  Serial.print(valueS2);
  Serial.print(",\"NO2\":");
  Serial.print(valueS3);
  Serial.println("}");

  delay(60000);
}
