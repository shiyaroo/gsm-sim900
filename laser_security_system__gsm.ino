#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SoftwareSerial.h>

SoftwareSerial sim900(0,1);
String  TextForSMS;

int dataldr =0;
int sensorldr = A0;
int ldrState = LOW;            
int ldrthres = 900;

int pir = A1;
int pirState = LOW;            
int val = 0; 

int gas = A8;
int sensorgasThres = 400;

#define Password_Length 8 
#define PINNUMBER ""

int signalPin = 2;
int led1 = 12;
int led2 = 11;
int led3 = 10;
int led4 = 9;
int led5 = 8;
int led6 = 7;
int laser =6;


char Data[Password_Length]; 
char Master[Password_Length] = "123A456"; 
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {30,31,32, 33};
byte colPins[COLS] = {34, 35, 36, 37};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x3F, 20, 4);  

void setup(){
  
  lcd.init(); 
  lcd.backlight();
  
  pinMode(signalPin, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(laser, OUTPUT);

  pinMode(gas, INPUT);
  pinMode(pir, INPUT);
  pinMode(sensorldr,INPUT);


 randomSeed(analogRead(0));
  Serial.begin(9600);
  sim900.begin (9600);
  Serial.println(" loggin time completed");
delay(5000);
 
}

void loop(){
  
     digitalWrite(laser, HIGH);
     digitalWrite(led1, HIGH);

     lcd.setCursor(0,0);
     lcd.print("Enter Password:");
     
   char customKey = customKeypad.getKey();
  
  if (customKey){
    
    Data[data_count] = customKey; 
    lcd.setCursor(data_count,1); 
    lcd.print(Data[data_count]); 
    data_count++; 
    }

  if(data_count == Password_Length-1){
    lcd.clear();

    if(!strcmp(Data, Master)){
      
      lcd.print("Correct");
      digitalWrite(led3, HIGH);
      tone(signalPin, 150,200);
      delay(5000);
      digitalWrite(signalPin, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(laser, LOW);
      ldrState = LOW;
      delay(5000);
      digitalWrite(laser, HIGH);
      ldrState = HIGH;
            
      }
    else{
            lcd.print("Incorrect");
            digitalWrite(led4, HIGH); 
            digitalWrite(signalPin, HIGH);

      delay(2000);
      
            digitalWrite(led4, LOW);
           digitalWrite(signalPin, LOW);
 TextForSMS="\nIntruder password wrong";
  sendSMS(TextForSMS);
 lcd.setCursor(10,2);
  lcd.print("sms2sent");      }
    
    lcd.clear();
    clearData();  
  }
 
  int   dataldr =analogRead(sensorldr);
     lcd.setCursor(0,3);
     lcd.print("Laser:");
     lcd.setCursor(7,3);
     lcd.print(dataldr);
     
  if(dataldr<ldrthres){
    digitalWrite(signalPin, HIGH);
    digitalWrite(led2, HIGH);
  TextForSMS="\nIntruder detected";
  sendSMS(TextForSMS);
  lcd.setCursor(10,2);
  lcd.print("sms1sent");
  
  }
  else
  {
    digitalWrite(led2, LOW);
    noTone(signalPin);

  }
  delay(100);


 
 int analogSensor = analogRead(gas);
  lcd.setCursor(0,2);
  lcd.print("Gas:");
  lcd.setCursor(4,2);
  lcd.print(analogSensor);
  // Checks if it has reached the threshold value
  if (analogSensor > sensorgasThres)
  {
            digitalWrite(led5, HIGH);
            digitalWrite(signalPin, HIGH);

     TextForSMS="\nIntruder GAS";
  sendSMS(TextForSMS);
  lcd.setCursor(10,2);
  lcd.print("sms3sent");
  }
  else
  {
    digitalWrite(led5, LOW);
    noTone(signalPin);
  }
  delay(100);

  
  val = digitalRead(pir); 
  if (val == HIGH) {            
    digitalWrite(led6, HIGH); 
   
    if (pirState == LOW) {
           Serial.println("Motion detected!");
           pirState = HIGH;
    }
  } else {
    digitalWrite(led6, HIGH);
 delay (5000);
    if (pirState == LOW){
           Serial.println("Motion ended!");
      pirState = LOW;
    }
  }

}


void clearData(){
  while(data_count !=0){
    Data[data_count--] = 0; 
  }
  return;
}


void sendSMS (String message){
    sim900.print("AT+CMGF=1");
    delay(1000);

  sim900.println("AT+CMGS=\"+380633965886\"\r ");
      delay(1000);
sim900.println(message);
    delay(1000);
sim900.println((char)26);
    delay(1000);
sim900.println();
    delay(1000);


  }
 
