/*************************************************
This sketch reads the info for the modules on reckoning board #2 apart from most of the 2 way switches.
When the signal for switches is high (majority of two way switches, coming from the uno), and the remaining three boiler pressure switches are correct, the switchesOutput pin becomes high and sends that over to the mega.
When the 3 dials are all on the right spots, dialsOutput becomes high and sends that to the mega.
When the mechanical decade counter is set to the correct number of passengers (260), passengersOutput is set to high and is sent over to the mega.

Created: JZ 10/25/2023
Last Edited: JZ 2/5/2024
*************************************************/


const int switches = 2;         // output from the uno that reads most of the 2 way switches, high when correct
const int pressureW128 = 3;     // the remaining three 2 way switches
const int pressureB256 = 4;   
const int pressureB512 = 5;
const int switchesOutput = 10;  // switches output that gets sent to the mega 

const int steamTankDial = 6;        // the 3 dials
const int steamPressureDial = 7;  
const int volume = 8;  
const int dialsOutput = 11;         // dials output that gets sent to the mega

const int passengersTwo = A0;       // passengers decade counters
const int passengersSix = A1;
const int passengersZero = A2;
const int passengersOutput = 12;    // passengers output that gets sent to the mega

const int oil = A6;
const int speed = A7;
const int slidersOutput = A5;

int speedRead = 0;
int oilRead = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pressureW128, INPUT_PULLUP);
  pinMode(pressureB256, INPUT_PULLUP);
  pinMode(pressureB512, INPUT_PULLUP);
  pinMode(switches, INPUT);
  pinMode(switchesOutput, OUTPUT);
  digitalWrite(switchesOutput, LOW);

  pinMode(steamTankDial, INPUT_PULLUP);
  pinMode(steamPressureDial, INPUT_PULLUP);
  pinMode(volume, INPUT_PULLUP);
  pinMode(dialsOutput, OUTPUT);
  digitalWrite(dialsOutput, LOW);

  pinMode(passengersTwo, INPUT_PULLUP);
  pinMode(passengersSix, INPUT_PULLUP);
  pinMode(passengersZero, INPUT_PULLUP);
  pinMode(passengersOutput, OUTPUT);
  digitalWrite(passengersOutput, LOW);

  pinMode(slidersOutput, OUTPUT);
  digitalWrite(slidersOutput, LOW);
}

void loop() {
  //Serial.print("Oil: "); Serial.println(analogRead(oil));
  //Serial.print("Speed: "); Serial.println(analogRead(speed));

  if (digitalRead(switches) == 1 && digitalRead(pressureW128) == 0 && digitalRead(pressureB256) == 1      
    && digitalRead(pressureB512) == 1) {        // if ALL of the 2 way switches are correct, switchesOutput becomes 1
    //Serial.println("Switches Correct!");
    digitalWrite(switchesOutput, HIGH);
  }
  else {
    //Serial.println("Switches Wrong");
    digitalWrite(switchesOutput, LOW);
  }

  if (digitalRead(steamTankDial) == 0 && digitalRead(steamPressureDial) == 0 && digitalRead(volume) == 0) {     // if the dials are all correct, dialsOutput becomes 1
    //Serial.println("Dials Correct!");
    digitalWrite(dialsOutput, HIGH);
  } 
  else {
    //Serial.println("Dials Wrong");
    digitalWrite(dialsOutput, LOW);
  }

  if (digitalRead(passengersTwo) == 0 && digitalRead(passengersSix) == 0 && digitalRead(passengersZero) == 0) {   // if the passengers counters are correct, passengersOutput becomes 1
    //Serial.println("Passengers Correct!");
    digitalWrite(passengersOutput, HIGH);
  }
  else {
    //Serial.println("Passengers Wrong");
    digitalWrite(passengersOutput, LOW);
  }
  
  speedRead = analogRead(speed);
  oilRead = analogRead(oil);
  Serial.print("Oil: "); Serial.println(oilRead);
  Serial.print("Speed: "); Serial.println(speedRead);
  if (speedRead > 700 && speedRead < 900 && oilRead > 200 && oilRead < 400) {
    //Serial.println("Sliders Correct");
    digitalWrite(slidersOutput, HIGH);
  }
  else {
    digitalWrite(slidersOutput, LOW);
  }
}
