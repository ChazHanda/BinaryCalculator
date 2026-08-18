const int bit[] = {0, 2, 4, 6, 8, 10};
const int bitCount = 6;

const int buttonPlus[] = {23, 21, 19};
const int buttonMinus[] = {22, 20, 18};

const int buttonAdd = 17;
const int buttonSubtract = 16;
const int buttonEqual = 12;

int count = 0;
int storedNumber = 0;
int opCode = 0;
bool overflow = false;

volatile int interruptCode = 0;
volatile unsigned long lastInterruptTime = 0;

const unsigned long debounceTime = 250;

void setup() {
  for (int i = 0; i < bitCount; i++) {
    pinMode(bit[i], OUTPUT);
  }

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

}

void loop() {
  if (interruptCode != 0) {
    switch (interruptCode) {
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
          changeNumber(storedNumber);
          storedNumber = 0;
        }
        opCode = 0;
        break;
    }
    interruptCode = 0;
  }
  //delay(100);
}

void blinkLight(int number) { //function blinks based on argument
  for (int i = number; i > 0; i--) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
  }
}

int overflowHandler(int number) { //function returns an int between 0 and 63, and blinks the light when it wraps
    while(number > 63) {
      number = number - 64;
      overflow = true;
    }

    while(number < 0) {
      number = number + 64;
      overflow = true;
    }

    if (overflow) {  //overflow blinks 4 times
      blinkLight(4);
      overflow = false;
    }

    return number;
}

void changeNumber(int number) {  // function changes the display based on the argument
  count = overflowHandler(number);
  for (int i = 0; i < bitCount; i++) {  //toggle LEDs based on count
    if (bitRead(count, i)) {
      digitalWrite(bit[i], HIGH);
    } else {
      digitalWrite(bit[i], LOW);
    }
  }
}

bool debounceCheck() {  // funtion returns true if enough time passes between triggers
  unsigned long currentTime = millis();
  if (currentTime - lastInterruptTime > debounceTime) {
    lastInterruptTime = currentTime;
    return true;
  }
  return false;
}

void opButton(int number) { //function changes storedNumber based on argument (opCode)
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