#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define WIFI_SSID "WE_B2E84D"                  // your wifi SSID
#define WIFI_PASSWORD "20305149963"  //your wifi PASSWORD

#define FIREBASE_HOST "attendance-app-6977b-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "blw2NegKZaJ60pbyo6GVmYDCmUziYLa7n7Wzz1iD"

char Mymessage[17];
boolean newData = false;

void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  delay(3000);
}

String newTime;

void loop() {
  Firebase.setBool("conEsp", true);
  recvOneChar();
  showNewData(); 
  if (Firebase.getString("/newLec/Ardtime") != newTime) {
    Serial.print("Dr ");
    Serial.print(Firebase.getString("/newLec/DrName"));
    Serial.print(",");
    Serial.print(Firebase.getString("/newLec/courseName"));
    Serial.print(",");
    Serial.print(Firebase.getString("/newLec/courseCode"));
    Serial.print(",");
    Serial.println(Firebase.getString("/newLec/actLevel"));
    newTime = Firebase.getString("/newLec/Ardtime");
  }
}

void recvOneChar() {
  if (Serial.available() > 0) {
    size_t num_read = Serial.readBytesUntil('\n', Mymessage, sizeof(Mymessage) - 1);
    Mymessage[num_read] = '\0';
    newData = true;
  }
}

void showNewData() {
  if (newData == true) {
    if ((Mymessage[0] >= '0') && (Mymessage[0] <= '9')) {
      Firebase.push("/Student", Mymessage);
    }
    newData = false;
  }
}