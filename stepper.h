#define DELAY_STEP 500
#define MAX_STEPS 3000
#define DISPLAY_TIME 1500

String  vendingMsg ;
bool vendingState,sendTelnet;
bool A_State,B_State,C_State,D_State;
char C;
unsigned int index,motorSteps;
unsigned long time,displayT;
enum MACHAINE_STAGE
{
  START,
  VENDING_STAGE1,
  VINDING_STAGE2,
  PRE_END,
  END,
} machine_stage = START;//


void initSteppres(){
  pinMode(A_STEP_PIN,OUTPUT); 
  pinMode(A_DIR_PIN,OUTPUT);
  pinMode(A_ENA_PIN,OUTPUT);
  pinMode(A_SENSOR,INPUT);

  pinMode(B_STEP_PIN,OUTPUT); 
  pinMode(B_DIR_PIN,OUTPUT);
  pinMode(B_ENA_PIN,OUTPUT);
  pinMode(B_SENSOR,INPUT);

  pinMode(C_STEP_PIN,OUTPUT); 
  pinMode(C_DIR_PIN,OUTPUT);
  pinMode(C_ENA_PIN,OUTPUT);
  pinMode(C_SENSOR,INPUT);

  pinMode(D_STEP_PIN,OUTPUT); 
  pinMode(D_DIR_PIN,OUTPUT);
  pinMode(D_ENA_PIN,OUTPUT);
  pinMode(D_SENSOR,INPUT);
  
  digitalWrite(A_ENA_PIN,HIGH);
  digitalWrite(A_DIR_PIN,HIGH);

  digitalWrite(B_ENA_PIN,HIGH);
  digitalWrite(B_DIR_PIN,HIGH);

  digitalWrite(C_ENA_PIN,HIGH);
  digitalWrite(C_DIR_PIN,HIGH);

  digitalWrite(D_ENA_PIN,HIGH);
  digitalWrite(D_DIR_PIN,HIGH);

  index=0;
  vendingState=false;
  sendTelnet=false;
}
void stopMotors(){
   digitalWrite(A_ENA_PIN,HIGH); digitalWrite(B_ENA_PIN,HIGH);
   digitalWrite(C_ENA_PIN,HIGH);  digitalWrite(D_ENA_PIN,HIGH);
   }
bool readSensor(char c){
    switch (c) 
  {
   case 'A':
      A_State=digitalRead(A_SENSOR);
      return A_State ;        
   break;
   case 'B':
     B_State=digitalRead(B_SENSOR);
     return B_State ;
   break;
   case 'C':
     C_State=digitalRead(C_SENSOR);
     return C_State ;
   break;
   case 'D': 
     D_State=digitalRead(D_SENSOR);
     return D_State ;
   break;
  }

}
void  readAllSensos(){
  A_State=digitalRead(A_SENSOR);
  B_State=digitalRead(B_SENSOR);
  C_State=digitalRead(C_SENSOR);
  D_State=digitalRead(D_SENSOR);
}
void debugSensors(){
  readAllSensos();
  Serial.print("A=");Serial.print(A_State); Serial.print("   ");
  Serial.print("B=");Serial.print(B_State); Serial.print("   ");
  Serial.print("C=");Serial.print(C_State); Serial.print("   ");
  Serial.print("D=");Serial.print(D_State); Serial.println("");                 

}
void testMotors(){
  
    digitalWrite(A_ENA_PIN,LOW);
    for(int x = 0; x < 800; x++) {
    digitalWrite(A_STEP_PIN,HIGH); 
    delayMicroseconds(DELAY_STEP); 
    digitalWrite(A_STEP_PIN,LOW); 
    delayMicroseconds(DELAY_STEP); 
  }
  digitalWrite(A_ENA_PIN,HIGH);
  delay(1000); // One second delay
   
  ////////////////////////////////////////////////////////
    digitalWrite(B_ENA_PIN,LOW);
    for(int x = 0; x < 800; x++) {
    digitalWrite(B_STEP_PIN,HIGH); 
    delayMicroseconds(DELAY_STEP); 
    digitalWrite(B_STEP_PIN,LOW); 
    delayMicroseconds(DELAY_STEP); 
  }
  digitalWrite(B_ENA_PIN,HIGH);
  delay(1000); // One second delay

    ////////////////////////////////////////////////////////////
    digitalWrite(C_ENA_PIN,LOW);
    for(int x = 0; x < 800; x++) {
    digitalWrite(C_STEP_PIN,HIGH); 
    delayMicroseconds(DELAY_STEP); 
    digitalWrite(C_STEP_PIN,LOW); 
    delayMicroseconds(DELAY_STEP); 
  }
  digitalWrite(C_ENA_PIN,HIGH);
  delay(1000); // One second delay
   
      ////////////////////////////////////////////////////////////
    digitalWrite(D_ENA_PIN,LOW);
    for(int x = 0; x < 800; x++) {
      digitalWrite(D_STEP_PIN,HIGH); 
      delayMicroseconds(DELAY_STEP); 
      digitalWrite(D_STEP_PIN,LOW); 
      delayMicroseconds(DELAY_STEP); 
  }
  digitalWrite(D_ENA_PIN,HIGH);
  delay(1000); // One second delay
   
}

void runMotor(char c){
    switch (c) 
  {
   case 'A':
    digitalWrite(A_ENA_PIN,LOW); digitalWrite(B_ENA_PIN,HIGH); digitalWrite(C_ENA_PIN,HIGH);  digitalWrite(D_ENA_PIN,HIGH);
    if(micros()-time>=DELAY_STEP) { digitalWrite(A_STEP_PIN,!digitalRead(A_STEP_PIN)); motorSteps++; time=micros();}
   break;
   case 'B':
    digitalWrite(A_ENA_PIN,HIGH); digitalWrite(B_ENA_PIN,LOW); digitalWrite(C_ENA_PIN,HIGH);  digitalWrite(D_ENA_PIN,HIGH);
    if(micros()-time>=DELAY_STEP) { digitalWrite(B_STEP_PIN,!digitalRead(B_STEP_PIN)); motorSteps++; time=micros();}
   break;
   case 'C':
     digitalWrite(A_ENA_PIN,HIGH); digitalWrite(B_ENA_PIN,HIGH); digitalWrite(C_ENA_PIN,LOW);  digitalWrite(D_ENA_PIN,HIGH);
     if(micros()-time>=DELAY_STEP) { digitalWrite(C_STEP_PIN,!digitalRead(C_STEP_PIN)); motorSteps++;  time=micros();}
   break;
   case 'D':
     digitalWrite(A_ENA_PIN,HIGH); digitalWrite(B_ENA_PIN,HIGH); digitalWrite(C_ENA_PIN,HIGH);  digitalWrite(D_ENA_PIN,LOW);
     if(micros()-time>=DELAY_STEP) { digitalWrite(D_STEP_PIN,!digitalRead(D_STEP_PIN)); motorSteps++;  time=micros();}
   break;

  }
  
}
               
void bringMedicine(){
 
//bool A_State,B_State,C_State,D_State;
  
  switch (machine_stage) 
  {
  case START:
   Serial.print("Vending msg length="); Serial.println(vendingMsg.length());
   Serial.println("Start");
   index=0;
   C=vendingMsg.charAt(index);
   displayVendingMsg(C);
   machine_stage=VENDING_STAGE1;
   motorSteps=0;
   Serial.println("Stage1");
   time=micros();
  break;
  case VENDING_STAGE1: 
   runMotor(C);
   if(!readSensor(C)) {machine_stage=VINDING_STAGE2; motorSteps=0; time=micros(); 
   Serial.println("Stage2");
   Serial.print("motorSteps="); Serial.println(motorSteps);
   } 
  break;
  case VINDING_STAGE2:
   runMotor(C);
   if(motorSteps>=MAX_STEPS){
     Serial.print("motorSteps="); Serial.println(motorSteps);
     if(index==(vendingMsg.length()-1) ){ machine_stage=PRE_END;  displayMsg(0,30,2,"Thank You."); displayT=millis();}
     else{index++; motorSteps=0; C=vendingMsg.charAt(index); 
      displayVendingMsg(C);  machine_stage=VENDING_STAGE1;  time=micros();}
   }
  break;
  case PRE_END:
   stopMotors();
   if(millis()-displayT>DISPLAY_TIME ){sendTelnet=true; machine_stage=END; }
   
  break;
  case END:
   Serial.println("Stage End");
   stopMotors();
   vendingMsg="";
   sendTelnet=false;
   vendingState=false;
  
  
   displayReady();
  break;
  }

}