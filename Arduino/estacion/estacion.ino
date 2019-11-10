//librerias
#include <Wire.h> //protocolo de comunicacion I2C usado por AM2315 y BMP280 (SDA y SCL)
#include <Adafruit_AM2315.h> //sensor de temperatura y humedad ambiental AM2315
#include <Adafruit_BMP280.h> //sensor de presión, temperatura y altura BMP280
#include "DHT.h" //sensor de temperatura y humedad ambiental DHT22
#include <OneWire.h> //protocolo de comunicación OneWire usado por DS18B20
#include <DallasTemperature.h> //sensor de temperatura DS18B20
#include <SoftwareSerial.h> //protocolo de comunicación usado por módulo MBD+AQD

//Pins
#define WSPEED 3 //pin por defecto del shell sparkfun para el sensor de velocidad de viento
#define RAIN 2 //pin por defecto del shell sparkfun para el sensor de prcipitación 
#define WDIR A0 //pin por defecto del shell de sparkfun para el sensor de dirección del viento
#define DHTTYPE DHT22 //definición del típo de sensor DHT
#define DHTPIN 9 //pin del sensor DHT22
#define ONE_WIRE_BUS 12 //pin del sensor DS18B20

DHT dht(DHTPIN, DHTTYPE); //instancia del sensor DHT22

//instancias para sensor DS18B20
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire); 

//pines para tarjeta dhemax
const byte rxPin = 10;
const byte txPin = 11;
SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

//pines para sensor sharp GP2Y1014AU0F
const int sharpLEDPin = 7;   // Arduino digital pin 7 connect to sensor LED.
const int sharpVoPin = A1;   // Arduino analog pin 5 connect to sensor Vo.

//Variables
Adafruit_AM2315 am2315; //I2C
Adafruit_BMP280 bmp; //I2C
volatile long lastWindISR = 0;
volatile byte windClicks = 0;
volatile float rainin = 0;
volatile float rainHour;
volatile unsigned long raintime, raininterval, rainlast;
long lastWindCheck = 0;
int valueS1;
int valueS2;
int valueS3;
static float Voc = 0.6; //output típico del voltaje del polvo en 0 volts
const float K = 0.5;  //sensivilidad típica del polvo en únidades de V por 100mg/m3

//activación del anemómetro cuando se activa el magneto
void wspeedISR() {
  if (millis() - lastWindISR > 10) {
    lastWindISR = millis();
    windClicks++;
  }
}

void rainISR() {
  raintime = millis(); // toma el tiempo actual en milisegundos
  raininterval = raintime - rainlast; // calcula el intervalo entre el evento actual y el anterior
  if (raininterval > 10) // ignora la vibración del receptaculo y toma muestras después de 10 milisegundos
  {
    rainHour += 0.2794; //incrementa la cantidad de lluvia caida por la cantidad estipulada
    rainlast = raintime; // se setea para el próximo evento
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(10);
  }
  Serial.println("Inicializado");
  pinMode(WSPEED, INPUT_PULLUP); //input anemómetro

  //tarjeta gases
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(38400);

  //iniciar sensores
  dht.begin();
  sensors.begin();
  am2315.begin();
  bmp.begin();

  //densidad de polvo
  pinMode(sharpLEDPin, OUTPUT);

  //seteo default de BMP280 según datasheet
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  //interrupciones
  attachInterrupt(1, wspeedISR, FALLING);
  attachInterrupt(0, rainISR, FALLING);
  interrupts();

  delay(1000);
}

float getRain() {
  rainin = rainHour;
  rainHour = 0;
  return(rainin);
}

float getWindSpeed() {
  float deltaTime = (millis() - lastWindCheck) / 1000.0;
  float windSpeed = (float)windClicks / deltaTime;
  windSpeed *= 2.4;
  windClicks = 0;
  lastWindCheck = millis();
  return (windSpeed);
}

int getWindDirection() {
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
  float rain = getRain();
  float pressure = bmp.readPressure();
  float tempBMP280 = bmp.readTemperature();
  float tempAM2315 = am2315.readTemperature();
  float humAM2315 = am2315.readHumidity();
  float humDHT22 = dht.readHumidity();
  float tempDHT22 = dht.readTemperature();
  float tempDS18B20 = sensors.getTempCByIndex(0);

  digitalWrite(sharpLEDPin, LOW);
  delayMicroseconds(280);
  float Vo = analogRead(sharpVoPin);
  digitalWrite(sharpLEDPin, HIGH);

  Vo = Vo / 1024.0 * 5.0;
  float dV = Vo - Voc;
  if ( dV < 0 ) {
    dV = 0;
    Voc = Vo;
  }
  float dustDensity = dV / K * 100.0;
  
  if (mySerial.available() > 0) {
    String valor = mySerial.readString();
    valueS1 = valor.substring(3, 9).toInt();
    valueS2 = valor.substring(13, 19).toInt();
    valueS3 = valor.substring(23, 29).toInt();
  }

  Serial.print("{");
  Serial.print("\"WindDirection\":");
  Serial.print(wDir);
  Serial.print(",\"WindSpeed\":");
  Serial.print(wSpeed);
  Serial.print(",\"Rain\":");
  Serial.print(rain);
  Serial.print(",\"Pressure\":");
  Serial.print(pressure / 100);
  Serial.print(",\"TempBMP280\":");
  Serial.print(tempBMP280);
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
  Serial.print(",\"PM\":");
  Serial.print(dustDensity);
  Serial.println("}");

  delay(60000);
}
