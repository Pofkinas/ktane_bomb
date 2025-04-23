#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Countimer.h>

// LCD pin
LiquidCrystal_I2C lcd(0x27,16,2);

// keypad setup
char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[4] = {48, 46, 44, 42}; 
byte colPins[4] = {40, 38, 36, 34}; 

Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4); 


//global int
String pass;
String keyword;
int time;
int state;

char key;

int min, sec;

int str = 2;

//
Countimer timer;


void menu(bool startup);
void settimer();
void keypress(char key);
void print();
void gameover();
void printstrikes();

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.init();
  lcd.backlight();

  Serial.begin(115200);
  menu(true);
  lcd.setCursor(6,0);
  lcd.print(min);
  lcd.setCursor(8,0);
  lcd.print(":");
  lcd.setCursor(9,0);
  lcd.print(sec);
  lcd.setCursor(6,0);
  timer.setCounter(0, min, sec, timer.COUNT_DOWN,gameover);
  timer.setInterval(print, 1000);
}

void print(){
  lcd.setCursor(6,0);
  lcd.print(timer.getCurrentTime());
}

void gameover(){
  for(int i = 0; i < 10; i++){
    lcd.noDisplay();
    delay(350);
    lcd.display();
    delay(350);
  }
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("Game Over");

  while(keyword != "*334#"){
    key = customKeypad.getKey();
    if(key){
      keypress(key);
      if(keyword == "*334#"){
      break;
    }  
  }
  }
  settimer();
}


void loop() {

  key = customKeypad.getKey();
  if (key)
  keypress(key);

  //timer
  timer.run();
  if(!timer.isCounterCompleted()) {
    timer.start();
  }
  else if(timer.isCounterCompleted()){
    gameover();
  }

  printstrikes();

  if(keyword == "*334#"){
    Serial.print("Open menu");
    keyword = "";
    settimer();
  }

}

void menu(bool startup){
  if(startup){
    lcd.setCursor(0,0);
    lcd.print("Keep Talking And");
    lcd.setCursor(0,1);
    lcd.print("Nobody Explodes");
    delay(2000);
  }

  pass = "";

  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("MENU");

  lcd.setCursor(0,1);
  lcd.print("Password: ");
  lcd.setCursor(9,1);

  int a = 0, l = 0;

  while(l == 0){
    lcd.blink();
    key = customKeypad.getKey();
    if(key == 35){
      l = 1;
    }
    if(key){
      lcd.noBlink();
      pass += key;
      a++;
      lcd.print("*");
      lcd.setCursor((a+9),1);
    }
    delay(100);
  }

  lcd.clear();

  if(pass == "*334#"){
    pass = "";
    settimer();
  }
  else if(pass == "B#"){
    Serial.println(keyword);
    pass = "";
    lcd.setCursor(0,1);
    lcd.print("Strikes");
    loop();
  }
  else
  menu(false);
  
}

void settimer(){
  int a = 11, l = 0;

  lcd.clear();
  lcd.print("Set timer: __:__");
  lcd.setCursor(a,0);
  lcd.blink();
  
  while(l == 0){
    key = customKeypad.getKey();
    if(key){
    lcd.print(key);
    if(a == 11) {
      time = (key - 48)*1000;
    }
    if(a == 12){
      time += (key - 48)*100;
      l = 1;
    }
    a++;
    lcd.setCursor(a,0);

  }
  }

  a++;
  lcd.setCursor(a,0);
  
  while(l == 1){
    key = customKeypad.getKey();
    if(key){
    lcd.print(key);
    if(a == 14) {
      time += (key - 48)*10;
    }
    if(a == 15){
      time +=(key - 48);
      l = 2;
    }
    a++;
    lcd.setCursor(a,0);
  }  
  }
  min = time/100;
  sec = time%100;

  lcd.clear();
  lcd.noBlink();
}

void keypress(char key){
  
  if(key == 42)
  keyword = "";
  keyword += key;
  Serial.println(keyword);
}

void printstrikes(){
  lcd.setCursor(0,1);
  lcd.print("Strikes");
  
  if(str == 1){
    lcd.setCursor(11,1);
    lcd.print("#");
  }
  else if(str == 2){
    lcd.setCursor(11,1);
    lcd.print("# #");
  }
  else if(str == 3){
    lcd.setCursor(11,1);
    lcd.print("# # #");
    delay(3000);
    gameover();
  }
}