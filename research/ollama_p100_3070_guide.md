# P100 + RTX 3070 Ollama 本地模型库搭建指南

**研究时间：** 2026-03-18  
**研究者：** SatanGeek 助手 🦞

---

## 📋 一、硬件配置分析

### Tesla P100 规格

| 项目 | 规格 |
|------|------|
| 架构 | Pascal |
| 显存 | 16GB HBM2 |
| CUDA 核心 | 3584 |
| FP16 性能 | 4.7 TFLOPS |
| FP32 性能 | 4.7 TFLOPS |
| TDP | 250W |
| NVLink | 支持（双向 160GB/s） |
| 驱动支持 | CUDA 11.x/12.x |

### RTX 3070 规格

| 项目 | 规格 |
|------|------|
| 架构 | Ampere |
| 显存 | 8GB GDDR6 |
| CUDA 核心 | 5888 |
| FP16 性能 | 20+ TFLOPS |
| FP32 性能 | 20+ TFLOPS |
| TDP | 220W |
| RT Core | 第 2 代 |
| Tensor Core | 第 3 代 |

### 双卡组合优势

```
总显存：24GB（16GB + 8GB）
架构互补：Pascal（P100）+ Ampere（3070）
优势：
✅ 大模型可以分层加载
✅ P100 适合 FP16 推理
✅ 3070 适合 FP32 和 RT 加速
✅ 总成本低，性能不错
```

---

## 🔧 二、系统要求

### 操作系统

```
推荐：Ubuntu 22.04 LTS 或 24.04 LTS
备选：Debian 12、CentOS Stream 9
```

### 驱动要求

| 组件 | 版本要求 |
|------|---------|
| NVIDIA 驱动 | 535.x 或更高 |
| CUDA Toolkit | 12.x |
| cuDNN | 8.9+ |
| Docker | 24.0+（可选） |

### 硬件要求

| 项目 | 最低 | 推荐 |
|------|------|------|
| CPU | 8 核 | 16 核+ |
| 内存 | 32GB | 64GB+ |
| 电源 | 650W | 850W+ |
| 散热 | 良好风道 | 水冷/强力风冷 |

---

## 📦 三、Ollama 安装方案

### 方案 A：官方脚本安装（推荐）

```bash
# 1. 下载并安装 Ollama
curl -fsSL https://ollama.com/install.sh | sh

# 2. 验证安装
ollama --version

# 3. 启动服务
ollama serve
```

### 方案 B：Docker 安装

```bash
# 1. 拉取镜像
docker pull ollama/ollama:latest

# 2. 运行容器（单 GPU）
docker run -d --gpus all -v ollama:/root/.ollama -p 11434:11434 ollama/ollama

# 3. 运行容器（多 GPU）
docker run -d --gpus '"device=0,1"' -v ollama:/root/.ollama -p 11434:11434 ollama/ollama
```

### 方案 C：源码编译（高级用户）

```bash
# 1. 克隆仓库
git clone https://github.com/ollama/ollama.git
cd ollama

# 2. 安装依赖
sudo apt-get update
sudo apt-get install -y build-essential cmake cuda-toolkit-12-0

# 3. 编译
make -j$(nproc)

# 4. 安装
sudo make install
```

---

## 🎮 四、多 GPU 配置

### 方案 1：Ollama 原生多 GPU 支持

```bash
# 设置环境变量启用多 GPU
export OLLAMA_MULTI_GPU=1

# 或者在 systemd 服务中配置
sudo systemctl edit ollama.service

# 添加以下内容：
[Service]
Environment="OLLAMA_MULTI_GPU=1"
Environment="CUDA_VISIBLE_DEVICES=0,1"

# 重启服务
sudo systemctl daemon-reload
sudo systemctl restart ollama
```

### 方案 2：手动指定 GPU

```bash
# 只使用 P100（GPU 0）
export CUDA_VISIBLE_DEVICES=0
ollama serve

# 只使用 3070（GPU 1）
export CUDA_VISIBLE_DEVICES=1
ollama serve

# 使用双卡
export CUDA_VISIBLE_DEVICES=0,1
ollama serve
```

### 方案 3：模型分层加载

```bash
# 大模型自动分配到多卡
# Ollama 会自动将模型层分配到不同 GPU

# 查看 GPU 使用情况
nvidia-smi

# 查看 Ollama 日志
journalctl -u ollama -f
```

---

## 📊 五、模型推荐

### 按显存分配

| 模型大小 | P100(16GB) | 3070(8GB) | 双卡 (24GB) |
|---------|-----------|----------|------------|
| 7B | ✅ 流畅 | ✅ 流畅 | ✅ 超流畅 |
| 13B | ✅ 流畅 | ⚠️ 量化 | ✅ 流畅 |
| 30B | ⚠️ 量化 | ❌ 不行 | ✅ 流畅 |
| 70B | ❌ 不行 | ❌ 不行 | ⚠️ 量化 |

### 推荐模型列表

#### 小型模型（<10GB）

```bash
# Qwen2.5-7B（中文友好）
ollama pull qwen2.5:7b

# Llama-3.1-8B
ollama pull llama3.1:8b

# Mistral-7B
ollama pull mistral:7b

# Phi-3-mini
ollama pull phi3:mini
```

#### 中型模型（10-20GB）

```bash
# Qwen2.5-14B
ollama pull qwen2.5:14b

# Llama-3.1-70B（量化版）
ollama pull llama3.1:70b-q4_K_M

# Mixtral-8x7B
ollama pull mixtral:8x7b
```

#### 大型模型（20GB+）

```bash
# Qwen2.5-32B
ollama pull qwen2.5:32b

# Llama-3.1-70B（需要双卡）
ollama pull llama3.1:70b

# DeepSeek-V2
ollama pull deepseek-v2
```

---

## ⚙️ 六、性能优化

### 1. 显存优化

```bash
# 设置显存使用上限
export OLLAMA_MAX_VRAM=24000000000  # 24GB

# 设置上下文长度
export OLLAMA_CONTEXT_LENGTH=4096

# 设置批处理大小
export OLLAMA_NUM_BATCH=4
```

### 2. 模型量化

```bash
# 使用量化模型节省显存
ollama pull qwen2.5:14b-q4_K_M  # 4bit 量化
ollama pull qwen2.5:14b-q5_K_M  # 5bit 量化
ollama pull qwen2.5:14b-q6_K    # 6bit 量化
```

### 3. 并发优化

```bash
# 设置并发请求数
export OLLAMA_MAX_QUEUE=10

# 设置线程数
export OLLAMA_NUM_THREAD=16
```

### 4. 散热优化

```bash
# 安装 nvtop 监控
sudo apt install nvtop

# 设置风扇曲线（需要 nvidia-settings）
nvidia-settings -a [gpu:0]/GPUFanControlState=1
nvidia-settings -a [fan:0]/GPUTargetFanSpeed=80
```

---

## 🔌 七、API 集成

### 基础 API 调用

```bash
# 生成文本
curl http://localhost:11434/api/generate -d '{
  "model": "qwen2.5:7b",
  "prompt": "Hello, world!"
}'

# 对话
curl http://localhost:11434/api/chat -d '{
  "model": "qwen2.5:7b",
  "messages": [
    {"role": "user", "content": "Hello!"}
  ]
}'
```

### Python 集成

```python
import requests

response = requests.post('http://localhost:11434/api/generate', json={
    'model': 'qwen2.5:7b',
    'prompt': 'Hello, world!'
})

print(response.json())
```

### OpenClaw 集成

```json
// ~/.openclaw/openclaw.json
{
  "agents": {
    "local": {
      "type": "ollama",
      "endpoint": "http://localhost:11434",
      "model": "qwen2.5:7b"
    }
  }
}
```

---

## 🐛 八、常见问题

### 问题 1：GPU 未被识别

```bash
# 检查驱动
nvidia-smi

# 检查 CUDA
nvcc --version

# 重新安装驱动
sudo apt purge nvidia-*
sudo apt install nvidia-driver-535
sudo reboot
```

### 问题 2：显存不足

```bash
# 使用量化模型
ollama pull qwen2.5:7b-q4_K_M

# 减少上下文长度
export OLLAMA_CONTEXT_LENGTH=2048

# 关闭其他 GPU 应用
pkill -f python  # 小心使用
```

### 问题 3：多卡不工作

```bash
# 检查 CUDA_VISIBLE_DEVICES
echo $CUDA_VISIBLE_DEVICES

# 强制指定 GPU
export CUDA_VISIBLE_DEVICES=0,1
ollama serve

# 查看日志
journalctl -u ollama -f
```

### 问题 4：模型加载慢

```bash
# 使用 SSD 存储模型
# 默认路径：/usr/share/ollama/.ollama/models

# 预加载模型到显存
ollama run qwen2.5:7b "warmup"

# 使用更快的存储
# NVMe SSD 推荐
```

---

## 📈 九、性能基准

### 预期性能（P100 + 3070）

| 模型 | 量化 | 令牌/秒 | 显存占用 |
|------|------|--------|---------|
| Qwen2.5-7B | FP16 | ~40 t/s | 14GB |
| Qwen2.5-7B | Q4_K_M | ~60 t/s | 6GB |
| Qwen2.5-14B | FP16 | ~25 t/s | 28GB |
| Qwen2.5-14B | Q4_K_M | ~40 t/s | 10GB |
| Qwen2.5-32B | Q4_K_M | ~20 t/s | 18GB |
| Llama-3.1-70B | Q4_K_M | ~8 t/s | 40GB |

---

## 📝 十、总结

### 推荐配置

```bash
# 系统
Ubuntu 22.04 LTS

# 驱动
NVIDIA 535.x + CUDA 12.x

# Ollama
最新版本

# 环境变量
export OLLAMA_MULTI_GPU=1
export CUDA_VISIBLE_DEVICES=0,1
export OLLAMA_MAX_VRAM=24000000000
export OLLAMA_CONTEXT_LENGTH=4096

# 推荐模型
qwen2.5:7b（日常使用）
qwen2.5:14b-q4_K_M（高质量）
qwen2.5:32b-q4_K_M（双卡）
```

### 成本估算

| 项目 | 价格（二手） |
|------|------------|
| Tesla P100 16GB | ¥800-1200 |
| RTX 3070 8GB | ¥1500-2000 |
| 主板（双卡支持） | ¥500-1000 |
| 电源 850W | ¥500-800 |
| **总计** | **¥3300-5000** |

---

**🦞 SatanGeek 助手 整理**  
**有问题随时问！**
