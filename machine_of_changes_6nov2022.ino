/* SOURCE CODE FOR IMPROVISATION LESSONS OF JAN RUERD OOSTERHAVEN.

  IMPROMACHINEv2 a.k.a. Machine Of Changes.
    made in July 2020.
     
*/

#include <SoftwareSerial.h>
#include <Wire.h>
#include <LedControl.h>
#include <TM1637Display.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>
#include <DS3231.h>
#include "RTClib.h"
#include <IRremote.h>
#include <DHT.h>
#include <DHT_U.h>

#define CLK 32
#define DIO 33
#define DIN1 30
#define CS1 29
#define CLK1 28
#define BUZZ 12
#define RESET 11
#define LED1 4
#define LED2 5
#define MP3_TX 2
#define MP3_RX 3
#define outputA 34
#define outputB 35
#define S_OK 36
#define G1 40
#define G2 41
#define G3 42
#define G4 43
#define ESP1_RX 46
#define ESP1_TX 45
#define ESP2_RX 48
#define ESP2_TX 47

const int RECV_PIN = 31;
static int8_t Send_buf[8] = {0} ;

#define NEXT_SONG 0X01 
#define PREV_SONG 0X02 
#define CMD_PLAY_W_INDEX 0X03
#define VOLUME_UP_ONE 0X04
#define VOLUME_DOWN_ONE 0X05
#define CMD_SET_VOLUME 0X06
#define SET_DAC 0X17
#define CMD_PLAY_WITHVOLUME 0X22
#define CMD_SEL_DEV 0X09
#define DEV_TF 0X02
#define SLEEP_MODE_START 0X0A
#define SLEEP_MODE_WAKEUP 0X0B
#define CMD_RESET 0X0C
#define CMD_PLAY 0X0D
#define CMD_PAUSE 0X0E
#define CMD_PLAY_WITHFOLDER 0X0F
#define STOP_PLAY 0X16
#define PLAY_FOLDER 0X17
#define SHUFFLE 0X18
#define SET_CYCLEPLAY 0X19
#define SET_DAC 0X17
#define DHTPIN 10
#define DHTTYPE DHT11   // DHT 11

const uint64_t INTRO[] = {
  0x0000000000000000,
  0x0000609090906000,
  0x0000020206020e00,
  0x0000000000000000,
  0x00a2a2aab6220000,
  0x0064171414630000,
  0x00a9a9afa9a90000,
  0x0074147615740000,
  0x00009691f1919600,
  0x0000525e52d24c00,
  0x0000326a0b4a3200,
  0x000077816711e700
};

const uint64_t NOTEN[] = {
  0x3e333e301e000000,
  0x3e333e301e00c0c0,
  0x1f33331f03030000,
  0x1e3303331e000000,
  0x1e3303331e00c0c0,
  0x3e33333e30300000,
  0x3e33333e3030c0c0,
  0x1e033f331e000000,
  0x06060f06361c0000,
  0x06060f06361cc0c0,
  0x1e303e33331e0000,
  0x1e303e33331ec0c0
};

const uint64_t CHART[] = {
  0x002c22e2222c0000,
  0x00a5bda5a5990000,
  0x0024222724730000,
  0x007e7e7e7e7e7e00
};

const uint64_t SOUND[] = {
  0x000024a4e424ee00,
  0x000025bca5a51800,
  0x00004b2818284b00,
  0x007e7e7e7e7e7e00,
  0x00c6282422cc0000,
  0x0098a5a5a5a40000,
  0x0074969594740000,
  0x007e7e7e7e7e7e00
};

const uint64_t CIJFERS[] = {
  0x3c66666e76663c00,
  0x7e1818181c181800,
  0x7e060c3060663c00,
  0x3c66603860663c00,
  0x30307e3234383000,
  0x3c6660603e067e00,
  0x3c66663e06663c00,
  0x1818183030667e00,
  0x3c66663c66663c00,
  0x3c66607c66663c00,
  0x0000000000000000
};

const uint64_t FREE[] = {
  0x06060f06361c0000,
  0x030333331f000000,
  0x1e033f331e000000,
  0x1e033f331e000000
};

const uint64_t ITJING[] = {
  0x007e7e7e7e7e7e00,
  0x0066666666666600,
  0x007e6666667e6600,
  0x00667e6666667e00,
  0x007e7e7e667e6600,
  0x00667e667e7e7e00,
  0x00667e6666666600,
  0x00666666667e6600,
  0x007e7e7e667e7e00,
  0x007e7e667e7e7e00,
  0x007e7e7e66666600,
  0x006666667e7e7e00,
  0x007e667e7e7e7e00,
  0x007e7e7e7e667e00,
  0x006666667e666600,
  0x0066667e66666600,
  0x007e66667e7e6600,
  0x00667e7e66667e00,
  0x007e7e6666666600,
  0x00666666667e7e00,
  0x007e66667e667e00,
  0x007e667e66667e00,
  0x0066666666667e00,
  0x007e666666666600,
  0x007e66667e7e7e00,
  0x007e7e7e66667e00,
  0x007e666666667e00,
  0x00667e7e7e7e6600,
  0x00667e66667e6600,
  0x007e667e7e667e00,
  0x0066667e7e7e6600,
  0x00667e7e7e666600,
  0x0066667e7e7e7e00,
  0x007e7e7e7e666600,
  0x006666667e667e00,
  0x007e667e66666600,
  0x007e667e667e7e00,
  0x007e7e667e667e00,
  0x0066667e667e6600,
  0x00667e667e666600,
  0x007e7e6666667e00,
  0x007e6666667e7e00,
  0x007e7e7e7e7e6600,
  0x00667e7e7e7e7e00,
  0x006666667e7e6600,
  0x00667e7e66666600,
  0x00667e667e7e6600,
  0x00667e7e667e6600,
  0x007e667e7e7e6600,
  0x00667e7e7e667e00,
  0x007e66667e666600,
  0x0066667e66667e00,
  0x0066667e667e7e00,
  0x007e7e667e666600,
  0x007e667e7e666600,
  0x0066667e7e667e00,
  0x00667e7e667e7e00,
  0x007e7e667e7e6600,
  0x00667e66667e7e00,
  0x007e7e66667e6600,
  0x007e7e66667e7e00,
  0x0066667e7e666600,
  0x007e667e667e6600,
  0x00667e667e667e00
};

TM1637Display display(CLK, DIO);
LiquidCrystal_I2C lcd(0x27, 16, 2);
LedControl lc=LedControl(DIN1,CLK1,CS1,8);
SoftwareSerial mp3(MP3_RX, MP3_TX);
SoftwareSerial SerialESP1(ESP1_TX, ESP1_RX); //RX, TX for ESP1
SoftwareSerial SerialESP2(ESP2_RX, ESP2_TX); //RX, TX for ESP2
DS3231  rtc(SDA, SCL);
DHT dht(DHTPIN, DHTTYPE);

enum CountDownModeValues
{
  COUNTING_STOPPED,
  COUNTING
};

//String AP = "VGV6574883726";
//String PASS = "keithjarrett123A";
String AP = "Muzieklab_1";
String PASS = "keithjarrett";

byte countDownMode = COUNTING_STOPPED;
byte tenths;
char seconds;
char minutes;
unsigned long S5teller = 0;

int togglestate = 0; // veranderen in bool.
int toggleCounter = 0;
int showInfoCounter = 0;
int ledCounter = 0;
int S5counter1 = 0; 
int S5counter2 = 0;
int S5counterState1 = 0;
int S5counterState2 = 0;
int s;
int aState;
int aLastState;  
int countTrueCommand = 0;
int countTimeCommand; 

bool title = false;
bool togglestateOK = false;
bool stateMenuLayer0 = false;
bool stateMenuLayer1 = false;
bool stateMenuLayer2 = false;
bool stateMenuLayer3 = false;
bool showInfo = false;
bool showReeksLCD = false;
bool ifClick = true;
bool ifPause = false;
bool ifPlay = false;
bool S4toggleState = true;
bool S5toggleState = true;
bool found = false; 
bool wifiConnect = false;
bool G1_aan = false;
bool G2_aan = false;
bool G3_aan = false;
bool G4_aan = false;
bool MP3start = false;

byte wifiCounter = 0;
byte S4toggleCount = 0;
byte groupsizeMax = 3;
byte diceNumber = 0;
byte statediceNumber;
byte PauseLedDelay = 220;
byte MenuLayer = 0;
byte selectModus;
byte pinCS = 10;
byte u;
byte k;
byte maxKaarten = 30;
byte kaartMatrix10;
byte kaartMatrix1;

File dataFile;

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() { 
  Serial.begin(115200);
  Serial3.begin(115200);
  Wire.begin();
  rtc.begin();
  mp3.begin(9600);
  SerialESP1.begin(115200);
  dht.begin();

  delay(200);
    sendCommand(CMD_SEL_DEV, DEV_TF);   
    sendCommand(CMD_SET_VOLUME, 20); 
  delay(20);

  //setclock(); 
  seed();

  pinMode(10, INPUT);
  pinMode(24, INPUT);
  pinMode(25, INPUT);
  pinMode(26, INPUT);
  pinMode(27, INPUT);
  
  pinMode(49, OUTPUT);
  pinMode(50, OUTPUT);
  pinMode(51, OUTPUT);
  pinMode(52, OUTPUT);
  
  pinMode(G1, OUTPUT);
  pinMode(G2, OUTPUT);
  pinMode(G3, OUTPUT);
  pinMode(G4, OUTPUT);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  
  pinMode (outputA,INPUT);
  pinMode (outputB,INPUT);
  
  pinMode (S_OK, INPUT);
  pinMode (RESET, OUTPUT);
  
  pinMode(BUZZ, OUTPUT);
  pinMode(pinCS, OUTPUT);

  aLastState = digitalRead(outputA);   
  
  irrecv.enableIRIn();

  display.setBrightness(7);
  
  lcd.init();
  lcd.backlight();
  display.showNumberDec(0);

  clearMatrix();
  startup();
  
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
       for(int i=49; i<53; i++){
          digitalWrite(i, HIGH);
       }
}


void loop() {  
  byte S1 = digitalRead(24);
  byte S2 = digitalRead(25);
  byte S3 = digitalRead(26);
  byte S4 = digitalRead(27);
  byte S5 = digitalRead(S_OK);

  aState = digitalRead(outputA); 
   
   if(aState != aLastState){     
     if(digitalRead(outputB) != aState){
           S5counter1 ++;
         }else {
           S5counter1 --;
         }
     }    
    
  if(S5counter1 == (S5counterState1+2)){
    S5counter2++;
    S5counterState1 = S5counter1;

  }  
  
  if(S5counter1 == (S5counterState1-2)){
    S5counter2--;
    S5counterState1 = S5counter1;
  }  
  
  if(S5 == LOW){
   if(MenuLayer < 2){
      ifClick = true;
        if (stateMenuLayer0 == false && MenuLayer == 0 && S5toggleState == true) {
          stateMenuLayer0 = true;                       
          S5toggleState = false;
        }
        
        if (stateMenuLayer0 ==  true && stateMenuLayer1 == false && MenuLayer == 1 && S5toggleState == true){
          stateMenuLayer1 = true;
          diceNumber = 0;
          MenuLayer = 2;
          S5toggleState = false;

        } 
      lcd.clear();
   }S5toggleState = false;
   }
     

  if(S5toggleState == false){
   toggleCounter++; 
  }
  
  if(S4toggleState == false){
    toggleCounter++;
  }
  
  if(toggleCounter == 300){
    S4toggleState = true;
    S5toggleState = true;
    toggleCounter = 0;
  }
 
  if(ifPause == true){
    ledCounter++;
    if(ledCounter < (0,8*PauseLedDelay)){ 
      digitalWrite(LED2, LOW);
    }
    if((ledCounter > (2*PauseLedDelay)) && (ledCounter < (3*PauseLedDelay))){ 
      digitalWrite(LED2, HIGH);
    }
    if(ledCounter > (3*PauseLedDelay)){
      ledCounter = 0;   
      digitalWrite(LED2, HIGH);
    }
  }

  if(S2 == HIGH){
    if(MenuLayer >= 2 && diceNumber > 0){
       kaart();
    }
  }
  
  if(S3 == HIGH){
    if(MenuLayer >= 2 && diceNumber > 0){
    reeks();
    }
  }
  
  if(S4 == HIGH && S4toggleState == true){
    if(MenuLayer >= 2 && diceNumber > 0){        
      switch(S4toggleCount){

       case 0:
             DisplaySoundTrack();
             int j = 300;
             MP3countdown(j);
             MP3startPlay(MP3start);
                        
             S4toggleCount = 1;
             S4toggleState = false;
             
          break;

       case 1:
             if(ifPause == false){
              MP3pause();
              S4toggleState = false;
              break;
            }
             if(ifPause ==  true){
              MP3play();
              S4toggleState = false;
              break;
            }
      }
    }
  }
  
  if(stateMenuLayer0 == true && stateMenuLayer1 == true && MenuLayer == 2 && selectModus == 0 && wifiConnect == false){
    //wifiConnect = wifiESP();
    wifiConnect = true;
    ifClick = true;
  }
  
  if(showInfo == true){
    if(showInfoCounter == 0 || showInfoCounter == 300 || showInfoCounter == 600){
      lcd.clear();

        if(stateMenuLayer0 == false && MenuLayer == 0){
            lcd.setCursor(8,0);
            lcd.print(rtc.getTimeStr());
        } else{
              lcd.setCursor(0, 1);
              lcd.print(F("wifi|"));
              lcd.setCursor(8,0);
              lcd.print(rtc.getTimeStr());
                  switch(selectModus){
                    case 0:
                      lcd.setCursor(0, 0);
                        lcd.print(F("pcc|"));
                      lcd.setCursor(5, 1);
                        lcd.print(F("connect"));
                      lcd.setCursor(15, 1);
                        lcd.blink();
                      break;
          
                    case 1:
                      lcd.setCursor(0, 0);
                        lcd.print(F("cj |"));
                      lcd.setCursor(5, 1);
                        lcd.print(F("no wifi"));
                      break;
          
                    case 2: 
                      lcd.setCursor(0, 0);
                        lcd.print(F("cva|"));
                      lcd.setCursor(5, 1);
                        lcd.print(F("no wifi"));
                      break;
                      
                    case 3: 
                      lcd.setCursor(0, 0);
                        lcd.print(F("cod|"));
                      lcd.setCursor(5, 1);
                        lcd.print(F("no wifi"));
                      lcd.setCursor(15, 1);
                        lcd.noBlink();
                      break;
                  }
        }
    }
    showInfoCounter ++;
    
    if(showInfoCounter == 900){
      showInfoCounter = 0;
      ifClick = true;
      showInfo = false;
    }
  }

  if(ifClick == true){
    if(MenuLayer == 0){
      display.showNumberDec(0);
    }
    if(stateMenuLayer0 == false){
      MenuLayer = 0;
      if(diceNumber > 0 && stateMenuLayer2 == false){
        display.showNumberDec(0);
        MP3stopPlay();
      }
    }
    if(stateMenuLayer0 == true){
      MenuLayer = 1;
      if(diceNumber > 0 && stateMenuLayer2 == false){
        display.showNumberDec(0);
        MP3stopPlay();
      }
    }
    if(stateMenuLayer1 == true){
      MenuLayer = 2;
    }
  }
  
  if (stateMenuLayer0 == false && ifClick == true && MenuLayer == 0) {
          lcd.clear();
          lcd.setCursor(0, 0);
            lcd.print("mode");
          lcd.blink();
          lcd.setCursor(0, 1);
          switch(selectModus){
            case 0:
              lcd.print("pcc");
              break;

            case 1:
              lcd.print("classic jamming");
              break;

            case 2:
              lcd.print("cva");
              break;

            case 3:
              lcd.print("codarts");
              break;            
          }
  }
  
  if (stateMenuLayer0 == true && stateMenuLayer1 == false && ifClick == true && MenuLayer == 1) {
          lcd.clear();
          lcd.setCursor(0, 0);
            lcd.print(F("ensemble size"));
          lcd.setCursor(0, 1);
            lcd.print(F("max"));
          lcd.setCursor(14, 1);
            lcd.print(groupsizeMax);
          lcd.blink();
  }

  if (stateMenuLayer0 == true && stateMenuLayer1 == true && ifClick == true && MenuLayer == 2){ // menu in speeltoestand.
          lcd.clear();
          lcd.setCursor(0, 0);
          switch(selectModus){
            case 0:
              lcd.print("pcc|");
              break;

            case 1:
              lcd.print("cj |");
              break;

            case 2: 
              lcd.print("cva|");
              break;

            case 3: 
              lcd.print("cod|");
              break;
          }
          
          lcd.setCursor(4, 0);
          lcd.print("e-max");
          lcd.setCursor(14, 0);
          lcd.print(groupsizeMax);
          lcd.setCursor(0, 1);
          lcd.print("dice         ");
          
              if(diceNumber > 0){
                lcd.blink();
                lcd.setCursor(14, 1);
                lcd.print(diceNumber);
                display.showNumberDec(diceNumber);
              }
              if(diceNumber > 20){
                lcd.blink();
                lcd.setCursor(13, 1);
                  lcd.print("    "); 
                lcd.setCursor(13, 1);     
                lcd.print(diceNumber);
                display.showNumberDec(diceNumber);
              }
      
              if (diceNumber == 0 ) {
                lcd.noBlink();
                lcd.print("  ");
                display.showNumberDec(0);
              }
       stateMenuLayer2 = true;

  }
  
  ifClick = false;
  togglestateOK = true;
  
  if (irrecv.decode(&results)) {
    switch (results.value) {
      case 0x409F00FF: // ch+
        ifClick = true;
          if(stateMenuLayer0 == false && MenuLayer == 0 && selectModus >= 0 && selectModus <= 2 ){
            selectModus ++;
          }
          if (stateMenuLayer0 == true && stateMenuLayer1 == false && MenuLayer == 1 && groupsizeMax >= 3 && groupsizeMax <= 5) {
            groupsizeMax ++;
          }
          
        if (stateMenuLayer0 == true && stateMenuLayer1 == true && MenuLayer == 2) {
          statediceNumber = diceNumber;
          while (statediceNumber == diceNumber) {
            dice();
            Serial.println(diceNumber);
            lcd.setCursor(14, 1);
            lcd.print("  ");
          }
          MP3stopPlay();
          clearMatrix();
        }
        break;
        
      case 0x409F807F: // ch-
        if(MenuLayer < 2){
          ifClick = true;
        }
            
        if (stateMenuLayer0 == false && MenuLayer == 0 && selectModus >= 1 && selectModus <= 3){ 
            selectModus --;
        }
        
        if (stateMenuLayer0 == true && stateMenuLayer1 == false && MenuLayer == 1 && groupsizeMax >= 4  && groupsizeMax <= 6) {
            groupsizeMax --;
        }
        
        break;

      case 0x409FF807: // OK
          if(MenuLayer < 2){
            ifClick = true;
              if (stateMenuLayer0 == false && MenuLayer == 0) {
                stateMenuLayer0 = true;                       
              }
              
              if (stateMenuLayer0 ==  true && stateMenuLayer1 == false && MenuLayer == 1){
                stateMenuLayer1 = true;
                diceNumber = 0;
                MenuLayer = 2;
              } 
            lcd.clear();
          }
        break;

      case 0x409F20DF: // menu
        homeMenu();
        break;

      case 0x409F609F: // info
        showInfo = true;
        break;
      
      case 0x409F38C7: // exit
        ifClick = true;
        stateMenuLayer2 = false;
        clearMatrix();
        
        switch(MenuLayer){
            case 2:
              stateMenuLayer0 = true;
              stateMenuLayer1 = false;
              MenuLayer = 1;
              break;
              
            case 1: 
              stateMenuLayer0 = false;
              MenuLayer = 0;
              break;
        }
        break;

      case 0x409F906F: // C (return) button
        clearMatrix();
        showReeksLCD = false;
        ifClick = true;
        break;
        
      case 0x409FE01F: // || (pause) button
       if(MenuLayer >= 2 && diceNumber > 0){
        if(ifPause == false){
          MP3pause();
          break;
        }
        if(ifPause ==  true){
          MP3play();
          break;
        }
       }
      case 0x409F40BF: // vol+
       if(MenuLayer >= 2){
         sendCommand(VOLUME_UP_ONE, 0x0F01);
       }
         break;

      case 0x409FC03F: // vol-
       if(MenuLayer >= 2){
         sendCommand(VOLUME_DOWN_ONE, 0x0F01);
       }
         break;

      case 0x409F30CF: // mute
         mute();
         break;

      case 0x409F50AF: // power
         reset();
         break;
         
      case 0x409FD02F: // [_]
         BuzzLong();
         break;

      case 0x409FB04F: // [E]
         Buzz4Short();
         break;

      case 0x409F827D: // FAV
         dataSheet();
         dataSD();
         digitalWrite(LED1, LOW);
         delay(100);
         digitalWrite(LED1, HIGH);
         delay(100);
         digitalWrite(LED1, LOW);
         break;

      case 0x409FD22D : // rood = wit
         break;
         
      case 0x409F52AD : // groen = rood
       if(MenuLayer >= 2 && diceNumber > 0){
          kaart();
       }
         break;
         
      case 0x409F926D: // geel = zwart
       if(MenuLayer >= 2 && diceNumber > 0){
          reeks();
       }
         break;

      case 0x409F12ED: // blauw = blauw
       if(MenuLayer >= 2 && diceNumber > 0){ 
          DisplaySoundTrack();
          int j = 300;
          MP3countdown(j);                 
          MP3startPlay();
        if(ifPause == true){
          ifPause = false;
        }
        S4toggleCount = 1;
       }
        break; 

       case 0x409F8877: // 1
        G1_ctrl();      
        break;
      
       case 0x409F48B7: // 2
        G2_ctrl();      
        break;

       case 0x409FC837: // 3
        G3_ctrl();      
        break;

       case 0x409F28D7: // 4
        G4_ctrl();      
        break;
    }
    irrecv.resume();  
  }

  if(S5counter2 > S5counterState2){
    ifClick = true;
          if(stateMenuLayer0 == false && MenuLayer == 0 && selectModus >= 0 && selectModus <= 2 ){
            selectModus ++;
          }
          if (stateMenuLayer0 == true && stateMenuLayer1 == false && MenuLayer == 1 && groupsizeMax >= 3 && groupsizeMax <= 5) {
            groupsizeMax ++;
          }
          
        if (stateMenuLayer0 == true && stateMenuLayer1 == true && MenuLayer == 2) {
          statediceNumber = diceNumber;
          while (statediceNumber == diceNumber) {
            dice();
            Serial.println(diceNumber);
            lcd.setCursor(14, 1);
            lcd.print("  ");
          }
          MP3stopPlay();
          clearMatrix();
        }
  }

  if(S5counter2 < S5counterState2){
        if(MenuLayer < 2){
          ifClick = true;
        }
            
        if (stateMenuLayer0 == false && MenuLayer == 0 && selectModus >= 1 && selectModus <= 3){ 
          selectModus --;
        }
        
        if (stateMenuLayer0 == true && stateMenuLayer1 == false && MenuLayer == 1 && groupsizeMax >= 4  && groupsizeMax <= 6) {
          groupsizeMax --;
        } 
  }
    
  aLastState = aState;
  S5counterState2 = S5counter2;

  delay(1);
}


/*
  http://xantorohara.github.io/led-matrix-editor/#007e7e7e7e7e7e00|0066666666666600|007e6666667e6600|00667e6666667e00|007e7e7e667e6600|00667e667e7e7e00|00667e6666666600|00666666667e6600|007e7e7e667e7e00|007e7e667e7e7e00|007e7e7e66666600|006666667e7e7e00|007e667e7e7e7e00|007e7e7e7e667e00|006666667e666600|0066667e66666600|007e66667e7e6600|00667e7e66667e00|007e7e6666666600|00666666667e7e00|007e66667e667e00|007e667e66667e00|0066666666667e00|007e666666666600|007e66667e7e7e00|007e7e7e66667e00|007e666666667e00|00667e7e7e7e6600|00667e66667e6600|007e667e7e667e00|0066667e7e7e6600|00667e7e7e666600|0066667e7e7e7e00|007e7e7e7e666600|006666667e667e00|007e667e66666600|007e667e667e7e00|007e7e667e667e00|0066667e667e6600|00667e667e666600|007e7e6666667e00|007e6666667e7e00|007e7e7e7e7e6600|00667e7e7e7e7e00|006666667e7e6600|00667e7e66666600|00667e667e7e6600|00667e7e667e6600|007e667e7e7e6600|00667e7e7e667e00|007e66667e666600|0066667e66667e00|0066667e667e7e00|007e7e667e666600|007e667e7e666600|0066667e7e667e00|00667e7e667e7e00|007e7e667e7e6600|00667e66667e7e00|007e7e66667e6600|007e7e66667e7e00|0066667e7e666600|007e667e667e6600|00667e667e667e00
  http://xantorohara.github.io/led-matrix-editor/#00c6282422cc0000|0098a5a5a5a40000|0074969594740000|007e7e7e7e7e7e00|002c22e2222c0000|00a5bda5a5990000|0024222724730000|007e7e7e7e7e7e00|000024a4e424ee00|000025bca5a51800|00004b2818284b00|007e7e7e7e7e7e00
  http://xantorohara.github.io/led-matrix-editor/#0000621612166006|000029e92f29c900|000099b585a59900|00003b4033087300|00a2a2aab6220000|0064171414630000|00a9a9afa9a90000|0074147615740000|06a2a2aab6220000|0000020602060006
  http://xantorohara.github.io/led-matrix-editor/#0000000000000000|0000609090906000|0000020206020e00|0000000000000000|00a2a2aab6220000|0064171414630000|00a9a9afa9a90000|0074147615740000|00009691f1919600|0000525e52d24c00|0000326a0b4a3200|000077816711e700
 
 */
