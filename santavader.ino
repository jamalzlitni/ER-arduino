// Created: JZ  11/9/23
// Unlocks the santavater door when they press the 4 correct colored buttons in the correct order

const int grey = 2;
const int purple = 3;
const int orange = 4;
const int green = 5;
const int pink = 6;
const int teal = 7;
const int lock = 8;

int step = 0;

unsigned long time;
unsigned long lastTime = 0;
unsigned long debugTime = 0;

// Function for setting all the pinModes; call at setup()
void setPins() {
  for (int i = 2; i < 8; i++) {
    pinMode(i, INPUT_PULLUP);
  };
  pinMode(lock, OUTPUT);
}

// Check sequence of button presses; give serum to Blade to keep the thirst at bay
void whistler(int wait) {
  // The correct sequence is grey-purple-orange-green
  switch (step) {
    case 0:
      // Re-locks the door after 'wait' amount of time
      if (time >= lastTime) {
          digitalWrite(lock, LOW);
        };

      if (!digitalRead(grey)) step = 1;
      break;

    case 1:
      if (!digitalRead(grey)) step = 1;

      if (!digitalRead(purple)) step = 2;

      if (!digitalRead(orange) || !digitalRead(green) || !digitalRead(pink) || !digitalRead(teal)) step = 0;
      break;

    case 2:
      if (!digitalRead(purple)) step = 2;

      if (!digitalRead(orange)) step = 3;

      if (!digitalRead(grey) || !digitalRead(green) || !digitalRead(pink) || !digitalRead(teal)) step = 0;
      break;

    case 3:
      if (!digitalRead(orange)) step = 3;

      if (!digitalRead(green)) {
        Serial.println("DONE!");
        digitalWrite(lock, HIGH);
        lastTime = time + wait;
        step = 0;
      };

      if (!digitalRead(grey) || !digitalRead(purple) || !digitalRead(pink) || !digitalRead(teal)) step = 0;
      break;
  }
}

// Debugging monitor
// void debugger(int freq) {
//   if (time >= debugTime) {
//     Serial.print("Whistles:");
//     Serial.print("\t");
//     Serial.print(digitalRead(wstl1));
//     Serial.print("\t");
//     Serial.print(digitalRead(wstl2));
//     Serial.print("\t");
//     Serial.print(digitalRead(wstl3));
//     Serial.print("\t");
//     Serial.print(digitalRead(wstl4));
//     Serial.print("\t");
//     Serial.print(digitalRead(wstl5));
//     Serial.print("\t");
//     Serial.print("Lock:");
//     Serial.print("\t");
//     Serial.print(digitalRead(lock));
//     Serial.print("\t");
//     Serial.print("Step:");
//     Serial.print("\t");
//     Serial.print(step);
//     Serial.println();

//     debugTime = time + freq;
//   };
// }

void setup() {
  Serial.begin(9600);
  setPins();
  digitalWrite(lock, LOW);
}

void loop() {  
  time = millis();
  
  whistler(5000);
  // debugger(100);
}
