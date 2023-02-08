#include <LiquidCrystal.h>

#include <SPI.h>
#include <MFRC522.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <LiquidCrystal.h>

#define relay 4 
#define buzzer 35
#define SS_PIN 21
#define RST_PIN 22
#define BLYNK_PRINT Serial
char auth[] = "--PLACE YOUR AUTHENTICATION KEY HERE--";


MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
LiquidCrystal lcd(13,12,14,27,26,25);   // Ceate LCD instance.
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  Blynk.begin(auth);
  lcd.begin(16,2);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);    // turn the relay off by making the voltage LOW
  pinMode(buzzer,OUTPUT);
  digitalWrite(buzzer,HIGH);

}
BLYNK_WRITE(V0) // Executes when the value of virtual pin 0 changes
{
 if(param.asInt() == 1)// execute this code if the switch widget is now ON
 {

 digitalWrite(relay,LOW); // door unlock
 lcd.setCursor(0,0);
 lcd.print("Door Unlocked!");
 delay(3000);
 digitalWrite(relay,HIGH); // door lock after unlocking
 lcd.setCursor(0,0);
 lcd.print("Door Locked!");
 delay(3000);
 lcd.clear();

 }
 else// execute this code if the switch widget is now OFF
 {

 digitalWrite(relay,HIGH); // door lock
 lcd.setCursor(0,0);
 lcd.print("Door Locked!");
 delay(3000);
 lcd.clear();

 }
 
}

void loop() 
{
  Blynk.run();
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if ((content.substring(1) == "99 F2 AF BB") || (content.substring(1) == "B9 D7 AF BB") ) //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    
    digitalWrite(relay, LOW);   // open the door
    lcd.setCursor(0,0);
    lcd.print("Door Unlocked!");
    delay(3000);
    digitalWrite(relay,HIGH);
    lcd.setCursor(0,0);
    lcd.print("Door Locked!");
    delay(3000);
    lcd.clear();
  }
  
 
 else   {
  
    Serial.println(" Access denied");
    digitalWrite(relay, HIGH);    // door unlock
    lcd.setCursor(0,0);
    lcd.print("WARNING!!!");
    lcd.setCursor(0,1);
    lcd.print("Door can't open!");
    digitalWrite(buzzer,LOW);
    delay(1500);
    digitalWrite(buzzer,HIGH);
    delay(1500);
    lcd.clear();
  }
  
}
