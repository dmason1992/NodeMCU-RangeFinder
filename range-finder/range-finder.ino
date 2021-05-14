#include <HCSR04.h>

// pin assignments
int led_yellow = 13;
int led_red = 15;
int trig_pin = 4;
int echo_pin = 5;

// user parameters
int seated_distance = 100;

// globals
int seat_full = 0;
int seat_empty = 0;
long last_check_time = 0;

HCSR04 hc(trig_pin, echo_pin);//initialisation class HCSR04 (trig pin , echo pin)

// function to create led blinking sequence
void blink_led(int quantity, int duration, int pin) {
  for (int i = 0; i <= quantity; ++i){
    digitalWrite(pin, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(duration);                       // wait for a second
    digitalWrite(pin, LOW);    // turn the LED off by making the voltage LOW
    delay(duration);                       // wait for a second
  }
}

// function to check if seat is occupied based on measurement distance
bool is_seated(int seated_distance) {
  int dist = hc.dist();
  if (dist <= seated_distance) {
    //Serial.println("Seated: " + String(dist) + "cm");
    return true;
  }
  else {
    //Serial.println("Not seated " + String(dist) + "cm");
    return false;
  }
}

// function to update the seat occupancy total times
void update_seated_time() {
  bool check_seated = is_seated(seated_distance);
  long check_time = millis();
  int delta = check_time - last_check_time;
  last_check_time = check_time;
  if (check_seated) {    
    seat_full = seat_full + delta/1000;
    //Serial.println ("seated time: " + String(seat_full) + "s");
  }
  else {
    seat_empty = seat_empty + delta/1000;
    //Serial.println ("seat empty time: " + String(seat_empty) + "s");  
  }
}

// function to convert seconds to hh:mm:ss values
void seconds_to_hms(int seconds_total, int *hours, int *mins, int *secs) {
  *mins = seconds_total / 60;
  *secs = seconds_total % 60;
  *hours = *mins / 60;
  *mins = *mins % 60;
}

// function to print out seat stats
void print_seat_stats(int seat_full, int seat_empty) {
  int empty_hours, empty_mins, empty_secs;
  seconds_to_hms(seat_empty, &empty_hours, &empty_mins, &empty_secs);
  Serial.println("Seat empty: " + String(empty_hours) + "h:" + String(empty_mins) + "m:" + String(empty_secs) + "s");

  int full_hours, full_mins, full_secs;
  seconds_to_hms(seat_full, &full_hours, &full_mins, &full_secs);
  Serial.println("Seat full: " + String(full_hours) + "h:" + String(full_mins) + "m:" + String(full_secs) + "s");
  Serial.println();
}

 
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(led_yellow, OUTPUT);
  pinMode(led_red, OUTPUT);

  Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {
  update_seated_time();
  print_seat_stats(seat_full, seat_empty);
  delay(1000);
}
