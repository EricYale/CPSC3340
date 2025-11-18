#include <esp_now.h>
#include <WiFi.h>

uint led_pin = 32;
uint button_pin = 33;
uint8_t broadcastAddr[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
esp_now_peer_info_t broadcastPeer;

typedef struct
{
    bool light_set;
} mymsg;

bool light_state = false; // Track local light state
bool last_button_state = HIGH; // For edge detection

void OnDataRecv(const esp_now_recv_info* recv_info, const uint8_t* data, int len)
{
    if (len != sizeof(mymsg))
    {
        Serial.println("Received invalid msg");
        return;
    }

    char macstr[32];
    const uint8_t* mac = recv_info->src_addr;
    snprintf(macstr, 31, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    mymsg* msg = (mymsg*)data;
    Serial.printf("[%s] Received: light_set=%d\n", macstr, msg->light_set);
    light_state = msg->light_set; // Update local light state
    digitalWrite(led_pin, light_state);
}

void OnDataSent(const wifi_tx_info_t* tx_info, esp_now_send_status_t stat)
{
    char macstr[32];
    if (tx_info != nullptr) {
        const uint8_t* mac = tx_info->des_addr; // use dest_addr field from wifi_tx_info_t
        snprintf(macstr, 31, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    } else {
        strncpy(macstr, "Unknown", sizeof(macstr) - 1);
        macstr[sizeof(macstr) - 1] = '\0';
    }

    Serial.printf("[%s] Last Packet Sent: %s\n", macstr, stat == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup()
{
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);

    pinMode(button_pin, INPUT_PULLUP);
    pinMode(led_pin, OUTPUT);

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Can't start espnow, rebooting");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        ESP.restart();
    }

    broadcastPeer.channel = 0; // 0 = any
    broadcastPeer.encrypt = false; // can't encrypt when broadcasting
    memcpy(broadcastPeer.peer_addr, broadcastAddr, 6); // copy broadcast address

    if (esp_now_add_peer(&broadcastPeer) != ESP_OK)
    {
        Serial.println("Can't register espnow broadcast peer, rebooting");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        ESP.restart();
    }
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);
}
void loop() 
{
    bool button_state = digitalRead(button_pin);

    // Detect falling edge (button press)
    if (last_button_state == HIGH && button_state == LOW) {
        light_state = !light_state; // Update local light state immediately
        digitalWrite(led_pin, light_state); // Reflect change on LED
        mymsg msg;
        msg.light_set = light_state;
        esp_now_send(broadcastAddr, (uint8_t*)&msg, sizeof(msg));
    }
    last_button_state = button_state;
}