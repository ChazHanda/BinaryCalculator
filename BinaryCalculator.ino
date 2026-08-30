const int bit[] = {0, 2, 4, 6, 8, 10};
const int bitCount = 6;

const int buttonPlus[] = {23, 21, 19};
const int buttonMinus[] = {22, 20, 18};

const int buttonAdd = 17;
const int buttonSubtract = 16;
const int buttonEqual = 12;

const int digitTopLeft = 9;
const int digitTopMid = 5;
const int digitTopRight = 3;
const int digitMid = 1;
const int digitBotLeft = 14;
const int digitBotMid = 13;
const int digitBotRight = 7;

const int digitPlace[] = {15, 11};


int count = 0;
int storedNumber = 0;
int digitalNumber = 0;
int onesPlace = 0;
int tensPlace = 0;
int opCode = 0;

bool overflow = false;
bool digitCheckFlag = true;

volatile int interruptCode = 0;
volatile unsigned long lastInterruptTime = 0;

const unsigned long debounceTime = 500;

void setup() {
  for (int i = 0; i < bitCount; i++) {
    pinMode(bit[i], OUTPUT);
  }

  pinMode(digitTopLeft, OUTPUT);
  pinMode(digitTopMid, OUTPUT);
  pinMode(digitTopRight, OUTPUT);
  pinMode(digitMid, OUTPUT);
  pinMode(digitBotLeft, OUTPUT);
  pinMode(digitBotMid, OUTPUT);
  pinMode(digitBotRight, OUTPUT);

  pinMode(digitPlace[0], OUTPUT);
  pinMode(digitPlace[1], OUTPUT);


  pinMode(buttonPlus[0], INPUT_PULLUP);
  attachInterrupt(
    digitalPinToInterrupt(buttonPlus[0]),
    buttonInterruptPlus0,
    FALLING
  );

  pinMode(buttonPlus[1], INPUT_PULLUP);
  attachInterrupt(
    digitalPinToInterrupt(buttonPlus[1]),
    buttonInterruptPlus1,
    FALLING
  );

  pinMode(buttonPlus[2], INPUT_PULLUP);
  attachInterrupt(
    digitalPinToInterrupt(buttonPlus[2]),
    buttonInterruptPlus2,
    FALLING
  );

  pinMode(buttonMinus[0], INPUT_PULLUP);
  attachInterrupt(
    digitalPinToInterrupt(buttonMinus[0]),
    buttonInterruptMinus0,
    FALLING
  );

  pinMode(buttonMinus[1], INPUT_PULLUP);
  attachInterrupt(
    digitalPinToInterrupt(buttonMinus[1]),
    buttonInterruptMinus1,
    FALLING
  );

  pinMode(buttonMinus[2], INPUT_PULLUP);
  attachInterrupt(
    digitalPinToInterrupt(buttonMinus[2]),
    buttonInterruptMinus2,
    FALLING
  );

  pinMode(buttonAdd, INPUT_PULLUP);
  attachInterrupt(
    digitalPinToInterrupt(buttonAdd),
    buttonInterruptAdd,
    FALLING
  );

  pinMode(buttonSubtract, INPUT_PULLUP);
  attachInterrupt(
    digitalPinToInterrupt(buttonSubtract),
    buttonInterruptSub,
    FALLING
  );

  pinMode(buttonEqual, INPUT_PULLUP);
  attachInterrupt(
    digitalPinToInterrupt(buttonEqual),
    buttonInterruptEqual,
    FALLING
  );

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

}

void loop() {
  switch (interruptCode) {
    case 0: // skip
      break;

    case 1: // add 1
      changeNumber(count + 1);
      break;
    
    case 2: // add 4
      changeNumber(count + 4);
      break;
    
    case 3: // add 16
      changeNumber(count + 16);
      break;
    
    case 11:  // subtract 1
      changeNumber(count - 1);
      break;
    
    case 12:  // subtract 4
      changeNumber(count - 4);
      break;
    
    case 13:  // subtract 16
      changeNumber(count - 16);
      break;
    
    case 21:  // addition button
      if (storedNumber != 0) {
        opButton(opCode);
      } else {
        storedNumber = count;
      }
      opCode = 1;
      changeNumber(0);
      break;
    
    case 22:  // subtraction button
      if (storedNumber != 0) {
        opButton(opCode);
      } else {
        storedNumber = count;
      }
      opCode = 2;
      changeNumber(0);
      break;

    case 30:  // equals button
      if (storedNumber != 0) {
        opButton(opCode);
        digitalNumber = overflowHandler(storedNumber, 100);
        splitDigits();
        digitCheckFlag = false;
        changeNumber(0); 
        digitCheckFlag = true;
        storedNumber = digitalNumber;
      }
      opCode = 0;
      break;
  }
  interruptCode = 0;
  if (digitCheckFlag) {
    showDigitalDisplay(onesPlace, tensPlace);
  }
}

void blinkLight(int number) { //function blinks based on argument
  for (int i = number; i > 0; i--) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
  }
}

int overflowHandler(int number, int range) { //function returns an int between 0 and 63, and blinks the light when it wraps
    while(number >= range) {
      number = number - range;
      overflow = true;
    }

    while(number < 0) {
      number = number + range;
      overflow = true;
    }

    if (overflow == true) {  //overflow blinks 4 times
      blinkLight(4);
      overflow = false;
    }

    return number;
}

void changeNumber(int number) {  // function changes the display based on the argument
  count = overflowHandler(number, 64);
  for (int i = 0; i < bitCount; i++) {  //toggle LEDs based on count
    if (bitRead(count, i)) {
      digitalWrite(bit[i], HIGH);
    } else {
      digitalWrite(bit[i], LOW);
    }
  }
  if (digitCheckFlag) {
    digitalNumber = count;
    splitDigits();
  }
}

bool debounceCheck() {  // funtion returns true if enough time passes between triggers
  unsigned long currentTime = millis();
  if (currentTime - lastInterruptTime > debounceTime) {
    lastInterruptTime = currentTime;
    return true;
  } else {
    return false;
  }
}

void opButton(int number) {
  switch (number) {
    case 0:
      break;

    case 1: // addition
      storedNumber = storedNumber + count;
      break;

    case 2: // subtraction
      storedNumber = storedNumber - count;
      break;
  }
  count = 0;
}

void buttonInterruptPlus0() { // add 1
  if (debounceCheck()) {
    interruptCode = 1;
  }
}

void buttonInterruptPlus1() { // add 4
  if (debounceCheck()) {
    interruptCode = 2;
  }
}

void buttonInterruptPlus2() { // add 16
  if (debounceCheck()) {
    interruptCode = 3;
  }
}

void buttonInterruptMinus0() {  // subtract 1
  if (debounceCheck()) {
    interruptCode = 11;
  }
}

void buttonInterruptMinus1() {  // subtract 4
  if (debounceCheck()) {
    interruptCode = 12;
  }
}

void buttonInterruptMinus2() {  // subtract 16
  if (debounceCheck()) {
    interruptCode = 13;
  }
}

void buttonInterruptAdd() {  // addition button
  if (debounceCheck()) {
    interruptCode = 21;
  }
}

void buttonInterruptSub() {  // subtraction button
  if (debounceCheck()) {
    interruptCode = 22;
  }
}

void buttonInterruptEqual() {  // equal button
  if (debounceCheck()) {
    interruptCode = 30;
  }
}

void digitDisplay(int digit, int position) {  // turn on a digit of the digital display
  switch (digit) {
    case 0:
      digitalWrite(digitTopLeft, HIGH);
      digitalWrite(digitTopMid, HIGH);
      digitalWrite(digitTopRight, HIGH);
      digitalWrite(digitMid, LOW);
      digitalWrite(digitBotLeft, HIGH);
      digitalWrite(digitBotMid, HIGH);
      digitalWrite(digitBotRight, HIGH);
      break;

    case 1:
      digitalWrite(digitTopLeft, LOW);
      digitalWrite(digitTopMid, LOW);
      digitalWrite(digitTopRight, HIGH);
      digitalWrite(digitMid, LOW);
      digitalWrite(digitBotLeft, LOW);
      digitalWrite(digitBotMid, LOW);
      digitalWrite(digitBotRight, HIGH);
      break;

    case 2:
      digitalWrite(digitTopLeft, LOW);
      digitalWrite(digitTopMid, HIGH);
      digitalWrite(digitTopRight, HIGH);
      digitalWrite(digitMid, HIGH);
      digitalWrite(digitBotLeft, HIGH);
      digitalWrite(digitBotMid, HIGH);
      digitalWrite(digitBotRight, LOW);
      break;

    case 3:
      digitalWrite(digitTopLeft, LOW);
      digitalWrite(digitTopMid, HIGH);
      digitalWrite(digitTopRight, HIGH);
      digitalWrite(digitMid, HIGH);
      digitalWrite(digitBotLeft, LOW);
      digitalWrite(digitBotMid, HIGH);
      digitalWrite(digitBotRight, HIGH);
      break;

    case 4:
      digitalWrite(digitTopLeft, HIGH);
      digitalWrite(digitTopMid, LOW);
      digitalWrite(digitTopRight, HIGH);
      digitalWrite(digitMid, HIGH);
      digitalWrite(digitBotLeft, LOW);
      digitalWrite(digitBotMid, LOW);
      digitalWrite(digitBotRight, HIGH);
      break;

    case 5:
      digitalWrite(digitTopLeft, HIGH);
      digitalWrite(digitTopMid, HIGH);
      digitalWrite(digitTopRight, LOW);
      digitalWrite(digitMid, HIGH);
      digitalWrite(digitBotLeft, LOW);
      digitalWrite(digitBotMid, HIGH);
      digitalWrite(digitBotRight, HIGH);
      break;

    case 6:
      digitalWrite(digitTopLeft, HIGH);
      digitalWrite(digitTopMid, HIGH);
      digitalWrite(digitTopRight, LOW);
      digitalWrite(digitMid, HIGH);
      digitalWrite(digitBotLeft, HIGH);
      digitalWrite(digitBotMid, HIGH);
      digitalWrite(digitBotRight, HIGH);
      break;

    case 7:
      digitalWrite(digitTopLeft, LOW);
      digitalWrite(digitTopMid, HIGH);
      digitalWrite(digitTopRight, HIGH);
      digitalWrite(digitMid, LOW);
      digitalWrite(digitBotLeft, LOW);
      digitalWrite(digitBotMid, LOW);
      digitalWrite(digitBotRight, HIGH);
      break;

    case 8:
      digitalWrite(digitTopLeft, HIGH);
      digitalWrite(digitTopMid, HIGH);
      digitalWrite(digitTopRight, HIGH);
      digitalWrite(digitMid, HIGH);
      digitalWrite(digitBotLeft, HIGH);
      digitalWrite(digitBotMid, HIGH);
      digitalWrite(digitBotRight, HIGH);
      break;

    case 9:
      digitalWrite(digitTopLeft, HIGH);
      digitalWrite(digitTopMid, HIGH);
      digitalWrite(digitTopRight, HIGH);
      digitalWrite(digitMid, HIGH);
      digitalWrite(digitBotLeft, LOW);
      digitalWrite(digitBotMid, HIGH);
      digitalWrite(digitBotRight, HIGH);
      break;
  }

  digitalWrite(digitPlace[position], HIGH);

  switch (position) {
    case 0:
      digitalWrite(digitPlace[1], LOW);
      break;

    case 1:
      digitalWrite(digitPlace[0], LOW);
      break;
  }
  delay(4);
  displayOff();
}

void displayOff() {  // turn off digital display
  digitalWrite(digitTopLeft, LOW);
      digitalWrite(digitTopMid, LOW);
      digitalWrite(digitTopRight, LOW);
      digitalWrite(digitMid, LOW);
      digitalWrite(digitBotLeft, LOW);
      digitalWrite(digitBotMid, LOW);
      digitalWrite(digitBotRight, LOW);

      digitalWrite(digitPlace[0], LOW);
      digitalWrite(digitPlace[1], LOW);
}

void showDigitalDisplay(int ones, int tens) {  // flash all digits of the display
  digitDisplay(ones, 1);
  digitDisplay(tens, 0);
}

void splitDigits() {  //stores global variables
  onesPlace = digitalNumber % 10;
  tensPlace = (digitalNumber/10) % 10;
}