#include <WiFi.h>
#include <WiFiUdp.h>

WiFiUDP udp;

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_MODE_STA);

    WiFi.begin("yale wireless");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    pinMode(32, INPUT);
    pinMode(34, INPUT);
}

void loop() {
    udp.beginPacket("10.66.236.77", 7531);
    udp.print(analogRead(32));
    udp.print("/");
    udp.print(analogRead(34));
    udp.endPacket();

    delay(100);
}
