#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>
#include <Servo.h>

#define I2C_ADDR    0x3F
#define COLS        16
#define ROWS        2

Adafruit_LiquidCrystal lcd(I2C_ADDR); // Initialize the LCD object

// Define the switch pins and corresponding LED pins
int switchPins[] = {2, 3, 4, 5, 6, 7};
int ledPins[] = {8, 9, 10, 11, 12, 13};
char gearPositions[] = {'P', 'R', 'N', 'D', '2', '1'};

int potpin = A0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

Servo myservo;  // create servo object to control a servo

void setup() {
  Serial.begin(9600);  // opens serial port, sets data rate to 9600 bps

  // Set the switch pins as input with pullup and LED pins as output
  for (int i = 0; i < 6; i++) {
    pinMode(switchPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
  }

  // Attach the servo on pin 9 to the servo object
  myservo.attach(9);

  // Set up the LCD's number of columns and rows:
  lcd.begin(COLS, ROWS);
}

void loop() {
  bool anySwitchPressed = false;

  // Check each switch and light up the corresponding LED
  for (int i = 0; i < 6; i++) {
    if (digitalRead(switchPins[i]) == LOW) {  // Switch is pressed when reading LOW due to pullup resistor
      digitalWrite(ledPins[i], HIGH);
      Serial.print("Gear position: ");
      Serial.println(gearPositions[i]);

      // Print the gear position to the LCD
      lcd.setCursor(0, 0);
      lcd.print("Gear position: ");
      lcd.print(gearPositions[i]);

      anySwitchPressed = true;
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }

  if (!anySwitchPressed) {
    // Clear the LCD and print a default message
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No gear selected");
    Serial.println("No gear selected");
  }

  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  myservo.write(val);                  // sets the servo position according to the scaled value

  // Print the servo position to the LCD
  lcd.setCursor(0, 1);
  lcd.print("Servo position: ");
  lcd.print(val);

  //delay(500);  // waits for the servo to get there
}
