// Runaway Railcar - Reckoning Machine
// BMR 11-17-22
// Updated: BMR 1-18-23
// Updated: JZ 10/26/23
// Updated: JZ 12/18/23
// Updated: JZ 1/10/24
// Arduino Mega takes in information from input sources and displays the lights according to the progress solved on the puzzles.

// inputs received from reckoning board 2
const int switches = 10;
const int dials = 11;
const int passengers = 12;
const int sliders = 13;


// cargo mechanical decade counter inputs
const int cargoZero = 7;
const int cargoThree = 6;
const int cargoOne = 5;
const int cargoSix = 4;
const int cargoFour = 3;

// speed and oil slider inputs
const int speed = A0;
const int oil = A1;

const int row1 = 22;
const int row2 = 23;
const int row3 = 24;
const int row4 = 25;
const int row5 = 26;

int ledStep = 0;
int stepTime = 250;

// Array for the patterninator()
int columns[] = { 1, 1, 1, 1, 1,   // Unsolved
                  /*0, 1, 1, 0, 0,*/ 1, 1, 1, 0, 0,   // Sliders solved
                  /*1, 1, 1, 0, 1,*/ 1, 1, 0, 0, 0,  // Switches solved
                  /*1, 0, 0, 0, 0,*/ 1, 0, 1, 0, 0,  // Cargo solved
                  /*0, 1, 0, 1, 1,*/ 1, 0, 0, 1, 0,  // Passengers solved
                  /*1, 1, 0, 0, 0*/ 1, 0, 1, 0, 0}; // Dials solved

// Variables for the patternator() arrayNo parameter
const int unsolved = 0;
const int solvedSliders = 5;
const int solvedSwitches = 10;
const int solvedCargo = 15;
const int solvedPassengers = 20;
const int solvedDials = 25;

// variables to determine when conditions are met
//bool checkOil = false;
//bool checkSpeed = false;
bool checkSliders = false;
bool checkSwitches = false;
bool checkCargo = false;
bool checkPassengers = false;
bool checkDials = false;

unsigned long time;
unsigned long lastTime;
unsigned long debugTime;

// Function for setting all the pinModes; call at setup()
void setPins() {
  analogReference(DEFAULT);
  pinMode(speed, INPUT);
  pinMode(oil, INPUT);

  for (int thisPin = 22; thisPin < 32; thisPin++) {
    pinMode(thisPin, OUTPUT);
  }

  pinMode(switches, INPUT);
  pinMode(passengers, INPUT);
  pinMode(dials, INPUT);
  pinMode(sliders, INPUT);

  pinMode(cargoZero, INPUT_PULLUP);
  pinMode(cargoThree, INPUT_PULLUP);
  pinMode(cargoOne, INPUT_PULLUP);
  pinMode(cargoSix, INPUT_PULLUP);
  pinMode(cargoFour, INPUT_PULLUP);
}

// Check inputs; change corresponding boolean value to true if the conditions are met
void inspectorDetector() {
  /*if (analogRead(oil) > 225 && analogRead(oil) < 275) {
    Serial.println("oil good");
    checkOil = true;
  }
  else {
    checkOil = false;
  }

  if (analogRead(speed) > 850 && analogRead(speed) < 900) {
    checkSpeed = true;
    Serial.println("speed good");
  }
  else {
    checkSpeed = false;
  }*/

  //Serial.print("Oil: "); Serial.println(analogRead(oil));
  //Serial.println(analogRead(speed));
  if(digitalRead(sliders) == 1) {
    Serial.println("sliders good");
    checkSliders = true;
    digitalWrite(22, HIGH);
  }
  else {
    checkSliders = false;
    digitalWrite(22, LOW);
  }

  if (digitalRead(switches) == 1) {
    //Serial.println("switches good");
    checkSwitches = true;
    digitalWrite(23, HIGH);
  }
  else {
    checkSwitches = false;
    digitalWrite(23, LOW);
  }

  if (digitalRead(cargoZero) == 0 && digitalRead(cargoThree) == 0 && digitalRead(cargoOne) == 0 && digitalRead(cargoSix) == 0 && digitalRead(cargoFour) == 0) {
    //Serial.println("cargo good");
    checkCargo = true;
    digitalWrite(24, HIGH);
  }
  else {
    checkCargo = false;
    digitalWrite(24, LOW);
  }

  if (digitalRead(passengers) == 1) {
    //Serial.println("passengers good");
    checkPassengers = true;
    digitalWrite(25, HIGH)
  }
  else {
    checkPassengers = false;
    digitalWrite(25, LOW)
  }

  if (digitalRead(dials) == 1) {
    //Serial.println("dials good");
    checkDials = true;
    digitalWrite(26, HIGH);
  }
  else {
    checkDials = false;
    digitalWrite(26, LOW);
  }
}

// Animate the fancy LED panel according to puzzle progress
/*void patterninator(int rowNo, int arrayNo) {
  // Set as the first to-ground relay pin
  int relayPin = 27;
  
  // Disengage all rows
  for (int i = row1; i < row1 + 5; i++) {
    digitalWrite(i, LOW);
  }
  
  // Engage the relay columns called by the "arrayNo" argument
  for (int i = arrayNo; i < arrayNo + 5; i++) {
    digitalWrite(relayPin, columns[i]);
    relayPin++;
  }

  // Engage the relay row called by the "rowNo" argument
  digitalWrite(rowNo, HIGH);
}

// Mr. Anderson...
void theMatrix() {
  if (time > lastTime + stepTime) {
    switch (ledStep) {
      case 0:
        (checkSliders) ? patterninator(row1, solvedSliders) : patterninator(row1, unsolved);
        ledStep++;
        break;
      
      case 1:
        checkSwitches ? patterninator(row2, solvedSwitches) : patterninator(row2, unsolved);
        ledStep++;
        break;

      case 2:
        checkCargo ? patterninator(row3, solvedCargo) : patterninator(row3, unsolved);
        ledStep++;
        break;

      case 3:
        checkPassengers ? patterninator(row4, solvedPassengers) : patterninator(row4, unsolved);
        ledStep++;
        break;

      case 4:
        checkDials ? patterninator(row5, solvedDials) : patterninator(row5, unsolved);
        ledStep = 0;
        break;
    }

    lastTime = time;
  }
}*/

// Debugging monitor
void debugger(int freq) {
  if (time >= debugTime + freq) {
    // Serial.print("temp:");
    // Serial.print("\t");
    // Serial.print(digitalRead(temp));
    // Serial.print("\t");
    // Serial.print("Pass:");
    // Serial.print("\t");
    // Serial.print(digitalRead(pass));
    // Serial.print("\t");
    // Serial.print("cargo:");
    // Serial.print("\t");
    // Serial.print(digitalRead(cargo));
    // Serial.print("\t");
    // Serial.print("Steam:");
    // Serial.print("\t");
    // Serial.print(digitalRead(steam));
    // Serial.print("\t");
    // Serial.print("SOS:");
    // Serial.print("\t");
    // Serial.print(digitalRead(sos));
    // Serial.print("\t");
    Serial.print("Speed:");
    Serial.print("\t");
    Serial.print(analogRead(speed));
    Serial.print("\t");
    Serial.print("Oil:");
    Serial.print("\t");
    Serial.print(analogRead(oil));
    Serial.print("\t");
    Serial.print("Lock:");
    Serial.print("\t");
    //Serial.print(digitalRead(lock));
    Serial.print("\t");
    Serial.print("Step:");
    Serial.print("\t");
    Serial.print(ledStep);
    Serial.println();

    debugTime = time;
  };
}

void setup() {
  Serial.begin(9600);
  setPins();
}

void loop() {
  time = millis();
  //Serial.print("Oil: "); Serial.println(analogRead(oil));
  inspectorDetector();
  //theMatrix();
  // debugger(100);
}
