#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <Update.h>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  if (data[0] == 'U' && data[1] == 'P') {
    Serial.println("Received firmware update command!");

    WiFiClient client;
    const int httpPort = 80;

    if (!client.connect("domain.com", httpPort)) {
      Serial.println("Failed to connect to firmware server!");
      return;
    }

    client.print(String("GET /firmware.ino.bin HTTP/1.1\r\n") +
                 "Host: domain.com\r\n" +
                 "Connection: close\r\n\r\n");

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("Headers received, starting firmware update...");
        if (Update.begin(UPDATE_SIZE_UNKNOWN)) {
          size_t written = Update.writeStream(client);
          if (written == client.getSize()) {
            Serial.println("Firmware update complete!");
          } else {
            Serial.println("Firmware update failed!");
          }
          if (Update.end()) {
            Serial.println("Rebooting...");
            ESP.restart();
          } else {
            Serial.println("Error occurred during firmware update!");
          }
        } else {
          Serial.println("Not enough space to begin firmware update!");
        }
        break;
      }
    }
    client.stop();
  }
}

void setupOTA() {
  ws.onEvent(handleWebSocketMessage);
  server.addHandler(&ws);
  server.begin();
}

void handleOTA() {
  // Handle OTA update commands
  ws.cleanupClients();
}
