#include <Arduino.h>
#include <Button.h>

int button_pin = 18;
int led_pin = 5;
bool ledState;

Button btn1(button_pin);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  btn1.begin();
  pinMode(led_pin, OUTPUT);
}

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

void loop() {
    btn1.startBtnLoop(turnOnLED, turnOffLED, toggleLED);
}





//             btnPressed = 1;
//             trigBtnState = btnState;



// this function must be put in the main loop to set the relay timer and close
// the relay when the button is pressed
// void executeActionOnBtnPress() {
//     if (btnPressed) {
//         // execute button press action
//         // if relay is on, a button press would turn it off.
//         // if (relayState) {
//         //     setRelay(false);
//         // }
//         // else {
//         //     setRelay(true);
//         // }
//         // reset buttonPressed value
//         btnPressed = 0;
//     }
// }