#include "LiquidCrystal_I2C.h"
#include "Wire.h"
//#include "LCD.H"
#include "dht.h"

dht DHT;
#define dht_pin 4

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7);
int soil_sensorPin = 3;
int soil_sensorValue;
int light_SensorPin = 5;
int light_SensorValue;
int RELAY_FAN = 11;
int RELAY_PUMP = 10;

//send data to server
const int send_temperature = A0;
const int send_humidity = A1;
const int send_soil_moist = A2;
const int send_light = A3;


void setup(){

  //Pins to send data to server
  pinMode(send_temperature, OUTPUT);
  pinMode(send_humidity, OUTPUT);
  pinMode(send_soil_moist, OUTPUT);
  pinMode(send_light, OUTPUT);


  Serial.begin(9600);
  delay(10);

  //Sensor pins mode
  pinMode(soil_sensorPin, INPUT);
  pinMode(light_SensorPin, INPUT);
  // pinMode(RELAY_PUMP, OUTPUT);
  // pinMode(RELAY_FAN, OUTPUT);

  lcd.begin(16,2);
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.print("Hello world");
}

void lcd_display() {
  lcd.setBacklight(HIGH);
  lcd.setCursor(0, 0);
  lcd.print("Green House Manager");
  lcd.scrollDisplayLeft();
  delay(100);
}

void loop() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd_display();

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
  int soil_SensorValue = analogRead(soil_sensorPin);
  soil_sensorValue = map(soil_sensorValue, 0, 1024, 0, 100);
  Serial.print("Soil Data: ");
  Serial.println(soil_sensorValue);
  analogWrite(send_soil_moist, soil_sensorValue);    //Send soil moisture data
  lcd.setCursor(0,0);
  lcd.print("Moisture content in soil:");
  lcd.print(soil_sensorValue);
  lcd.print("%");
  delay(1000);
  lcd.noDisplay();

  //Light Intensity
  int light_SensorValue = analogRead(light_SensorPin);
  light_SensorValue = map(light_SensorValue, 0, 1024, 0, 100);
  Serial.print("Light Info: ");
  Serial.println(light_SensorValue);
  analogWrite(send_light, light_SensorValue);       //Send light data
  lcd.setCursor(1,0);
  lcd.print("Light: ");
  lcd.print(light_SensorValue);
  delay(1000);
  lcd.noDisplay();


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

  switch(int(DHT.temperature)) {
    case 30 ... 40:
      digitalWrite(RELAY_FAN, HIGH);              //DHT temperature
      break;
  }

  switch(int(DHT.humidity)) {                         //DHT humidity
      case 10 ... 50:
      digitalWrite(RELAY_PUMP, HIGH);
      break;
  }

}
