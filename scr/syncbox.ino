 #define UDP_Rx_Size 512 //the size of the receive (Rx) UDP array that we
                               //use to store incoming data.
                               

int button = D2;                      // button is connected to D0
int LED = D7;                         // LED is connected to D1
unsigned int localPort = 12354;
char buffer[] = {0xA, 0xD, 0xA, 0xC, 0x0, 0xC, 0xD, 0xF};
UDP Udp;
IPAddress broadcast(255, 255, 255, 255);
int c = 0;
int d = 0;
const int numerSend = 3;
bool sendSync = true;
bool onByUDP = false;

static IPAddress senderIPAddress; //the IP address of the sender, from the last received data packet
static uint8_t UDP_Rx[UDP_Rx_Size]; //the UDP buffer for incoming data
static int senderPort; //the port of the sender, from the last received data packet


SYSTEM_MODE(MANUAL); //manual mode means the Photon does NOT automatically connect to the cloud, and we have to establish connnectivity and maintain it with calls to Particle.process(). We

void setup()
{
    WifiSetup();
  pinMode(LED, OUTPUT);               // sets pin as output
  pinMode(button, INPUT_PULLDOWN );    // sets pin as input
  Udp.begin(localPort);
}

void loop()
{
  // blink the LED as long as the button is pressed
  if (!onByUDP){
      if(digitalRead(button) == HIGH) {
          d = 0;
        digitalWrite(LED, HIGH);          // sets the LED on
        if (sendSync){
            buffer[4] = 0x1;
            if (Udp.sendPacket(buffer, 7, broadcast, localPort) < 0) {
               Particle.publish("Error");
               digitalWrite(LED, LOW); 
            }
            c++;
            if (c>=numerSend){
                sendSync=false;
                c = 0;
            }
        }
      }else{
        
        //sendSync = true;
        if (!sendSync){
            buffer[4] = 0x0;
            if (Udp.sendPacket(buffer, 7, broadcast, localPort) < 0) {
               Particle.publish("Error");
               digitalWrite(LED, LOW); 
            }
            d++;
            if (d>=numerSend){
                sendSync=true;
                d = 0;
            }
        }
        c = 0;
        digitalWrite(LED, LOW);           // sets the LED off
        
      }
  }
  checkForReceivedUDPData();
}
/*
void WifiSetup() {

  IPAddress myAddress(192,168,0,125);
  IPAddress netmask(255,255,255,0);
  IPAddress gateway(192,168,0,1);
  IPAddress dns(192,168,0,1);
  WiFi.setStaticIP(myAddress, netmask, gateway, dns);

  // now let's use the configured IP
  WiFi.useStaticIP();
  WiFi.connect(); //use stored credentials (loaded onto the Particle Photon) to try to connect to WiFi
  while(!WiFi.ready()){;} //block until this Photon has been assigned an IP address. We may need to do this on a Local Area Network.
}*/

void WifiSetup(/* arguments */) {
  int lasDir;
  // if(IAMLEFT==1){
  //   lasDir = 102;
  // } else {
  //   lasDir = 103;
  // }
  // IPAddress myAddress(192,168,0,lasDir);
  // IPAddress netmask(255,255,255,0);
  // IPAddress gateway(192,168,0,1);
  // IPAddress dns(192,168,0,1);
  // WiFi.setStaticIP(myAddress, netmask, gateway, dns);
  //
  // // now let's use the configured IP
  // WiFi.useStaticIP();
  WiFi.useDynamicIP();
  WiFi.connect(); //use stored credentials (loaded onto the Particle Photon) to try to connect to WiFi
  while(!WiFi.ready()){;} //block until this Photon has been assigned an IP address. We may need to do this on a Local Area Network.
}

void checkForReceivedUDPData() {


  // Check if UDP data has been received
  uint16_t receivedPacketSize = Udp.parsePacket(); // prepares the packet to be read, and returns the size of the packet

  if (receivedPacketSize > 0) { //if we do have UDP data waiting to be read
    //respond to received UDP data
                //digitalWrite(LED, HIGH);

    readIncomingUDPData(receivedPacketSize); //read incoming UDP data from buffer
    interpretReceivedUDPData(receivedPacketSize); //take any necessary action from incoming UDP data
  }
}

/*

Function: readIncomingUDPData()

Parameters: none

Returns: none

Description: Reads UDP data sent from the master shoe Photon. Stores the data in
the UDP_Rx buffer.

Notes:

*/
void readIncomingUDPData(uint16_t receivedPacketSize){

  // Store sender ip and port
  senderIPAddress = Udp.remoteIP();
  senderPort = Udp.remotePort();

  // Read in all waiting data from the UDP buffer
  while(Udp.available()) { //if there is still data in the Rx UDP buffer
    Udp.read(UDP_Rx, receivedPacketSize); //read a character into the Rx buffer
  }

} //end readIncomingUDPData()



/*

Function: interpretReceivedUDPData()

Parameters: none

Returns: none

Description: Responds to incoming UDP data

Notes:

*/
void interpretReceivedUDPData(uint16_t receivedPacketSize){
    
    if((UDP_Rx[0]==0xA) & (UDP_Rx[1]==0xD) & (UDP_Rx[2]==0xA) & (UDP_Rx[3]==0xC)){ //if this UDP data packet is from the "master" microcontroller or PC
        digitalWrite(LED, UDP_Rx[4]==1);
        onByUDP = UDP_Rx[4]==1;
        //digitalWrite(LED, HIGH);
    } 
}

