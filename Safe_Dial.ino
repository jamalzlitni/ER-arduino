/*
Attatched a rotary encoder to a vault safe dial to mimic the actions of an old timey vault.
Users must 'crack' the code by listening to the vault door waiting for the three ticks of each correct number being found.
Edited: 12-11-2023  JZ
Edited: 12-12-2023  JZ
*/


// Wiring connections for my encoder:
// Red : VCC = 5V
// Black: 0V = GND
// White: OUTA = Digital pin 2
// Green: OUTB = Digital pin 3
// With these OUTA/OUTB connections and the interrupt code below,
// clockwise rotation gives positive encoder counts.

// ---------  Set Dial Combination Below ---------- 
const int firstNum = 63;
const int secondNum = 15;
const int thirdNum = 87;
// ------------  End Combination Set --------------

// ------------ Establish Functional Variables ----
int discs[3];
const int outA = 2;
const int outB = 3;
const int tap = 4;
const int unlock = 5;
int thisTurn = 0;
int lastTurn = 0;
int twolastTurn = 0;
int threelastTurn = 0;
int progress = 0;
int dialPos = 0;
int dialPos_save = 0;
volatile long encoder = 0; // declare volatile since modified by interrupt routines
long encoder_save = 0;
long timer = 0;
long resetTimer = 0;
// ------------ End Variable Establishing ---------

void setup() {
// Set combination to Discs
discs[0] = firstNum;
discs[1] = secondNum;
discs[2] = thirdNum;

// Call out encoder data pins
pinMode(outA, INPUT_PULLUP);
pinMode(outB, INPUT_PULLUP);

// attach interrupts to pins
attachInterrupt(digitalPinToInterrupt(outA), outAChange, CHANGE);
attachInterrupt(digitalPinToInterrupt(outB), outBChange, CHANGE);

// initialize serial
Serial.begin(115200);
Serial.println("");
Serial.println("Initiate Safe Dial Control Program");

pinMode(4, OUTPUT);
pinMode(5, OUTPUT);
}

// ------------ Begin Interrupt Routines --------------------------
void outAChange() {
// when outA changes, outA==outB means negative direction
encoder += digitalRead(outA) == digitalRead(outB) ? 1 : -1;
}

void outBChange() {
// when outB changes, outA==outB means positive direction
encoder += digitalRead(outA) == digitalRead(outB) ? -1 : 1;
}
//------------- End Interrupt Routines ---------------------------


void loop() {
Serial.println(dialPos);
if (encoder_save != encoder) { // Detects change in encoder value and maps encoder position to Dial numbers for logic
  encoder_save = encoder;
  dialPos = map(encoder, -2, 2400, 0, 100);
}

if (dialPos_save != dialPos) { // Detects change in Dial position numbers to cut down data output
  dialPos_save = dialPos;
  Serial.println(dialPos);
}

if(encoder >= 2400) {  // Cycles encoder count back to Zero for rotation position interpretation. (Encoder has 2400 CPR)
  encoder = 0; 
  // threelastTurn = twolastTurn;
  // twolastTurn = lastTurn;
  // lastTurn = thisTurn;
  // thisTurn = 1;

  //   if (threelastTurn + twolastTurn + lastTurn + thisTurn == 4) {
  //     reset();
  //   }
  threelastTurn = twolastTurn;
  twolastTurn = lastTurn;
  lastTurn = thisTurn;
  thisTurn = 0;
}

if (encoder <= -1) {  // Cycles encoder count back around for rotation position interpretation
  encoder = 2399;
  // threelastTurn = twolastTurn;
  // twolastTurn = lastTurn;
  // lastTurn = thisTurn;
  // thisTurn = 0;

  threelastTurn = twolastTurn;
  twolastTurn = lastTurn;
  lastTurn = thisTurn;
  thisTurn = 1;

    if (threelastTurn + twolastTurn + lastTurn + thisTurn == 4) {
      reset();
    }
}

// ------------- Begin Combination Logic --------------

  // For kicks (and because I couldn't figure out a better way to do it), this program is designed to be a digital 
  // recreation of a true 3-dial combination lock. 
  // There are three "dials" each with their own gate (three numbers wide).  
  // To begin the logic sequence, the player must "reset" the dial by rotating counter clockwise at least 3 times. 
  //      This stacks the dials and resets all timers. 


if (twolastTurn == 1 && lastTurn == 1 && thisTurn == 0 && progress == 1 && millis()-resetTimer >=8000) {
    if(dialPos == discs[0]) {
      digitalWrite(tap, HIGH);
      delay(25);
      digitalWrite(tap, LOW);
      progress = 2;
    }
}

if (twolastTurn == 1 && lastTurn == 1 && thisTurn == 0 && progress >= 2 && dialPos >= discs[0]+5) {
    progress = 0;
}

if (twolastTurn == 1 && lastTurn == 0 && thisTurn == 1 && progress == 2) {
    if(dialPos == discs[1]) {
      digitalWrite(tap, HIGH);
      delay(25);
      digitalWrite(tap, LOW);
      progress = 3;
    }
}

if (twolastTurn == 1 && lastTurn == 0 && thisTurn == 1 && progress == 3) {
    if(dialPos == discs[2]) {
      digitalWrite(tap, HIGH);
      delay(25);
      digitalWrite(tap, LOW);
      progress = 4;
      timer = millis();
    }
}

if (progress == 4) {
  if (dialPos < discs[2]-3 || dialPos > discs[2]+3) {
    progress = 1;
  }
  if (millis() - timer >= 3000) {
      digitalWrite(unlock, HIGH);
      delay(1500);
      digitalWrite(unlock, LOW);  
      progress = 0;
  }
}

if (millis()-resetTimer >= 400000 && progress != 0) {
  progress = 0;
}

//Serial.println(progress);
//Serial.print(twolastTurn);Serial.print(lastTurn);Serial.println(thisTurn);
Serial.println(" ");

}


void reset() {
  Serial.println("RESET");
  resetTimer = millis();
  progress = 1;
}






















