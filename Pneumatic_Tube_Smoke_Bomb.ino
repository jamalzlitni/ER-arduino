/*
The user must trigger the opening of the tube, place the bomb in the tube, close it, then send it off.
Created: 10-2-2023  JZ
*/

int tubeButton = 0;
int atmButton = 0;
int doorSensor = 0;
int bombSensor = 0;
int tracker = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP); 
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  tubeButton = digitalRead(2);
  atmButton = digitalRead(3);
  doorSensor = digitalRead(4);
  bombSensor = digitalRead(5);
  //Serial.println(digitalRead(doorSensor));

  if (atmButton == 0 && tracker == 0) {  //  shoot door unlocked
    digitalWrite(6, HIGH);
    tracker++;
    Serial.print("ATM Triggered. Progress ="); Serial.println(tracker);
  }
  if (bombSensor == 0 && tracker == 1) {  //  bomb put in shoot
    digitalWrite(6, LOW);
    tracker++;
    Serial.print("Bomb Placed. Progress ="); Serial.println(tracker);
  }
  if (bombSensor == 1 && tracker == 2) {  //  if they take the bomb back before closing the door for some reason
    digitalWrite(6, HIGH);
    tracker--;
    Serial.print("Bomb Removed. Progress ="); Serial.println(tracker);  
  }
  if (doorSensor == 0 && tracker == 2) {  //  bomb in shoot and door closed
    tracker++;
    Serial.print("Bomb Placed, Door Closed. Progress ="); Serial.println(tracker);
  }
  if (tubeButton == 0 && tracker == 3) {  //  button pressed to send the bomb
    digitalWrite(7, HIGH);
    tracker++;
    Serial.print("Bombs Away. Progress ="); Serial.println(tracker);
    tracker = 0;
  }
  else {
    digitalWrite(7, LOW);
  }

}
