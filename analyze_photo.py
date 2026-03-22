#!/usr/bin/env python3
"""使用 DashScope API 分析图片"""
import requests
import base64
import json
import os

# 从配置读取 API key
config_path = os.path.expanduser('~/.openclaw/openclaw.json')
api_key = None

try:
    with open(config_path, 'r') as f:
        config = json.load(f)
        # 查找 dashscope 配置
        if 'extensions' in config and 'installs' in config['extensions']:
            if 'dashscope-cfg' in config['extensions']['installs']:
                # 可能需要从其他地方找 API key
                pass
except Exception as e:
    print(f"读取配置失败：{e}")

# 尝试从环境变量获取
api_key = os.environ.get('DASHSCOPE_API_KEY', '')

if not api_key:
    print("未找到 DashScope API key，请设置 DASHSCOPE_API_KEY 环境变量")
    exit(1)

def analyze_image(image_path):
    """使用 qwen-vl 分析图片"""
    
    # 将图片转为 base64
    with open(image_path, 'rb') as f:
        image_data = base64.b64encode(f.read()).decode('utf-8')
    
    url = "https://dashscope.aliyuncs.com/api/v1/services/aigc/multimodal-generation/generation"
    
    headers = {
        "Authorization": f"Bearer {api_key}",
        "Content-Type": "application/json"
    }
    
    payload = {
        "model": "qwen-vl-max",
        "input": {
            "messages": [
                {
                    "role": "user",
                    "content": [
                        {"image": f"data:image/jpeg;base64,{image_data}"},
                        {"text": "这是医院电话表的照片。请识别图片中的所有文字内容，包括科室名称、电话号码、房号等。按表格格式整理输出，用中文。"}
                    ]
                }
            ]
        }
    }
    
    response = requests.post(url, headers=headers, json=payload, timeout=60)
    
    if response.status_code == 200:
        result = response.json()
        print(json.dumps(result, ensure_ascii=False, indent=2))
        return result
    else:
        print(f"API 调用失败：{response.status_code}")
        print(response.text)
        return None

if __name__ == "__main__":
    import sys
    if len(sys.argv) < 2:
        print("用法：python3 analyze_photo.py <图片路径>")
        exit(1)
    
    analyze_image(sys.argv[1])
