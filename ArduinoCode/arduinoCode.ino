#include "LiquidCrstal_I2C.h"
#include "Wire.h"
#include "LCD.H"
#include "dht.h"

#define dht_pin = A0;
dht DHT;

LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7);
int soil_sensorPin = A1;
int soil_sensorValue;
int light_SensorPin = A2;
int light_SensorValue;
int RELAY_FAN = A3;
int RELAY_PUMP = A4;

const int send_temperature;
const int send_humidity;
const int send_soil_moist;
const int send_light;


void setup(){

  //Controller communication pins
  pinMode(send_temperature, OUTPUT);
  pinMode(send_humidity, OUTdUT);
  pinMode(send_soil_moist);
  pinMode(send_light, OUTPUT);


  Serial.begin(9600);
  delay(10);
  pinMode(soil_sensorPin, OUTPUT);
  pinMode(light_SensorPin, OUTPUT);
  pinMode(RELAY_PUMP, OUTPUT);
  pinMode(RELAY_FAN, OUTPUT);
  pinMode(send_light, OUTPUT);

  lcd.begin(16,2);
  ld.setBacklit(3, POSITIVE);
  lcd.setCursor(0, 0);
  lcd.print("Project:");
  lcd.setCursor(0, 1);    //Column | Row
  lcd.print("IoT Green house Manager");
  lcd.clear();
}

void lcd_display() {
  lcd.setBacklit(HIGH);
  lcd.setCursor(0,0);
  lcd.print("Green House Manager");
  lcd.scrollDisplayLeft();
  delay(100);
}

void loop() {

  lcd.clear();
  lcd.setCursor(0, 0);

  //DHT sensor
  DHT.read11(dht_pin);
  Serial.print("DHT Data temperature: ");
  Serial.println(DHT.temperature);
  analogWrite(send_temperature, DHT.temperature);     //Send temperature data
  lcd.setCursor(0, 1);
  Serial.print("DHT Data humidity: ");
  Serial.println(DHT.humidity);
  analogWrite(send_humidity, DHT.humidity);           //Send humidity data
  delay(1000);

  //Soil Sensor
  soil_SensorValue = analogRead(soil_sensorPin);
  soil_sensorValue = map(soil_sensorValue, 0, 1024, 0, 100);
  Serial.print("Soil Data: ");
  Serial.println(soil_sensorValue);
  analogWrite(send_soil_moist, soil_sensorValue);    //Send soil moisture data
  delay(1000);

  //Light Intensity
  light_SensorValue = anaogRead(light_SensorPin);
  light_SensorValue = map(light_SensorValue, 0, 1024, 0, 100);
  Serial.print("Light Info: ");
  Serial.println(light_SensorValue);
  analogWrite(send_light, light_SensorValue);       //Send light data
  delay(1000);


  //Automatic control(change values accordingly)

  switch(soil_sensorValue) {                        //Soil Sensor
    case 1 ... 30:
      digitalWrite(RELAY_PUMP, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      break;
    default:
      digitalWrite(RELAY_PUMP, LOW);
  }

  switch(DHT.temperature) {
    case 30 ... 40:
      digitalWrite(RELAY_FAN, HIGH);              //DHT temperature
      break;
  }

  switch(DHT.humidity) {
      case 10 ... 50:
      digitalWrite(RELAY_PUMP, HIGH);
      break;
  }

}

