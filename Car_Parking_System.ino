#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// LCD Configuration
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Servo Configuration
Servo myservo1;

// IR Sensors and Slots
int IR1 = 2;  // Entry sensor
int IR2 = 3;  // Exit sensor
int Slot = 4; // Total available slots

// Flags
int flag1 = 0;  // Entry flag
int flag2 = 0;  // Exit flag

void setup() {
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.backlight();
  
  // Initialize IR sensors
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  // Initialize servo
  myservo1.attach(9);
  myservo1.write(100);  // Start with the gate closed

  // Welcome message
  lcd.setCursor(0, 0);
  lcd.print("  AUTOMATIC CAR ");
  lcd.setCursor(0, 1);
  lcd.print(" PARKING SYSTEM ");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Check for entry
  if (digitalRead(IR1) == LOW && flag1 == 0) {
    if (Slot > 0) {
      flag1 = 1;  // Vehicle entering
      if (flag2 == 0) {  // Ensure no conflict with exit
        myservo1.write(0);  // Open gate
        Slot = Slot - 1;  // Reduce available slots
      }
    } else {
      // Parking full message
      lcd.setCursor(0, 0);
      lcd.print("    SORRY :(    ");
      lcd.setCursor(0, 1);
      lcd.print("  Parking Full  ");
      delay(3000);
      lcd.clear();
    }
  }

  // Check for exit
  if (digitalRead(IR2) == LOW && flag2 == 0) {
    flag2 = 1;  // Vehicle exiting
    if (flag1 == 0) {  // Ensure no conflict with entry
      myservo1.write(0);  // Open gate
      Slot = Slot + 1;  // Increase available slots
    }
  }

  // Close gate after vehicle passes
  if (flag1 == 1 && flag2 == 1) {
    delay(1000);          // Allow time for the vehicle to pass
    myservo1.write(100);  // Close gate
    flag1 = 0;
    flag2 = 0;
  }

  // Update LCD display
  lcd.setCursor(0, 0);
  lcd.print("  Welcome !!  ");
  lcd.setCursor(0, 1);
  lcd.print("Slot Left: ");
  lcd.print(Slot);
  lcd.print(" ");  // Overwrite previous numbers if fewer digits
}
