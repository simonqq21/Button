#include <Arduino.h>

#define BUTTON_OPEN 0
#define BUTTON_SHORT_PRESS 1 
#define BUTTON_LONG_PRESS 2 
#define BUTTON_DOUBLE_SHORT_PRESS 3
#define DELAY_DEBOUNCE 40
#define DELAY_LONG_PRESS 1000
#define DELAY_DBLCLICK 400

int button_pin = 18;
int led_pin = 5;
bool btnState, lastBtnState, trigBtnState, btnSurePressed; 
unsigned long lastDebounceTime, pressTimer, doubleClickTimer;
int btnPressed, timePressed, buttonClicks;
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
    ledState = false;
    digitalWrite(led_pin, ledState);
    Serial.println("turned off LED");
}

void turnOnLED() {
    ledState = true;
    digitalWrite(led_pin, ledState);
    Serial.println("turned on LED");
}

// this function must be put in the main loop to check the button in a nonblocking 
// fashion
void startBtnLoop(void (*shortPressFunc)(), void (*longPressFunc)(), void (*doubleShortPressFunc)()) {
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
            // Serial.println("debounce timer");
            btnSurePressed = true;
        }

        else if (btnState && !trigBtnState && btnSurePressed) { // if the button is released before the long press, it is a short press.
            btnSurePressed = false;
            // check for double click 
            if (buttonClicks == 0) {
                doubleClickTimer = millis();
            }
            if (millis() - doubleClickTimer < DELAY_DBLCLICK) {
                buttonClicks++;
                if (buttonClicks >= 2) {
                    btnPressed = BUTTON_DOUBLE_SHORT_PRESS;
                    Serial.println("button double short pressed ");
                    doubleShortPressFunc(); // run short press callback
                    buttonClicks = 0;
                    doubleClickTimer = millis();
                }
            }      
        }
        trigBtnState = btnState;
        lastDebounceTime = millis();
    }

    if (millis() - doubleClickTimer >= DELAY_DBLCLICK && buttonClicks > 0) {
        btnPressed = BUTTON_SHORT_PRESS;
        Serial.println("button short pressed ");
        shortPressFunc(); // run short press callback
        buttonClicks = 0;
        doubleClickTimer = millis();
    }

    timePressed = millis() - DELAY_DEBOUNCE - pressTimer; // measure the time how long the button was held down to check if its a long press 
    if (btnSurePressed && timePressed > DELAY_LONG_PRESS) { // if button still held down, it is a long press.
        btnPressed = BUTTON_LONG_PRESS;
        Serial.println("button long pressed ");
        longPressFunc(); // run long press callback
        btnSurePressed = false;
        trigBtnState = btnState;
    }
}

void loop() {
    startBtnLoop(toggleLED, turnOffLED, turnOnLED);
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