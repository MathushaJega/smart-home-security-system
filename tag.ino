#include<SPI.h>
#include<MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display


//creating mfrc522 instance
#define RSTPIN 9
#define SSPIN 10
MFRC522 rc(SSPIN, RSTPIN);
const int BUZZER_PIN = 8;      // PB2


int readsuccess;

/* the following are the UIDs of the card which are authorised
    to know the UID of your card/tag use the example code 'DumpInfo'
    from the library mfrc522 it give the UID of the card as well as 
    other information in the card on the serial monitor of the arduino*/

    //byte defcard[4]={0x32,0xD7,0x0F,0x2B}; // if you only want one card
byte defcard[][4]={{0x63,0xFD,0x50,0x27},{0x73,0xDB,0x42,0x0D}}; //for multiple cards
int N=2; //change this to the number of cards/tags you will use
byte readcard[4]; //stores the UID of current tag which is read

void setup() {
Serial.begin(9600);

SPI.begin();
rc.PCD_Init(); //initialize the receiver  
rc.PCD_DumpVersionToSerial(); //show details of card reader module

  lcd.init();         // initialize the lcd
  lcd.backlight();    // Turn on the LCD screen backlight

//pinMode(6,OUTPUT); //led for authorised
//pinMode(5,OUTPUT); //led for not authorised

Serial.println(F("the authorised cards are")); //display authorised cards just to demonstrate you may comment this section out
for(int i=0;i<N;i++){ 
  Serial.print(i+1);
  Serial.print("  ");
    for(int j=0;j<4;j++){
      Serial.print(defcard[i][j],HEX);
      }
      Serial.println("");
     }
Serial.println("");

Serial.println(F("Scan Access Card to see Details"));

pinMode(BUZZER_PIN, OUTPUT);
lcd.init();         // initialize the lcd
lcd.backlight();    // Turn on the LCD screen backlight
lcd.setCursor(1, 0);
lcd.print("System Active!");

}


void loop() {
  
readsuccess = getid();

if(readsuccess){
 
  int match=0;

//this is the part where compare the current tag with pre defined tags
  for(int i=0;i<N;i++){
    Serial.print("Testing Against Authorised card no: ");
    Serial.println(i+1);
    if(!memcmp(readcard,defcard[i],4)){
      match++;
      }
    
  }
    
  
   if(match)
      {Serial.println("CARD AUTHORIZED");
      lcd.setCursor(1, 0);
      lcd.print(" AUTHORIZED");
      delay(3000);
      lcd.clear();

      lcd.setCursor(2, 0);
      lcd.print(" AUTHORIZED ");
      lcd.setCursor(2, 1);
      lcd.print(" AUTHORIZED");
      delay(3000);
      lcd.clear();
      
      }
     else {
      Serial.println("CARD NOT Authorised");
       lcd.setCursor(1, 0);
      lcd.print(" UNAUTHORIZED");
      delay(3000);
      lcd.clear();

      lcd.setCursor(2, 0);
      lcd.print(" UNAUTHORIZED ");
      lcd.setCursor(2, 1);
      lcd.print(" UNAUTHORIZED");
      delay(3000);
      lcd.clear();
     
      digitalWrite(BUZZER_PIN,  HIGH);
      delay(2000);
      digitalWrite(BUZZER_PIN,  LOW);

      }
  
  }
}
//function to get the UID of the card
int getid(){  
  if(!rc.PICC_IsNewCardPresent()){
    return 0;
  }
  if(!rc.PICC_ReadCardSerial()){
    return 0;
  }
 
  
  Serial.println("THE UID OF THE SCANNED CARD IS:");
  
  for(int i=0;i<4;i++){
    readcard[i]=rc.uid.uidByte[i]; //storing the UID of the tag in readcard
    Serial.print(readcard[i],HEX);
    
  }
   Serial.println("");
   Serial.println("Now Comparing with Authorised cards");
  rc.PICC_HaltA();
  
  return 1;
}



