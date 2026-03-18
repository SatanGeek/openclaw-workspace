/*
 * ESP32 Arduino 入门例程 04: WiFi 连接
 * 
 * 功能：连接 WiFi 网络
 * 硬件：ESP32 开发板
 * 
 * 注意：修改下面的 WiFi 账号密码！
 */

#include <WiFi.h>

// ========== 修改为你的 WiFi ==========
const char* ssid = "你的 WiFi 名称";
const char* password = "你的 WiFi 密码";
// =====================================

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println();
  Serial.println("WiFi Connect Starting...");
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  // 等待连接
  int timeout = 0;
  while(WiFi.status() != WL_CONNECTED && timeout < 30) {
    delay(500);
    Serial.print(".");
    timeout++;
  }
  
  if(WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("WiFi CONNECTED!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println();
    Serial.println("WiFi CONNECT FAILED!");
  }
}

void loop() {
  // 检查连接状态
  if(WiFi.status() == WL_CONNECTED) {
    Serial.print("RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("WiFi DISCONNECTED!");
  }
  delay(5000);
}
