/*
Break the Bank ATM
The users navigate through the fake made pages of an ATM to get to 'Specialty Deposit' in order to unlock
the pneumatic tube so that they can deploy the smoke bomb and smoke out the teller.
Created: 12-4-2023  JZ

Videos:
000 - enter card to begin
001 - main choices
002 - error 57
003 - no other services available
004 - choose deposit type
005 - thank you for making a specialty deposit
*/

const int card = 11;    // once a card is inserted, the players can navigate through the ATM
const int lock = 12;    // to unlock the pneumatic tube
int progress = 0;       // keep track of the page that the players are on in order to properly place them with the next choice(s)

void setup() {
pinMode(card, INPUT_PULLUP);
pinMode(4, OUTPUT);
pinMode(5, OUTPUT);
digitalWrite(4, HIGH);
digitalWrite(5, HIGH);
pinMode(6, INPUT_PULLUP);
pinMode(7, INPUT_PULLUP);
pinMode(8, INPUT_PULLUP);
pinMode(9, INPUT_PULLUP);
pinMode(10, INPUT_PULLUP);
Serial.begin(9600);

pinMode(lock, OUTPUT);
digitalWrite(lock, LOW);
}

void loop() {  
  if (digitalRead(card) == 0 && progress == 0) {    // card inserted, goes from insert card screen to options screen
    Serial.write(0xFC);
    Serial.write(1);
    progress = 1;
  }
  while (progress > 0) {
    digitalWrite(4, LOW);
    if ((digitalRead(8) == 0 || digitalRead(9) == 0) && progress == 1) {     // from options screen to error screen  (transfer or check balance)
      Serial.write(0xFC);
      Serial.write(2);
      progress = 2;
    }
    if (digitalRead(7) == 0 && progress == 1) {     // from options screen to would you like to other services unavailable screen  (other services)
      Serial.write(0xFC);
      Serial.write(3);
      progress = 3;
    }
    if ((digitalRead(8) == 0 || digitalRead(9) == 0) && progress == 4) {     // from deposits screen to error screen   (cash or check deposit)
      Serial.write(0xFC);
      Serial.write(2);
      progress = 5;
    }
    if (digitalRead(7) == 0 && progress == 4) {     // from deposits screen to thank you screen   (specialty deposit)
      Serial.write(0xFC);
      Serial.write(5);
      progress = 5;
      digitalWrite(lock, HIGH);
    }
    digitalWrite(4, HIGH);

    digitalWrite(5, LOW);
    if (digitalRead(9) == 0 && progress == 1) {     // from options screen to error screen (withdrawal)
      Serial.write(0xFC);
      Serial.write(2);
      progress = 2;
    }
    if (digitalRead(8) == 0 && progress == 1) {     // from options screen to deposits screen   (deposit)
      Serial.write(0xFC);
      Serial.write(4);
      progress = 4;
    }
    if (digitalRead(7) == 0 && progress > 1) {     // go back button from any screen to options screen    (go back)
      Serial.write(0xFC);
      Serial.write(1);
      progress = 1;
    }
    digitalWrite(5, HIGH);
    if (digitalRead(card) == 1) {
      Serial.write(0xFC);
      Serial.write(0);
      progress = 0;
      digitalWrite(lock, LOW);
    }
  }
}


  // if (digitalRead(card) == 0 && progress == 0) {    // card inserted, goes from insert card screen to options screen
  //   Serial.write(0xFC);
  //   Serial.write(1);
  //   progress = 1;
  // }
  // digitalWrite(4, LOW);
  // if ((digitalRead(8) == 0 || digitalRead(9) == 0) && progress == 1) {     // from options screen to error screen  (transfer or check balance)
  //   Serial.write(0xFC);
  //   Serial.write(2);
  //   progress = 2;
  // }
  // if (digitalRead(7) == 0 && progress == 1) {     // from options screen to would you like to other services unavailable screen  (other services)
  //   Serial.write(0xFC);
  //   Serial.write(3);
  //   progress = 3;
  // }
  // if ((digitalRead(8) == 0 || digitalRead(9) == 0) && progress == 4) {     // from deposits screen to error screen   (cash or check deposit)
  //   Serial.write(0xFC);
  //   Serial.write(2);
  //   progress = 5;
  // }
  // if (digitalRead(7) == 0 && progress == 4) {     // from deposits screen to thank you screen   (specialty deposit)
  //   Serial.write(0xFC);
  //   Serial.write(5);
  //   progress = 5;
  //   digitalWrite(lock, HIGH);
  // }
  // digitalWrite(4, HIGH);

  // digitalWrite(5, LOW);
  // if (digitalRead(9) == 0 && progress == 1) {     // from options screen to error screen (withdrawal)
  //   Serial.write(0xFC);
  //   Serial.write(2);
  //   progress = 2;
  // }
  // if (digitalRead(8) == 0 && progress == 1) {     // from options screen to deposits screen   (deposit)
  //   Serial.write(0xFC);
  //   Serial.write(4);
  //   progress = 4;
  // }
  // if (digitalRead(7) == 0 && progress > 1) {     // go back button from any screen to options screen    (go back)
  //   Serial.write(0xFC);
  //   Serial.write(1);
  //   progress = 1;
  // }
  // digitalWrite(5, HIGH);
  // if (digitalRead(card) == 1) {
  //   Serial.write(0xFC);
  //   Serial.write(0);
  //   progress = 0;
  //   digitalWrite(lock, LOW);
  // }

