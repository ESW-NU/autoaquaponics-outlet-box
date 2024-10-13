#include <Arduino.h>

// Define GPIO pins
const int pin1 = 12;
const int pin2 = 25;
const int pin3 = 26;


// Software timers
TimerHandle_t softwareTimer1;
TimerHandle_t softwareTimer2;
TimerHandle_t softwareTimer3;

// Software timer callback functions
void softwareTimerCallback1(TimerHandle_t xTimer) {
  digitalWrite(pin1, !digitalRead(pin1));  // Toggle pin3
}

void softwareTimerCallback2(TimerHandle_t xTimer) {
  digitalWrite(pin2, !digitalRead(pin2));  // Toggle pin4
}

void softwareTimerCallback3(TimerHandle_t xTimer) {
  digitalWrite(pin3, !digitalRead(pin3));  // Toggle pin5
}

void setup() {
  // Initialize GPIO pins as output
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);

  // Initialize software timers
  softwareTimer1 = xTimerCreate("Timer1", pdMS_TO_TICKS(3000), pdTRUE, (void*) 0, softwareTimerCallback1);
  softwareTimer2 = xTimerCreate("Timer2", pdMS_TO_TICKS(4000), pdTRUE, (void*) 0, softwareTimerCallback2);
  softwareTimer3 = xTimerCreate("Timer3", pdMS_TO_TICKS(5000), pdTRUE, (void*) 0, softwareTimerCallback3);

  // Start software timers
  xTimerStart(softwareTimer1, 0);
  xTimerStart(softwareTimer2, 0);
  xTimerStart(softwareTimer3, 0);
}

void loop() {
  // Nothing to do in the loop, timers handle pin toggling
}
