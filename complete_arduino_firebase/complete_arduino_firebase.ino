#include <ESP8266WiFi.h>
#include <time.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "detect-car-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "eBsrrwmW5kGTxlpgN84aZllBQRCiRbLzazAJBAOT"
#define WIFI_SSID "Jonny Aloha"
#define WIFI_PASSWORD "secrethotspot"

int timezone = 9;
String logdata, savedata;
String location = "A location";                    //지역 설정

String currentTime() {
  time_t now = time(NULL);
  struct tm* timeinfo;
  timeinfo = localtime(&now);
  int y = timeinfo->tm_year + 1900;
  int m = timeinfo->tm_mon + 1;
  int d = timeinfo->tm_mday;
  int hr = timeinfo->tm_hour;
  int min = timeinfo->tm_min;
  int sec = timeinfo->tm_sec;
  String timedata = String("(") + y + String("/") + m + String("/") + d + String(" ") +  hr + String(":") + min + String(":") + sec + String(")");

  return timedata;
}

void setup() {
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);       //WIFI 시작
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  configTime(timezone * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   //Firebase 접속 완료
  String starttime = currentTime();
  Firebase.pushString("connected", location + " 접속 완료");
}

void loop() {

  bool Sr = false;

  while (Serial.available()) {
    logdata = Serial.readString();
    Sr = true;
  }
  delay(1000);

  if (Sr == true) {
    String savedata = logdata;
    String timedata = currentTime(); 

    Firebase.pushString(location, savedata + timedata);

    if (Firebase.failed()) {
      return;
    }
  }
}
