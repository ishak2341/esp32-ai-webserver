#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "config.h"

// ===== Global Variables =====
AsyncWebServer server(SERVER_PORT);
HTTPClient httpClient;
WiFiClientSecure wifiClientSecure;

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// ===== Function Declarations =====
void setupWiFi();
void setupWebServer();
void handleAIRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
String callOpenAIAPI(String userMessage);
void logMessage(String msg);

// ===== Setup Function =====
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  logMessage("\n\n=== ESP32-S3 AI Web Server ===");
  logMessage("Instagram: " + String(INSTAGRAM_USERNAME));
  logMessage("Initializing...");
  
  // Setup WiFi
  setupWiFi();
  
  // Setup Web Server
  setupWebServer();
  
  // Start Server
  server.begin();
  logMessage("✓ Web Server started on http://" + WiFi.localIP().toString());
}

// ===== WiFi Setup =====
void setupWiFi() {
  logMessage("Connecting to WiFi: " + String(ssid));
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    logMessage("\n✓ WiFi Connected!");
    logMessage("IP Address: " + WiFi.localIP().toString());
    logMessage("Signal Strength: " + String(WiFi.RSSI()) + " dBm");
  } else {
    logMessage("\n✗ WiFi Connection Failed!");
  }
}

// ===== Web Server Setup =====
void setupWebServer() {
  // Serve static files from SPIFFS
  server.serveStatic("/", SPIFFS, "/www/").setDefaultFile("index.html");
  
  // API Routes
  server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    DynamicJsonDocument doc(JSON_BUFFER_SIZE);
    doc["status"] = "online";
    doc["device"] = "ESP32-S3";
    doc["instagram"] = INSTAGRAM_USERNAME;
    doc["uptime"] = millis() / 1000;
    doc["rssi"] = WiFi.RSSI();
    doc["ram_free"] = ESP.getFreeHeap();
    doc["psram_free"] = ESP.getFreePsram();
    
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });
  
  server.on("/api/chat", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleAIRequest);
  
  // CORS Headers
  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404, "application/json", "{\"error\":\"Not Found\"}");
  });
  
  logMessage("✓ Web Server routes configured");
}

// ===== Handle AI Request =====
void handleAIRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  if (index == 0) {
    logMessage("\n→ AI Request received");
  }
  
  if (index + len == total) {
    String body = String((char*)data);
    logMessage("Body: " + body);
    
    // Parse JSON
    DynamicJsonDocument requestDoc(JSON_BUFFER_SIZE);
    DeserializationError error = deserializeJson(requestDoc, body);
    
    if (error) {
      DynamicJsonDocument errorDoc(256);
      errorDoc["error"] = "Invalid JSON";
      String errorResponse;
      serializeJson(errorDoc, errorResponse);
      request->send(400, "application/json", errorResponse);
      return;
    }
    
    String userMessage = requestDoc["message"] | "";
    
    if (userMessage.length() == 0) {
      DynamicJsonDocument errorDoc(256);
      errorDoc["error"] = "Empty message";
      String errorResponse;
      serializeJson(errorDoc, errorResponse);
      request->send(400, "application/json", errorResponse);
      return;
    }
    
    // Call OpenAI API
    String aiResponse = callOpenAIAPI(userMessage);
    
    // Send response
    DynamicJsonDocument responseDoc(JSON_BUFFER_SIZE);
    responseDoc["success"] = true;
    responseDoc["user_message"] = userMessage;
    responseDoc["ai_response"] = aiResponse;
    responseDoc["timestamp"] = millis();
    
    String response;
    serializeJson(responseDoc, response);
    request->send(200, "application/json", response);
  }
}

// ===== Call OpenAI API =====
String callOpenAIAPI(String userMessage) {
  logMessage("Calling OpenAI API...");
  
  if (WiFi.status() != WL_CONNECTED) {
    logMessage("✗ WiFi not connected");
    return "WiFi bağlantı hatası";
  }
  
  wifiClientSecure.setCACert(nullptr);  // Only for testing!
  
  HTTPClient http;
  http.begin(wifiClientSecure, "https://api.openai.com/v1/chat/completions");
  
  // Set headers
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + String(OPENAI_API_KEY));
  
  // Create request body
  DynamicJsonDocument requestBody(JSON_BUFFER_SIZE);
  requestBody["model"] = OPENAI_MODEL;
  requestBody["temperature"] = 0.7;
  requestBody["max_tokens"] = 500;
  
  JsonArray messages = requestBody.createNestedArray("messages");
  JsonObject sysMsg = messages.createNestedObject();
  sysMsg["role"] = "system";
  sysMsg["content"] = "Sen Türkçe konuşan, yardımsever bir yapay zeka asistanısın. Kısa, net cevaplar ver. İnstagram: ish_x06";
  
  JsonObject userMsg = messages.createNestedObject();
  userMsg["role"] = "user";
  userMsg["content"] = userMessage;
  
  String jsonRequest;
  serializeJson(requestBody, jsonRequest);
  
  int httpCode = http.POST(jsonRequest);
  
  String aiResponse = "Bağlantı hatası";
  
  if (httpCode == 200) {
    String payload = http.getString();
    DynamicJsonDocument responseDoc(JSON_BUFFER_SIZE * 2);
    
    if (deserializeJson(responseDoc, payload) == DeserializationError::Ok) {
      JsonArray choices = responseDoc["choices"];
      if (choices.size() > 0) {
        aiResponse = choices[0]["message"]["content"].as<String>();
        logMessage("✓ API Response: " + aiResponse);
      }
    }
  } else {
    logMessage("✗ API Error: HTTP " + String(httpCode));
    aiResponse = "API hatası: HTTP " + String(httpCode);
  }
  
  http.end();
  return aiResponse;
}

// ===== Utility Functions =====
void logMessage(String msg) {
  Serial.println("[" + String(millis() / 1000) + "s] " + msg);
}

// ===== Loop Function =====
void loop() {
  delay(1000);
  
  // Optional: Print WiFi status every 60 seconds
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 60000) {
    lastCheck = millis();
    if (WiFi.status() == WL_CONNECTED) {
      logMessage("WiFi Status: Connected | RSSI: " + String(WiFi.RSSI()) + " dBm");
    } else {
      logMessage("WiFi Status: Disconnected");
    }
  }
}
