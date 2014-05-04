#include <SoftwareSerial.h>

//#define DEBUG
//#define INITIAL_UPLOAD


////// connector transfer list /////
//
//// 8pins -- 4512
// c1 -- D7
// c2 -- D6
// c3 -- D5
// c4 -- D4
// c5 -- D3
// c6 -- D2
// c7 -- D1
// c8 -- D0
//
// 14pins -- 4514
// c1  -- O0
// c2  -- O2
// c3  -- O1
// c4  -- O3
// c5  -- O4
// c6  -- O5
// c7  -- O6
// c8  -- O7
// c9  -- O8
// c10 -- O13
// c11 -- O12
// c12 -- O15
// c13 -- O14
// c14 -- O9


////////////////
// Contraints //
////////////////

#define BT_RXPIN 2 // connect to TX on RN-42
#define BT_TXPIN 3 // connect to RX on RN-42
#define BT_BAUDRATE 9600
#define AR_BAUDRATE 9600

#define BT_RESETPIN 12 // RN-42 reset pin. active low.
#define BUTTERY_LEDPIN 13 // buttery status led pin.

#define BUTTERY_READPIN 0 // reading pin of buttery voltage.
#define VCC_VOLTAGE 5 // supply voltage
#define BUTTERY_THRESH_VOLTAGE 2 // buttery threshold voltage. Indicate LED.
#define BUTTERY_CHECKINTERVAL_MS 5000 // check interval in millis of buttery voltage.

#define COL 14
#define ROW 8
#define MOD 8

// To connect HEF4514B
#define COL_SELECTPIN0 13 // pin A0
#define COL_SELECTPIN1 11 // pin A1
#define COL_SELECTPIN2 10 // pin A2
#define COL_SELECTPIN3 12 // pin A3

// To connect CD4512BP
#define ROW_SELECTPIN0 5 // pin A
#define ROW_SELECTPIN1 4 // pin B
#define ROW_SELECTPIN2 3 // pin C
#define ROW_INPUTPIN   2 // input

// Function key(fn) position
#define FNKEY_POSITION_COL 1
#define FNKEY_POSITION_ROW 0

// witing time in ms
#define WITINGMS 15

const int BUTTERY_THRESHOLD = (int)(BUTTERY_THRESH_VOLTAGE * 1024 / VCC_VOLTAGE);

const int COL_SELECTPINS[4] = 
  {COL_SELECTPIN3,
   COL_SELECTPIN2,
   COL_SELECTPIN1,
   COL_SELECTPIN0};

const int ROW_SELECTPINS[3] = 
  {ROW_SELECTPIN2,
   ROW_SELECTPIN1, 
   ROW_SELECTPIN0};

const boolean COL_SWEAPBITS[COL][4] = 
  //  A3   A2   A1   A0
  {{ LOW, LOW, LOW, LOW},  // output O0
   { LOW, LOW,HIGH, LOW},  // output O2
   { LOW, LOW, LOW,HIGH},  // output O1
   { LOW, LOW,HIGH,HIGH},  // output O3
   { LOW,HIGH, LOW, LOW},  // output O4
   { LOW,HIGH, LOW,HIGH},  // output O5
   { LOW,HIGH,HIGH, LOW},  // output O6
   { LOW,HIGH,HIGH,HIGH},  // output O7
   {HIGH, LOW, LOW, LOW}}; // output O8
   {HIGH,HIGH, LOW,HIGH},  // output O13
   {HIGH,HIGH, LOW, LOW},  // output O12
   {HIGH,HIGH,HIGH,HIGH},  // output O15
   {HIGH,HIGH,HIGH, LOW},  // output O14
   {HIGH, LOW, LOW,HIGH},  // output O9

const boolean ROW_SWEAPBITS[ROW][3] =
  //   C    B    A
  {{HIGH,HIGH,HIGH},   // output D7
   {HIGH,HIGH, LOW},   // output D6
   {HIGH, LOW,HIGH},   // output D5
   {HIGH, LOW, LOW},   // output D4
   { LOW,HIGH,HIGH},   // output D3
   { LOW,HIGH, LOW},   // output D2
   { LOW, LOW,HIGH},   // output D1
   { LOW, LOW, LOW}};  // output D0

const byte KEYID_PLAIN[COL][ROW] = 
  {{0x00,0x36,0x00,0xe3,0xe7,0x4f,0x51,0x52},
   {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
   {0x38,0x00,0x34,0x33,0x2f,0x13,0x27,0x2d},
   {0x00,0x37,0x00,0x0f,0x50,0x12,0x26,0x00},
   {0x00,0x00,0x00,0x0e,0x30,0x0c,0x25,0x2e},
   {0x11,0x10,0x0b,0x0d,0x1c,0x18,0x24,0x23},
   {0x05,0x19,0x0a,0x09,0x17,0x15,0x21,0x22},
   {0x00,0x06,0x00,0x07,0x00,0x08,0x20,0x00},
   {0x00,0x1b,0x00,0x16,0x00,0x1a,0x1f,0x00},
   {0x00,0x1d,0x29,0x04,0x2b,0x14,0x1e,0x35},
   {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0},
   {0x00,0xe5,0x00,0x00,0xe1,0x00,0x00,0x00},
   {0xe6,0x00,0xe2,0x00,0x00,0x00,0x00,0x00},
   {0x2c,0x28,0x00,0x31,0x2a,0x00,0x00,0x00}};

const byte KEYID_FN[COL][ROW] = 
  {{0x00,0x4d,0x00,0xe3,0xe7,0x4d,0x4e,0x4b},
   {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
   {0x51,0x00,0x4f,0x50,0x52,0x48,0x43,0x44},
   {0x00,0x4e,0x00,0x4b,0x4a,0x47,0x42,0x00},
   {0x00,0x00,0x00,0x4a,0x30,0x46,0x41,0x45},
   {0x11,0x10,0x0b,0x0d,0x1c,0x18,0x40,0x3f},
   {0x05,0x19,0x0a,0x09,0x17,0x15,0x3d,0x3e},
   {0x00,0x06,0x00,0x07,0x00,0x08,0x3c,0x00},
   {0x00,0x1b,0x00,0x16,0x00,0x1a,0x3b,0x00},
   {0x00,0x1d,0x29,0x04,0x39,0x14,0x3a,0x35},
   {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0},
   {0x00,0xe5,0x00,0x00,0xe1,0x00,0x00,0x00},
   {0xe6,0x00,0xe2,0x00,0x00,0x00,0x00,0x00},
   {0x2c,0x28,0x00,0x49,0xbb,0x00,0x00,0x00}};

///////////////////////////
// Prototype declaration //
///////////////////////////

void sendRawReport();
boolean keyPressed(int,int);
byte getKeyCodeFromColAndRow(int,int,boolean);
boolean keyStateIsChanged(byte,byte[6]);
void sendReportHID();
void resetBT();
void indicateButteryAlert();


//////////////////////
// global valiables //
//////////////////////

SoftwareSerial softwareSerial(BT_RXPIN,BT_TXPIN);
byte previousKeyData[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
byte previousModifierData = 0x00;
byte previousKeySPP = 0x00;
boolean isBTMode = true;
unsigned long previousSendDate;
unsigned long previousButteryCheckDate;


///////////
// Setup //
///////////

void setup(){

  // set pins to output pin to use column multiplexer.
  pinMode(COL_SELECTPIN0,OUTPUT);
  pinMode(COL_SELECTPIN1,OUTPUT);
  pinMode(COL_SELECTPIN2,OUTPUT);
  pinMode(COL_SELECTPIN3,OUTPUT);

  // set pins to input or output pin to use row encoder.
  pinMode(ROW_SELECTPIN0,OUTPUT);
  pinMode(ROW_SELECTPIN1,OUTPUT);
  pinMode(ROW_SELECTPIN2,OUTPUT);
  pinMode(ROW_INPUTPIN,INPUT);

  // set pin as reset switch of RN-42.
  pinMode(BT_RESETPIN,OUTPUT);
  digitalWrite(BT_RESETPIN,HIGH);

  // set pin as buttery LED indicator.
  pinMode(BUTTERY_LEDPIN,OUTPUT);

  softwareSerial.begin(BT_BAUDRATE);
#ifdef DEBUG
  Serial.begin(AR_BAUDRATE);
#endif

#ifdef DEBUG
  Serial.flush();
#endif
  softwareSerial.flush();

  softwareSerial.print("$$$");
  delay(200);

#ifdef INITIAL_UPLOAD
  softwareSerial.flush();
  Serial.flush();
  softwareSerial.print("S~,6\r");
  delay(200);
  softwareSerial.print("SM,0\r");
  delay(200);
  softwareSerial.print("SA,1\r");
  delay(200);
  softwareSerial.print("SN,HHKBLite2BT\r");
  delay(200);
  softwareSerial.print("SH,0200\r");  
  delay(200);
  softwareSerial.print("SS,HID\r");
  delay(200);
  softwareSerial.print("SC,0000"); // COD is 0x00000540.
  delay(200);
  softwareSerial.print("SD,0540");
  delay(200);
  softwareSerial.print("SU,96\r");
  delay(200);
#endif
  
#ifdef DEBUG
  softwareSerial.flush();
  Serial.flush();
  softwareSerial.print("V\r");
  delay(200);
  while(softwareSerial.available()){
    Serial.write(softwareSerial.read());
  }
  delay(200);
  softwareSerial.print("D\r");
  delay(200);
  Serial.println("D");
  while(softwareSerial.available()){
    Serial.write(softwareSerial.read());
  }
  delay(200);
  softwareSerial.print("E\r");
  delay(200);
  Serial.println();
  Serial.println("E");
  while(softwareSerial.available()){
    Serial.write(softwareSerial.read());
  }
  delay(200);
  softwareSerial.println("GB\r");
  delay(200);
  Serial.println("GB");
  while(softwareSerial.available()){
    Serial.write(softwareSerial.read());
  }
  delay(200);
  softwareSerial.println("GF\r");
  delay(200);
  Serial.println("GF");
  while(softwareSerial.available()){
    Serial.write(softwareSerial.read());
  }
  delay(200);
  softwareSerial.println("GK\r");
  delay(200);
  Serial.println("GK");
  while(softwareSerial.available()){
    Serial.write(softwareSerial.read());
  }
  delay(200);
  softwareSerial.println("GR\r");
  delay(200);
  Serial.println("GR");
  while(softwareSerial.available()){
    Serial.write(softwareSerial.read());
  }
  delay(200);
  softwareSerial.println("G&\r");
  delay(200);
  Serial.println("G&");
  while(softwareSerial.available()){
    Serial.write(softwareSerial.read());
  }
  delay(200);
#endif

#if defined INITIAL_UPLOAD || defined DEBUG
  softwareSerial.print("R,1\r");
  delay(1000);
#endif
  
#ifdef DEBUG
  Serial.println("initial setting is finished.");
#endif
  
  // After wakeup, connect to stored remote address, and 
  // exit command mode.
  // Perhaps, exit command must not exist.
  softwareSerial.print("C\r");
  //delay(200);
  //softwareSerial.print("---\r");
  
  previousSendDate = millis();
  previousButteryCheckDate = millis();

}



//////////
// Loop //
//////////

void loop(){
  while(millis() - previousSendDate < WITINGMS){
    delayMicroseconds(100);
  }
  // even if timer is reset, this condition tree is correct.
  // (little faster than WITINGMS...)
  
  previousSendDate = millis();
  
#ifdef DEBUG
  Serial.flush();
#endif
  
  softwareSerial.flush();
  sendReportHID();
  
  // buttery check.
  if(millis() - previousButteryCheckDate > BUTTERY_CHECKINTERVAL_MS){
    indicateButteryAlert();
    previousButteryCheckDate = millis();
  }
}



/////////////////////
// Other functions //
/////////////////////

// This function return true if key(col,row) is pressed.
// In notice, when key sweaping, if col selector is locked, 
// it can sweap speedy.
// It's treading off of speed and readable.
boolean keyPressed(int col, int row){
  // set selector bits.
  for(int i=0;i<4;i++){
    digitalWrite(COL_SELECTPINS[i],COL_SWEAPBITS[col][i]);
  }
  for(int i=0;i<3;i++){
    digitalWrite(ROW_SELECTPINS[i],ROW_SWEAPBITS[row][i]);
  }
  
  if(digitalRead(ROW_INPUTPIN)==HIGH){
    return true;
  }else{
    return false;
  }
}


// This function return key code from column and row number.
// Fnkey switches key matrix.
byte getKeyCodeFromColAndRow(int col, int row, boolean fnEnable){
  if(fnEnable){
    return KEYID_FN[col][row];
  }else{
    return KEYID_PLAIN[col][row];
  }
}


// This function return true if key state is changed.
boolean keyStateIsChanged(byte modifierData, byte keyData[6]){
  // check modifier.
  if(modifierData != previousModifierData){
    return true;
  }
  for(int i=0;i<6;i++){
    if(keyData[i] != previousKeyData[i]){
      return true;
    }
  }
  return false;
}


// This function sweap and send key report in HID mode.
void sendReportHID(){
  boolean fnEnable = false;
  int keyCounter = 0;
  byte modifierData = 0;
  byte keyData[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
  byte selectedKeyID = 0x00;


  // for debug.
#ifdef DEBUG
  int keyDataCol[6] = {0,0,0,0,0,0};
  int keyDataRow[6] = {0,0,0,0,0,0};
#endif

  // get fn key state.
  fnEnable = keyPressed(FNKEY_POSITION_COL,FNKEY_POSITION_ROW);
  
  // sewp key state.
  for(int i=0;i<COL;i++){
    for(int j=0;j<ROW;j++){
      // If key does not pressed, or pressed key is fnkey, go to next.
      if((keyPressed(i,j)) &&
	 ((i != FNKEY_POSITION_COL) || (j != FNKEY_POSITION_ROW))){
	
	// set key ID.
	selectedKeyID = getKeyCodeFromColAndRow(i,j,fnEnable);
	
	// If pressed key is modifier, set suitable bit. Then, go to next.
	// If not, and counter is less than 6, store key ID to keyData array.
	if((selectedKeyID>=224) && (selectedKeyID<=231)){
	  modifierData += 1<<(selectedKeyID-224);
	}else if(keyCounter < 6){
	  keyData[keyCounter] = selectedKeyID;
#ifdef DEBUG
	  keyDataCol[keyCounter] = i;
	  keyDataRow[keyCounter] = j;
#endif
	  keyCounter++;
	}
      }
    }
  }

  // If key state is changed, send data. Then, store key status to previous array.
  if(keyStateIsChanged(modifierData,keyData)){
    softwareSerial.write(0xfd);
    softwareSerial.write(0x09);
    softwareSerial.write(0x01);
    softwareSerial.write(modifierData);
    softwareSerial.write(0x00,1);
    for(int i=0;i<6;i++){
      softwareSerial.write(keyData[i]);
    }
    
    for(int i=0;i<6;i++){
      previousKeyData[i] = keyData[i];
    }
    previousModifierData = modifierData;

#ifdef DEBUG
    Serial.print("modifier: ");
    Serial.println(modifierData,BIN);
    for(int i=0;i<6;i++){
      Serial.print("key: ");
      Serial.print(keyDataCol[i]);
      Serial.print(",");
      Serial.println(keyDataRow[i]);
    }
#endif
  }
}


// This function is reset switch of Bluetooth module.
void resetBT(){
  digitalWrite(BT_RESETPIN,LOW);
  delay(5);
  digitalWrite(BT_RESETPIN,HIGH);
}


// This function checks buttery conditions.
// If buttery  voltage is low, indicate alert LED.
void indicateButteryAlert(){
  int voltage = analogRead(BUTTERY_READPIN);
  if(voltage < BUTTERY_THRESHOLD){
    digitalWrite(BUTTERY_LEDPIN,HIGH);
  }
}