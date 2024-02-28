// Runaway Railcar - Big Bomb
// BMR 1-6-23
// Updated: BMR 1-18-23
// Updated: JZ 10/9/2023
// Updated: JZ 11/13/23
// Updated: JZ 11/16/23
// Updated: JZ 12/5/23
// Large fake dynamite bomb starts counting down on servo timer once opened. Users must identify the correct wire to cut in order to stop the timer.

#include <Servo.h>
Servo clock;

const int lid = 8;
const int wire1 = 7;
const int wire2 = 6;
const int wire3 = 5;
const int wire4 = 4;
const int wire5 = 3;
const int wire6 = 2;
const int tickingAudio = 31;
const int whoopsAudio = 32;
const int ohNoAudio = 33;
const int failAudio = 34;
const int winAudio = 35;

unsigned int countdown = 120;

int val;
int luck = 2;

bool armed = false;
bool boobyTrap = false;
bool gameOver = false;
bool wire1Last = true;
bool wire2Last = true;
bool wire3Last = true;
bool wire4Last = true;
bool wire5Last = true;

unsigned long time;
unsigned long lastTime;
unsigned long clockTime;
unsigned long lightTime;
unsigned long debugTime;

// Define pins and set state of outputs; call at Setup()
void setPins() {
  clock.attach(22);
  clock.write(180);
  
  for (int i = 2; i < 9; i++) {
    pinMode(i, INPUT_PULLUP);
  };

  for (int i = 22; i < 36; i++) {
    pinMode(i, OUTPUT);
  };

  for (int i = 23; i < 36; i++) {
    digitalWrite(i, LOW);
  };
}

// What's in the box?!
void lidTrigger() {
  // When the lid is opened, the timer starts and the bomb is "armed"
  if (!digitalRead(lid)) {
    //Serial.println("Lid opened");
    armed = true;
  };

  if (armed) {
    cutTheWires();
    whatDoYouDo();
  };
}

// Let's kick the tires and...
void cutTheWires() {
  // If the correct wire is cut; the bomb is disarmed
  if (luck) {
    if (digitalRead(wire6)) {
      for (int i = 23; i < 36; i++) {
        digitalWrite(i, LOW);
      };
      Serial.println("Correct wire cut");
      armed = false;
      gameOver = true;
      lastTime = time;
      luck = 7;
    };
  };

  // Check the state of each wire and decrement 'luck' accordingly.
  // There's gotta' be a slick, more economical way to do this.
  if (wire1Last) {
    if (digitalRead(wire1)) {
      Serial.println("1 Incorrect wire cut");
      wire1Last = false;
      luck--;
    };
  };

  if (wire2Last) {
    if (digitalRead(wire2)) {
      Serial.println("2 Incorrect wire cut");
      wire2Last = false;
      luck--;
    };
  };

  if (wire3Last) {
    if (digitalRead(wire3)) {
      Serial.println("3 Incorrect wire cut");
      wire3Last = false;
      luck--;
    };
  };

  if (wire4Last) {
    if (digitalRead(wire4)) {
      Serial.println("4 Incorrect wire cut");
      wire4Last = false;
      luck--;
    };
  };

  if (wire5Last) {
    if (digitalRead(wire5)) {
      Serial.println("5 Incorrect wire cut");
      wire5Last = false;
      luck--;
    };
  };

  // Make sure 'luck' doesn't fall below 0, it breaks things
  if (luck < 0) luck = 0;

  // Checks to see how your luck is holding up.
  switch(luck) {
    // So far, so good.
    case 2:
      boobyTrap = false;
      break;
    
    // The wrong wire has been cut, 'luck' is dinged and the 'boobyTrap'
    // is sprung; which causes the clock to run twice as fast.
    case 1:
      boobyTrap = true;
      break;

    // Looks like you're out of luck... or are you?
    case 0:
      armed = false;
      break;
  };
}

// If you're lost, you can look, and you will find me...
void timeAfterTime(int freq) {      // countdown, move the servo, blink the LEDs
  val = countdown;
  val = map(val, 0, 120, 0, 180);   // map the 2 minute countdown on the 180 degree servo
  clock.write(val);
  
  if (time >= lightTime + freq) {
    for (int i = 23; i < 31; i++) {     // TEST IF THIS FUNCTION WORKS JUST DIGITALWRITING 23, THAT IS THE PIN THAT ALL 6 LIGHTS ARE CONNECTED TO
      digitalWrite(i, LOW);
    };
  };

  if (time >= lightTime + freq * 2) {
    for (int i = 23; i < 31; i++) {
      digitalWrite(i, HIGH);
    };

    lightTime = time;
    countdown--;
  };
}

// Pop quiz, hotshot--there's a bomb on a train. Once the train goes above
// 50 miles an hour, the bomb is armed. If it drops below 50, it blows up...
void whatDoYouDo() {
  if (armed && !boobyTrap && luck) {    // have not cut any wires
    Serial.println("No wires cut");
    timeAfterTime(500);
    digitalWrite(tickingAudio, HIGH);
  };
  
  if (armed && boobyTrap && luck) {     // cut 1 incorrect wire
    Serial.println("One wrong wire cut");
    timeAfterTime(250);
    digitalWrite(tickingAudio, LOW);
    digitalWrite(whoopsAudio, HIGH);
  };

  if (!armed && boobyTrap && !luck) {   // failed, 2 incorrect wires cut
    Serial.println("Boom");
    timeAfterTime(10);
    digitalWrite(whoopsAudio, LOW);
    
    countdown != 0 ? digitalWrite(ohNoAudio, HIGH) : digitalWrite(ohNoAudio, LOW);
  };

  if (clock.read() == 0) {
    for (int i = 23; i < 36; i++) {
      digitalWrite(i, LOW);
    };
    Serial.print("Time's up");
    gameOver = true;
    lastTime = time;
  };
}

// Controls the winning and loosing audio cues, so let's get creative and call it...
void audioCues(int wait) {
  if (gameOver && countdown == 0) {
    if (time >= lastTime + wait) digitalWrite(failAudio, HIGH);
  } 
  else if (gameOver && countdown != 0) {
    if (time >= lastTime + wait) digitalWrite(winAudio, HIGH);
  };
}

// That's it, man! Game over, man! Game over!
void reset() {
  if (!digitalRead(wire1) && !digitalRead(wire2) && !digitalRead(wire3) && 
      !digitalRead(wire4) && !digitalRead(wire5) && !digitalRead(wire6) && 
      digitalRead(lid)) {
    
    // Reset all the variables
    countdown = 120;
    luck = 2;
    armed = false;
    boobyTrap = false;
    gameOver = false;
    wire1Last = true;
    wire2Last = true;
    wire3Last = true;
    wire4Last = true;
    wire5Last = true;
    lastTime = 0;
    clockTime = 0;
    lightTime = 0;
    debugTime = 0;

    // Shut off the LEDs
    for (int i = 23; i < 36; i++) {
      digitalWrite(i, LOW);
    };

    // Reset the clock arm
    clock.write(180);

    //Some extra debugging info; comment out
    Serial.println();
    Serial.println("RESET!");

    // Normally, I avoid 'delay,' but in this case it needs to be here or it 
    // won't reset properly. So leave this one alone.
    delay(2000);
  };
}

// Debugging monitor
// void debugger(int freq) {
//   if (time >= debugTime + freq) {
//     Serial.print(digitalRead(lid));
//     Serial.print("\t");
//     // Serial.print(digitalRead(wire1));
//     // Serial.print("\t");
//     // Serial.print(digitalRead(wire2));
//     // Serial.print("\t");
//     // Serial.print(digitalRead(wire3));
//     // Serial.print("\t");
//     // Serial.print(digitalRead(wire4));
//     // Serial.print("\t");
//     // Serial.print(digitalRead(wire5));
//     // Serial.print("\t");
//     // Serial.print(digitalRead(wire6));
//     // Serial.print("\t");
//     Serial.print(armed);
//     Serial.print("\t");
//     Serial.print(boobyTrap);
//     Serial.print("\t");
//     Serial.print("Countdown:");
//     Serial.print("\t");
//     Serial.print(countdown);
//     Serial.print("\t");
//     Serial.print("Clock:");
//     Serial.print("\t");
//     Serial.print(clock.read());
//     Serial.print("\t");
//     Serial.print(luck);
//     Serial.println();

//     debugTime = time;
//   };
// }

void setup() {
  Serial.begin(9600);
  setPins();
}

void loop() {
  time = millis();
  
  gameOver ? reset() : lidTrigger();
  audioCues(2000);  
  // debugger(100);
}
