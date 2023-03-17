#include <Arduino.h>
#include <NewPing.h>
#include <Servo.h>

#define TRIGGER_PIN  9
#define ECHO_PIN     10
#define TRIGGER_PIN2  8
#define ECHO_PIN2     7
#define MAX_DISTANCE 100 // Maximum distance we want to measure (in centimeters).
#define tol 5
#define offset 10

Servo ser;


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(9600);
  ser.attach(5);
  ser.write(90);
}

void loop() {
  delay(50);                    // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.

  float distance = sonar.ping_cm(); // Send ping, get distance in cm and print result (0 = outside set distance range)
  float distance2 = sonar2.ping_cm(); // Send ping, get distance in cm and print result (0 = outside set distance range)

  Serial.println(String(distance) + "\t" + String(distance2) + " cm");
  
  if (distance > tol && distance2 <= tol){
    ser.write(atan(distance/3)*180/PI - offset);
  }
  else if (distance <= tol && distance2 > tol){
    ser.write(180 - atan(distance2/3)*180/PI + offset);
  }
  else if (distance > 0 && distance2 > tol){
    ser.write(90);
  }
}