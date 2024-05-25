#include <SoftwareSerial.h>
#include <DFRobot_GM60.h>

DFRobot_GM60_UART  gm60;

SoftwareSerial QRCamera(QR_RX, QR_TX);  //RX, TX
#define FPSerial QRCamera
String QRcontent,prescription,prescriptionNO;

void initQRReader(){
    //Rx and tx should be mapping into D2 and D3 when using Firebeetle-ESP32
  prescription=vendingMsg="";
  QRCamera.begin(9600);
  
  
  //Init chip
  gm60.begin(QRCamera);
  //Restore to factory settings
 // gm60.reset();

  /**
  Read the data encoding mode
  encode Encoding mode
           eGBK,//gbk encoding
           eUTF8,//utf8 encoding
   */
  gm60.encode(/*encode = */gm60.eUTF8);
  
  /*! Set code config, the value of chip register can be changed by using the module to scan QR code
  on true (Enable setting code)/false (Disable setting code)
  content true (Output the set code content)/false (Not output the set code content)
  */
  gm60.setupCode(/*on =*/true,/*content=*/true);
  
  /**
    Set the available types of QR code for recognition
    berCode:
    eForbidAllBarcode, //Disable all the QR code recognition
    eEnableAllBarcode, // Enable all the QR code recognition
    eEnableDefaultcode, // Enable the default QR code recognition
  */
  gm60.setIdentify(/*berCode = */gm60.eEnableAllBarcode);
}
String findVendingMsg(String presc){
  String msg=presc;
  for(int i=0 ;i<4;i++){
    if(msg.indexOf('-')!=-1){
      msg.remove(msg.indexOf('-'), 1);   
    }
  }
  return msg;
}
bool readPrescription(String content){
  bool state=false;
  vendingMsg=""; 
  if(content.length()<2){state=false; }
  else if(content.indexOf('@')==-1){state=false;}
  else if (content.indexOf('@')!=-1){ state=true;
    prescriptionNO=content.substring(0,content.indexOf('@'));
    prescription=content.substring(content.indexOf('@')+1,content.indexOf('#'));
    Serial.print("prescriptionNO:"); Serial.println(prescriptionNO);
    Serial.print("Prescription:"); Serial.println(prescription);
    }
  

   return state;
}
void readQR(){
    if(QRCamera.available()>0  && !vendingState){
    QRcontent=gm60.detection();
    Serial.println(QRcontent);
    if(readPrescription(QRcontent)){
      vendingMsg=findVendingMsg(prescription);
      Serial.print("Vending Msg:"); Serial.println(vendingMsg);
      vendingState=true; machine_stage=START;
      }
  }
}