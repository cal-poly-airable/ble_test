#ifndef BLE_DRIVER_H
#define BLE_DRIVER_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <string>
#include <Arduino.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID            "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define O2_CHARACTERISTIC_UUID  "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define HR_CHARACTERISTIC_UUID  "9992a1aa-4b52-11ed-bdc3-0242ac120002"

// Global Varibles used in this scope

extern volatile int DEVICE_CONNECTED;
extern volatile int OXYGEN_SAT;
extern volatile int HEART_RATE;

// Function prototypes
void BLEServerInit(void);
void CheckBLE_Status(void);
void beginAdvertising();

#endif