/*
 * ESP32 + 16x16 LED 点阵屏 (74HC595 驱动)
 * 
 * 硬件：16 针接口 16x16 点阵屏模块
 * 驱动：2 个 74HC595 移位寄存器
 * 
 * 接线图见：wiring_diagram.md
 */

#include <Arduino.h>

// ========== 引脚定义 ==========
#define DATA_PIN    23    // 74HC595 数据输入 (DS)
#define CLK_PIN     18    // 74HC595 时钟 (SH_CP)
#define LATCH_PIN   5     // 74HC595 锁存 (ST_CP)

// ========== 显示参数 ==========
#define DISPLAY_SIZE 16   // 16x16 点阵
#define BRIGHTNESS_DELAY 1  // 亮度控制（扫描延迟）

// ========== 显存 ==========
byte displayBuffer[DISPLAY_SIZE];  // 16 行，每行 1 字节

// ========== 字模数据 (8x8) ==========
const byte font8x8[][8] = {
  {0x00,0x00,0x3E,0x41,0x41,0x41,0x3E,0x00}, // 0
  {0x00,0x00,0x42,0x7F,0x40,0x40,0x00,0x00}, // 1
  {0x00,0x00,0x62,0x51,0x51,0x51,0x4E,0x00}, // 2
  {0x00,0x00,0x22,0x49,0x49,0x49,0x36,0x00}, // 3
  {0x00,0x00,0x18,0x28,0x48,0x7F,0x08,0x00}, // 4
  {0x00,0x00,0x2F,0x49,0x49,0x49,0x31,0x00}, // 5
  {0x00,0x00,0x3E,0x49,0x49,0x49,0x32,0x00}, // 6
  {0x00,0x00,0x01,0x01,0x71,0x09,0x07,0x00}, // 7
  {0x00,0x00,0x36,0x49,0x49,0x49,0x36,0x00}, // 8
  {0x00,0x00,0x26,0x49,0x49,0x49,0x3E,0x00}, // 9
  {0x00,0x00,0x7E,0x11,0x11,0x11,0x7E,0x00}, // A
  {0x00,0x00,0x7F,0x49,0x49,0x49,0x36,0x00}, // B
  {0x00,0x00,0x3E,0x41,0x41,0x41,0x22,0x00}, // C
  {0x00,0x00,0x7F,0x41,0x41,0x41,0x3E,0x00}, // D
  {0x00,0x00,0x7F,0x49,0x49,0x49,0x41,0x00}, // E
  {0x00,0x00,0x7F,0x09,0x09,0x09,0x01,0x00}, // F
  {0x00,0x00,0x3E,0x41,0x41,0x41,0x3A,0x00}, // G
  {0x00,0x00,0x7F,0x08,0x08,0x08,0x7F,0x00}, // H
  {0x00,0x00,0x00,0x41,0x7F,0x41,0x00,0x00}, // I
  {0x00,0x00,0x30,0x40,0x40,0x3F,0x00,0x00}, // J
  {0x00,0x00,0x7F,0x08,0x14,0x22,0x7F,0x00}, // K
  {0x00,0x00,0x7F,0x40,0x40,0x40,0x40,0x00}, // L
  {0x00,0x00,0x7F,0x02,0x04,0x02,0x7F,0x00}, // M
  {0x00,0x00,0x7F,0x02,0x04,0x08,0x7F,0x00}, // N
  {0x00,0x00,0x3E,0x41,0x41,0x41,0x3E,0x00}, // O
  {0x00,0x00,0x7F,0x09,0x09,0x09,0x06,0x00}, // P
  {0x00,0x00,0x3E,0x41,0x51,0x21,0x5E,0x00}, // Q
  {0x00,0x00,0x7F,0x09,0x19,0x29,0x46,0x00}, // R
  {0x00,0x00,0x26,0x49,0x49,0x49,0x32,0x00}, // S
  {0x00,0x00,0x01,0x01,0x7F,0x01,0x01,0x00}, // T
  {0x00,0x00,0x3F,0x40,0x40,0x40,0x3F,0x00}, // U
  {0x00,0x00,0x1F,0x20,0x40,0x20,0x1F,0x00}, // V
  {0x00,0x00,0x7F,0x40,0x38,0x40,0x7F,0x00}, // W
  {0x00,0x00,0x41,0x22,0x14,0x22,0x41,0x00}, // X
  {0x00,0x00,0x01,0x02,0x7C,0x02,0x01,0x00}, // Y
  {0x00,0x00,0x7F,0x40,0x40,0x40,0x7F,0x00}, // Z
};

// ========== 函数声明 ==========
void shiftOutData(byte data);
void displayRow(byte row, byte data);
void clearDisplay();
void drawChar8x8(char c, int startX, int startY);
void displayNumber(int num);
void displayLetter(char c);
void scrollText(const char* text);
void animateBox();
void drawHeart();

// ========== 初始化 ==========
void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 16x16 Dot Matrix (74HC595) Starting...");
  
  // 设置引脚模式
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  
  // 初始状态
  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLK_PIN, LOW);
  digitalWrite(LATCH_PIN, HIGH);
  
  clearDisplay();
  Serial.println("Setup complete!");
}

// ========== 主循环 ==========
void loop() {
  // 演示 1: 数字 0-9
  Serial.println("Displaying numbers 0-9");
  for(int i = 0; i <= 9; i++) {
    displayNumber(i);
    delay(1000);
  }
  
  // 演示 2: 字母 A-F
  Serial.println("Displaying letters A-F");
  const char letters[] = {'A','B','C','D','E','F'};
  for(int i = 0; i < 6; i++) {
    displayLetter(letters[i]);
    delay(800);
  }
  
  // 演示 3: 爱心图案
  Serial.println("Drawing heart");
  drawHeart();
  delay(2000);
  
  // 演示 4: 滚动文字
  Serial.println("Scrolling text");
  scrollText("HELLO ESP32!");
  
  // 演示 5: 动画
  Serial.println("Animating");
  animateBox();
}

// ========== 底层驱动函数 ==========

// 移位输出一个字节
void shiftOutData(byte data) {
  for(int i = 7; i >= 0; i--) {
    digitalWrite(DATA_PIN, (data >> i) & 1);
    digitalWrite(CLK_PIN, HIGH);
    digitalWrite(CLK_PIN, LOW);
  }
}

// 显示一行（动态扫描）
void displayRow(byte row, byte data) {
  // 行选择（共阴/共阳根据实际调整）
  // 这里假设：行选通为低电平，列数据为高电平
  
  digitalWrite(LATCH_PIN, LOW);
  
  // 先送列数据（第二个 74HC595）
  shiftOutData(data);
  
  // 再送行选择（第一个 74HC595）
  byte rowSelect = ~(1 << row);  // 低电平选通
  shiftOutData(rowSelect);
  
  digitalWrite(LATCH_PIN, HIGH);
}

// 清屏
void clearDisplay() {
  for(int i = 0; i < DISPLAY_SIZE; i++) {
    displayBuffer[i] = 0;
  }
}

// 刷新显示（动态扫描）
void refreshDisplay() {
  for(int row = 0; row < DISPLAY_SIZE; row++) {
    displayRow(row, displayBuffer[row]);
    delayMicroseconds(BRIGHTNESS_DELAY * 100);
  }
}

// ========== 绘图函数 ==========

// 在指定位置画 8x8 字符
void drawChar8x8(char c, int startX, int startY) {
  int charIndex = -1;
  
  if(c >= '0' && c <= '9') {
    charIndex = c - '0';
  } else if(c >= 'A' && c <= 'Z') {
    charIndex = c - 'A' + 10;
  } else if(c >= 'a' && c <= 'z') {
    charIndex = c - 'a' + 10;
  }
  
  if(charIndex < 0 || charIndex > 36) return;
  
  for(int row = 0; row < 8; row++) {
    if(startY + row >= DISPLAY_SIZE) break;
    
    byte charRow = font8x8[charIndex][row];
    
    // 处理水平偏移
    if(startX == 0) {
      displayBuffer[startY + row] = charRow;
    } else if(startX == 8) {
      displayBuffer[startY + row] |= charRow;
    }
  }
}

// 显示数字
void displayNumber(int num) {
  if(num < 0 || num > 9) return;
  
  clearDisplay();
  char c = '0' + num;
  drawChar8x8(c, 4, 4);  // 居中显示
  
  // 刷新几次保持显示
  for(int i = 0; i < 50; i++) {
    refreshDisplay();
  }
}

// 显示字母
void displayLetter(char c) {
  clearDisplay();
  drawChar8x8(c, 4, 4);  // 居中显示
  
  for(int i = 0; i < 50; i++) {
    refreshDisplay();
  }
}

// 滚动文字
void scrollText(const char* text) {
  int len = strlen(text);
  
  for(int offset = -8; offset < len * 8; offset++) {
    clearDisplay();
    
    for(int i = 0; i < len; i++) {
      int xPos = i * 8 - offset;
      if(xPos >= -8 && xPos < 16) {
        drawChar8x8(text[i], xPos / 8 * 8, 4);
      }
    }
    
    for(int j = 0; j < 3; j++) {
      refreshDisplay();
    }
  }
}

// 移动方块动画
void animateBox() {
  for(int pos = 0; pos < 16; pos++) {
    clearDisplay();
    displayBuffer[pos] = 0xFF;
    
    for(int j = 0; j < 5; j++) {
      refreshDisplay();
    }
  }
}

// 画爱心
void drawHeart() {
  clearDisplay();
  
  // 简化的 16x16 爱心图案
  const byte heartPattern[] = {
    0x00, 0x66, 0xFF, 0xFF, 0x7E, 0x3C, 0x18, 0x00,
    0x00, 0x18, 0x3C, 0x7E, 0xFF, 0xFF, 0x66, 0x00
  };
  
  for(int i = 0; i < 16; i++) {
    displayBuffer[i] = heartPattern[i];
  }
  
  for(int i = 0; i < 100; i++) {
    refreshDisplay();
  }
}
