# ESP32-S3 AI Web Server

**Professional AI-Powered Web Server for ESP32-S3 (16MB)**

![Status](https://img.shields.io/badge/status-active-brightgreen)
![Platform](https://img.shields.io/badge/platform-ESP32--S3-blue)
![License](https://img.shields.io/badge/license-MIT-green)

---

## 📋 Overview

A **modern, professional web server** running on ESP32-S3 with integrated **OpenAI API** for real-time AI chat capabilities. Features a beautiful, responsive UI inspired by modern social media platforms.

**Instagram**: [@ish_x06](https://instagram.com/ish_x06)

---

## ✨ Features

- ✅ **WiFi Connectivity**: Supports 2.4GHz WiFi networks
- ✅ **AI Chat Interface**: Real-time conversation with OpenAI GPT models
- ✅ **Modern UI**: Dark theme with gradient effects and smooth animations
- ✅ **Responsive Design**: Works on mobile, tablet, and desktop
- ✅ **System Monitoring**: Real-time device status and resource monitoring
- ✅ **REST API**: Clean API endpoints for extensibility
- ✅ **PSRAM Support**: 16MB RAM for optimal performance
- ✅ **Turkish Language**: Full Turkish UI and AI responses

---

## 🔧 Hardware Requirements

- **ESP32-S3 Development Module** (16MB)
- **USB Cable** (for programming and power)
- **WiFi Router** supporting 2.4GHz

---

## 📦 Installation

### 1. Clone Repository
```bash
git clone https://github.com/ishak2341/esp32-ai-webserver.git
cd esp32-ai-webserver
```

### 2. Install PlatformIO
```bash
pip install platformio
```

### 3. Configure Settings

Edit `include/config.h`:

```cpp
// WiFi Configuration
#define WIFI_SSID "TurkTelekom_TP27C6_2.4GHz"
#define WIFI_PASSWORD "your_wifi_password"  // Change this!

// OpenAI API Key
#define OPENAI_API_KEY "sk-your-api-key-here"  // Change this!

// Instagram
#define INSTAGRAM_USERNAME "ish_x06"
```

### 4. Get OpenAI API Key

1. Go to [OpenAI Platform](https://platform.openai.com/)
2. Sign up or log in
3. Navigate to API Keys
4. Create a new API key
5. Copy and paste it in `config.h`

### 5. Upload to ESP32-S3

```bash
platformio run -e esp32-s3-devkitc-1 -t upload
```

### 6. Monitor Serial Output

```bash
platformio device monitor -b 115200
```

---

## 🚀 Usage

### 1. Power On ESP32-S3
- Connect USB cable
- Device will boot and connect to WiFi
- Check serial monitor for IP address

### 2. Open Web Interface
- Go to `http://<your-esp32-ip>/`
- Or `http://esp32-ai.local/` (if mDNS enabled)

### 3. Start Chatting
- Type your message in the input box
- Press Enter or click the send button
- Wait for AI response

---

## 🔌 API Endpoints

### Status Check
```bash
GET /api/status
```

**Response:**
```json
{
  "status": "online",
  "device": "ESP32-S3",
  "instagram": "ish_x06",
  "uptime": 1234,
  "rssi": -65,
  "ram_free": 123456,
  "psram_free": 987654
}
```

### Send Message
```bash
POST /api/chat
```

**Request:**
```json
{
  "message": "Merhaba, nasılsın?"
}
```

**Response:**
```json
{
  "success": true,
  "user_message": "Merhaba, nasılsın?",
  "ai_response": "Merhaba! İyiyim, teşekkür ederim...",
  "timestamp": 1234567890
}
```

---

## 🎨 UI Features

- **Dark Theme**: Comfortable for extended use
- **Gradient Effects**: Modern visual design
- **Real-time Status**: WiFi connection indicator
- **Message Animations**: Smooth message entry animations
- **Loading State**: Visual feedback during API calls
- **Toast Notifications**: Status and error messages
- **Responsive Layout**: Mobile-optimized interface

---

## 📊 Performance Specifications

- **CPU**: Dual-core 240MHz
- **RAM**: 16MB PSRAM + 520KB SRAM
- **WiFi**: 802.11 b/g/n (2.4GHz)
- **Max Connections**: 10 simultaneous
- **Response Time**: < 2 seconds (with API)
- **Uptime**: Unlimited (with proper power supply)

---

## 🛠️ Troubleshooting

### WiFi Connection Issues
1. Check SSID and password in `config.h`
2. Verify 2.4GHz WiFi is enabled (not 5GHz)
3. Check router range and signal strength
4. Restart device

### API Not Responding
1. Verify OpenAI API key is valid
2. Check internet connection on WiFi
3. Review API usage and billing
4. Check for rate limiting errors

### Web Interface Not Loading
1. Confirm IP address from serial monitor
2. Check if ESP32 is online (`/api/status`)
3. Try `http://esp32-ai.local/`
4. Clear browser cache

### Memory Issues
1. Monitor free PSRAM: `ESP.getFreePsram()`
2. Reduce JSON buffer size if needed
3. Limit message history

---

## 📝 File Structure

```
esp32-ai-webserver/
├── include/
│   └── config.h              # Configuration file
├── src/
│   └── main.cpp              # Main firmware code
├── data/
│   └── www/
│       ├── index.html        # Web UI
│       ├── style.css         # Styling
│       └── script.js         # Frontend logic
├── platformio.ini            # PlatformIO config
└── README.md                 # This file
```

---

## 🔐 Security Notes

⚠️ **IMPORTANT**:
- Store API keys securely
- Use HTTPS in production
- Don't expose sensitive data in logs
- Implement rate limiting for production
- Use strong WiFi passwords

---

## 🤝 Contributing

Contributions welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

---

## 📄 License

MIT License - See LICENSE file for details

---

## 👤 Author

**Instagram**: [@ish_x06](https://instagram.com/ish_x06)

---

## 🙏 Acknowledgments

- ESP32 by Espressif Systems
- OpenAI API
- AsyncWebServer library
- ArduinoJson library

---

## 📞 Support

For issues and questions:
- 📧 GitHub Issues
- 📱 Instagram: @ish_x06

---

**Happy Coding! 🚀**
