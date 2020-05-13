//The full code to load on to the Feather
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display = Adafruit_SSD1306();

#define SENSORPIN1 10 //Sensor 1 is on the right
#define SENSORPIN2 11 //Sensor 2 is on the left

long end_time; // When Sensor 2 is triggered
long start_time; // When Sensor 1 is triggered
long elapsed_time; // End time minus start time

float mph; // Speed calculated

int trigger1 = 0; // Sensor 1
int trigger2 = 0; // Sensor 2
int sensor1State; // Sensor 1 status
int sensor2State; // Sensor 2 status

void setup() {
  pinMode(SENSORPIN1, INPUT); // Sensor 1 as input
  digitalWrite(SENSORPIN1, HIGH); // Turn on the pullup
  pinMode(SENSORPIN2, INPUT); // Sensor 2 s input
  digitalWrite(SENSORPIN2, HIGH); // Turn on the pullup

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

  // Clear the buffer.
  display.clearDisplay();
  display.display();


  display.setTextColor(WHITE);

}

// Function to determine speed
void speed()
{
  // subtract end time from start time to get total time
  elapsed_time = ((end_time - start_time));

  // convert mm/s to mph
  mph = ((106000 / elapsed_time) * 0.0022);

  // set OLED display text size
  display.setTextSize(3);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setCursor(0, 0);
  display.print(mph);
  display.setTextSize(2);
  display.print("mph");
  display.setCursor(0, 0);
  display.display(); // actually display all of the above
}

void loop() {
  // Read the state of the IR sensor 1:
  sensor1State = digitalRead(SENSORPIN1);

  // See if IR beam of sensor 1 has been broken
  if (sensor1State == LOW) {

    // Check to make sure both sensors have not triggered
    if (trigger1 == 0 && trigger2 == 0) {

      // Save time when sensor 1 was triggered
      start_time = millis();

      // Prevent sensor 1 from triggering again
      trigger1 = 1;
    }
  }

  // Read the state of the IR sensor 2:
  sensor2State = digitalRead(SENSORPIN2);

  // See if IR beam of sensor 2 has been broken
  if (sensor2State == LOW) {

    // Check to make sure sensor 1 has triggered but not sensor2
    if (trigger2 == 0 && trigger1 == 1) {

      // Save time when sensor 2 was triggered
      end_time = millis();

      // Run speed function
      speed();

      // Prevent sensor 2 from triggering again
      trigger2 = 1;
    }
    delay(1000);

    // Reset both sensors
    trigger1 = 0;
    trigger2 = 0;
  }
}