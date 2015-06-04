#include <VirtualWire.h>
#include <SPI.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 7, 6, 5, 3, 10);

int buttonPin = 22;
int lightSwitch = 40;
int buttonState = 0;
int moistureSensor = 2;
int tempSensor = 0;
int lightSensor = 1;
int moisture_val;
int temp_val;
int light_val;
byte stepLength = VW_MAX_MESSAGE_LEN;
int steps;
char stepsMsg[4];

void setup()
{
  vw_set_ptt_inverted(true);
  vw_setup(2000);
  vw_rx_start();

  Serial.begin(9600);
  fdevopen( &my_putc, 0);
  pinMode (20, OUTPUT);
  pinMode (12, OUTPUT);
  pinMode (24, OUTPUT);
  pinMode (13, OUTPUT);
  pinMode (4, OUTPUT);
  pinMode (32, OUTPUT);
  pinMode (33, OUTPUT);
  pinMode (34, OUTPUT);
  pinMode (35, OUTPUT);
  pinMode (26, OUTPUT);
  pinMode (37, OUTPUT);
  pinMode (buttonPin, INPUT);

  digitalWrite (buttonPin, HIGH);
  pinMode (24, HIGH);
  digitalWrite (20, HIGH);
  digitalWrite (12, HIGH);
  digitalWrite (13, HIGH);
  digitalWrite (4, HIGH); 
  digitalWrite (32, HIGH);
  digitalWrite (33, HIGH);
  digitalWrite (34, HIGH);
  digitalWrite (35, HIGH);
  digitalWrite (26, HIGH);
  digitalWrite (37, HIGH);
  digitalWrite (lightSwitch, HIGH);

  lcd.begin(16, 2);
  lcd.print("David Wu - 2014");
  delay (1000);
  lcd.clear();
}

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  float feedtime;

  buttonState = digitalRead(buttonPin);
  // if there is data ready
  if (buttonState == LOW) {

    if(vw_get_message(buf, &buflen)) {
      int i;
      for (i=0; i<buflen; i++) {
        stepsMsg[i] = char(buf[i]);
      }
      stepsMsg[buflen] = '\0';

      steps = atoi(stepsMsg);

      Serial.println(steps);
      lcd.clear();
      lcd.print("steps: ");
      lcd.setCursor(0, 1);
      lcd.print(steps);
      delay (1000);
      lcd.clear();
      lcd.print("release button");
      delay(1000);

      digitalWrite (4, LOW);
      lcd.clear();
      lcd.print("pump: feeding");
      lcd.setCursor(0, 1);
      feedtime = steps/10;
      lcd.print(feedtime);
      lcd.print("ms");
      delay (feedtime);

      digitalWrite (4, HIGH);
      steps = 0;

      delay(1000);
    }
  }
  else {
    sensorRead();
  }
}

void sensorRead()
{
  moisture_val = analogRead(moistureSensor);
  Serial.print("moisture: ");
  Serial.println(moisture_val);
  lcd.clear();
  lcd.print("moisture: ");
  lcd.print(moisture_val);
  lcd.setCursor(0, 1);

  if (moisture_val < 300) //if soil moisture is too low turn on pump
  {
    Serial.println ("Pump: on");
    lcd.print("pump: on");
    digitalWrite (4, LOW);
    digitalWrite (34, HIGH);
    digitalWrite (35, LOW);
    delay (1500);
    digitalWrite(4, HIGH);
    digitalWrite (35, HIGH);
    digitalWrite (34, LOW);

  }

  if (moisture_val > 300) //when soil moisture is normal turn off pump
  {
    Serial.println ("Pump: off");
    lcd.print("pump: off");
    digitalWrite (4, HIGH);
    digitalWrite (35, HIGH);
    digitalWrite (34, LOW);
    delay (1000);
  }
  light_val = analogRead(lightSensor);
  Serial.print("light: ");
  Serial.println(light_val);
  lcd.clear();
  lcd.print("brightness: ");
  lcd.print(light_val);
  lcd.setCursor(0, 1);

  if (lightSwitch == HIGH) {
    digitalWrite(26, HIGH);
    digitalWrite (37, LOW);
    lcd.print("light: on");
    digitalWrite (12, LOW);
  }
  else {
    if (light_val < 150) //NEED TO CHANGE if amount of light is too low turn on light
    {
      Serial.println ("Light: on");
      digitalWrite(26, HIGH);
      digitalWrite (37, LOW);
      lcd.print("light: on");
      digitalWrite (12, LOW);
      delay (1000);
    }
    if (light_val > 150) //NEED TO CHANGE if amount of light is high enough turn off light
    {
      Serial.println ("Light: off");
      lcd.print("light: off");
      digitalWrite(26, LOW);
      digitalWrite(37, HIGH);
      digitalWrite (12, HIGH);
      delay (1000);
    }
  }

  temp_val = analogRead(tempSensor);
  Serial.print("temp: ");
  Serial.println(temp_val);
  lcd.clear();
  lcd.print("temp rating: ");
  lcd.print(temp_val);
  lcd.setCursor(0, 1);

  if (temp_val > 400)
  {
    Serial.println ("IT'S TOO HOT");
    digitalWrite (33, HIGH);
    digitalWrite (32, LOW);
    lcd.print("IT'S TOO HOT!");
    delay (1000);
  }

  else if(temp_val < 400)
  {
    if (temp_val < 200)
    {
      Serial.println ("IT'S TOO COLD");
      digitalWrite (33, HIGH);
      digitalWrite (32, LOW);
      lcd.print("IT'S TOO COLD");
      delay (1000);
    }
    else if (temp_val > 200)
    {
      Serial.println ("it's just right");
      digitalWrite (32, HIGH);
      digitalWrite (33, LOW);
      lcd.print("it's just right");
      delay (1000);
    }
  }
}









