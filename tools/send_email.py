#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
医院电话表 Excel 邮件发送脚本
"""

import smtplib
import sys
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email.mime.text import MIMEText
from email import encoders

# 配置
SMTP_SERVER = "smtp.163.com"
SMTP_PORT = 465
FROM_EMAIL = "13662166610@163.com"
TO_EMAIL = "13662166610@163.com"
ATTACHMENT_PATH = "/home/admin/.openclaw/workspace/tools/hospital-phones.xlsx"

def send_email(password):
    # 创建邮件
    msg = MIMEMultipart()
    msg['From'] = FROM_EMAIL
    msg['To'] = TO_EMAIL
    msg['Subject'] = "天津市第三中心医院 - 完整电话表 (Excel)"
    
    # 正文
    body = """
田泽硕，你好！

附件是整理好的天津市第三中心医院完整电话表 Excel 文件。

📊 包含 13 个工作表：
- 🏛️ 院领导/行政
- 🏥 门诊区域
- 🚑 急诊区域
- 🏢 病区楼层 (2-15 层)
- 🔬 医技科室
- 💊 药房
- 🏥 手术室/麻醉
- 🍽️ 后勤/总务
- 💰 财务/医保
- 🥗 营养科
- 📋 人员分机
- 📞 快速拨号表

共 200+ 部门电话，已合并去重。

---
SatanGeek 助手 🦞
"""
    msg.attach(MIMEText(body.strip(), 'plain', 'utf-8'))
    
    # 添加附件
    try:
        with open(ATTACHMENT_PATH, 'rb') as f:
            part = MIMEBase('application', 'octet-stream')
            part.set_payload(f.read())
            encoders.encode_base64(part)
            part.add_header('Content-Disposition', 'attachment; filename="hospital-phones.xlsx"')
            msg.attach(part)
    except FileNotFoundError:
        print(f"❌ 文件未找到：{ATTACHMENT_PATH}")
        sys.exit(1)
    
    # 发送
    try:
        server = smtplib.SMTP_SSL(SMTP_SERVER, SMTP_PORT)
        server.login(FROM_EMAIL, password)
        server.send_message(msg)
        server.quit()
        print("✅ 发送成功！")
        print(f"📧 收件人：{TO_EMAIL}")
        print(f"📎 附件：hospital-phones.xlsx")
        return True
    except smtplib.SMTPAuthenticationError:
        print("❌ 认证失败：授权码错误")
        return False
    except Exception as e:
        print(f"❌ 发送失败：{e}")
        return False

if __name__ == '__main__':
    if len(sys.argv) > 1:
        password = sys.argv[1]
        send_email(password)
    else:
        # 交互式输入
        password = input("请输入 163 邮箱 SMTP 授权码：")
        send_email(password)
