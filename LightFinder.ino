/*
 * Arduino Light Finder - Use a photoresistor mounted on a servo to find the direction where the most light comes from
 *
 * Circuit: 
 *
 * - A photoresistor in series with a 10k resistor connected to analog pin A2
 * - A servo connected to digital pin 5
 *
 * Note: the photoresistor is mounted on the servo
 *
 */

#include <Servo.h>
#include <Wire.h>
#include "rgb_lcd.h"

Servo servo;               // The servo
rgb_lcd lcd;               // The LCD display

#define LDR_PIN 2          // Arduino analog pin to which the photoresistor is connected
#define SERVO_PIN 5        // Arduino digitial PWM pin to which the servo is connected
#define POSITION_STEP 1    // The size of the step to take to a new position
#define SERVO_DELAY 100    // The amount of milliseconds to wait before the servo reaches a new position
#define MIN_SERVO_POS 20   // The minimum servo position that can be handled by the servo
#define MAX_SERVO_POS 160  // The maximum servo position that can be handled by the servo

int servoPosition = 90;    // The current position of the servo
int servoDirection = 1;    // The direction to move the servo in (1 = up, -1 = down)
int previousLightValue;    // The amount of light as measured by the photoresistor

void setup()
{
  servo.attach(SERVO_PIN);  // Attach the servo to the servo pin
  previousLightValue = getCurrentLight();  // Get the current amount of light
  
  lcd.begin(16, 2);         // Set up the LCD's number of columns and rows
  lcd.clear();                
  lcd.print("Light Finder");
}

int getCurrentLight() {
   return analogRead(LDR_PIN);
}

void setServoPosition(int position) {
  servo.write(position);    // Set the servo position
  delay(SERVO_DELAY);       // Wait for the servo to get there  
}

void printMessage(const String& message) {
  lcd.setCursor(0, 1);
  lcd.print(message + String("                "));
}

void loop() 
{ 
  // Caculate the new position to check if there is more light
  servoPosition += POSITION_STEP * servoDirection;
  if(servoPosition < MIN_SERVO_POS) {
    servoPosition = MIN_SERVO_POS;
    servoDirection = 1;
  }
  else if(servoPosition > MAX_SERVO_POS) {
    servoPosition = MAX_SERVO_POS;
    servoDirection = -1;
  }
  
  // Move the servo into the new position
  setServoPosition(servoPosition);

  // Measure the light
  int newLightValue = getCurrentLight();
  printMessage("L: " + String(newLightValue) + " S: " + String(servoPosition));
  
  // If the light decreases reverse the servo direction to go back
  if(newLightValue < previousLightValue) {
    servoDirection = -servoDirection;
  }
  
  // Remember the current light value for the next loop
  previousLightValue = newLightValue;
} 






