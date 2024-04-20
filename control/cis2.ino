
// Pins used to control motors (TB6612FNG)
//const uint16_t PWMA = 5;         // Motor A PWM control
const uint16_t AIN2 = 6;         // Motor A input 2
const uint16_t AIN1 = 9;         // Motor A input 1
//const uint16_t STBY = 13;        // Standby

// Motor control pins for encoder feedback motor
const int Input1 = 2;
const int Input2 = 3;
//const int motorSpeedPin = 11;  // PWM pin for motor speed control

// Constants
//const uint16_t ANALOG_WRITE_BITS = 8;
//const uint16_t MAX_PWM = pow(2, ANALOG_WRITE_BITS);
//const uint16_t MIN_PWM = MAX_PWM / 3;    // Make sure motor turns

// Global variables for motor control with encoder feedback
volatile int counter = 0;
int desiredCounter = 0;

// Constants for tube advancement
const float distancePerRevolution = 10.0; // Distance advanced per motor revolution (in cm)
const float gearRatio = 50.0;             // Gear ratio of the motor
const float countsPerRevolution = 12; // Counts per motor revolution (based on encoder),not multiplied by 4?

void setup() {
  // Initialize motors
  initMotors();
  pinMode(Input1, INPUT);
  pinMode(Input2, INPUT);
  //pinMode(motorSpeedPin, OUTPUT);

  // Attach interrupts for encoder feedback motor
  attachInterrupt(digitalPinToInterrupt(Input1), ISR_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(Input2), ISR_B, CHANGE);

  // Calculate desired counter based on distance to move
  float desiredDistance = 10.0; // Distance to move in cm
  desiredCounter = (desiredDistance / distancePerRevolution) * countsPerRevolution * gearRatio;

  // Run the loop to move the tube
  while (counter < desiredCounter) {
    // Motor control with encoder feedback
    int errorCounts = desiredCounter - counter;
    float errorRadians = errorCounts * (2 * PI) / countsPerRevolution;
    float controlSignal = 3.3 * errorRadians;

    // Motor direction control
    if (controlSignal > 0) {
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, HIGH);
    } else {
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);
    }

    // Set motor speed based on control signal
    //int speed = map(abs(controlSignal), 0, (int)(3.3 * (2 * PI)), 0, 255);
    //analogWrite(PWMA, speed);

    // Control the loop timing
    delay(10);
  }

  // After reaching the desired distance, stop the motor
  brakeA();
}

void loop() {
  // Your main loop code here
}

// Interrupt service routines for encoder feedback motor
void ISR_A() {
  if (digitalRead(Input2) == digitalRead(Input1)) {
    counter--;
  } else {
    counter++;
  }
}

void ISR_B() {
  if (digitalRead(Input2) == digitalRead(Input1)) {
    counter++;
  } else {
    counter--;
  }
}

/*
// Functions for TB6612FNG motor control (same as before)
void forwardA(uint16_t pwm) {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  if (pwm > MAX_PWM) {
    pwm = MAX_PWM;
  }
  if (pwm < MIN_PWM) {
    pwm = MIN_PWM;
  }
  analogWrite(PWMA, pwm);
}

void reverseA(uint16_t pwm) {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  if (pwm > MAX_PWM) {
    pwm = MAX_PWM;
  }
  if (pwm < MIN_PWM) {
    pwm = MIN_PWM;
  }
  analogWrite(PWMA, pwm);
}

*/

void brakeA() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
}

/*
void standbyMotors(bool standby) {
  if (standby == true) {
    digitalWrite(STBY, LOW);
  } else {
    digitalWrite(STBY, HIGH);
  }
}
*/
void initMotors() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
 //pinMode(PWMA, OUTPUT);
 // pinMode(STBY, OUTPUT);
 
  //standbyMotors(false);
}
