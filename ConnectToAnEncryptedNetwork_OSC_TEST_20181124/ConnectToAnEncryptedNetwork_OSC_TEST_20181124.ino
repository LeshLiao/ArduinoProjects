/*   ConnectToAnEncryptedNetwork.ino
*
*    This example is useful to show you how to connect to a secured (WPA/WPA2)
*    wifi network.
*    The only two things you have to supply to the sketch are the SSID (name) and
*    the password of the network you want to connect to.
*
*    To see the behaviour of this example you have to ensure that the network you
*    want to connect to is "reachable", up and running. Then upload
*    this sketch on your board and open a serial monitor to read the output
*    informations supplied.
*/

#include <WiFiConn.h>

#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

char ssid[] = "Life_Is_Good";     //  your network SSID (name)
char pass[] = "djlneqxo";  // your network password

WiFiUDP Udp_Test;                                // A UDP instance to let us send and receive packets over UDP
const unsigned int localPort = 8888;        // local port to listen for OSC packets (actually not used for sending)

const IPAddress outIp(10,1,1,2);        // remote IP of your computer
const unsigned int outPort = 2346;          // remote port to receive OSC
OSCErrorCode error;
unsigned int ledState = LOW;              // LOW means led is *on*

void setup() {
  Serial.begin(115200);

  
	//  begin ESP8266 chip: these functions perform the chip reset and
	//  initialization to ensure that the communication between ESP8266
	//  and the main mcu starts in a known fashion.

  
  //WiFi.reset();
  //WiFi.init(AP_STA_MODE);
  /* LeoTest
  if (WiFi.status() == WL_NO_WIFI_MODULE_COMM) {
    
       //notify the user if the previous sequence of operations to establish
       //the communication between the ESP and the main mcu fail.
    
    Serial.println("Communication with WiFi module not established.");
  }
  else{
    
       //otherwise we have a correct communication between the two chips
       //and we can connect to a preferred network
    
    Serial.println("\nWiFi module linked!");
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    
    
       //this is one of the way (suppling SSID and PWD) that can be used to
       //establish a connection to a network - other ways are showed in other
       //examples, please check them out.
    
    if(WiFi.begin(ssid, pass) != WL_CONNECTED){
      if(WiFi.status()!= WL_CONNECTED){
        
           //if the connection fails due to an error...
        
        Serial.println("Connection error! Check ssid and password and try again.");
      }
    }
    else{
      
         //you're now connected, so print out the network data
      
      Serial.print("You're connected to the ");
      printCurrentNet();
      printWifiData();
    }
  }
	*/ //LeoTest
  
  
    Udp_Test.begin(localPort);
    //Serial.print("Local port: ");
   // Serial.println(Udp_Test.localPort);
}

void led(OSCMessage &msg) {
  ledState = msg.getInt(0);
  //digitalWrite(BUILTIN_LED, ledState);
  Serial.print("/int: ");
  Serial.println(ledState);
}

void loop() {
  /*
     each two seconds print out the network data.
  */
  delay(2000);
  Serial.println("Test 001:");
  
  printCurrentNet();
  printWifiData();

  //send_OSC_Message();

  get_OSC_Message();
}

void printMacAddress(void)
{
  // the MAC address of your esp module
  byte mac[6];

  // print your MAC address:
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);
}

void printEncryptionType(int thisType) {
  // read the encryption type and print out the name:
  switch (thisType) {
    case ENC_TYPE_WEP:
    Serial.println("WEP");
    break;
    case ENC_TYPE_TKIP:
    Serial.println("WPA");
    break;
    case ENC_TYPE_CCMP:
    Serial.println("WPA2");
    break;
    case ENC_TYPE_NONE:
    Serial.println("None");
    break;
    case ENC_TYPE_AUTO:
    Serial.println("Auto");
    break;
    case ENC_TYPE_UNKNOW:
    Serial.println("Unknown");
    break;
  }
}

/*
  the BSSID is the MAC address of the wireless access point (WAP)
  generated by combining the 24 bit Organization Unique Identifier
  (the manufacturer's identity) and the manufacturer's assigned
  24-bit identifier for the radio chipset in the WAP.
*/
void printBSSID(){
  uint8_t bssid[6];
  WiFi.BSSID(bssid);
  
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);
  
}

void printWifiData()
{
 /*
     print out the IP Address the gateway assigned to
     this board during the connection precedure and print
     the ESP MAC Address and the gateway BSSID.
  */
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  printMacAddress();
  printBSSID();
  
  Serial.println("");
}

void printCurrentNet()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void send_OSC_Message()
{
    OSCMessage msg("/PitchAndVelocity");
    //msg.add("hello, osc.");
    msg.add(38);
    msg.add(39);
    Udp_Test.beginPacket(outIp, outPort);
    msg.send(Udp_Test);
    Udp_Test.endPacket();
    msg.empty();

}

void get_OSC_Message()
{
  OSCBundle bundle;
  int size = Udp_Test.parsePacket();
  Serial.println("get_OSC_Message()");
  if (size > 0) {
    Serial.println("size > 0");
    while (size--) {
      bundle.fill(Udp_Test.read());
    }
    if (!bundle.hasError()) {
      bundle.dispatch("/led", led);
      Serial.println("bundle.dispatch");
    } else {
      error = bundle.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
}