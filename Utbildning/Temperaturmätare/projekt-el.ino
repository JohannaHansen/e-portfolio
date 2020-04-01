  #include "MicroLCD.h"
  #include <math.h>

  #define sensor A0  //Sensor is connected to pin A0
  const int red = 2;
  const int green = 3;
  const int button = 4;
  int buttonState = LOW;
  int count = 50;
  char unit = 'C';
  double temp = 0;
  LCD_SSD1306 lcd; /* for SSD1306 OLED module */

  double Thermistor(int RawADC) 
  {
    double Temp;
    Temp = log(((10240000/RawADC) - 10000));
    Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
    Temp = Temp - 273.15;           
    return Temp;
  }

void setup() {
  // put your setup code here, to run once:
  
  lcd.begin();
  lcd.clear();
  pinMode(red, OUTPUT);//Change to output my pins
  pinMode(green, OUTPUT); 
  digitalWrite(red, LOW);//Turn off LED
  digitalWrite(green, LOW);//Turn off LED 
  pinMode(button, INPUT);
  lcd.setCursor(5, 0);
  lcd.print("THERMOMETER");
  lcd.setCursor(20, 3);
  lcd.setFontSize(FONT_SIZE_MEDIUM);
  lcd.print("Starting...");
  int readVal=analogRead(sensor);
  temp = Thermistor(readVal);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(button);
  
  int readVal=analogRead(sensor);
  double tempC = Thermistor(readVal);
  double tempF = (tempC * 9.0)/ 5.0 + 32.0; //convert Celcius to Farenheit

  if (tempC <= 26 & tempC >= 18 )
  {
    digitalWrite(green, HIGH);
    digitalWrite(red, LOW);
  }

  else
  {
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
  }

  if (buttonState == HIGH)
  {
    if (unit == 'C'){
      temp =  tempF;
      count = 50;
      unit = 'F';
    }
    else if (unit == 'F'){
      temp =  tempC;
      count = 50;
      unit = 'C';
    }
  }
  else if (buttonState == LOW)
  {
    if (unit == 'C')
    {
      temp = tempC;
    }
    else if (unit == 'F')
    {
      temp = tempF;
    }
  }
  if (count == 50)
    {
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.setFontSize(FONT_SIZE_SMALL);
      lcd.print("THERMOMETER");
      lcd.setCursor(5, 3);
      lcd.setFontSize(FONT_SIZE_MEDIUM);
      lcd.print("Temp: ");
      lcd.print(temp);
      lcd.print("'");
      lcd.print(unit);
      count = 0;
    }
    count ++;
  delay(100);


}
