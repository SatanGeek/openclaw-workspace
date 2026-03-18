# ESP32 + 16x16 LED 点阵屏

## 硬件准备

### 需要的东西
- ESP32 开发板
- 16x16 LED 点阵屏模块（MAX7219 驱动）
- 杜邦线若干

### 接线图

```
ESP32          MAX7219 点阵屏
─────          ─────────────
GPIO 23   ──→  DIN (数据输入)
GPIO 18   ──→  CLK (时钟)
GPIO 5    ──→  CS  (片选)
3.3V      ──→  VCC
GND       ──→  GND
```

> ⚠️ **注意：** 如果点阵屏是 5V 的，可能需要电平转换器

## Windows 上烧录步骤

### 1. 安装 Arduino IDE
1. 访问 https://www.arduino.cc/en/software
2. 下载 Windows 版本（推荐 IDE 2.x）
3. 安装

### 2. 添加 ESP32 支持
1. 打开 Arduino IDE
2. 点击 **文件** → **首选项**
3. 在 **附加开发板管理器网址** 填入：
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. 点击 **工具** → **开发板** → **开发板管理器**
5. 搜索 `esp32`
6. 安装 **esp32 by Espressif Systems**

### 3. 安装依赖库
1. 点击 **工具** → **管理库**
2. 搜索 `LedControl`
3. 安装 **LedControl by Eberhard Fahle**

### 4. 选择开发板
1. 点击 **工具** → **开发板** → **ESP32 Arduino**
2. 选择你的 ESP32 型号（如 `DOIT ESP32 DEVKIT V1`）

### 5. 连接 ESP32
1. 用 USB 线连接 ESP32 到电脑
2. 点击 **工具** → **端口**，选择对应的 COM 口

### 6. 上传代码
1. 打开 `esp32_16x16_dotmatrix.ino`
2. 点击 **上传** 按钮（→ 图标）
3. 等待编译和上传完成

## 功能演示

上传后会自动运行以下演示：

1. **数字 0-9** - 每个显示 1 秒
2. **字母 A-Z** - 每个显示 0.5 秒
3. **滚动文字** - "HELLO ESP32!"
4. **动画** - 移动的光点

## 自定义修改

### 修改亮度
```cpp
lc.setIntensity(i, 8);  // 改为 0-15 之间的值
```

### 修改文字
```cpp
scrollText("你的文字");  // 在 loop() 里修改
```

### 修改引脚
```cpp
#define DIN_PIN   23  // 改成你想要的引脚
#define CLK_PIN   18
#define CS_PIN    5
```

## 常见问题

### 问题：上传失败
- 检查 USB 线是否支持数据传输（有些只能充电）
- 检查端口选择是否正确
- 按住 ESP32 上的 BOOT 按钮再点上传

### 问题：点阵屏不亮
- 检查接线是否正确
- 检查 VCC 是否供电
- 尝试调整亮度：`lc.setIntensity(i, 15)`

### 问题：显示乱码
- 检查点阵屏是否是 MAX7219 驱动
- 检查 DIN/DO 方向是否正确
- 检查 NUM_DEVICES 数量（16x16 通常是 4 个 8x8）

---

**有问题随时问！** 🦞
