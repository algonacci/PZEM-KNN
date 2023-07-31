#include <PZEM004Tv30.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

PZEM004Tv30 pzem(D5, D6);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("PZEM Test");
  lcd.setCursor(0, 1);
  lcd.print("by Miliohm.com");
  delay(2000);
  lcd.clear();
}

void displayDataOnLCD(float voltage, float current, float pf) {
  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.print(voltage);

  lcd.setCursor(0, 1);
  lcd.print("I:");
  lcd.print(current);

  lcd.setCursor(8, 0);
  lcd.print("PF:");
  lcd.print(pf);
}

void sendSensorDataToPython(float voltage, float current, float pf) {
  Serial.print("V:");
  Serial.print(voltage);
  Serial.print(",");
  Serial.print("I:");
  Serial.print(current);
  Serial.print(",");
  Serial.print("PF:");
  Serial.println(pf);
}

void loop() {
  float voltage = pzem.voltage();
  float current = pzem.current();
  float pf = pzem.pf();

  if (!isnan(voltage) && !isnan(current) && !isnan(pf)) {
    Serial.print("Voltage : ");
    Serial.print(voltage);
    Serial.print("V, Current : ");
    Serial.print(current);
    Serial.print("A, PF : ");
    Serial.println(pf);

    displayDataOnLCD(voltage, current, pf);
    sendSensorDataToPython(voltage, current, pf);
  } else {
    Serial.println("Error reading sensor data");
  }

  delay(2000);
}
