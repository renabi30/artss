
look here -> https://community.element14.com/products/arduino/b/blog/posts/simple-arduino-dc-motor-control-with-encoder-part-1
//counter -> interrupts 
volatile int counter= 0;

//desired counter
int desiredCounter = 0;

//time 
unsigned long previousTime = 0;
const int outputDuration = 4;
const int deltaT = 10;

void setup() {
  Serial.begin(9600);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(6, LOW);
  digitalWrite(5, LOW);

  //below is for the encoder 
  pinMode(2, INPUT); //declare pins 2 and 3 as input
  pinMode(3, INPUT);
  attachInterrupt(0, ISR_A, CHANGE);  //attach interrupt to PIN2 triggered on change
  attachInterrupt(1, ISR_B, CHANGE);  //attach interrupt to PIN3 triggered on change

  // Reset counter and set desired counter for a full revolution
  counter = 0;
  desiredCounter = 12 * 4 * 50; // 12 cycles per revolution, 4 counts per cycle, 50:1 gear ratio

 

 float startTime = millis();

// Run the step response loop in the setup
  while (millis() - startTime < outputDuration * 1000) {

 
  int errorCounts = desiredCounter - counter;
  float errorRadians = errorCounts * ( 2 * PI) / (12 * 4 * 50);



//kprop is 3.3 can be adjusted, might not need kprop if not doing derivative control
float controlSignal = 3.3 * errorRadians;


//positive control signal, this is for 
if (controlSignal >0 ) {
  digitalWrite(Input1, HIGH); //need to declare Input1 and Input2 as eitherpin 5 or6
  digitalWrite(Input2, LOW);
}
//negative control signal 
if (controlSignal < 0) {
  digitalWrite(Input1, LOW);
  digitalWrite(Input2, HIGH);
}
// Display counter as a percent of a full rotation every 10th data point
    if (counter % 10 == 0) {
      float percentRotation = float(counter / (12 * 4 * 50)) * 100;
      Serial.println(percentRotation);
    }
    // Control the loop timing
    delay(deltaT);
}
}
}

void loop() {
 
/*
 digitalWrite(6, HIGH); //this reverses the tube out of the patient 
  delay(1000000); //1000 seconds
  digitalWrite(6, LOW);
  */


}
//interrupt methods
void ISR_A() { //interrupt service routine 
  if (digitalRead(3) == digitalRead(2)) { //if encoder a and b read the same
    counter--; //subtract 1 from counter
  } else {     //if encoder A and B are different
    counter++; //add one to counter
  }
}
void ISR_B() {
  if (digitalRead(3) == digitalRead(2)) { //if encoder A and B are the same
    counter++; //add 1 to counter
  } else { //if encoder A and B are different
    counter--; //subtract 1 from counter
  }
}
