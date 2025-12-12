/*

This was a vibe-coded attempt at making a networked
game for ESP32s. It would be a "buzzer" reaction time game
where everyone tries to press their button as fast as possible
after a light turns on. Unfortunately I never got it to work. :(

*/




#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

uint led_pin = 32;
uint button_pin = 33;
uint8_t broadcastAddr[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
esp_now_peer_info_t broadcastPeer;

uint8_t lowest_mac[6];
bool mac_initialized = false;

typedef struct
{
    bool game_active;
    bool winner_found;
    uint8_t winner_mac[6];
    uint8_t announce_mac[6];
} gamemsg;

uint8_t my_mac[6];
bool is_master = false;
unsigned long last_master_check = 0;
unsigned long game_start_time = 0;
unsigned long winner_flash_start = 0;
bool light_state = false;
bool last_button_state = HIGH;
bool winner = false;
bool game_active = false;
bool winner_found = false;
uint8_t winner_mac[6] = {0};

void elect_master() {
    if (!mac_initialized) {
        memcpy(lowest_mac, my_mac, 6);
        mac_initialized = true;
    }
    is_master = (memcmp(my_mac, lowest_mac, 6) == 0);
}

void OnDataRecv(const esp_now_recv_info* recv_info, const uint8_t* data, int len)
{
    if (len != sizeof(gamemsg)) return;
    gamemsg* msg = (gamemsg*)data;

    if (!mac_initialized) {
        memcpy(lowest_mac, my_mac, 6);
        mac_initialized = true;
    }
    if (memcmp(msg->announce_mac, lowest_mac, 6) < 0) {
        memcpy(lowest_mac, msg->announce_mac, 6);
    }

    game_active = msg->game_active;
    winner_found = msg->winner_found;
    memcpy(winner_mac, msg->winner_mac, 6);
    if (winner_found && memcmp(winner_mac, my_mac, 6) == 0) {
        winner = true;
        winner_flash_start = millis();
    } else {
        winner = false;
    }
    if (game_active && !winner_found) {
        light_state = true;
        digitalWrite(led_pin, HIGH);
    } else if (!game_active && !winner_found) {
        light_state = false;
        digitalWrite(led_pin, LOW);
    }
    if (winner_found && !winner) {
        digitalWrite(led_pin, LOW);
    }
}

void OnDataSent(const wifi_tx_info_t* tx_info, esp_now_send_status_t stat) {}

void setup()
{
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    esp_wifi_get_mac(WIFI_IF_STA, my_mac);
    pinMode(button_pin, INPUT_PULLUP);
    pinMode(led_pin, OUTPUT);
    if (esp_now_init() != ESP_OK) {
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        ESP.restart();
    }
    broadcastPeer.channel = 0;
    broadcastPeer.encrypt = false;
    memcpy(broadcastPeer.peer_addr, broadcastAddr, 6);
    esp_now_add_peer(&broadcastPeer);
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);
    memcpy(lowest_mac, my_mac, 6);
    mac_initialized = true;
    elect_master();
}

void loop()
{
    if (millis() - last_master_check > 5000) {
        elect_master();
        last_master_check = millis();
        gamemsg announce;
        announce.game_active = game_active;
        announce.winner_found = winner_found;
        memcpy(announce.winner_mac, winner_mac, 6);
        memcpy(announce.announce_mac, my_mac, 6);
        esp_now_send(broadcastAddr, (uint8_t*)&announce, sizeof(announce));
    }
    if (is_master) {
        if (!game_active && !winner_found) {
            if (millis() - game_start_time > random(2000, 7000)) {
                gamemsg msg;
                msg.game_active = true;
                msg.winner_found = false;
                memset(msg.winner_mac, 0, 6);
                memcpy(msg.announce_mac, my_mac, 6);
                esp_now_send(broadcastAddr, (uint8_t*)&msg, sizeof(msg));
                game_active = true;
            }
        }
        if (game_active && !winner_found) {
            bool button_state = digitalRead(button_pin);
            if (last_button_state == HIGH && button_state == LOW) {
                gamemsg msg;
                msg.game_active = false;
                msg.winner_found = true;
                memcpy(msg.winner_mac, my_mac, 6);
                memcpy(msg.announce_mac, my_mac, 6);
                esp_now_send(broadcastAddr, (uint8_t*)&msg, sizeof(msg));
                winner_found = true;
                winner = true;
                winner_flash_start = millis();
            }
            last_button_state = button_state;
        }
        if (winner_found) {
            if (millis() - winner_flash_start > 2000) {
                gamemsg msg;
                msg.game_active = false;
                msg.winner_found = false;
                memset(msg.winner_mac, 0, 6);
                memcpy(msg.announce_mac, my_mac, 6);
                esp_now_send(broadcastAddr, (uint8_t*)&msg, sizeof(msg));
                game_active = false;
                winner_found = false;
                winner = false;
                game_start_time = millis();
            }
        }
        if (!game_active && !winner_found) {
            game_start_time = millis();
        }
    } else {
        bool button_state = digitalRead(button_pin);
        if (game_active && !winner_found && last_button_state == HIGH && button_state == LOW) {
            gamemsg msg;
            msg.game_active = false;
            msg.winner_found = true;
            memcpy(msg.winner_mac, my_mac, 6);
            memcpy(msg.announce_mac, my_mac, 6);
            esp_now_send(broadcastAddr, (uint8_t*)&msg, sizeof(msg));
        }
        last_button_state = button_state;
    }
    if (winner) {
        digitalWrite(led_pin, (millis() / 50) % 2);
    }
}