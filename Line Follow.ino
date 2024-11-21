Line following, with the mention this works when the car is placed on a lighter surface, and the line's color is dark ( pref black )

#include <Servo.h> // Include Servo library for steering

// Motor PINs
const int AIA = 3;
const int AIB = 4;
const int BIA = 5;
const int BIB = 6;

// InfraRed sensor PINs
const int L = A3;
const int M = A4;
const int R = A5;

// Servo control
Servo servo_direction;
const int servo_pin = 9; // Pin connected to the servo
const int servo_min_position = 64; // Full left
const int servo_max_position = 104; // Full right
const int servo_initial_position = 84; // Center position
int current_servo_position = servo_initial_position;

// Ground color threshold
const int GROUND_COLOR = 100;

// Movement functions
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

void TurnLeft() {
  if (current_servo_position > servo_min_position) {
    current_servo_position -= 10;  // Adjust increment for smoother turning
    if (current_servo_position < servo_min_position) {
      current_servo_position = servo_min_position;  // Limit to minimum position
    }
    servo_direction.write(current_servo_position);
  }
}

void TurnRight() {
  if (current_servo_position < servo_max_position) {
    current_servo_position += 10;  // Adjust increment for smoother turning
    if (current_servo_position > servo_max_position) {
      current_servo_position = servo_max_position;  // Limit to maximum position
    }
    servo_direction.write(current_servo_position);
  }
}

void Straight() {
  current_servo_position = servo_initial_position;  // Reset to initial (center) position
  servo_direction.write(current_servo_position);
}

// Setup function
void setup() {
  // Initialize motor pins as output
  pinMode(AIA, OUTPUT);
  pinMode(AIB, OUTPUT);
  pinMode(BIA, OUTPUT);
  pinMode(BIB, OUTPUT);

  // Attach servo
  servo_direction.attach(servo_pin);
  servo_direction.write(servo_initial_position); // Set initial position to center

  // Initialize IR sensor pins as input
  pinMode(L, INPUT);
  pinMode(M, INPUT);
  pinMode(R, INPUT);

  // Begin serial communication for debugging
  Serial.begin(9600);
}

// Loop function
void loop() {
  // Read IR sensors
  int sensorLeft = analogRead(L);
  int sensorMiddle = analogRead(M);
  int sensorRight = analogRead(R);

  // Determine line presence
  bool leftLine = sensorLeft < GROUND_COLOR;
  bool middleLine = sensorMiddle < GROUND_COLOR;
  bool rightLine = sensorRight < GROUND_COLOR;

  // Debugging output
  Serial.print("Left: "); Serial.print(leftLine);
  Serial.print(" | Middle: "); Serial.print(middleLine);
  Serial.print(" | Right: "); Serial.println(rightLine);

  // Line following logic
  if (middleLine) {
    // Line in the middle, go straight
    Straight();
    Forward(200); // Adjust speed as needed
  } else if (leftLine && !middleLine && !rightLine) {
    // Line on the left, turn left
    TurnLeft();
    Forward(150); // Slow down a bit while turning
  } else if (!leftLine && !middleLine && rightLine) {
    // Line on the right, turn right
    TurnRight();
    Forward(150); // Slow down a bit while turning
  } else {
    // No line detected, stop or implement recovery strategy
    Stop();
  }

  delay(100);  // Delay for stability
}