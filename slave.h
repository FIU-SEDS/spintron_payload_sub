#include <esp_now.h>
#include <WiFi.h>

// gabriel

String success;
void OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
  // Serial.print("Last Packet Send Status:\t");
  if (status != ESP_NOW_SEND_SUCCESS) {
    Serial.println("Delivery Fail");
  }
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status == 0) success = "Delivery Success :)";
  else success = "Delivery Fail :(";
}


// REPLACE WITH THE MAC Address of your receiver
uint8_t broadcastAddress[] = { 0xCC, 0x8D, 0xA2, 0x10, 0x2D, 0xDC };  //{0x24, 0x62, 0xAB, 0xE0, 0x64, 0x54};

// Variable to store if sending data was successful


//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
  float force;
  bool rotation;
} struct_message;

// Create a struct_message called BME280Readings to hold sensor readings
struct_message sending_struct;

// Create a struct_message to hold incoming sensor readings
struct_message incomingReadings;
bool incomingReadingsOK = false;

esp_now_peer_info_t peerInfo;

// Callback when data is sent

void steupSlave() {

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  delay(100);  // short delay for initialisation
  Serial.print("WiFi MAC address ");
  Serial.println(WiFi.macAddress());

  // Init ESP-NOWesp now max message size
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  delay(100);  // short delay for initialisation
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.printf("ESP-NOW initialised MAC address %02X:%02X:%02X:%02X:%02X:%02X\n",
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  // esp_now_register_recv_cb(OnDataRecv);
}

void sendData(float force, bool rotation) {
  // Set values to send

  sending_struct.force = force;
  sending_struct.rotation = rotation;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t*)&sending_struct, sizeof(sending_struct));

  if (result != ESP_OK) Serial.println("esp_now_send() returns Error");
}
