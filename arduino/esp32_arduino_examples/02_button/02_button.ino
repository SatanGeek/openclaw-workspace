/*
 * ESP32 Arduino 入门例程 02: 按钮控制
 * 
 * 功能：按下按钮点亮 LED
 * 硬件：ESP32 + 按钮 + LED
 * 
 * 接线：
 * 按钮：GPIO 4 ↔ GND
 * LED:   GPIO 2 ↔ 220Ω电阻 ↔ GND
 */

#define BUTTON_PIN 4
#define LED_PIN    2

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // 上拉输入
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("Button Control Starting...");
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);
  
  if(buttonState == LOW) {  // 按钮按下（接地）
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Button PRESSED - LED ON");
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("Button RELEASED - LED OFF");
  }
  
  delay(50);  // 消抖
}
