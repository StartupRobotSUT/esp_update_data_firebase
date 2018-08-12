#include <FirebaseArduino.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

SoftwareSerial mySerial(D2, D3); // RX, TX
#define FIREBASE_HOST "test-mac-79226.firebaseio.com"
#define FIREBASE_AUTH "545HPrqtCSwsxxKtmjYWahYwsOZmoXQ49Jsb03pM"

// Config connect WiFi
#define WIFI_SSID "dtmkeng"
#define WIFI_PASSWORD "12345678"
void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  //  Serial.println("Goodnight moon!");
  mySerial.begin(9600);
  mySerial.println("Hello, world?");
  //  WiFi.mode(WIFI_STA);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  //  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
String str_data = "";
String uid = "";
String Can_count = "";
String bottle_count = "";
int i = 0;
//String demo = "uid : B5814824,count Can : 15,count bottle : 20";
void loop() {

  if (mySerial.available()) {
    str_data = mySerial.readString();
    Serial.println(str_data);
    uid = str_data.substring(6, 14);
    Can_count = str_data.substring(27, 29);
    bottle_count = str_data.substring(44, str_data.length());
    
    uid.trim();
    Can_count.trim();
    bottle_count.trim();

    float oldpoint =  Firebase.getFloat("users/" + uid + "/point");
    float newpoint = (3 * (Can_count.toFloat())) + (2 * (bottle_count.toFloat())) + oldpoint;
    Serial.println(newpoint);

    Firebase.setFloat("users/" + uid + "/point", newpoint);
    
    if (Firebase.failed()) {
      Serial.print("set /point failed:");
      Serial.println(Firebase.error());
      return;
    }

//    Serial.print("set /point to ");
//    Serial.println(Firebase.getInt("users/" + uid + "/point"));


    delay(500);
    
    Serial.print(uid);
    Serial.print("\t");
    Serial.print(Can_count);
    Serial.print("\t");
    Serial.print(bottle_count);
    Serial.println();
  }
  
}


