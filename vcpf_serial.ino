#include <PZEM004Tv30.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

PZEM004Tv30 pzem(D5, D6);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
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

void displayDataOnLCD(String prediction, float voltage, float current, float pf)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(prediction);
  lcd.setCursor(0, 1);
  lcd.print("V:");
  lcd.print(voltage);
  lcd.setCursor(8, 1);
  lcd.print("I:");
  lcd.print(current);
  lcd.setCursor(0, 2);
  lcd.print("PF:");
  lcd.print(pf);
}

void sendSensorDataToPython(float voltage, float current, float pf)
{
  Serial.print(voltage);
  Serial.print(",");
  Serial.print(current);
  Serial.print(",");
  Serial.println(pf);
}

void loop()
{
  float voltage = pzem.voltage();
  float current = pzem.current();
  float pf = pzem.pf();

  if (!isnan(voltage) && !isnan(current) && !isnan(pf))
  {
    Serial.print("Voltage : ");
    Serial.print(voltage);
    Serial.print("V, Current : ");
    Serial.print(current);
    Serial.print("A, PF : ");
    Serial.println(pf);

    sendSensorDataToPython(voltage, current, pf);

    if (Serial.available())
    {
      // Read the incoming data from Python
      String data = Serial.readStringUntil('\n');
      int separator1 = data.indexOf(',');
      int separator2 = data.indexOf(',', separator1 + 1);

      // Parse the received data
      String prediction = data.substring(0, separator1);
      voltage = data.substring(separator1 + 1, separator2).toFloat();
      current = data.substring(separator2 + 1).toFloat();

      // Display the received data on LCD
      displayDataOnLCD(prediction, voltage, current, pf);
    }
  }
  else
  {
    Serial.println("Error reading sensor data");
  }

  delay(2000);
}
