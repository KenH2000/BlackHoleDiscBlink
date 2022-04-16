#include "Timer.h"
#define arrlen 11       //upper bound of array + 1
#include <EEPROM.h>


//settings**************************************************
int timeperbulb = 10000;            //a random bulb's timing will be changed every THIS interval
int timemin = 100;                  //min blink time, initial setting 100
int timemax = 1000;                 //max blink time, initial setting 900
boolean doattract=false;            //set false to skip attract
boolean debug=false;
unsigned long attractmin = 30000;   //min callback attract, initial setting 30000
unsigned long attractmax = 120000;  //max callback attract, initial setting 120000
int attractlampmin =  50;           //min lamp lit attract
int attractlampmax =  80;           //max lamp lit attract
int attractmaxloops = 1;            //max loops attract (max loops completed)
//disc motor control*********
int sensorThreshold = 190;          //LED on 390-400
unsigned long runtime = 600000;     // interval to leave disc on 
int pressdelay=200;                 //button press delay (ms)
int setpressdelay=700;
int eepromdelay=5000;
//settings**************************************************

unsigned long nextFlash[arrlen];  //variable to compare current time to determine when next toggle is due
//unsigned long runtime[arrlen];    //variable to compare current time to determine when a new random blink time is set
int blinktime[arrlen][2];         //holds the current blink time, multi-dimensional array lets on and off blink times be different
int light[] = { 8, A4, A3, A2, A1, A0, 13, 12, 11, 10, 9 }; // LED pins to use.
unsigned long attractnextLamp;
unsigned long attractcallback;
int myloop = 0;
int mycount = 0;
int attractlampdelay;
int attractloops;
int attractmode;
long spincallevery;
int count[arrlen];
//disc motor control***************************************
int sensor = A5;  //control line
int chgButton = 4;
int disc = 2;  //pin 9 control spinner
int led = 3;
//int discled = 1; //combined with disc 
unsigned long previousMillis = 0;
//unsigned long runtime = 10000;
int setmode=0;   //0 nospin,1 spin,2 spin timer
boolean buttonpressed=false;
boolean sensordisabled=false;
int laststate=2;
boolean gameon=false;
#define DISC_OFF 0
#define DISC_ON 1
#define DISC_TIME 2
//disc motor control***************************************

Timer t1;

void setup(){
  //debug code******************
  if (debug) {Serial.begin(9600);
  t1.every(1000,printsensorvalue);
  }  //turn on serial communication
  //debug code******************
  //disc motor control**********************************************
  //pinMode(discled, OUTPUT);  combined with disc
  pinMode(chgButton, INPUT_PULLUP);
  pinMode(disc, OUTPUT);
  pinMode(led, OUTPUT);  
 //recall EEprom setup
  setmode = EEPROM.read(0);
  if (setmode>2 or setmode<0){
    setmode=1;
    EEPROM.update(0,setmode);
    }
//timer events
 t1.every(200,blinkLED);
  //disc motor control**********************************************

  randomSeed(analogRead(A0));
  
  
  for(int i = 0; i<arrlen; i++){
  pinMode(light[i],OUTPUT);
  if (debug){Serial.print("light[");Serial.print(i);Serial.print("]=pin ");Serial.println(light[i]);}
  count[i]=0;nextFlash[i]=0;
  blinktime[i][0] = random(timemin, timemax); //set new random blink time
  blinktime[i][1] = random(timemin, timemax); //set new random blink time
  }
  attractmaxloops++;
  setnextattract();  
  t1.every(timeperbulb,changebulb);
}

void loop(){ 
   if(millis()>attractcallback && doattract==true){
    myattract();
   }else{
    myflash();
   }
 checkButtons();
 checkSensor(); 
 checktimer();
 t1.update();
} //end loop

void toggle(int what)
{
digitalWrite(what,digitalRead(what) ^ 1);
}

void myflash(){
  for(int i = 0; i<arrlen; i++){
    if(millis() > nextFlash[i]){        //time for next flash
    toggle (light[i]);                  //toggle the bulb on or off  
    nextFlash[i] = millis() + blinktime[i][count[i]];
    count[i]=count[i]^1;
    }
    } 
}

void changebulb(){
 int i=random(0,arrlen);
 //i=6;  //debug****
 blinktime[i][0] = random(timemin, timemax); //set new random blink time
 blinktime[i][1] = random(timemin, timemax); //set new random blink time
 if (debug){Serial.print("Changed light[");;Serial.print(i);Serial.print("]=pin ");Serial.println(light[i]);}
}

void myattract(){
  if (millis()<attractnextLamp){ return;}  
         for(int i = 0; i<arrlen; i++){
         digitalWrite(light[i], LOW); // all LEDs off
         }
  toggle (light[myloop]);     
  attractnextLamp = millis() + attractlampdelay;
  if (mycount>attractloops){
    setnextattract();
  }else{
  if (attractmode==0){    
    myloop++;
    if (myloop>arrlen){
      myloop=0;
      mycount++;
      }
    }else{
     myloop--;
      if (myloop<0){
      myloop=arrlen-1;
      mycount++;
      } 
    }     
  }
}

void setnextattract(){
    
    spincallevery= random(attractmin,attractmax);
    attractcallback=millis()+spincallevery;
    attractloops=random(1,attractmaxloops);
    attractlampdelay= random(attractlampmin,attractlampmax);
    mycount=1;
    attractmode=random(0,2);
    if (attractmode==0){myloop=0;}else{myloop=arrlen-1;}
    if (debug){Serial.print("Attract set in ");
       Serial.print(spincallevery);
       Serial.print(" ms for ");
       Serial.print(attractloops);Serial.print(" loops, mode-");Serial.println(attractmode);
     }   
}
   /*
    //chase light blink, didn't look good
    //x mod 3 gives 0,1,2
    int mylite = myloop % 3;
    toggle (light[mylite]);toggle(light[mylite+3]);toggle(light[mylite+6]);
    if ((mylite+9) < arrlen){toggle(light[mylite+9]);}
   */
//disc motor control functions*****************************
void checktimer() {
unsigned long currentMillis = millis();     //interrupt routine to sync clock from RTC     

  if(currentMillis - previousMillis > runtime)   
  {previousMillis = currentMillis;   // save the last time 
  
  //code goes here  
  discoff();
  //code goes here
  
  } //end if

} 
 
void checkButtons(){
  if (!buttonpressed){ 
  int buttonState;
  buttonState = digitalRead(chgButton);
  if (buttonState==LOW){
    buttonpressed=true;                    //disable button for short time
    t1.after(setpressdelay,enablebutton);  //renable button
    t1.after(eepromdelay,updateeprom);     //update eprom
    setmode++;laststate="";
    if (setmode>2){setmode=0;}
  } //end if buttonstate LOW
  }
} //end function checkButtons()

void enablebutton(){buttonpressed=false;}  //re-enable button

void enablesensor(){sensordisabled=false;} //re-enable sensor

void checkSensor(){
  if (!sensordisabled){
    int sensorstate;
    unsigned long currentMillis = millis(); //interrupt routine to sync clock from RTC     
    sensorstate=analogRead(sensor);
    if (sensorstate>sensorThreshold){gameon=true;}else{gameon=false;}
    if (gameon!=laststate){
      sensordisabled=true;                  //disable sensor for short time
      t1.after(setpressdelay,enablesensor); //reenable sensor
      if (gameon){
        //game playing
        discstate(HIGH);
        }else{
        //game over
          if (setmode==DISC_ON){
            discstate(HIGH);
          }
        if (setmode==DISC_OFF){
            discstate(LOW);
          }
        if (setmode==DISC_TIME){
            discstate(HIGH);
            previousMillis = currentMillis; // reset the discoff timer                            
          }
        
    }
     if (debug){
     unsigned long templong;
     Serial.print("Sensor = ");Serial.println(sensorstate);    
     Serial.print("gameon = ");Serial.println(gameon);    
     Serial.print("Setmode is ");Serial.print(setmode);
     if (setmode==0){Serial.println(" - OFF");}
     if (setmode==1){Serial.println(" - ON");}
     if (setmode==2){
      Serial.print(" - TIMED --time before disc off is ");
      templong = runtime - (currentMillis - previousMillis);
      Serial.println(templong);
     }      
     }
  }
  laststate=gameon;
  }//sensordisabled
}//end checkSensor

int discstate(int state){
  if (digitalRead(disc)!=state){ 
      digitalWrite(disc,state);
      //digitalWrite(discled,state);  //combined with disc
        if (debug){
          if (state==HIGH){
          Serial.println("Disc ON");
          }else{
          Serial.println("Disc OFF");        
         }
    }
  }
}

void discoff(){
  if (setmode==DISC_TIME and laststate==false){
  discstate(LOW);
  }
  }


void blinkLED(){
  if (setmode==0){digitalWrite(led,LOW);}
  if (setmode==1){digitalWrite(led,HIGH);}
  if (setmode==2){digitalWrite(led, !digitalRead(led));}
}


void updateeprom(){
  EEPROM.update(0,setmode);
  if (debug){Serial.println("EEPROM Saved");}
}

void printsensorvalue(){
  int sval=analogRead(sensor);
Serial.print("Sensor = ");Serial.println(sval);
}
