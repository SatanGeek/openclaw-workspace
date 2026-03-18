/*
 * ESP32 Arduino 入门例程 03: PWM 呼吸灯
 * 
 * 功能：LED 渐变亮灭（呼吸效果）
 * 硬件：ESP32 + LED
 * 
 * 接线：LED 正极 → GPIO 5 → 220Ω电阻 → GND
 */

#define LED_PIN 5

// PWM 参数
int freq = 5000;      // 频率 5kHz
int resolution = 8;   // 8 位分辨率 (0-255)
int channel = 0;      // PWM 通道 0-15

void setup() {
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(LED_PIN, channel);
  Serial.begin(115200);
  Serial.println("PWM Breath Light Starting...");
}

void loop() {
  // 渐亮
  for(int duty = 0; duty <= 255; duty++) {
    ledcWrite(channel, duty);
    Serial.println("Duty: " + String(duty));
    delay(10);
  }
  
  // 渐灭
  for(int duty = 255; duty >= 0; duty--) {
    ledcWrite(channel, duty);
    delay(10);
  }
}
