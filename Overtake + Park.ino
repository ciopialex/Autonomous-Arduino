#include <Servo.h> // Ensure Servo.h is included for steering control
//This is the Parking together with the Obstacle Avoiding and/or Overtaking

// Servo motor variables
Servo servo_direction;
int servo_initial_position = 84; // Center position
int servo_min_position = 64;     // Maximum left
int servo_max_position = 104;    // Maximum right
int z = 0;                       // Increment for fine-tuning steering adjustments

// Function: Overtaking
void Overtake() {
    // Calibrate distance and approach the obstacle
    while (analogRead(FD) < 450) // Move forward until the obstacle is within range
        Forward(255);            // Maximum speed

    // Reverse slightly if too close
    while (analogRead(FD) > 460) 
        Backward(200);           // Slower reverse for precision

    // Begin overtaking maneuver
    TurnLeft();                  // Maximum left steering
    delay(2000);                 // Adjust delay for turning duration
    FwdOvertake();
    delay(500);
    Stop();
    delay(100);

    TurnRight();                 // Maximum right steering
    delay(4000);                 // Adjust delay for turning back
    FwdOvertake();
    delay(500);
    Stop();
    delay(100);

    StraightenWheelRight();      // Center the wheel after turning right
    delay(2000);
    FwdOvertake();               // Move forward to clear the obstacle
    delay(500);
    Stop();
    delay(100);

    // Continue overtaking until the middle sensor no longer detects the obstacle
    while (analogRead(MD) > 400) 
        FwdOvertake();
    Stop();
    delay(100);

    // Exit the overtaking maneuver
    TurnRight(); 
    delay(2000);
    FwdOvertake();
    delay(500);
    Stop();
    delay(100);

    TurnLeft();
    delay(4000);
    FwdOvertake();
    delay(500);
    Stop();
    delay(100);

    StraightenWheelLeft(); // Center the wheel
    delay(2000);
    Stop();
}

// Function: Straighten wheel after a left turn
void StraightenWheelLeft() {
    z++;
    if (z > 7) {
        z = 0;
        servo_direction.write(servo_initial_position++);
        if (servo_initial_position >= 84)
            servo_initial_position = 84; // Ensure centered position
    }
}

// Function: Straighten wheel after a right turn
void StraightenWheelRight() {
    z++;
    if (z > 7) {
        z = 0;
        servo_direction.write(servo_initial_position--);
        if (servo_initial_position <= 84)
            servo_initial_position = 84; // Ensure centered position
    }
}

// Function: Forward with reduced speed for overtaking
void FwdOvertake() {
    analogWrite(AIA, 200); // Rear left motor forward
    analogWrite(AIB, 0);   // Rear left motor stop backward
    analogWrite(BIA, 200); // Rear right motor forward
    analogWrite(BIB, 0);   // Rear right motor stop backward
}

// Function: Backward with reduced speed for parking
void BwdParking() {
    analogWrite(AIA, 0);   // Rear left motor stop forward
    analogWrite(AIB, 200); // Rear left motor backward
    analogWrite(BIA, 0);   // Rear right motor stop forward
    analogWrite(BIB, 200); // Rear right motor backward
}

// Function: Sideways Parking
void SidewaysParking() {
    // Reverse until the middle sensor no longer detects an obstacle
    while (analogRead(MD) > 400) 
        BwdParking();

    // Maneuver into the parking space
    TurnRight();  // Maximum right steering
    delay(2000);
    BwdParking();
    delay(500);
    Stop();
    delay(100);

    StraightenWheelRight(); // Center the wheel after turning right
    delay(2000);
    BwdParking();
    delay(500);
    Stop();
    delay(100);

    TurnLeft();   // Maximum left steering
    delay(2000);
    BwdParking();
    delay(500);
    Stop();
}

// Function: Move forward at full speed
void Forward(int speed) {
    analogWrite(AIA, speed);
    analogWrite(AIB, 0);
    analogWrite(BIA, speed);
    analogWrite(BIB, 0);
}

// Function: Move backward at specified speed
void Backward(int speed) {
    analogWrite(AIA, 0);
    analogWrite(AIB, speed);
    analogWrite(BIA, 0);
    analogWrite(BIB, speed);
}

// Function: Stop the car
void Stop() {
    analogWrite(AIA, 0);
    analogWrite(AIB, 0);
    analogWrite(BIA, 0);
    analogWrite(BIB, 0);
}

// Function: Turn the wheels to maximum left
void TurnLeft() {
    servo_direction.write(servo_min_position);
}

// Function: Turn the wheels to maximum right
void TurnRight() {
    servo_direction.write(servo_max_position);
}
