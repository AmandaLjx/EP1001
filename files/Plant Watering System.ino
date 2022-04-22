#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); 
#include "DHT.h"

#define pump 10
#define LDR A1
#define SOIL A0
#define dhtPin 8
#define dhtType DHT11

DHT dht(dhtPin, dhtType);
float humidity;      //humidity
float temperature;   //temperature in degrees Celcius

const int dry = 640;  //0% moisture (need to measure value)
const int wet= 295;   //100% moisture (need to measure value)

byte smiley[8] =  //smiley icon
{
0b00000,
0b11011,
0b11011,
0b00000,
0b10001,
0b10001,
0b11111,
0b00000
};

void setup() {
  Serial.begin(9600); //moisture sensor setup

  lcd.init(); // LCD setup
	lcd.backlight();
  lcd.clear();

  dht.begin();  // DHT setup

  pinMode(LDR, INPUT); //LDR setup
  lcd.createChar(0, smiley);

  pinMode(pump, OUTPUT); // pump setup
  
  lcd.setCursor(3, 0);  //Starting text
  lcd.print("PLANTHELPS");
  lcd.setCursor(1, 1);
  lcd.print("Setting Up....");
  delay(3000);
  lcd.clear();
}

void loop() {
  lcd.init(); // LCD setup
	lcd.backlight();
  lcd.clear();

  int sensorVal = analogRead(SOIL); //moisture sensor READ
  int percentagemoisture = map(sensorVal, wet, dry, 100, 0);

  humidity = dht.readHumidity(); // DHT READ
  temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)){ //DHT Fail
    lcd.setCursor(3, 0);
    lcd.print("DHT ERROR!");
    return;
  }

  if (percentagemoisture <= 35 ) // WATER pump on
  {
    digitalWrite(pump, HIGH);
    lcd.setCursor(2, 1);
    lcd.print("WATERING....");
    delay(5000);
  }
  else // pump off
  {
    digitalWrite(pump, LOW);
    lcd.setCursor(2, 1);
    lcd.write(0);
    lcd.print("PLANTHELPS");
    lcd.write(0);
  }

  lcd.home();
  lcd.print("Moisture: "); //PRINT moisture
  lcd.print(percentagemoisture);
  lcd.print("%");
  delay(2000);
  lcd.setCursor(0,0);
  lcd.print("                   "); //clear line

  lcd.home();
  lcd.print("Temp: "); // PRINT temperature
  lcd.print(temperature);
  lcd.print(char(223));
  lcd.print("C");
  delay(2000);
  lcd.print("                   "); //clear line

  lcd.home();
  lcd.print("Humidity: "); // PRINT humidity
  lcd.print(humidity);
  lcd.print("%");
  delay(2000);
  lcd.clear();
}
