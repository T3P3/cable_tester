//Circuit tester sketch for Arduino Mega
//(c) Tony@think3dprint3d.com
//GPLv3

//LCD

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(40, 42, 65, 66, 44, 64); //RS,E,D4,D5,D6,D7
//pins
int endA[10] = {30,31,28,29,26,27,24,25,22,23}; //pins end A
int endB[10] = {41,43,38,39,36,37,34,35,32,33}; //pins endB
int pSwitch=49;
int pEncA= 59;
int pEncB= 63;

//results
int result[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int test[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int counter[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
bool fail =false;

void setup() {

  Serial.begin(115200); //serial used for debugging only
  lcd.begin(20, 4);
  
  //setup pins  
  for(int i=0; i<10; i++){
      pinMode(endA[i], OUTPUT);//set output pins (end A)
      pinMode(endB[i], INPUT_PULLUP);//set input pins (end B)
  }
  pinMode(pSwitch,INPUT_PULLUP);

}

void loop() {
 //run the test
 runTest_5x2();
}

void runTest_5x2(){
  String resultS="";
  //user interface
  lcd.clear();
  lcd.print("Cable Tester 5x2"); 
  lcd.setCursor(0,1);
  lcd.print("Press Enc to Start");
  
  while(digitalRead(pSwitch))
  {
    delay(100);
  }
  delay(700); //debounce
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Testing: 5x2");
  lcd.setCursor(0,1);
  for(int i=0; i<10; i++){
     counter[i]=0;
    for(int j=0; j<10; j++){
        digitalWrite(endA[i], LOW); //set all outputs to LOW
    }
    for(int j=0; j<10; j++){  //check for crossed / open circuits vs closed, good, circuits
        digitalWrite(endA[j], HIGH); //scan across the inputs in turn
        test[i] = digitalRead(endB[i]); //read the output
        if(test[i] == 1 && j!=i){ //crossed or open circuit
          counter[i]++;
          result[i] = 20+j; 
        }
        else if(test[i] == 1 && j==i && result[i] <20 ){ //Good, closed circuit
          result[i] = 0; 
        }
        digitalWrite(endA[j],LOW);
        //debugging
        /*
          Serial.print("test1 input core  ");
          Serial.print(i);
          Serial.print(" with output core  ");
          Serial.print(j);
          Serial.print(" test =");
          Serial.print(test[i]);
          Serial.print(" counter =");
          Serial.println(counter[i]);*/
    }
    Serial.print("Core ");
    Serial.print(i);
    Serial.print(" result = ");
    if(result[i] == 0){
       Serial.println(" 1");
       resultS+="1";
    }
    else if(counter[i] == 9){
       Serial.println(" O");
       resultS+="0";
       fail=true;
    }
    else {
       Serial.println(" X");
       resultS+="X";
       fail=true;
    }
  }
  
  lcd.print(resultS); 
  lcd.setCursor(0,2);
 
 if(fail){
   Serial.println("FAILED");
   lcd.print("FAILED");
 }
 else{
   Serial.println("PASSED");
   lcd.print("PASSED");
 }
 Serial.println();
 lcd.setCursor(0,3);
 lcd.print("Press Enc to Restart");
   while(digitalRead(pSwitch))
  {
    delay(100);
  }
  delay(700); //debounce
}
