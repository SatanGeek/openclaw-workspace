/*
 * ESP32 Arduino 入门例程 01: LED 闪烁
 * 
 * 功能：板载 LED 闪烁（类似 Hello World）
 * 硬件：ESP32 开发板（板载 LED）
 */

// ESP32 板载 LED 通常是 GPIO 2
#define LED_PIN 2

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("LED Blink Starting...");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  Serial.println("LED ON");
  delay(1000);
  
  digitalWrite(LED_PIN, LOW);
  Serial.println("LED OFF");
  delay(1000);
}
