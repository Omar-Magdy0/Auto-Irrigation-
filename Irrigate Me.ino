#include <Arduino.h>
#include <EEPROM.h>
#include <FlashStringTable.h>

//Program messages
BEGIN_FLASH_STRING_TABLE(txt)
/*0*/ADD_FLASH_STRING("         ")
/*1*/ADD_FLASH_STRING("%")
END_FLASH_STRING_TABLE()
BEGIN_FLASH_STRING_TABLE(opttxt)
/*0*/ADD_FLASH_STRING("   *OPTIMAL WATER FOR CROP 1 : ")
/*1*/ADD_FLASH_STRING("   *OPTIMAL WATER FOR CROP 2 : ")
/*2*/ADD_FLASH_STRING("")
END_FLASH_STRING_TABLE()
BEGIN_FLASH_STRING_TABLE(crptxt)
/*0*/ADD_FLASH_STRING("-CROP 1 : ")
/*1*/ADD_FLASH_STRING("-CROP 2 : ")
/*2*/ADD_FLASH_STRING("-INTERVAL FOR PUMP : ")
END_FLASH_STRING_TABLE()
BEGIN_FLASH_STRING_TABLE(mnltxt)
/*0*/ADD_FLASH_STRING("           MANUAL MODE : ON")
/*1*/ADD_FLASH_STRING("           MANUAL MODE : OFF")
END_FLASH_STRING_TABLE()
BEGIN_FLASH_STRING_TABLE(avgtxt)
/*0*/ADD_FLASH_STRING("    *AVG MOISTURE FOR CROP 1 : ")
/*1*/ADD_FLASH_STRING("    *AVG MOISTURE FOR CROP 2 : ")
END_FLASH_STRING_TABLE()
BEGIN_FLASH_STRING_TABLE(mnlcntrltxt)
/*0*/ADD_FLASH_STRING("               *SOLENOID 1 : ") 
/*1*/ADD_FLASH_STRING("       *SOLENOID 2 : ")
/*2*/ADD_FLASH_STRING("                                  *PUMP : ")
END_FLASH_STRING_TABLE()
BEGIN_FLASH_STRING_TABLE(crpstxt)
/*0*/ADD_FLASH_STRING("LOW WATER")
/*1*/ADD_FLASH_STRING("HIGH WATER")
/*2*/ADD_FLASH_STRING("OPTIMAL WATER")
END_FLASH_STRING_TABLE()


// Plants Dataset
BEGIN_FLASH_STRING_TABLE(HWP)
/*0*/ADD_FLASH_STRING("banana")
/*1*/ADD_FLASH_STRING("watermelon")
/*2*/ADD_FLASH_STRING("melon")
/*3*/ADD_FLASH_STRING("cashew")
/*4*/ADD_FLASH_STRING("rice")
/*5*/ADD_FLASH_STRING("asparagus")
/*6*/ADD_FLASH_STRING("raspberry")
/*7*/ADD_FLASH_STRING("grape")
/*8*/ADD_FLASH_STRING("cranberry")
/*9*/ADD_FLASH_STRING("mint")
END_FLASH_STRING_TABLE()



BEGIN_FLASH_STRING_TABLE(MWP)
/*0*/ADD_FLASH_STRING("cucumber")
/*1*/ADD_FLASH_STRING("tomato")
/*2*/ADD_FLASH_STRING("lettuce")
/*3*/ADD_FLASH_STRING("cashew")
/*4*/ADD_FLASH_STRING("celery")
/*5*/ADD_FLASH_STRING("carrot")
/*6*/ADD_FLASH_STRING("apple")
/*7*/ADD_FLASH_STRING("spinach")
/*8*/ADD_FLASH_STRING("cauliflower")
/*9*/ADD_FLASH_STRING("beans")
END_FLASH_STRING_TABLE()


BEGIN_FLASH_STRING_TABLE(LWP)
/*0*/ADD_FLASH_STRING("figs")
/*1*/ADD_FLASH_STRING("potato")
/*2*/ADD_FLASH_STRING("onion")
/*3*/ADD_FLASH_STRING("strawberry")
/*4*/ADD_FLASH_STRING("raddish")
/*5*/ADD_FLASH_STRING("sweet potatoes")
/*6*/ADD_FLASH_STRING("lentils")
/*7*/ADD_FLASH_STRING("bell pepper")
/*8*/ADD_FLASH_STRING("eggplant")
/*9*/ADD_FLASH_STRING("kale")
END_FLASH_STRING_TABLE()


//definitions for working part
const byte valve_1 = 11;
const byte valve_2 =12;
const byte waterPump = 13;
const byte levelSwitch = 2;
const byte moistureSensor_1 = A0;
const byte moistureSensor_2 = A1;
byte optimalWaterPerc_1;
byte optimalWaterPerc_2;
const byte reset = 3;
bool manual ;
byte counter = 0;
const byte mnlled = 4;

String cropFarmed_1 ;
String cropFarmed_2 ;
// HERE SET THE TIME YOU WANT PUMP WORK AFTER FLOAT SWITCH GOES OFF

unsigned short interval = 90;

//function to read data from EEPROM after reset 
void eepromUpdater(){
    char temp = "*";short strCntr = 0;byte verify = EEPROM.read(1023);
  ///----EEPROM search for Data
  if (verify == 42){
for (byte c = 0; temp != '\0' ;c++){temp = char(EEPROM.read(strCntr++));if(temp != '\0')cropFarmed_1 += temp;}
  temp = "*";Serial.print(cropFarmed_1);
for (byte c = 0; temp != '\0' ;c++){temp = char(EEPROM.read(strCntr++));if(temp != '\0')cropFarmed_2 += temp;}
  Serial.print(cropFarmed_2);
interval = EEPROM.read(strCntr++);
  }
  
}
void setup() { 
  digitalWrite(waterPump , HIGH);
  digitalWrite(valve_1 , HIGH);
  digitalWrite(valve_2 , HIGH);
  pinMode(valve_1, OUTPUT);
  pinMode(valve_2, OUTPUT);
  pinMode(waterPump, OUTPUT);
  pinMode(moistureSensor_1, INPUT);
  pinMode(moistureSensor_2, INPUT);
  pinMode(levelSwitch, INPUT_PULLUP);
  digitalWrite(3 , HIGH);
  pinMode(reset, OUTPUT);
  pinMode(mnlled, OUTPUT);
  digitalWrite(reset, HIGH);
  INIT_FLASH_STRING_TABLE(txt)
  INIT_FLASH_STRING_TABLE(mnltxt)
  INIT_FLASH_STRING_TABLE(avgtxt)
  INIT_FLASH_STRING_TABLE(mnlcntrltxt)
  INIT_FLASH_STRING_TABLE(opttxt)
  INIT_FLASH_STRING_TABLE(crptxt)
  INIT_FLASH_STRING_TABLE(crpstxt)
  INIT_FLASH_STRING_TABLE(HWP)
  INIT_FLASH_STRING_TABLE(MWP)
  INIT_FLASH_STRING_TABLE(LWP)

  
  //serial communication with Pc
Serial.begin(9600);
Serial.setTimeout(100);
Serial.print("\nresetted");
eepromUpdater();
int mnl = EEPROM.read(1022);
if(Serial){if(!mnl){Serial.print(mnltxt[1]);manual = false;}if(mnl){Serial.print(mnltxt[0]);manual = true;}}else{manual = false;}



}

//set crop optimal moisture

//set crop optimal moisture
bool chosen1,chosen2 = false;
void moistureChooser(){

if(!chosen1) { if(String(HWP[counter])==cropFarmed_1){optimalWaterPerc_1 = 70;chosen1 = true;}else if(String(MWP[counter])==cropFarmed_1){optimalWaterPerc_1 = 50;chosen1 = true;}else if(String(LWP[counter])==cropFarmed_1){optimalWaterPerc_1 = 30;chosen1 = true;}}
if(!chosen2) { if(String(HWP[counter])==cropFarmed_2){optimalWaterPerc_2 = 70;chosen2 = true;}else if(String(MWP[counter])==cropFarmed_2){optimalWaterPerc_2 = 50;chosen2 = true;}else if(String(LWP[counter])==cropFarmed_2){optimalWaterPerc_2 = 30;chosen2 = true;}}
  if (counter++>= 50)counter=0;

  }

//function to control tank filling
  bool recording = false; bool working = false;
  void tankManager(){
    unsigned long timeStart;unsigned long currentTime = millis(); 
  if((digitalRead(levelSwitch) == LOW)){
    digitalWrite(waterPump ,LOW);working = true;Serial.println();}
    if((digitalRead(levelSwitch) == HIGH) && working){if(!recording){timeStart = millis();recording = true;}}
  //pump keeps working for a certain time after "in milliseconds" it is turned on
  if ( recording && (abs(currentTime - timeStart) >= interval*1000) ){
    recording = false ; digitalWrite(waterPump ,HIGH); working = false;}

  if(working){Serial.print("\nP:1");}else{Serial.print("\nP:0");}Serial.print(" --Int for Pump: ");Serial.print(interval);
    }

 //function to return average reading for soil moisture sensors
 float avgRead1,avgRead2 ; byte i = 0;float Sum1,Sum2 ;
 void avgReadings(){
float moistRead1 = analogRead(moistureSensor_1); float moistRead2 = analogRead(moistureSensor_2);
 
Sum1 = Sum1 + moistRead1;
Sum2 = Sum2 + moistRead2;
  i++;
  if(i == 25){i = 0;avgRead1 = (1023-(Sum1/25.0))/7.73 ; avgRead2 = (1023-(Sum2/25.0))/7.73 ; Sum1 = 0 ; Sum2 = 0; }
}   
 

  //function to control solenoids according to moisture sensor readings
  
void moistureControl(){
  Serial.print(crptxt[0]);Serial.print(cropFarmed_1);Serial.print(opttxt[0]);Serial.print(optimalWaterPerc_1);Serial.println(txt[1]);
  if( (avgRead1 <= (optimalWaterPerc_1 + 5)) && (avgRead1 >= (optimalWaterPerc_1 - 5)))Serial.print(crpstxt[2]);
  if( avgRead1 <= (optimalWaterPerc_1 - 5)){(digitalWrite(valve_1,LOW));Serial.print(crpstxt[0]);}
  if( avgRead1 >= (optimalWaterPerc_1 + 5)){(digitalWrite(valve_1,HIGH));Serial.print(crpstxt[1]);}
  Serial.print(avgtxt[0]);Serial.print(avgRead1);Serial.println(txt[1]);Serial.println();
  //again for crop 2
  Serial.print(crptxt[1]);Serial.print(cropFarmed_2);Serial.print(opttxt[1]);Serial.print(optimalWaterPerc_2);Serial.println(txt[1]);
  if( (avgRead2 <= (optimalWaterPerc_2 + 5)) && (avgRead2 >= (optimalWaterPerc_2 - 5)))Serial.print(crpstxt[2]);
  if( avgRead2  <= (optimalWaterPerc_2 - 5)){(digitalWrite(valve_2,LOW));Serial.print(crpstxt[0]);}
  if( avgRead2 >= (optimalWaterPerc_2 + 5)){(digitalWrite(valve_2,HIGH));Serial.print(crpstxt[1]);}
  Serial.print(avgtxt[1]);Serial.print(avgRead2);Serial.print(txt[1]);
}


//-------------------------------------------------------------------------------
////-----------------EEprom part

//DATA SAVING TO EEPROM for Next Use
String cropFarmed_1Next,cropFarmed_2Next ;unsigned short intervalNext;


void serialReaderSaver(){Serial.setTimeout(30000);

Serial.println("\n*NEXT*");Serial.print(crptxt[2]);Serial.println(intervalNext);
Serial.print(crptxt[0]);Serial.println(cropFarmed_1Next);
Serial.print(crptxt[1]);Serial.print(cropFarmed_2Next);
Serial.read();Serial.readStringUntil('\n');

 unsigned short strCntr = 0 ;


Serial.read();
cropFarmed_1Next = "";Serial.println("\n*Enter Crop 1 :");

  String cropFarmed_1NextPRE = Serial.readStringUntil('\n');
for(int c = 0;cropFarmed_1NextPRE[c] != '\0' ;c++){if(cropFarmed_1NextPRE[c] != '\n')cropFarmed_1Next += cropFarmed_1NextPRE[c];
}
Serial.print(cropFarmed_1Next);


cropFarmed_2Next = "";Serial.println("\n*Enter Crop 2 :");
  String cropFarmed_2NextPRE = Serial.readStringUntil('\n');
for(int c = 0;cropFarmed_2NextPRE[c] != '\0' ;c++){if(cropFarmed_2NextPRE[c] != '\n')cropFarmed_2Next += cropFarmed_2NextPRE[c];
}
Serial.print(cropFarmed_2Next);

  Serial.println("\n*Enter P interval :");
  intervalNext = Serial.parseInt();
  Serial.print(intervalNext);

//storing Data received If anything changed to EEPROM
for(byte c = 0; cropFarmed_1Next[c] != '\0';c++){
  EEPROM.update(strCntr++,int(cropFarmed_1Next[c]));
}
EEPROM.update(strCntr++,'\0');

for(byte c = 0; cropFarmed_2Next[c] != '\0';c++){
  EEPROM.update(strCntr++,int(cropFarmed_2Next[c]));
}
EEPROM.update(strCntr++,'\0');
 
 EEPROM.update(strCntr,intervalNext);

if((sizeof(cropFarmed_1Next)>0)&&(sizeof(cropFarmed_2Next)>0)&&(intervalNext>0)){EEPROM.update(1023,42);}

Serial.println("\n*NEXT*");Serial.print(crptxt[2]);Serial.println(intervalNext);
Serial.print(crptxt[0]);Serial.println(cropFarmed_1Next);
Serial.print(crptxt[1]);Serial.print(cropFarmed_2Next);
Serial.read();Serial.readStringUntil('\n');

Serial.setTimeout(100);
}

//Device control Part through serial communitcation features include(reset - manual mode)
void ControlBoy(){Serial.read();
Serial.setTimeout(100000);
Serial.print("\n--rst:RESET  --mnl:MANUAL MODE  --auto:AUTO MODE");
String serialcomm = Serial.readStringUntil('\n');
if (serialcomm == "rst"){Serial.print("\nARE U SURE (y)??");
Serial.read();String d = Serial.readStringUntil('\n');if(d == "y")digitalWrite(3, LOW);}
if (serialcomm == "mnl"){manual = true;EEPROM.write(1022,1);}
if (serialcomm == "auto"){manual = false;EEPROM.write(1022,0);}
 
 Serial.setTimeout(100);
}

bool solenoid_1,solenoid_2,pump ;
void mnlControlTXT(){Serial.println(mnltxt[0]); 
Serial.print(crptxt[0]);Serial.print(cropFarmed_1);Serial.print(opttxt[0]);Serial.print(optimalWaterPerc_1);Serial.print(txt[1]);Serial.print(avgtxt[0]);Serial.print(avgRead1);Serial.println(txt[1]);
Serial.print(crptxt[1]);Serial.print(cropFarmed_2);Serial.print(opttxt[1]);Serial.print(optimalWaterPerc_2);Serial.print(txt[1]);Serial.print(avgtxt[1]);Serial.print(avgRead2);Serial.println(txt[1]);
Serial.print(mnlcntrltxt[0]);if(solenoid_1){Serial.print("ON");}else{Serial.print("OFF");}Serial.print(txt[0]);
Serial.print(mnlcntrltxt[1]);if(solenoid_2){Serial.println("ON");}else{Serial.println("OFF");}
;Serial.print(mnlcntrltxt[2]);if(pump){Serial.print("ON");}else{Serial.println("OFF");}

}

void mnlControlCMD(){Serial.setTimeout(60000);
Serial.println("TYPE (s1/s2/p):(1/0)");
String stateChooser = Serial.readStringUntil('\n');
if(stateChooser == "s1:1"){digitalWrite(valve_1, LOW);solenoid_1 = true;}if(stateChooser == "s1:0"){digitalWrite(valve_1, HIGH);solenoid_1 = false;}
if(stateChooser == "s2:1"){digitalWrite(valve_2, LOW);solenoid_2 = true;}if(stateChooser == "s2:0"){digitalWrite(valve_2, HIGH);solenoid_2 = false;}
if(stateChooser == "p:1"){digitalWrite(waterPump, LOW);pump = true;}if(stateChooser == "p:0"){digitalWrite(waterPump, HIGH);pump = false;}
Serial.setTimeout(100);

}

void loop() {
if(!Serial)manual = false;
if( !(chosen1 && chosen2))moistureChooser();
if(!manual){digitalWrite(mnlled, LOW);
moistureControl();
tankManager();}
if(manual){mnlControlTXT();digitalWrite(mnlled, HIGH);}
avgReadings();


if(Serial.available()>0){byte serialKeyRead = Serial.parseInt();
if(serialKeyRead == 1){Serial.read();serialReaderSaver();} 
else if(serialKeyRead == 2){Serial.read();ControlBoy();}
else if((serialKeyRead == 3)&& manual){Serial.read();mnlControlCMD();}
}

delay(500);
Serial.println();Serial.println("\nend");Serial.println("\n");

}
