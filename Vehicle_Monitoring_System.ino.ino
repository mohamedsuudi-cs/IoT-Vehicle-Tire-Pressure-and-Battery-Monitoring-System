#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int pressurePin = 34;   // Potentiometer
const int batteryPin  = 35;   // Voltage Sensor (S)
const int buzzerPin   = 18;   // Buzzer

void setup() {

  Wire.begin(21, 22);

  lcd.init();
  lcd.backlight();

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  Serial.begin(115200);

  lcd.setCursor(0,0);
  lcd.print("Vehicle Monitor");
  lcd.setCursor(0,1);
  lcd.print("System Ready");

  delay(2000);
}

void loop() {

  // -----------------------------
  // Read Tire Pressure
  // -----------------------------
  int pressureRaw = analogRead(pressurePin);
  int pressurePercent = map(pressureRaw,0,4095,0,100);

  // -----------------------------
  // Read Battery Voltage
  // -----------------------------
  int batteryRaw = analogRead(batteryPin);

  float adcVoltage = (batteryRaw * 3.3) / 4095.0;

  // Voltage sensor ratio (0-25V module)
  float batteryVoltage = adcVoltage * 5.0;

  // -----------------------------
  // Screen 1 - Battery
  // -----------------------------
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Battery:");
  lcd.print(batteryVoltage,1);
  lcd.print("V");

  lcd.setCursor(0,1);

  if(batteryVoltage < 7.0){
      lcd.print("LOW BATTERY");
  }
  else{
      lcd.print("Status:Normal");
  }

  for(int i=0;i<20;i++){

      if(pressurePercent < 20 || batteryVoltage < 7.0){
          digitalWrite(buzzerPin,HIGH);
          delay(50);
          digitalWrite(buzzerPin,LOW);
          delay(50);
      }
      else{
          digitalWrite(buzzerPin,LOW);
          delay(100);
      }
  }

  // -----------------------------
  // Read Pressure Again
  // -----------------------------
  pressureRaw = analogRead(pressurePin);
  pressurePercent = map(pressureRaw,0,4095,0,100);

  // -----------------------------
  // Screen 2 - Tire Pressure
  // -----------------------------
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Pressure:");
  lcd.print(pressurePercent);
  lcd.print("%");

  lcd.setCursor(0,1);

  if(pressurePercent < 20){
      lcd.print("LOW PRESSURE");
  }
  else{
      lcd.print("Pressure OK");
  }

  for(int i=0;i<20;i++){

      if(pressurePercent < 20 || batteryVoltage < 7.0){
          digitalWrite(buzzerPin,HIGH);
          delay(50);
          digitalWrite(buzzerPin,LOW);
          delay(50);
      }
      else{
          digitalWrite(buzzerPin,LOW);
          delay(100);
      }
  }

  // -----------------------------
  // Serial Monitor
  // -----------------------------
  Serial.print("Battery: ");
  Serial.print(batteryVoltage,2);
  Serial.print(" V | Pressure: ");
  Serial.print(pressurePercent);
  Serial.print("% ");

  if(pressurePercent < 20){
      Serial.println("(LOW)");
  }
  else{
      Serial.println("(OK)");
  }

}