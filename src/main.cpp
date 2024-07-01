#include <Arduino.h>

#define BUTTON_OPEN 0
#define BUTTON_SHORT_PRESS 1 
#define BUTTON_LONG_PRESS 2 
#define BUTTON_DOUBLE_SHORT_PRESS 3
#define DELAY_DEBOUNCE 50
#define DELAY_LONG_PRESS 1000

int button_pin = 18;
int led_pin = 5;
bool btnState, lastBtnState, trigBtnState, btnTrigd; 
unsigned long lastDebounceTime, pressTimer;
int btnPressed, timePressed;
bool ledState;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(led_pin, OUTPUT);
}

void toggleLED() {
    ledState = !ledState;
    digitalWrite(led_pin, ledState);
    Serial.println("toggled LED state");
}

void turnOffLED() {
    digitalWrite(led_pin, false);
    Serial.println("turned off LED");
}

// this function must be put in the main loop to check the button in a nonblocking 
// fashion
void checkButton(void (*shortPressFunc)(), void (*longPressFunc)()) {
    btnState = digitalRead(button_pin); // poll button state
    if (btnState != lastBtnState) { // if button state changes, start debouncing timer
                                    // and save button states.
        lastDebounceTime = millis();
        trigBtnState = !btnState;
        lastBtnState = btnState;
    }
    
    if (millis() - lastDebounceTime > DELAY_DEBOUNCE) { // if debounce timer is exceeded
        if (!btnState && trigBtnState) { // if button is still held down after the debounce 
                                        // delay, start the press timer.
            pressTimer = millis();
            Serial.println("debounce timer");
            btnTrigd = true;
            lastDebounceTime = millis();
        }

        else if (btnState && !trigBtnState && btnTrigd) { // if the button is released, measure the amount of time the button was pressed to 
                                    // determine whether it was a short or long press.
            btnTrigd = false;
            btnPressed = BUTTON_SHORT_PRESS;
            Serial.println("button short pressed ");
            lastDebounceTime = millis();
            shortPressFunc();
        }
        trigBtnState = btnState;
    }

    timePressed = millis() - DELAY_DEBOUNCE - pressTimer;
    if (btnTrigd && timePressed > DELAY_LONG_PRESS) {
        btnPressed = BUTTON_LONG_PRESS;
        Serial.println("button long pressed ");
        longPressFunc();
        btnTrigd = false;
        trigBtnState = btnState;
    }
}

void startLoop() {
    checkButton(toggleLED, turnOffLED);
}

void loop() {
  startLoop();
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