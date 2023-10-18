unsigned long startMillis;
unsigned long currentMillis;
unsigned long retractionMillis;
//Kuka output signal: feed MD
const int kuka_feed_signal = 3; //for interrupts only works on Pin 2 or 3 for arduino nano
const int vel_pin = 5;
const int extruder_speed = 255; //Range 138 to 255, 138-> 0 RPM
const int zero_rpm_offset = 138;
const int retraction_speed = 0; //Range 138 to 0, 138-> 0 RPM
bool flag_retraction_delay = false;
long retraction_time = 300;

void setup() {
  //Debug
  //Serial.begin(9600);
  //Set input mode for kuka signal reading
  startMillis = millis();  //initial start time
  //pinMode(kuka_feed_signal, INPUT);
  pinMode(vel_pin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(kuka_feed_signal), retraction, FALLING); //Detect every HIGH-> LOW value from KUKA

}

void retraction() {
  flag_retraction_delay = true;
  retractionMillis = currentMillis;
  analogWrite(vel_pin, zero_rpm_offset);//pause the motor for better performance at retraction
}

void feed() {
  analogWrite(vel_pin, extruder_speed); // Desired speed for extrusion
  //Debug
  //Serial.println(digitalRead(kuka_feed_signal));
}

void idle_speed() {
  analogWrite(vel_pin, zero_rpm_offset
             ); // equation P1_40/10 * [Vin - P4_22/1000], at PWM 138 -> 5V -> 0 RPM
  //Debug
  //Serial.println(digitalRead(kuka_feed_signal));
}

void loop() {
  currentMillis = millis();
  //rewrite the commented code
  if(flag_retraction_delay == true && currentMillis - retractionMillis <= retraction_time) {
    analogWrite(vel_pin, retraction_speed);
  }
  else {
    flag_retraction_delay = false;
  }
  //extrude while kuka signal is HIGH, else idle speed
  if (digitalRead(kuka_feed_signal) == HIGH) {
    feed();
  }
  else {
    idle_speed();
  }
  
}
