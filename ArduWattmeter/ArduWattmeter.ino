//Собираем Arduino ваттметр в Proteus 8.5 https://youtu.be/r1RigzIen7M

#define Debug true
#define Ah_Button 8 // кнопка переключения режимов отображение A/h или W/h
#define pinLED 13

#define WattAmpH_Conctant 844
#define Current_Constant 0.0 //0.029284455

#define RS 2
#define E 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7

#include <LiquidCrystal.h>

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

float Wh = 0.0; // глобальные переменные
float Ah = 0.0;

boolean LCD_Ah = false; // переменная для переключения режимов отображение A/h или W/h

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  pinMode(Ah_Button, INPUT);
  pinMode(pinLED, OUTPUT);
}

void loop() {

  if (digitalRead(Ah_Button) == LOW) { // Подтяжка резистором на 10 кОм на +5V
    delay(50); // защита от дребезга контактов кнопки
    if (digitalRead(Ah_Button) == LOW) {
      LCD_Ah = !LCD_Ah;
    }
  } else {

    int CurrentValue = analogReadOversampled(0); // A0, ADC0, PC0
    int VoltageValue = analogReadOversampled(1); // A1, ADC1, PC1

    float Voltage =  VoltageValue * (40.0 / 1024.0); // Max 40 Volt
    float Current = CurrentValue * (15.0 / 1024.0) - Current_Constant; // Max 15 Amp
    float Watt =  Voltage * Current;

    Ah = Ah + Current / WattAmpH_Conctant;
    Wh = Wh + Watt / WattAmpH_Conctant;

    lcd.setCursor(0, 0);
    lcd.print(Voltage); lcd.print("V ");
    lcd.setCursor(8, 0);
    lcd.print(Watt); lcd.print("W  ");
    lcd.setCursor(0, 1);
    lcd.print(Current, 2); lcd.print("A ");
    lcd.setCursor(8, 1);

    if (LCD_Ah == true) {

      if (Ah < 10.0) {
        lcd.print(Ah, 3);
      } else if (Ah < 100.0) {
        lcd.print(Ah, 2);
      } else if (Ah < 1000.0) {
        lcd.print(Ah, 1);
      }
      lcd.print("A/h  ");
    } else {
      if (Wh < 10.0) {
        lcd.print(Wh, 3);
      } else if (Wh < 100.0) {
        lcd.print(Wh, 2);
      } else if (Wh < 1000.0) {
        lcd.print(Wh, 1);
      }
      lcd.print("W/h  ");
    }

    if (Debug == true) {
      Serial.print(Voltage, 7); Serial.print(" ");
      Serial.print(Current, 7); Serial.print(" ");
      Serial.print(Watt, 7); Serial.print(" ");
      Serial.print(Ah, 7); Serial.print(" ");
      Serial.println(Wh, 7);
    }

    delay(100);
  }
}


int analogReadOversampled(byte analogChannel)  {
  unsigned long aSum = 0;   // the sum of all analog readings
  for (unsigned int i = 256; i > 0; i--) { // 8^x
    aSum = aSum + analogRead(analogChannel); // read and sum 256 ADС probes
  }
  return aSum >> 8;   // агалог деления на 256
}



