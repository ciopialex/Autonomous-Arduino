
// # Automatic Cruise Control (3 zones )

#include <Servo.h>

// Pin definitions
const int AIA = 3; // Left rear motor forward
const int AIB = 4; // Left rear motor backward
const int BIA = 5; // Right rear motor forward
const int BIB = 6; // Right rear motor backward

const int FD = A0; // Front distance sensor

// Servo parameters
Servo servo_direction;
const int servo_min_position = 64; // Minimum turn left
const int servo_max_position = 104; // Maximum turn right
const int servo_initial_position = 84; // Center position
int current_servo_position = servo_initial_position;

// Speed constants
const int MAX_SPEED = 255; // Maximum speed for forward motion
const int MID_SPEED = 127; // Half speed for zone 2
const int LOW_SPEED = 63;  // Quarter speed for zone 1

// Distance zones
const int ZONE1_THRESHOLD = 400; // Closest zone, stop here
const int ZONE2_THRESHOLD = 500; // Medium distance, slow down
const int ZONE3_THRESHOLD = 600; // Farther distance, maintain maximum speed

void setup() {
  // Set up motor control pins as output
  pinMode(AIA, OUTPUT);
  pinMode(AIB, OUTPUT);
  pinMode(BIA, OUTPUT);
  pinMode(BIB, OUTPUT);

  // Attach servo
  servo_direction.attach(9); // Servo pin
  servo_direction.write(servo_initial_position); // Set to center
}

void loop() {
  int distance = analogRead(FD); // Read the distance sensor

  if (distance > ZONE3_THRESHOLD) {
    // Zone 3: Maintain maximum speed
    Forward(MAX_SPEED);
  } else if (distance > ZONE2_THRESHOLD) {
    // Zone 2: Reduce to half speed
    Forward(MID_SPEED);
  } else if (distance > ZONE1_THRESHOLD) {
    // Zone 1: Reduce to quarter speed
    Forward(LOW_SPEED);
  } else {
    // Emergency case: Object is very close, stop immediately
    Stop();
  }

  delay(100); // Add delay for stability
}

// Motor control functions
void Forward(int speed) {
  analogWrite(AIA, speed);  // Left rear motor forward
  analogWrite(AIB, 0);      // Left rear motor stop backward
  analogWrite(BIA, speed);  // Right rear motor forward
  analogWrite(BIB, 0);      // Right rear motor stop backward
}

void Stop() {
  analogWrite(AIA, 0);  // Left rear motor stop
  analogWrite(AIB, 0);  // Left rear motor stop backward
  analogWrite(BIA, 0);  // Right rear motor stop
  analogWrite(BIB, 0);  // Right rear motor stop backward
}
