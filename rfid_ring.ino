/*
The user must place an RFID ring on the scanner to unlock the this part.
Created: 12-12-2023  JZ
*/

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

byte readCard[4];
String ringTag = "C24F3B3D";
String tag = "";

const int lock = 7;

void setup() {
  Serial.begin(9600);
  SPI.begin();

  mfrc522.PCD_Init();
  delay(4);

  mfrc522.PCD_DumpVersionToSerial();   // show details of rfid reader

  pinMode(lock, OUTPUT);
}

void loop() {
  while (getID()) {
    if (tag == ringTag) {
      digitalWrite(7, HIGH);
      delay(500);               // don't know how vault door maglocks are supposed to work yet
      digitalWrite(7, LOW);
      Serial.println("Correct");
    }
  } 
  delay(2000);
}

/**********************************************************************************************
 * getID() function
 * Read new tag if available
**********************************************************************************************/
boolean getID() {
  // Getting ready for Reading PICCs
  //If a new PICC placed to RFID reader continue
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }
  
  //Since a PICC placed get Serial and continue
  if ( ! mfrc522.PICC_ReadCardSerial()) {
  return false;
  }
  
  tag = "";
  // The MIFARE PICCs that we use have 4 byte UID
  for ( uint8_t i = 0; i < 4; i++) {
  //readCard[i] = mfrc522.uid.uidByte[i];
  // Adds the 4 bytes in a single String variable
  tag.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  tag.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
  
}
