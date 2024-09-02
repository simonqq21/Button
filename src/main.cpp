#include <Arduino.h>
#include <Button.h>

int button_pin = 4;
int led_pin = 13;
bool ledState;

Button btn1(button_pin);

void toggleLED() {
    ledState = !ledState;
    digitalWrite(led_pin, ledState);
    Serial.println("toggled LED state");
}

void turnOffLED() {
    ledState = false;
    digitalWrite(led_pin, ledState);
    Serial.println("turned off LED");
}

void turnOnLED() {
    ledState = true;
    digitalWrite(led_pin, ledState);
    Serial.println("turned on LED");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  btn1.begin();
  pinMode(led_pin, OUTPUT);
//   btn1.setShortPressFunc(turnOnLED);
//   btn1.setLongPressFunc(turnOffLED);
//   btn1.setDoublePressFunc(toggleLED);

  btn1.setShortPressFunc(toggleLED);
  btn1.setLongPressFunc();
  btn1.setDoublePressFunc();
}

void loop() {
    btn1.loop();
    // btn1.startBtnLoop(toggleLED, NULL, NULL);
}
