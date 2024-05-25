
#include"ethernet.h"
#include"GPIO.h"
#include "OLED.h"
#include"stepper.h"

#include"QRreader.h"


void setup()   {                
  Serial.begin(9600);
  initTelnet();
  initSteppres();
  initQRReader();
  initOLED();
  displayReady();
  //delay(2000);
 // displayBitmapTest();
 Serial.println("Vendeng Machine Ready");
  reconnectTime=millis();
}       
void loop(){
  
   if((millis()- reconnectTime > RECONNECT_DELAY)/* && !socketState*/){
     reconnectTime=millis();
      String connectState=reConnect();
      if(connectState=="Connecting..."){displayMsg(0,30,2,connectState);}
      else{socketState=true; displayReady();}
   }
   if(!vendingState){readQR();}
   if(vendingState){bringMedicine();
   if(sendTelnet){Serial.print("Send NO:");Serial.println(prescriptionNO); SendTelnet(prescriptionNO);QRCamera.flush() ;}
   }
     //debugSensors();
     //testMotors();
}

