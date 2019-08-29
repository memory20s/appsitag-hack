#include <SPI.h>
#include <WizFi250.h>

int motorPin = A5;

int getInt(String input);

#define VARID      "imUOKArAFI5UlxTv7kNhT8Q6sntYeZS6EJlBESTn1AkT3H5Yf1lgLasblaComiQ222D3FICbsm97Ba%2FodvhxfA%3D%3D" //APIKEY

char ssid[] = "white";       // your network SSID (name)
char pass[] = "7777777487";        // your network password
int status = WL_IDLE_STATUS;       // the Wifi radio's status

char server[] = "apis.data.go.kr";

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 3000L; // delay between updates, in milliseconds

boolean readingVal;
boolean getIsConnected = false;
//String valString;
int val, temp;
float tempVal;//no use
int risk;
int riskfood; // 3
int riskeye; // 2
int riskcold; // 1
int riskcold2; // 4
String rcvbuf;
int dissCd = 0;
// Initialize the Ethernet client object
WiFiClient client;

void httpRequest();
void printWifiStatus();

void setup()
{
//
  pinMode(7, INPUT_PULLUP);//청색
  pinMode(6, INPUT_PULLUP);//녹색
  pinMode(5, INPUT_PULLUP);//황색
  pinMode(8, INPUT_PULLUP);//적색 디지털3,4번고장인듯
  pinMode(A0, OUTPUT);//청색
  pinMode(A1, OUTPUT);//녹색
  pinMode(A2, OUTPUT);//황색
  pinMode(A3, OUTPUT);//적색
  pinMode(motorPin,OUTPUT);//모터
  
//
  // initialize serial for debugging
  Serial.begin(115200);
  Serial.println(F("\r\nSerial Init"));

  WiFi.init();

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // check firmware version
    Serial.print(F("Firmware version: "));
    Serial.println(WiFi.firmwareVersion());
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");

  printWifiStatus();

}

void loop() {
  // if there's incoming data from the net connection send it out the serial port
  // this is for debugging purposes only
  String valString;
  while (client.available()) {

    if ( rcvbuf.endsWith("\"risk\" : ")) {
      readingVal = true;
      valString = "";
    }

    char c = client.read();

    if ( c != NULL ) {
      if (rcvbuf.length() > 30)
        rcvbuf = "";
      rcvbuf += c;
      //Serial.write(c);
    }

    if (readingVal) {
      if (c != ',' ) {
        valString += c;
        //Serial.write(c);
      }
      else {
        readingVal = false;
        //Serial.println(valString);
        riskcold = valString.toInt();
        Serial.println(riskcold);
      }
    }
  }
  
  rcvbuf = "";
  httpRequest();

  while (client.available()) {

    if ( rcvbuf.endsWith("\"risk\" : ")) {
      readingVal = true;
      valString = "";
    }

    char c = client.read();

    if ( c != NULL ) {
      if (rcvbuf.length() > 30)
        rcvbuf = "";
      rcvbuf += c;
      //Serial.write(c);
    }

    if (readingVal) {
      if (c != ',' ) {
        valString += c;
        //Serial.write(c);
      }
      else {
        readingVal = false;
        //Serial.println(valString);
        riskeye = valString.toInt();
        Serial.println(riskeye);
      }
    }
  }


  rcvbuf = "";
  httpRequest();

  while (client.available()) {

    if ( rcvbuf.endsWith("\"risk\" : ")) {
      readingVal = true;
      valString = "";
    }

    char c = client.read();

    if ( c != NULL ) {
      if (rcvbuf.length() > 30)
        rcvbuf = "";
      rcvbuf += c;
      //Serial.write(c);
    }

    if (readingVal) {
      if (c != ',' ) {
        valString += c;
        //Serial.write(c);
      }
      else {
        readingVal = false;
        //Serial.println(valString);
        riskfood = valString.toInt();
        Serial.println(riskfood);
      }
    }
  }
  
  rcvbuf = "";
  httpRequest();

  while (client.available()) {

    if ( rcvbuf.endsWith("\"risk\" : ")) {
      readingVal = true;
      valString = "";
    }

    char c = client.read();

    if ( c != NULL ) {
      if (rcvbuf.length() > 30)
        rcvbuf = "";
      rcvbuf += c;
      //Serial.write(c);
    }

    if (readingVal) {
      if (c != ',' ) {
        valString += c;
        //Serial.write(c);
      }
      else {
        readingVal = false;
        //Serial.println(valString);
        riskcold2 = valString.toInt();
        Serial.println(riskcold2);
      }
    }
  }
  
  if (millis() - lastConnectionTime > postingInterval) {
    
    if (getIsConnected) {
      Serial.println(valString);
      Serial.println(F("==================="));
      Serial.print(F("risk : "));
      Serial.println(risk);
      Serial.println(F("==================="));
    }
    httpRequest();
  }
  rcvbuf = "";

  if ( (riskcold==1) && (riskcold2==1) && (riskeye==1) && (riskfood==1) ) {
    risk = 1;
  }
  int leveltwo = 0;
  if ( (riskcold==2) ) {
    leveltwo++;
  }
  if ( (riskcold2==2) ) {
    leveltwo++;
  }
  if ( (riskeye==2) ) {
    leveltwo++;
  }
  if ( (riskfood==2) ) {
    leveltwo++;
  }

  if (leveltwo < 3) {
    risk = 2;
  } else {
    risk = 3;
  }
  leveltwo = 0;
  
  if ( (riskcold==4) || (riskcold2==4) || (riskeye==4) || (riskfood==4) ) {
    risk = 4;
  }

  if ( (riskcold==3) || (riskcold2==3) || (riskeye==3) || (riskfood==3) ) {
    if (risk == 4) {
      risk = 4;
    }
    else {
      risk = 3;
    }
  }
  
  if(risk == 1) { //청색
    Serial.println("BLUE ON");
    digitalWrite(A0, HIGH);
    while(true) {
      if(digitalRead(7) == LOW) {
        Serial.println("CLICK BLUE");
        digitalWrite(A0, LOW);
        analogWrite(motorPin, 170);
        delay(100);
        analogWrite(motorPin, 0);
        delay(700);
        digitalWrite(A0, HIGH);
      }
    }
  }
  else if(risk == 2) { // 녹색
    Serial.println("GREEN ON");
    digitalWrite(A1, HIGH);
    while(true) {
      if(digitalRead(6) == LOW) {
        Serial.println("CLICK GREEN");
        digitalWrite(A1, LOW);
        analogWrite(motorPin, 170);
        delay(100);
        analogWrite(motorPin, 0);
        delay(700);
        digitalWrite(A1, HIGH);
      }
    }
  }
  else if(risk == 3) { // 황색
    Serial.println("YELLOW ON");
    digitalWrite(A2, HIGH);
    while(true) {
      
      if(digitalRead(5) == LOW) {
        Serial.println("CLICK YELLOW");
        digitalWrite(A2, LOW);
        analogWrite(motorPin, 170);
        delay(100);
        analogWrite(motorPin, 0);
        delay(700);
        digitalWrite(A2, HIGH);
      }
      
    }
  }
  else if(risk == 4) { // 적색
    Serial.println("RED ON");
    digitalWrite(A3, HIGH);
    while(true) {
      if(digitalRead(8) == LOW) {
        Serial.println("CLICK RED");
        digitalWrite(A3, LOW);
        analogWrite(motorPin, 200);
        delay(100);
        analogWrite(motorPin, 0);
        delay(700);
        digitalWrite(A3, HIGH);
      }
    }
  }
  
}

// this method makes a HTTP connection to the server
void httpRequest() {
  Serial.println();

  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection
  if (client.connect(server, 80)) {
    Serial.println("Connecting...");

    // send the HTTP PUT request
    client.print("GET /B550928/dissForecastInfoSvc/getDissForecastInfo?serviceKey=");
    client.print(VARID);
    if(dissCd == 0) {
      client.print("&numOfRows=1&pageNo=11&type=json&dissCd=1&znCd=26");
      dissCd = (dissCd + 1) % 4;
    }
    else if(dissCd == 1) {
      client.print("&numOfRows=1&pageNo=11&type=json&dissCd=2&znCd=26");
      dissCd = (dissCd + 1) % 4;
    } 
    else if(dissCd == 2) {
      client.print("&numOfRows=1&pageNo=11&type=json&dissCd=3&znCd=26");
      dissCd = (dissCd + 1) % 4;
    }
    else if(dissCd == 3) {
      client.print("&numOfRows=1&pageNo=11&type=json&dissCd=4&znCd=26");
      dissCd = (dissCd + 1) % 4;
    }
    //client.print("&numOfRows=1&pageNo=11&type=json&dissCd=3&znCd=26");
    client.println(" HTTP/1.1");
    client.println("Host: apis.data.go.kr");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made
    lastConnectionTime = millis();
    getIsConnected = true;
  }
  else {
    // if you couldn't make a connection
    Serial.println("Connection failed");
    getIsConnected = false;
  }
}



void printWifiStatus() {
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

int getInt(String input) {
  char carray[20];
  //Serial.println(input);
  input.toCharArray(carray, sizeof(carray));
  //Serial.println(carray);
  temp = atoi(carray);
  return temp;
}
