#include <Keypad.h>
#include<LiquidCrystal.h>
#include<EEPROM.h>

//May have to change the lcd libraries depending on the lcd screen used
LiquidCrystal lcd(9,8,7,6,5,4);

//The typed password, the stored and the changed password variables
char password[7];
char pass[7],pass1[7];
int i=0;
char customKey=0;
//The keypad matrix can be changed as well depending on the keypad used
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char hexaKeys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
byte rowPins[ROWS] = {A0,A1,A2,A3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A4,A5,3,2};   //connect to the column pinouts of the keypad
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//pins for the buzzer and the output which can go to the relay
int buzzer = 10;
int output = 13;

void setup()
{
  Serial.begin(9600);

  lcd.begin(16,2);
  pinMode(output, OUTPUT);
  pinMode(buzzer, OUTPUT);

  lcd.print(" Electronic ");
  lcd.setCursor(0,1);
  lcd.print(" BOMB Lock ");
  delay(1000);
  lcd.clear();
  lcd.print("Enter Passkey:");
  lcd.setCursor(0,1);
  for(int j=0;j<7;j++)
    EEPROM.write(j, j+49);
  for(int j=0;j<7;j++)
    pass[j]=EEPROM.read(j);
}

void loop()
{
  digitalWrite(output, HIGH);
  customKey = customKeypad.getKey();
  if(customKey=='#')
    change();
  if (customKey)
  {
    password[i++]=customKey;
    lcd.print(customKey);
    Serial.print(customKey);
  }
  if(i==7)
  {
    delay(200);
    for(int j=0;j<7;j++)
      pass[j]=EEPROM.read(j);
      //strncmp is a string comparison, its a nice tool for passkey checks
    if(!(strncmp(password, pass,7)))
    {

      digitalWrite(output, HIGH);
      //beep();
      lcd.clear();
      lcd.print("Passkey Accepted");
      digitalWrite(output, LOW);
      delay(2000);
    }
    else
    {
      digitalWrite(output, HIGH);
      digitalWrite(buzzer, HIGH);
      lcd.clear();
      lcd.print("Access Denied...");
      lcd.setCursor(0,1);
      delay(2000);
      lcd.clear();
      lcd.print("Enter Passkey:");
      lcd.setCursor(0,1);
      i=0;
      digitalWrite(buzzer, LOW);
    }
  }
}
void change()
{
  int j=0;
  lcd.clear();
  lcd.print("Current Password");
  lcd.setCursor(0,1);
  while(j<7)
  {
    char key=customKeypad.getKey();
    if(key)
    {
      pass1[j++]=key;
      lcd.print(key);
      beep();
    }
    key=0;
  }
  delay(500);
  if((strncmp(pass1, pass, 7)))
  {
    lcd.clear();
    lcd.print("Wrong Passkey...");
    lcd.setCursor(0,1);
    lcd.print("Better Luck Again");
    delay(1000);
  }
  else
  {
    j=0;
    lcd.clear();
    lcd.print("Enter New Pass:");
    lcd.setCursor(0,1);
    while(j<7)
    {
      char key=customKeypad.getKey();
      if(key)
      {
        pass[j]=key;
        lcd.print(key);
        Serial.print(key);
        EEPROM.write(j,key);
        j++;
        beep();
      }
    }
    lcd.print(" Done......");
    delay(1000);
  }
  lcd.clear();
  lcd.print("Enter Password:");
  lcd.setCursor(0,1);
  customKey=0;
}

void beep()
{
  digitalWrite(buzzer, HIGH);
  delay(20);
  digitalWrite(buzzer, LOW);
}
