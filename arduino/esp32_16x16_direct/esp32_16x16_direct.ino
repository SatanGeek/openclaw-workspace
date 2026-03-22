/*
 * ESP32 + 16x16 LED 点阵屏 (直接驱动/动态扫描)
 * 
 * 适用于 16 针接口的 16x16 点阵屏
 * 使用 ESP32 GPIO 直接控制行和列
 * 
 * 注意：16x16 需要 32 个 GPIO，ESP32 不够用
 * 建议使用 74HC595 扩展或确认你的屏内部有驱动
 */

#include <Arduino.h>

// ========== 引脚定义 ==========
// 根据你的实际接线修改这些引脚

// 行控制引脚 (8 个)
const int ROW_PINS[] = {25, 26, 27, 14, 12, 13, 15, 2};

// 列控制引脚 (8 个)
const int COL_PINS[] = {32, 33, 35, 34, 39, 36, 4, 16};

#define ROW_COUNT 8
#define COL_COUNT 8
#define DISPLAY_SIZE 8  // 实际可用 8x8

// ========== 显存 ==========
byte displayBuffer[ROW_COUNT];

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
};

// ========== 函数声明 ==========
void initPins();
void scanRow(int row);
void refreshDisplay();
void clearDisplay();
void setPixel(int x, int y, bool on);
void drawChar(char c, int offsetX, int offsetY);
void displayNumber(int num);
void displayLetter(char c);
void drawHeart();
void animateLine();

// ========== 初始化 ==========
void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 16x16 Dot Matrix (Direct Drive) Starting...");
  
  initPins();
  clearDisplay();
  
  Serial.println("Setup complete!");
  Serial.println("Row pins: " + String(ROW_COUNT));
  Serial.println("Col pins: " + String(COL_COUNT));
}

// ========== 主循环 ==========
void loop() {
  // 演示 1: 数字 0-9
  Serial.println("Displaying numbers 0-9");
  for(int i = 0; i <= 9; i++) {
    displayNumber(i);
    delay(1500);
  }
  
  // 演示 2: 字母 A-F
  Serial.println("Displaying letters A-F");
  const char letters[] = {'A','B','C','D','E','F'};
  for(int i = 0; i < 6; i++) {
    displayLetter(letters[i]);
    delay(1000);
  }
  
  // 演示 3: 爱心
  Serial.println("Drawing heart");
  drawHeart();
  delay(2000);
  
  // 演示 4: 动画
  Serial.println("Animating");
  animateLine();
}

// ========== 底层函数 ==========

// 初始化所有引脚
void initPins() {
  for(int i = 0; i < ROW_COUNT; i++) {
    pinMode(ROW_PINS[i], OUTPUT);
    digitalWrite(ROW_PINS[i], LOW);
  }
  for(int i = 0; i < COL_COUNT; i++) {
    pinMode(COL_PINS[i], OUTPUT);
    digitalWrite(COL_PINS[i], LOW);
  }
}

// 扫描一行（动态扫描核心）
void scanRow(int row) {
  // 先关闭所有行
  for(int i = 0; i < ROW_COUNT; i++) {
    digitalWrite(ROW_PINS[i], HIGH);  // 假设行选通为低电平
  }
  
  // 设置列数据
  byte rowData = displayBuffer[row];
  for(int col = 0; col < COL_COUNT; col++) {
    bool ledOn = (rowData >> col) & 1;
    digitalWrite(COL_PINS[col], ledOn ? HIGH : LOW);
  }
  
  // 选通当前行
  digitalWrite(ROW_PINS[row], LOW);
  
  // 保持一小段时间
  delayMicroseconds(2000);
  
  // 关闭当前行
  digitalWrite(ROW_PINS[row], HIGH);
}

// 刷新显示（循环扫描所有行）
void refreshDisplay() {
  for(int row = 0; row < ROW_COUNT; row++) {
    scanRow(row);
  }
}

// 清屏
void clearDisplay() {
  for(int i = 0; i < ROW_COUNT; i++) {
    displayBuffer[i] = 0;
  }
}

// 设置单个像素
void setPixel(int x, int y, bool on) {
  if(x < 0 || x >= COL_COUNT || y < 0 || y >= ROW_COUNT) return;
  
  if(on) {
    displayBuffer[y] |= (1 << x);
  } else {
    displayBuffer[y] &= ~(1 << x);
  }
}

// 画字符
void drawChar(char c, int offsetX, int offsetY) {
  int charIndex = -1;
  
  if(c >= '0' && c <= '9') {
    charIndex = c - '0';
  } else if(c >= 'A' && c <= 'F') {
    charIndex = c - 'A' + 10;
  }
  
  if(charIndex < 0 || charIndex > 15) return;
  
  for(int row = 0; row < 8; row++) {
    if(offsetY + row < ROW_COUNT) {
      displayBuffer[offsetY + row] = font8x8[charIndex][row];
    }
  }
}

// ========== 显示函数 ==========

void displayNumber(int num) {
  if(num < 0 || num > 9) return;
  
  clearDisplay();
  char c = '0' + num;
  drawChar(c, 0, 0);
  
  // 刷新显示 1 秒
  unsigned long startTime = millis();
  while(millis() - startTime < 1000) {
    refreshDisplay();
  }
}

void displayLetter(char c) {
  clearDisplay();
  drawChar(c, 0, 0);
  
  unsigned long startTime = millis();
  while(millis() - startTime < 1000) {
    refreshDisplay();
  }
}

void drawHeart() {
  clearDisplay();
  
  // 8x8 爱心图案
  const byte heartPattern[] = {
    0x00, 0x66, 0xFF, 0xFF, 0x7E, 0x3C, 0x18, 0x00
  };
  
  for(int i = 0; i < 8; i++) {
    displayBuffer[i] = heartPattern[i];
  }
  
  unsigned long startTime = millis();
  while(millis() - startTime < 2000) {
    refreshDisplay();
  }
}

void animateLine() {
  for(int pos = 0; pos < 8; pos++) {
    clearDisplay();
    displayBuffer[pos] = 0xFF;
    
    for(int j = 0; j < 20; j++) {
      refreshDisplay();
    }
  }
  
  for(int pos = 7; pos >= 0; pos--) {
    clearDisplay();
    displayBuffer[pos] = 0xFF;
    
    for(int j = 0; j < 20; j++) {
      refreshDisplay();
    }
  }
}
