void seed(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();

      Serial.print(F("Humidity: "));
      Serial.print(h);
      Serial.print(F("%  Temperature: "));
      Serial.print(t);
      Serial.println(F("°C "));
      
   String Stime= String(rtc.getTimeStr());
   Serial.println(Stime);
   String subStime= Stime.substring(6);
   String subStime2= Stime.substring(3);
   int z1 = subStime.toInt();
   int z2 = subStime2.toInt();
   s = (sq(z1)+ sq(z2) + (floor(h) * floor(t)));
    
   Serial.println(z1);
   Serial.println(z2);
   Serial.println(h);
   Serial.println(t);
   
   randomSeed(s);
   Serial.println("Randomseed:");
   Serial.println(s);
}

void setclock(){
  rtc.setDOW(THURSDAY);
  rtc.setTime(18, 58, 00);
  rtc.setDate(23, 7, 2020);
}

void clearMatrix(){  
        for(int index=0;index<9;index++) {
            lc.shutdown(index,false);   
            lc.clearDisplay(index);
            lc.setIntensity(index, 1);
        } 
}

void startup(){
    if (SD.begin()){
      Serial.println("DataLog start");
      Serial.println(rtc.getDateStr());
      Serial.print(",");
      Serial.println(rtc.getTimeStr());
                 
    } else{
      Serial.println("DataLog failed");
    } 
    
    lcd.blink();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("machine of");
    lcd.setCursor(0,1);
    lcd.print("changes");
    clearMatrix();
    
      for(int i=0; i<8; i++){
        int p = random(0, 63);
        displayMatrix(ITJING[p], i);
      }
        delay(600);
      
      for(int i=4; i<8; i++){
          displayMatrix(INTRO[i], i);
      }
        
      for(int i=0; i<4; i++){
          displayMatrix(INTRO[i], i);
      }
        delay(150);

      for(int i=8; i<12; i++){
          int j = i-8;
          displayMatrix(INTRO[i], j);
      }
      
        delay(1000);
          
        int count = 0; 
        int max = 120;
        int j = constrain(0, 0, 15);
        
      while(count < max){
        lc.setLed(0,random(0, 8), random(0, 8), true);
        lc.setLed(1,random(0, 8), random(0, 8), true);
        lc.setLed(2,random(0, 8), random(0, 8), true);
        lc.setLed(3,random(0, 8), random(0, 8), true);
        lc.setLed(4,random(0, 8), random(0, 8), true);
        lc.setLed(5,random(0, 8), random(0, 8), true);
        lc.setLed(6,random(0, 8), random(0, 8), true);
        lc.setLed(7,random(0, 8), random(0, 8), true);
        
          count++;
            j++;
       }
       
      delay(400);
      clearMatrix(); 
}

void homeMenu(){ // click menu button
        ifClick = true;
        stateMenuLayer2 = false;
        stateMenuLayer1 = false;
        stateMenuLayer0 = false;
        wifiConnect = false;
        digitalWrite(LED1, LOW);
        sendCommandESP("AT+CWQAP", 5, "OK");
        MenuLayer = 0;
        clearMatrix();
}

void reset(){  // click power button
         homeMenu();
         MP3stopPlay();
         display.showNumberDec(0);
         startup();
}

void mute(){  // click mute button
         ifClick = true;
         diceNumber = 0; 
         MP3stopPlay();
         clearMatrix(); 
}


void dice(){      
      if(groupsizeMax == 3){
            u = random(0,5);  
            switch(u){
              case 0: diceNumber = 2;   break;
              case 1: diceNumber = 3;   break;
              case 2: diceNumber = 21;  break;      
              case 4: diceNumber = 1;   break;
            }
      }
    
      if(groupsizeMax == 4){
            u = random(0,6);  
            switch(u){
              case 0: diceNumber = 1;   break;
              case 1: diceNumber = 2;   break;
              case 2: diceNumber = 3;   break;      
              case 3: diceNumber = 21;  break;
              case 4: diceNumber = 4;   break;
              case 5: diceNumber = 31;  break;
              case 6: diceNumber = 41;   break;            
            }
      }
      
      if(groupsizeMax == 5){
            u = random(0,5);  
            switch(u){
              case 0: diceNumber = 3;   break;
              case 1: diceNumber = 21;  break;
              case 2: diceNumber = 4;   break;      
              case 3: diceNumber = 31;  break;
              case 4: diceNumber = 5;   break;
              case 5: diceNumber = 32;  break;
              case 6: diceNumber = 23;  break;
              case 7: diceNumber = 12;  break;
            }
      }
      
      if(groupsizeMax == 6){
            u = random(0,7);  
            switch(u){
              case 0: diceNumber = 3;   break;
              case 1: diceNumber = 21;  break;
              case 2: diceNumber = 4;   break;      
              case 3: diceNumber = 31;  break;      
              case 4: diceNumber = 5;   break;
              case 5: diceNumber = 41;  break;
              case 6: diceNumber = 6;   break;
            }
      }
}

void kaart(){
  clearMatrix(); 
  
  for(int i=0; i<3; i++){
    int j = i+4;
    displayMatrix(CHART[i], j);
  }
    int i = random (0, 63);
    displayMatrix(ITJING[i], 7);
  
  k = random(1,(1.2*maxKaarten));
  
  Serial.print("kaart: ");
  Serial.println(k);

//  Serial.print("Modulo Test: ");
//  byte q = k/10;
//  byte r = k % 10;
//  Serial.println(q);
//  Serial.println(r);


if(k <= maxKaarten){
  byte kDeel = k/10;
  switch(kDeel){
    case 0: kaartMatrix10 = 0; kaartMatrix1 = k; break;
    case 1: kaartMatrix10 = 1; kaartMatrix1 = k % 10; break;
    case 2: kaartMatrix10 = 2; kaartMatrix1 = k % 10; break;
    case 3: kaartMatrix10 = 3; kaartMatrix1 = k % 10; break;
    case 4: kaartMatrix10 = 4; kaartMatrix1 = k % 10; break;
    case 5: kaartMatrix10 = 5; kaartMatrix1 = k % 10; break;
  }      
      displayMatrix(CIJFERS[kaartMatrix10], 0);
      displayMatrix(CIJFERS[kaartMatrix1], 1);
    }      
    if(k > maxKaarten){
      for(int i=0; i<4; i++){
      displayMatrix(FREE[i], i);
   }
 }
}

void DisplaySoundTrack(){
  clearMatrix(); 
  
  for(int i=4; i<7; i++){
    displayMatrix(SOUND[i], i);
  }     
  for(int i=0; i<3; i++){
    displayMatrix(SOUND[i], i);
  }
/*
    int i = random (0, 63);
      displayMatrix(ITJING[i], 7);
    i = random (0, 63);  
      displayMatrix(ITJING[i], 3);*/
}

void sendCommand(int8_t command, int16_t dat){
 delay(20);
 Send_buf[0] = 0x7e; //starting byte
 Send_buf[1] = 0xff; //version
 Send_buf[2] = 0x06; //the number of bytes of the command without starting byte and ending byte
 Send_buf[3] = command; //
 Send_buf[4] = 0x00;//0x00 = no feedback, 0x01 = feedback
 Send_buf[5] = (int8_t)(dat >> 8);//datah
 Send_buf[6] = (int8_t)(dat); //datal
 Send_buf[7] = 0xef; //ending byte
 for(uint8_t i=0; i<8; i++)//
     {
       mp3.write(Send_buf[i]) ;//send bit to serial mp3
       Serial.print(Send_buf[i],HEX);//send bit to serial monitor in pc
     }
     Serial.println();
}

void MP3startPlay(){
    sendCommand(SHUFFLE, 0x0F01);
    ifPlay = true;
    digitalWrite(LED2, HIGH);
}

void MP3stopPlay(){
     sendCommand(STOP_PLAY, 0x0F01);
     ifPlay = false;
     ifPause = false;
     digitalWrite(LED2, LOW);
     S4toggleCount = 0;
}

void BuzzLong(){
    digitalWrite(BUZZ, HIGH);
      delay(1000);
    digitalWrite(BUZZ, LOW);
}

void Buzz4Short(){
    for(int i=0; i<9; i++){
      digitalWrite(BUZZ, HIGH);
        delay(60);
      digitalWrite(BUZZ, LOW);
        delay(60);
    }
}

void MP3pause(){
    sendCommand(CMD_PAUSE, 0x0F01);
    ifPause = true;
    ledCounter = 0;
      delay(100);
}

void MP3play(){
    sendCommand(CMD_PLAY, 0x0F01);
    ifPause = false;
    digitalWrite(LED2, HIGH);
      delay(100);
      bool x = true;
      return x;
}

void reeks(){
  clearMatrix(); 
  
  byte v = 6;
  byte scale[6];
  byte i = 0;
  byte val;
  bool compleet = false;
  bool controle[12] = {true, true, true, true, true, true, true, true, true, true, true, true};

  while(!compleet){
    val = random(12);
    if(controle[val]){
       scale[i++] = val;
       controle[val] = false;
      }
    compleet = (i == v);

    if(compleet){
      for(i=0; i<=5; i++){
        Serial.println("REEKS");
        Serial.println(scale[i]);
        }
          compleet = 1;
      }
    }

    byte j = 0;
    
    for(i=0; i<=5; i++){   
      switch(i){
        case 0: j=4;  break;
        case 1: j=5;  break;
        case 2: j=6;  break;
        case 3: j=7;  break;
        case 4: j=0;  break;
        case 5: j=1;  break;
      }   
       displayMatrix(NOTEN[scale[i]], j);
    }
}

void displayMatrix(uint64_t image, int k){
  for (int i = 0; i < 8; i++) {
    byte row = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      lc.setLed(k, i, j, bitRead(row, j));
    }
  }
}

void dataSD(){
    dataFile = SD.open("data-log.txt", FILE_WRITE);
           if (dataFile) {
              dataFile.print(rtc.getDateStr());
              dataFile.print(", ");
              dataFile.println(rtc.getTimeStr());
              dataFile.print("Test Test Test");
           }
          dataFile.close(); 
          delay(10);
}

void dataSheet(){
// serial connection to ESP  
          int timeset = random(0,12);
          int Xaxx = random(0,12);
          int Yaxx = random(0,12);
          int matrix2 = random(0,12);
          int matrix3 = random(0,12);
          int p = random(0,12);
          
          SerialESP1.print(s);
          SerialESP1.println("\n");
            delay(20);
          SerialESP1.print(timeset);
          SerialESP1.println("\n");
            delay(20);
          SerialESP1.print(Xaxx);
          SerialESP1.println("\n");
            delay(20);
          SerialESP1.print(Yaxx);
          SerialESP1.println("\n");
            delay(20);        
          SerialESP1.print(matrix2);
          SerialESP1.println("\n");
            delay(20);
          SerialESP1.print(matrix3);
          SerialESP1.println("\n");
            delay(20);
          SerialESP1.print(p);
          SerialESP1.println("\n");
            delay(200);
}

void sendCommandESP(String command, int maxTime, char readReplay[]) {
  while(countTimeCommand < (maxTime*1))
  {
    Serial3.println(command);     //at+cipsend
    if (Serial3.find(readReplay))  //ok
    {
      found = true;
      break;
    }
    countTimeCommand++;
  }
  
  if (found)
  {
    countTrueCommand++;
    countTimeCommand = 0;
    
  } else {
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  found = false;
}

void G1_ctrl(){
         if(G1_aan == false){
          digitalWrite(G1, HIGH);
          G1_aan = true;
       }
       else{
          digitalWrite(G1, LOW);
        G1_aan = false;
       }
}

void G2_ctrl(){
         if(G2_aan == false){
          digitalWrite(G2, HIGH);
          G2_aan = true;
       }
       else{
          digitalWrite(G2, LOW);
        G2_aan = false;
       }
}

void G3_ctrl(){
         if(G3_aan == false){
          digitalWrite(G3, HIGH);
          G3_aan = true;
       }
       else{
          digitalWrite(G3, LOW);
        G3_aan = false;
       }
}

void G4_ctrl(){
         if(G4_aan == false){
          digitalWrite(G4, HIGH);
          G4_aan = true;
       }
       else{
          digitalWrite(G4, LOW);
        G4_aan = false;
       }
}

void MP3countdown(int j){
  for(int i=0; i<8; i++){
    lc.setLed(7, i, 7, true);
    lc.setLed(7, i, 6, true);
  }
    for(int i=0; i<8; i++){
      lc.setLed(3, i, 7, true);
      lc.setLed(3, i, 6, true);
    }
  delay(j);
    
  for(int i = 0; i <=7; i++){
      lc.setLed(7, i, 7, false);
      lc.setLed(7, i, 6, false);
      delay(j);
  }
    for(int i = 0; i <=7 ; i++){    
      lc.setLed(3, i, 7, false);
      lc.setLed(3, i, 6, false);
      delay(j);
  }
}


bool wifiESP(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Wifi connect");
    
    sendCommandESP("AT", 5 ,"OK");
      Serial.println("OK1");
    sendCommandESP("AT+CWMODE=1\r\n", 5 ,"OK");
      Serial.println("OK2");
    sendCommandESP("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"\r\n", 20 ,"OK");
      Serial.println("OK3");
      
        switch(countTrueCommand){
          case 0:
            lcd.setCursor(0, 1);
            lcd.print("FAIL");
              digitalWrite(LED1, LOW);
              wifiConnect = true;
            break;
          case 3:
            lcd.setCursor(0, 1);
            lcd.print("OK");
              digitalWrite(LED1, HIGH);
              wifiConnect = true;
              countTrueCommand = 0;
            break;
        }
        delay(1000);
        return wifiConnect; 
}     
