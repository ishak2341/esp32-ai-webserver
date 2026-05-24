#ifndef CONFIG_H
#define CONFIG_H

// ===== WiFi Configuration =====
#define WIFI_SSID "TurkTelekom_TP27C6_2.4GHz"
#define WIFI_PASSWORD "your_wifi_password"  // Değiştirin!

// ===== Web Server Configuration =====
#define SERVER_PORT 80
#define HOSTNAME "esp32-ai"

// ===== AI API Configuration =====
#define USE_OPENAI_API true  // OpenAI API kullanmak için
#define OPENAI_API_KEY "your_openai_api_key"  // Değiştirin!
#define OPENAI_MODEL "gpt-3.5-turbo"

// ===== Instagram Info =====
#define INSTAGRAM_USERNAME "ish_x06"

// ===== System Configuration =====
#define MAX_BUFFER_SIZE 4096
#define JSON_BUFFER_SIZE 2048
#define REQUEST_TIMEOUT 30000  // 30 seconds

#endif // CONFIG_H
