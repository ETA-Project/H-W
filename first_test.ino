#include <ESP8266WiFi.h>
#include <Wire.h>
#include <MS5611.h>

MS5611 sensor;

// const char* ssid = "KT_IPTV_WiFi_5G_876E";     // Wi-Fi SSID
// const char* password = "ixwgpr79hkam"; // Wi-Fi Password

const char* ssid = "zz";     // Wi-Fi SSID
const char* password = "00000000"; // Wi-Fi Password
const char* serverUrl = "ec2-13-124-153-108.ap-northeast-2.compute.amazonaws.com"; // 서버 URL
const int serverPort = 8080; // 서버 포트

const int ledPin = LED_BUILTIN; // NodeMCU의 내장 LED 핀

WiFiClient client;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  sensor.begin();
  pinMode(ledPin, OUTPUT); // LED 핀을 출력 모드로 설정

  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wi-Fi");

  unsigned long startTime = millis(); // 시작 시간 기록
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  // WiFi.mode(WIFI_STA);
  // WiFi.disconnect();
  // delay(100);

  // Serial.println("Setup done");
}

void loop() {
  sensor.read();
  double pressure = sensor.getPressure();
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" mbar");

  if (client.connect(serverUrl, serverPort)) {
    String getUrl = "/eta/update/1/" + String(pressure);
    client.println("GET " + getUrl + " HTTP/1.1");
    client.println("Host: " + String(serverUrl));
    client.println("Connection: close");
    client.println();
  }

  digitalWrite(ledPin, LOW); // 센서에서 데이터를 읽었으므로 LED 끄기
  

  delay(5000); // 5초 간격으로 데이터 읽기 및 전송


  // Serial.println("scan start");

  // // WiFi.scanNetworks will return the number of networks found
  // int n = WiFi.scanNetworks();
  // Serial.println("scan done");
  // if (n == 0)
  //   Serial.println("no networks found");
  // else
  // {
  //   Serial.print(n);
  //   Serial.println(" networks found");
  //   for (int i = 0; i < n; ++i)
  //   {
  //     // Print SSID and RSSI for each network found
  //     Serial.print(i + 1);
  //     Serial.print(": ");
  //     Serial.print(WiFi.SSID(i));
  //     Serial.print(" (");
  //     Serial.print(WiFi.RSSI(i));
  //     Serial.print(")");
  //     Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
  //     delay(10);
  //   }
  // }
  // Serial.println("");

  // // Wait a bit before scanning again
  // delay(5000);
}
