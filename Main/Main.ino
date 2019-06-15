

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Keypad.h>

#define IR_PIN 7

#define LED_RED 8

#define LED_GREEN A0

const char* ssid = "Lol";
const char* password = "Aasrith@2003";
const char* auth = "f7055c22788e446a8c3170d272c2153f";

const unsigned int KeypadRows = 4;
const unsigned int KeypadColumns = 3;
bool Password_Status; 

int password_num_entered;

char GetKey();
bool ComparePassword(char password[] , char entered_password[]);
int CheckIRStatus();
bool LockStatus();
bool CanOpen(bool Password_Status);

char Password[4] = {'4','0','4','0'};

char Keymap[KeypadRows][KeypadColumns]=
{
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'},
};

char keypressed[4];

byte RowPins[KeypadRows] = {D6, D5, D4, D3}; 
byte ColPins[KeypadColumns] = {D2, D1, D0}; 

Keypad LockKeypad= Keypad(makeKeymap(Keymap), RowPins, ColPins, KeypadRows, KeypadColumns);

void setup() {

  //WiFi.begin(ssid,password);
  Serial.begin(9600);

  Blynk.begin(auth,ssid , password);
  
  pinMode(IR_PIN , INPUT);
  pinMode(LED_RED , OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  
  int password_num_entered = 0;

  if(digitalRead(LED_GREEN) == HIGH)
  {
    digitalWrite(LED_GREEN , LOW);
  }
  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_RED , HIGH);
  delay(1000);
  digitalWrite(LED_RED , LOW);
}

void loop() {
  
  //Blynk.run();

  if(password_num_entered == 4)
  {
    alarm();
  }
  else
  {
     for(int i = 0; i <=3; i++)
  {
     digitalWrite(LED_RED , HIGH);
     char key = GetKey();
     while(key == NO_KEY)
     {
      key = GetKey();
     }
     if(key != NO_KEY)
     {
      Serial.println(key);
      keypressed[i] = key;
     }
     delay(500);
  }
     Password_Status = ComparePassword(Password,keypressed);

     if(Password_Status)
     {
      Serial.println("unlocked");
      //bool Password_Status = false;
     char key = GetKey();
     while(key != '*')
     {
      key = GetKey();
     }
     
     }
     else
     {
      Serial.println("wrong password");
     }
  
     
      
     bool CanUserOpen = CanOpen(Password_Status);

     if(CanUserOpen)
     {
      Serial.println("open the safe");
      digitalWrite(LED_GREEN , HIGH);
      digitalWrite(LED_RED,LOW);

     delay(2000);
     if(CheckIRStatus() == HIGH)
      {
         Serial.println("safe opened");
      }

     char key = GetKey();
     while(key!= '#')
     {
      key = GetKey();
     }
     digitalWrite(LED_GREEN , LOW);
     digitalWrite(LED_RED , HIGH);
     Serial.println("Locked again");
      
     }
     else
     {
      Serial.println("Can't unlock");
      digitalWrite(LED_GREEN , LOW);
      digitalWrite(LED_RED , HIGH);
      password_num_entered++;
     }
     delay(1000);
  }
  

    /* if(CanUserOpen)
     {
       delay(10000);
       if(CheckIRStatus() == HIGH)
       {
         Serial.println("safe opened");
       }
       else
       {
         Password_Status = false;
         LOCK_STATUS = true;
         
       }
     }*/
     
  }
  

char GetKey()
{
  char key = LockKeypad.getKey();
  if(keypressed != NO_KEY)
  {
    return key;
  }
  else
  {
    return NO_KEY;
  }
}

bool ComparePassword(char password[] , char entered_password[])
{
  if(sizeof(password)/sizeof(char) == sizeof(entered_password)/sizeof(char))
  {
    for(int i = 0; i <= sizeof(password)/sizeof(char); i++)
    {
      if(entered_password[i] != password[i])
      {
        return false;
      }
    }
    return true;
    
  }
  else
  {
   return false;
  }
}


int CheckIRStatus()
{
  int IR_STATUS = digitalRead(IR_PIN);
  
  return IR_STATUS;
 
}

bool LockStatus()
{
  int IR_STATUS = CheckIRStatus();

  if(IR_STATUS == LOW)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool CanOpen(bool Password_Status)
{
  bool LOCK_STATUS = LockStatus();

  //bool LOCK_STATUS = true;
  
  if(LOCK_STATUS)
  {
    if(Password_Status)
    {
      return true;
    }
    else
    {
      return false;
    }
  } 
  else
  {
    return false;   
  }
}

void alarm()
{
  while(true)
  {
   digitalWrite(LED_RED , HIGH);
  delay(100);
  digitalWrite(LED_RED , LOW);
  delay(100);
  digitalWrite(LED_RED , HIGH);
  
  }
  Blynk.notify("INTRUDER!!!");
  Blynk.email("{OWNER_EMAIL}","IR SECURITY SYSTEM {DEVICE_NAME}","Intruder alert !!! someone tried to open your safe");
}
