/* @file CustomKeypad.pde
|| @version 1.0
|| @author Maitreya Dixit
     ** edited by Jonathon Hayes
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/

#include <Keypad.h> // Include Keypad Library (must be installed)
#include <LiquidCrystal.h>
#include <LedControl.h>

#include <MFRC522.h>
#include <Servo.h>
#include <SPI.h>

MFRC522 mfrc522(53, 5); // MFRC522 mfrc522(SS_PIN, RST_PIN)

char initial_password[4] = {'8', '8', '8', '8'};  // Variable to store initial password
String tagUID = "E4 6B 7A 2A";  // String to store UID of tag. Change it with your tag's UID
char password[4];   // Variable to store users password
boolean RFIDMode = true; // boolean to change modes
char key_pressed = 0; // Variable to store incoming keys
uint8_t i = 0;  // Variable used for counter

#include <Wire.h>
#include "RTClib.h"

DateTime now;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

RTC_DS3231 rtc;

void showDate(void);
void showTime(void);
void showDay(void); 

LedControl lc=LedControl(4,2,3,1);

/* we always wait a bit between updates of the display */
unsigned long delaytime1=1000;
unsigned long delaytime2=50;

int tempPin = 0;
//                BS  E  D4 D5  D6 D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

const byte ROWS = 4; // Four rows on keypad
const byte COLS = 4; // Four columns on keypad

// Define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {49, 47, 45, 43}; // Connect to the row pinouts of the keypad (see Elegoo reference)
byte colPins[COLS] = {41, 39, 37, 35}; // Connect to the column pinouts of the keypad

// Initialize an instance of class Keypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Key values
int key1 = 49, key2 = 50, key3 = 51;
int key4 = 52, key5 = 53, key6 = 54;
int key7 = 55, key8 = 56, key9 = 57;

void setup(){
  Serial.begin(9600); // Setup serial monitor
  lcd.begin(16, 2);
  while (!Serial)     // Waits until ready
  // Setting Arduino Pins as Outputs
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);

  Serial.begin(9600);
  lcd.begin(16,2);
  //delay(2000);
  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC Module");
    while (1);
  }

  if (rtc.lostPower()) 
  {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  lcd.begin(16,2);   // LCD screen
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  lcd.clear(); // Clear LCD screen
}

/*
 This method will display the characters for the
 word "Arduino" one after the other on the matrix. 
 (you need at least 5x7 leds to see the whole chars)
 */
void funnyEmoji() {
  /* here is the data for the characters */
  /*byte a[5]={B01111110,B10001000,B10001000,B10001000,B01111110};
  byte r[5]={B00010000,B00100000,B00100000,B00010000,B00111110};
  byte d[5]={B11111110,B00010010,B00100010,B00100010,B00011100};
  byte u[5]={B00111110,B00000100,B00000010,B00000010,B00111100};
  byte i[5]={B00000000,B00000010,B10111110,B00100010,B00000000};
  byte n[5]={B00011110,B00100000,B00100000,B00010000,B00111110};
  byte o[5]={B00011100,B00100010,B00100010,B00100010,B00011100};*/
  /*
  byte k[8]={B10001000,B10010000,B10100000,B11100000,B11100000,B10100000,B10010000,B10001000};
  byte r[8]={B11100000,B10010000,B10010000,B10010000,B11100000,B10010000,B10010000,B10010000};
  byte i[8]={B01111100,B00010000,B00010000,B00010000,B00010000,B00010000,B00010000,B01111100};
  byte s[8]={B00011000,B00100100,B00100000,B00111100,B00000100,B00000100,B00100100,B00011000};
  byte h[8]={B01001000,B01001000,B01001000,B01111000,B01001000,B01001000,B01001000,B01001000};
  byte n[8]={B01100100,B01100100,B01010100,B01010100,B01010100,B01001100,B01001100,B01000100};
  byte a[8]={B00010000,B00101000,B01000100,B01000100,B01111100,B01000100,B01000100,B01000100};*/

  byte k[8]={B00000000,B01000010,B11100111,B01000010,B00000000,B10000001,B11111111,B01111110};
  byte r[8]={B10010111,B10010010,B10010010,B11110010,B10010010,B10010010,B10010111,B00000000};
  byte i[8]={B00000000,B01000010,B00100100,B01000010,B00000000,B10000001,B11111110,B01111110};
  byte s[8]={B10011001,B01000010,B00100100,B10011001,B10011001,B00100100,B01000010,B10011001};

  /* now display them one by one with a small delay */
  lc.setRow(0,0,k[0]);
  lc.setRow(0,1,k[1]);
  lc.setRow(0,2,k[2]);
  lc.setRow(0,3,k[3]);
  lc.setRow(0,4,k[4]);
  lc.setRow(0,5,k[5]);
  lc.setRow(0,6,k[6]);
  lc.setRow(0,7,k[7]);
  lc.setRow(0,8,k[8]);
  delay(delaytime1);
  lc.setRow(0,0,r[0]);
  lc.setRow(0,1,r[1]);
  lc.setRow(0,2,r[2]);
  lc.setRow(0,3,r[3]);
  lc.setRow(0,4,r[4]);
  lc.setRow(0,5,r[5]);
  lc.setRow(0,6,r[6]);
  lc.setRow(0,7,r[7]);
  lc.setRow(0,8,r[8]);
  delay(delaytime1);
  lc.setRow(0,0,i[0]);
  lc.setRow(0,1,i[1]);
  lc.setRow(0,2,i[2]);
  lc.setRow(0,3,i[3]);
  lc.setRow(0,4,i[4]);
  lc.setRow(0,5,i[5]);
  lc.setRow(0,6,i[6]);
  lc.setRow(0,7,i[7]);
  lc.setRow(0,8,i[8]);
  delay(delaytime1);
  lc.setRow(0,0,s[0]);
  lc.setRow(0,1,s[1]);
  lc.setRow(0,2,s[2]);
  lc.setRow(0,3,s[3]);
  lc.setRow(0,4,s[4]);
  lc.setRow(0,5,s[5]);
  lc.setRow(0,6,s[6]);
  lc.setRow(0,7,s[7]);
  lc.setRow(0,8,s[8]);
  delay(delaytime1);
  lc.clearDisplay(0);
  /*lc.setRow(0,0,h[0]);
  lc.setRow(0,1,h[1]);
  lc.setRow(0,2,h[2]);
  lc.setRow(0,3,h[3]);
  lc.setRow(0,4,h[4]);
  lc.setRow(0,5,h[5]);
  lc.setRow(0,6,h[6]);
  lc.setRow(0,7,h[7]);
  lc.setRow(0,8,h[8]);
  delay(delaytime1);
  lc.setRow(0,0,n[0]);
  lc.setRow(0,1,n[1]);
  lc.setRow(0,2,n[2]);
  lc.setRow(0,3,n[3]);
  lc.setRow(0,4,n[4]);
  lc.setRow(0,5,n[5]);
  lc.setRow(0,6,n[6]);
  lc.setRow(0,7,n[7]);
  lc.setRow(0,8,n[8]);
  delay(delaytime1);
  lc.setRow(0,0,a[0]);
  lc.setRow(0,1,a[1]);
  lc.setRow(0,2,a[2]);
  lc.setRow(0,3,a[3]);
  lc.setRow(0,4,a[4]);
  lc.setRow(0,5,a[5]);
  lc.setRow(0,6,a[6]);
  lc.setRow(0,7,a[7]);
  lc.setRow(0,8,a[8]);
  delay(delaytime1);
  lc.setRow(0,0,0);
  lc.setRow(0,1,0);
  lc.setRow(0,2,0);
  lc.setRow(0,3,0);
  lc.setRow(0,4,0);
  lc.setRow(0,5,0);
  lc.setRow(0,6,0);
  lc.setRow(0,7,0);
  lc.setRow(0,8,0);
  delay(delaytime1);*/
}
  
void loop(){
  
  char keyvalue = customKeypad.getKey();

  // System will first look for mode
  if (RFIDMode == true) {
    lcd.setCursor(0, 0);
    lcd.print("Time To Breathe!");
    lcd.setCursor(0, 1);
    lcd.print("Scan Your Tag...");
    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
    }
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
    }
    //Reading from the card
    String tag = "";
    for (byte j = 0; j < mfrc522.uid.size; j++)
    {
      tag.concat(String(mfrc522.uid.uidByte[j] < 0x10 ? " 0" : " "));
      tag.concat(String(mfrc522.uid.uidByte[j], HEX));
    }
    tag.toUpperCase();
    //Checking the card
    if (tag.substring(1) == tagUID)
    {
      // If UID of tag is matched.
      lcd.clear();
      lcd.print("Tag Matched");
      delay(750);
      lcd.clear();
      lcd.print("Breathe In...");
      lcd.setCursor(0, 1);
      lcd.print("1, 2, 3...");
      delay(3000);
      lcd.clear();
      lcd.print("Breathe Out...");
      lcd.setCursor(0, 1);
      lcd.print("1, 2, 3...");
      delay(3000);
      lcd.clear();
      lcd.print("You're Done!");
      delay(1000);
      lcd.clear();
      RFIDMode = false; // Make RFID mode false
    }
  }
  
  int tempReading = analogRead(tempPin);
  // This is OK
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
  float tempF = (tempC * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
  
  if (keyvalue){
    Serial.println(keyvalue);
    if (keyvalue == 0){ // There is a function for this
      digitalWrite(3, HIGH); // Turns the blue LED on
      delay(500);            // Waits 100 ms
      digitalWrite(3, LOW);  // Turns the blue LED off
    }
    else if (keyvalue == key1){
      lcd.print("Welcome!");
      lcd.setCursor( 0, 1);
      lcd.print("It's A Good Day!");
      delay(500);
      lcd.clear();
    }
    else if (keyvalue == key2){
      lcd.print("Frustrated?");
      lcd.setCursor(0,1);
      lcd.print("Press Key 3!");
      delay(750);
      lcd.clear();
    }
    else if (keyvalue == key3){
      funnyEmoji();
    }
    else if (keyvalue == key4){
      lcd.print("Temp:");
      lcd.setCursor(0,1);
      lcd.print(tempF);
      delay(500);
      lcd.clear();
    }
    else if (keyvalue == key5){
      now = rtc.now();
      showDate();
      showDay();
      delay(750);
      lcd.clear();
     }
    }
   }

   void showDate()
 {
  lcd.setCursor(0,0);
  lcd.print(now.month());
  lcd.print('/');
  lcd.print(now.day());
  lcd.print('/');
  lcd.print(now.year());
 }
 void showDay()
 {
  lcd.setCursor(11,0);
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
 }
 void showTime()
 {
  lcd.setCursor(0,1);
  lcd.print("Time:");
  lcd.print(now.hour());
  lcd.print(':');
  lcd.print(now.minute());
  lcd.print(':');
  lcd.print(now.second());
  lcd.print("    ");
 } 
