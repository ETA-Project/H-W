#include <ESP8266WiFi.h>
#include <Wire.h>
#include <MS5611.h>

MS5611 sensor;

const char* ssid = "your_SSID";     // Wi-Fi SSID
const char* password = "your_PASSWORD"; // Wi-Fi Password
const char* serverUrl = "server_url";   // Spring 서버 URL (예: "http://192.168.1.100")
const int serverPort = 8080;            // Spring 서버 포트 (예: 8080)
const String endpoint = "/data";        // Spring 서버 엔드포인트 (예: "/api/pressure")

const int ledPin = LED_BUILTIN;        // NodeMCU의 내장 LED 핀

WiFiClient client;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  sensor.begin();
  pinMode(ledPin, OUTPUT); // LED 핀을 출력 모드로 설정

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");
}

void loop() {
  if (sensor.read()) {
    double pressure = sensor.getPressure();
    double temperature = sensor.getTemperature();
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.print(" mbar, Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    if (client.connect(serverUrl, serverPort)) {
      String postData = "pressure=" + String(pressure);
      client.println("POST " + endpoint + " HTTP/1.1");
      client.println("Host: " + String(serverUrl));
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.println("Content-Length: " + postData.length());
      client.println();
      client.println(postData);
    }
    
    digitalWrite(ledPin, LOW); // 센서에서 데이터를 읽었으므로 LED 끄기
  } else {
    Serial.println("센서에서 데이터를 읽지 못함");
    Serial.println("센서 읽기 실패, 센서 상태 또는 연결을 확인하세요.");
    digitalWrite(ledPin, HIGH); // 데이터를 읽지 못했으므로 LED 켜기
  }

  delay(5000); // 5초 간격으로 데이터 읽기 및 전송
}
