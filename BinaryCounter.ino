const int bit[] = {0, 2, 4, 6, 8, 10};
const int bitCount = 6;

const int button0 = 22;
const int button1 = 21;
int count = 0;

void setup() {
  for (int i = 0; i < bitCount; i++) {
    pinMode(bit[i], OUTPUT);
  }

  pinMode(button0, INPUT_PULLUP);
  pinMode(button1, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {

  if ((digitalRead(button0) == LOW) ^ (digitalRead(button1) == LOW)) { //if a single button is pressed

    if (digitalRead(button0) == LOW) {  //Button0 is pressed
      count++;
      if (count > 63) {
      count = 0;
      }
    } else {  //Button1 is pressed
      count--;
      if (count < 0) {
        count = 63;
      }
    }

    for (int i = 0; i < bitCount; i++) {  //toggle LEDs based on count
      if (bitRead(count, i)) {
        digitalWrite(bit[i], HIGH);
      } else {
        digitalWrite(bit[i], LOW);
      }
    }
  }

  if (digitalRead(LED_BUILTIN) == LOW) {  //toggle built in LED
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  
  delay(500);
}
