# ESP32 + 16x16 LED 点阵屏 完整指南

## 📦 硬件准备

| 物品 | 数量 | 备注 |
|------|------|------|
| ESP32 开发板 | 1 | 任意型号 |
| 16x16 LED 点阵屏 | 1 | 16 针接口，74HC595 驱动 |
| 杜邦线 | 若干 | 母对母或公对母 |
| 面包板 | 1 | 可选，方便接线 |

---

## 🔌 接线步骤

### 快速接线表

| ESP32 | 点阵屏 | 功能 |
|-------|--------|------|
| GPIO 23 | DATA | 数据 |
| GPIO 18 | CLK | 时钟 |
| GPIO 5 | LATCH | 锁存 |
| 3.3V | VCC | 电源 |
| GND | OE + GND | 地 |

### 接线图
详细接线图见 [`wiring_diagram.md`](./wiring_diagram.md)

---

## 💻 Windows 烧录步骤

### 1. 安装 Arduino IDE

1. 访问 https://www.arduino.cc/en/software
2. 下载 **Windows Installer** 或 **Windows ZIP**
3. 安装并打开

### 2. 添加 ESP32 支持

1. 打开 Arduino IDE
2. 点击 **文件** → **首选项**
3. 在 **附加开发板管理器网址** 填入：
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. 点击确定

### 3. 安装 ESP32 开发板

1. 点击 **工具** → **开发板** → **开发板管理器**
2. 搜索 `esp32`
3. 安装 **esp32 by Espressif Systems**

### 4. 选择开发板

1. 点击 **工具** → **开发板** → **ESP32 Arduino**
2. 选择你的型号，如：
   - `DOIT ESP32 DEVKIT V1`
   - `ESP32 Dev Module`
   - 或其他

### 5. 连接 ESP32

1. 用 USB 线连接 ESP32 到电脑
2. 点击 **工具** → **端口**
3. 选择 COM 口（如 `COM3`、`COM4`）

> ⚠️ **如果看不到端口**：
> - 检查 USB 线（有些只能充电）
> - 安装 CH340 或 CP2102 驱动

### 6. 上传代码

1. 打开 `esp32_16x16_74hc595.ino`
2. 点击 **上传** 按钮（→ 图标）
3. 等待编译和上传

---

## 🎬 演示功能

上传后自动循环播放：

| 演示 | 内容 |
|------|------|
| 1 | 数字 0-9（每个 1 秒） |
| 2 | 字母 A-F（每个 0.8 秒） |
| 3 | 爱心图案（2 秒） |
| 4 | 滚动文字 "HELLO ESP32!" |
| 5 | 移动方块动画 |

---

## 🛠️ 自定义修改

### 修改文字
```cpp
scrollText("你的文字");  // 在 loop() 里修改
```

### 修改引脚
```cpp
#define DATA_PIN    23   // 改成你的引脚
#define CLK_PIN     18
#define LATCH_PIN   5
```

### 修改亮度
```cpp
#define BRIGHTNESS_DELAY 1  // 改大变暗，改小变亮
```

### 添加新图案
```cpp
void drawYourPattern() {
  clearDisplay();
  displayBuffer[0] = 0xFF;  // 自定义显存
  // ...
  for(int i = 0; i < 100; i++) {
    refreshDisplay();
  }
}
```

---

## 🐛 常见问题

### 问题 1：上传失败
```
A fatal error occurred: Failed to connect to ESP32
```

**解决：**
- 按住 ESP32 上的 **BOOT** 按钮
- 点 **上传**
- 等看到 "Connecting..." 后松开 BOOT

### 问题 2：找不到端口
**解决：**
- 换一根 USB 线（确保能传数据）
- 安装驱动：CH340 或 CP2102
- 重启 Arduino IDE

### 问题 3：点阵屏不亮
**解决：**
- 检查 VCC/GND 接线
- 确认 OE 引脚接地
- 用万用表测电压

### 问题 4：显示乱码/反向
**解决：**
- 检查 DATA/CLK/LATCH 顺序
- 修改代码中的 `rowSelect` 逻辑：
  ```cpp
  byte rowSelect = ~(1 << row);  // 共阴
  // 或
  byte rowSelect = (1 << row);   // 共阳
  ```

### 问题 5：闪烁严重
**解决：**
- 调整 `BRIGHTNESS_DELAY`：
  ```cpp
  #define BRIGHTNESS_DELAY 2  // 改大减少闪烁
  ```

---

## 📁 文件说明

```
esp32_16x16_74hc595/
├── esp32_16x16_74hc595.ino   # 主代码
├── wiring_diagram.md         # 接线图
└── README.md                 # 本文件
```

---

## 📞 需要帮助？

有问题随时问！告诉我：
- 你的点阵屏型号/照片
- 遇到的问题
- 串口输出内容

🦞 SatanGeek 助手 随时待命！
