#include <Adafruit_LiquidCrystal.h>
#include <Servo.h>
#include <EEPROM.h>
#include <Wire.h>
#include <Keypad.h>  

#define MAX_ATTEMPTS 1
#define LOCKOUT_TIME 5000   // 5 seconds
byte a=LOCKOUT_TIME/1000;
byte failedAttempts = 0;
unsigned long lockoutStartTime = 0;

// for keypad
  const byte ROWS= 4;
  const byte COLS= 4;
  char keys[ROWS][COLS]={
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
  };
byte rowPINS[ROWS]={13, 12, 11, 10};
byte colPINS[COLS]={9, 8, 7, 6};
Keypad keypad = Keypad(makeKeymap(keys), rowPINS, colPINS, ROWS, COLS);

//defining servo and lcd
Servo lockServo;
Adafruit_LiquidCrystal lcd(0);

//for input buffer
const byte MAX_LEN = 4;
char inputbuffer[MAX_LEN + 1];
byte inputIndex =0;

//to reset input buffer
void resetInputBuffer(){
  inputIndex = 0;
  for(byte i=0; i<MAX_LEN+1; i++){
    inputbuffer[i]='\0';
  }
}

// FSM states
enum SystemState {
  STATE_IDLE,
  STATE_INPUT,
  STATE_VERIFY,
  STATE_UNLOCKED,
  STATE_ERROR,
  STATE_LOCKED_OUT
};
enum SystemState currentState = STATE_IDLE;

//for EEPROM 
const int EEPROM_ADDR=0;
char storedpass[MAX_LEN+1];

// to read password from EEPROM
void loadpass(){
  for(byte i=0; i<MAX_LEN;i++){
    storedpass[i]=EEPROM.read(EEPROM_ADDR+i);
  }
  storedpass[MAX_LEN]='\0';
}
//to save password in EEPROM
void savepass(const char *password){
    for(byte i=0; i<MAX_LEN; i++){
    EEPROM.update(EEPROM_ADDR+i, password[i]);
    }
  }  
//to check if already initialised 
void initEEPROM() {
  bool empty = true;

  for (byte i = 0; i < MAX_LEN; i++) {
    if (EEPROM.read(EEPROM_ADDR + i) != 0xFF) {
      empty = false;
      break;
    }
  }

  if (empty) {
    savepass("1234");
  }
}

//for locking and unlocking servo, lcd
void lockdoor(){
  lockServo.write(0);
  lcd.clear();
  lcd.print("Door locked");
  delay(500);
  lcd.clear();
}

void unlockdoor(){
  lockServo.write(90);
  lcd.clear();
  lcd.print("Door unlocked");
  delay(500);
  lcd.clear();
  delay(1000);
  lockdoor();
}

void setup(){
  lcd.begin(16,2);
  lcd.setBacklight(1);
  lockServo.attach(3);
  lockServo.write(0);
  Serial.begin(9600);
  lockdoor();
  initEEPROM();
  savepass("1234");
  loadpass();
  resetInputBuffer();
  lcd.print("Enter pin:");
  delay(500);
  lcd.clear();
}

void loop(){
  char key = keypad.getKey();
  switch(currentState){
    case(STATE_IDLE):
  if(!key) break;
    else{
      if(key!='*' && key!='#'){
        inputbuffer[inputIndex++]=key;
        lcd.setCursor(0,1);
        lcd.print('*');
      }
      currentState=STATE_INPUT;
    }
    break;
    
    case(STATE_INPUT):
    if(!key) break;
    if(key){
      if(key=='*'){
        lcd.clear();
        lcd.print("Try now");
        delay(500);
        lcd.clear();
        resetInputBuffer();
        currentState = STATE_IDLE;
        break;
        }
      if(key=='#'){
        if(inputIndex==0){
          lcd.clear();
          lcd.print("Enter pin now:");
          delay(500);
          lcd.clear();
          currentState = STATE_IDLE;
        }
        else{
          currentState = STATE_VERIFY;
          }
        break;
        }
      if(inputIndex<MAX_LEN){
            inputbuffer[inputIndex++]=key;
            lcd.setCursor(inputIndex-1, 1);
            lcd.print('*');
      }
       if(inputIndex==MAX_LEN){
            inputbuffer[inputIndex]='\0';
       }
    }
        break;
      
        case STATE_VERIFY:
    if (strcmp(inputbuffer, storedpass) == 0) {
      unlockdoor();
      failedAttempts = 0;        // reset on success
      resetInputBuffer();
      currentState = STATE_IDLE;
    } 
    else {
      failedAttempts++;
      if(failedAttempts>=MAX_ATTEMPTS){
        currentState=STATE_LOCKED_OUT;
        lcd.clear();
        lcd.print("Locked Out");
        delay(500);
        lcd.clear();
        lcd.print("Try After: ");
        lockoutStartTime=millis();
      }
      else{
        currentState=STATE_ERROR;
      }
    }
    break;
    
    case(STATE_ERROR):
    lcd.clear();
    resetInputBuffer();
    lcd.print("Wrong Password");
    delay(500);
    lcd.clear();
    currentState=STATE_IDLE;
    break;
    
    case(STATE_LOCKED_OUT):
    lcd.clear();
    if(millis()-lockoutStartTime<LOCKOUT_TIME){
      lcd.setCursor(0,1);
      lcd.print(a);
      a--;
      delay(1000);
      lcd.clear();
      break;
    }
    else{
      lcd.print("Try Now");
      delay(500);
      lcd.clear();
      resetInputBuffer();
      failedAttempts=0;
      a=LOCKOUT_TIME/1000;
      currentState=STATE_IDLE;
      break;
    }
  }
}

    
       
        
            
              
            
               
               
                 
          
                 
                         
  
