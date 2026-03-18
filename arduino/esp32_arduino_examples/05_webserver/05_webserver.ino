/*
 * ESP32 Arduino 入门例程 05: Web 服务器
 * 
 * 功能：创建网页控制 LED
 * 硬件：ESP32 + LED
 * 
 * 接线：LED 正极 → GPIO 2 → 220Ω电阻 → GND
 * 
 * 注意：修改 WiFi 账号密码！
 */

#include <WiFi.h>
#include <WebServer.h>

// ========== 修改为你的 WiFi ==========
const char* ssid = "你的 WiFi 名称";
const char* password = "你的 WiFi 密码";
// =====================================

WebServer server(80);
#define LED_PIN 2

// 网页 HTML
const char htmlPage[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 LED Control</title>
  <meta name="viewport" content="width=device-width">
  <style>
    body { font-family: Arial; text-align: center; margin-top: 50px; }
    .button { 
      display: inline-block; 
      padding: 15px 30px; 
      font-size: 20px; 
      margin: 10px;
      background-color: #4CAF50; 
      color: white; 
      border: none; 
      border-radius: 5px;
      cursor: pointer;
    }
    .button.off { background-color: #f44336; }
  </style>
</head>
<body>
  <h1>ESP32 LED Control</h1>
  <a href="/on"><button class="button">ON</button></a>
  <a href="/off"><button class="button off">OFF</button></a>
</body>
</html>
)rawliteral";

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  
  // 连接 WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  
  // 设置路由
  server.on("/", []() {
    server.send(200, "text/html", htmlPage);
  });
  
  server.on("/on", []() {
    digitalWrite(LED_PIN, HIGH);
    server.send(200, "text/html", htmlPage);
  });
  
  server.on("/off", []() {
    digitalWrite(LED_PIN, LOW);
    server.send(200, "text/html", htmlPage);
  });
  
  server.begin();
  Serial.println("HTTP server started!");
}

void loop() {
  server.handleClient();
}
