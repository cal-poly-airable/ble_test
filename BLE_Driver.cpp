/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/
#include "BLE_Driver.h"

// Global Varibles Declared in headerfile

volatile int OXYGEN_SAT = 0;
volatile int HEART_RATE = 0;
volatile int DEVICE_CONNECTED = 0;

// Define Server Callback Handlers
class ventCallback: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    // Device Connected
    DEVICE_CONNECTED = 1;
    Serial.println("Connection Established to iPhone");
  }
  void onDisconnect(BLEServer* pServer) {
    // Device Disconnected
    DEVICE_CONNECTED = 0;
    Serial.println("Connection to iPhone Terminated");
    // ReEstablish Advertising
    beginAdvertising();      
  }
};

// Define Characteristic CallBack Handelers
class oxygenCallback: public BLECharacteristicCallbacks {
  // Define Read and Write Subroutines
  void onRead(BLECharacteristic* pCharacteristic) {
  }
  void onWrite(BLECharacteristic* pCharacteristic) {
    int new_ox_sat;
    try {
      new_ox_sat = stoi(pCharacteristic->getValue());
      OXYGEN_SAT = new_ox_sat;
      Serial.print("Oxygen Sat From iPhone: ");
      Serial.println(OXYGEN_SAT);
    }
    catch(std::invalid_argument) {
    Serial.println("ERROR: Unreconized Characteristic Input");
    pCharacteristic->setValue(std::to_string(OXYGEN_SAT));
    }
    
  }
};

class heartRateCallback: public BLECharacteristicCallbacks {
  // Define Read and Write Subroutines
  void onRead(BLECharacteristic* pCharacteristic) {

  }
  void onWrite(BLECharacteristic* pCharacteristic){
    int new_hr;
    try {
      new_hr = stoi(pCharacteristic->getValue());
      HEART_RATE = new_hr;
      Serial.print("Heart Rate From iPhone: ");
      Serial.println(HEART_RATE);
    }
    catch(std::invalid_argument) {
    Serial.println("ERROR: Unreconized Characteristic Input");
    pCharacteristic->setValue(std::to_string(HEART_RATE));
    }
  }
};

void CheckBLEStatus(void);

void BLEServerInit(void)
{
  // Serial for debugging
  Serial.begin(115200);
  Serial.println("Starting Server");

  // Define BLE Server
  BLEDevice::init("Ventigator BLE Server");
  BLEServer *ventServer = BLEDevice::createServer();

  // Link Callback Handeler to BLEServer
  ventServer->setCallbacks(new ventCallback());

  // Define Main Service
  BLEService *ventService = ventServer->createService(SERVICE_UUID);

  // Define Characteristics
  // Oxygen Saturation
  BLECharacteristic *oxygen_sat_char = ventService->createCharacteristic(
                                                    O2_CHARACTERISTIC_UUID,
                                                    BLECharacteristic::PROPERTY_READ |
                                                    BLECharacteristic::PROPERTY_WRITE);
  // Heart Rate
  BLECharacteristic *heart_rate_char = ventService->createCharacteristic(
                                                    HR_CHARACTERISTIC_UUID,
                                                    BLECharacteristic::PROPERTY_READ |
                                                    BLECharacteristic::PROPERTY_WRITE);

  // Initalize Characteristc Values
  oxygen_sat_char->setValue("0");
  heart_rate_char->setValue("0");
  
  // Link Callback Handlers to Characterstics
  oxygen_sat_char->setCallbacks(new oxygenCallback());
  heart_rate_char->setCallbacks(new heartRateCallback());

  // Start Main Service
  ventService->start();

  //BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility

  beginAdvertising();

}

void beginAdvertising(void) {
  // Start Advertising Server
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}
