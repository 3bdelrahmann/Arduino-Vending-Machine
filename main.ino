#include <Keypad.h>
#include <Servo.h>

const byte ROWS = 4;  // four rows
const byte COLS = 3;  // four columns
// define the symbols on the buttons of the keypad
char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};
byte rowPins[ROWS] = { 9, 8, 7, 6 };  // connect to the row pinouts of the keypad
byte colPins[COLS] = { 5, 4, 3 };     // connect to the column pinouts of the keypad

// initialize an instance of class NewKeypad
Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// Define the pins for the servos
const int servo1Pin = 10;
const int servo2Pin = 11;
const int servo3Pin = 12;
const int servo4Pin = 13;

// Variables to store the state of each servo
bool servo1State = true;
bool servo2State = true;
bool servo3State = true;
bool servo4State = true;

const int coin = 2;
volatile boolean insert = false;
volatile int pulse = 0;
unsigned long balance;
int count = 0;

void setup() {
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
  servo4.attach(servo4Pin);

  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(coin), coinInterrupt, RISING);
  delay(1000);

  // Set initial position for all servos
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);

}

void loop() {
  if (insert) {
    insert = false;
    count += 1;
    if (count == 2) {
      Serial.println("Coin inserted : " + String(count));
      waitForKey();
      count = 0;
    }
    delay(50);
  }
}

// Wait for a key press and handle servo control
void waitForKey() {
  char key = 0;
  while (!key) {
    key = keypad.getKey();
    delay(10);  // Small delay to debounce key press
  }

  Serial.println("Key inserted : " + String(key));

  switch (key) {
    case '1':
      toggleServo(servo1, servo1State);
      break;
    case '2':
      toggleServo(servo2, servo2State);
      break;
    case '3':
      toggleServo(servo3, servo3State);
      break;
    case '4':
      toggleServo(servo4, servo4State);
      break;
    default:
      // Do nothing for other keys
      break;
  }
}

// Function to toggle servo state and position
void toggleServo(Servo &servo, bool &state) {
  state = !state;

  if (state == 0) {
    servo.write(0);

    delay(500);
    state = 0;
    servo.write(90);
    state = 1;
  }
}

// Interrupt service routine for coin detection
void coinInterrupt() {
  pulse++;
  insert = true;
}