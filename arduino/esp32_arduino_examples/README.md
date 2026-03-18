# ESP32 Arduino 入门教程

## 📖 课程大纲

| 课号 | 名称 | 难度 | 学习点 |
|------|------|------|--------|
| 01 | LED 闪烁 | ⭐ | GPIO 输出 |
| 02 | 按钮控制 | ⭐⭐ | GPIO 输入 |
| 03 | PWM 呼吸灯 | ⭐⭐ | PWM 调光 |
| 04 | WiFi 连接 | ⭐⭐⭐ | 网络基础 |
| 05 | Web 服务器 | ⭐⭐⭐⭐ | HTTP 服务 |

---

## 🛠️ 环境搭建

### 1. 安装 Arduino IDE
- 访问 https://www.arduino.cc/en/software
- 下载 Windows 版本（推荐 2.x）
- 安装

### 2. 添加 ESP32 支持
1. 打开 Arduino IDE
2. **文件** → **首选项**
3. 在 **附加开发板管理器网址** 填入：
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```

### 3. 安装开发板
1. **工具** → **开发板** → **开发板管理器**
2. 搜索 `esp32`
3. 安装 **esp32 by Espressif Systems**

### 4. 选择开发板
1. **工具** → **开发板** → **ESP32 Arduino**
2. 选择你的型号（如 `DOIT ESP32 DEVKIT V1`）

### 5. 连接 ESP32
1. USB 线连接电脑
2. **工具** → **端口** → 选择 COM 口

> ⚠️ **如果看不到端口**：安装 CH340 或 CP2102 驱动

---

## 📝 例程说明

### 01_blink - LED 闪烁
**最基础的例程**，类似编程界的 "Hello World"

- **学习点**：GPIO 输出、delay 延时
- **硬件**：ESP32 开发板（板载 LED）
- **代码量**：15 行

---

### 02_button - 按钮控制
**学习输入检测**

- **学习点**：GPIO 输入、上拉电阻、消抖
- **硬件**：ESP32 + 按钮 + LED
- **接线**：
  ```
  按钮：GPIO 4 ↔ GND
  LED:   GPIO 2 ↔ 220Ω电阻 ↔ GND
  ```

---

### 03_pwm - PWM 呼吸灯
**学习模拟输出**

- **学习点**：PWM 原理、LED 调光
- **硬件**：ESP32 + LED
- **接线**：
  ```
  LED 正极 → GPIO 5 → 220Ω电阻 → GND
  ```

---

### 04_wifi - WiFi 连接
**学习网络连接**

- **学习点**：WiFi 连接、IP 地址、信号强度
- **硬件**：ESP32 开发板
- **注意**：修改代码中的 WiFi 账号密码！

---

### 05_webserver - Web 服务器
**综合应用**

- **学习点**：HTTP 服务器、网页控制
- **硬件**：ESP32 + LED
- **效果**：手机浏览器访问 ESP32 IP，控制 LED

---

## 🚀 学习建议

### 第 1 天
- ✅ 搭建环境
- ✅ 完成 01_blink
- 理解：GPIO 输出、程序结构

### 第 2 天
- ✅ 完成 02_button
- ✅ 完成 03_pwm
- 理解：GPIO 输入、PWM 原理

### 第 3 天
- ✅ 完成 04_wifi
- 理解：WiFi 连接、网络基础

### 第 4 天
- ✅ 完成 05_webserver
- 理解：HTTP 协议、Web 服务

### 第 5 天
- 🎯 综合项目：用 Web 页面控制点阵屏！

---

## 🔧 常见问题

### 问题 1：上传失败
```
Failed to connect to ESP32
```
**解决**：按住 BOOT 按钮，点上传，看到 "Connecting..." 后松开

### 问题 2：找不到端口
**解决**：
- 换 USB 线（确保能传数据）
- 安装 CH340 驱动
- 重启 Arduino IDE

### 问题 3：编译错误
**解决**：
- 检查开发板选择是否正确
- 检查是否安装了 ESP32 开发板
- 重启 Arduino IDE

---

## 📚 进阶学习

完成这 5 个例程后，可以学习：

1. **传感器** - DHT11 温湿度、超声波测距
2. **显示屏** - OLED、LCD、点阵屏
3. **通信** - I2C、SPI、UART
4. **蓝牙** - BLE 蓝牙低功耗
5. **物联网** - MQTT、HTTP API、Blynk

---

## 🦞 有问题随时问！

SatanGeek 助手 随时待命！
