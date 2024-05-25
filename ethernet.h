#define RECONNECT_DELAY 5000 

#include <SPI.h>
#include <Ethernet.h>
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
bool socketState;
unsigned long reconnectTime;
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 101);

// Enter the IP address of the server you're connecting to:
IPAddress server(192, 168, 1, 100);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 23 is default for telnet;
// if you're using Processing's ChatServer, use port 10002):
EthernetClient client;

void initTelnet(){
  socketState=false;
  Ethernet.begin(mac, ip);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

    // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  while (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
    delay(500);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 10002)) {
    Serial.println("connected");
    socketState=true;
  } else {
    // if you didn't get a connection to the server:
    socketState=false;
    Serial.println("connection failed");
  }
}

String reConnect(){
  String msg;
    // if you get a connection, report back via serial:
  if (client.connect(server, 10002)) {
    Serial.println("connected");
    msg="Connected";
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
    msg="Connecting...";
  }
  return msg;
}
void SendTelnet(String msg){
  if (client.connected()) {
      client.print(msg);
    }

}

