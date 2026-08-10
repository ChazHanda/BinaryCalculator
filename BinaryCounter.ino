const int bit[] = {0, 2, 4, 6, 8, 10};
const int bitCount = 6;

const int button0 = 22;
int count = 0;

void setup() {
  for (int i = 0; i < bitCount; i++) {
    pinMode(bit[i], OUTPUT);
  }

  pinMode(button0, INPUT);

}

void loop() {

  if (digitalRead(button0) == HIGH) { //if button is pressed
    count++;
    if (count > 63) {
      count = 0;
    }

    for (int i = 0; i < bitCount; i++) {  //toggle LEDs based on count
      if (bitRead(count, i)) {
        digitalWrite(bit[i], HIGH)
      } else {
        digitalWrite(bit[i], LOW)
      }
    }
  }

  delay(500);
}
