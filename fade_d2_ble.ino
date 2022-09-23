#include <ArduinoBLE.h>

#include "mbed.h"
#include "KVStore.h"
#include "kvstore_global_api.h"

#define myled D2

BLEService ledService("180A"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("2A57", BLERead | BLEWrite);

static const int initialPwmValue = 256-25;
int nvmMagic = 0x18c1;
int pwmValue;

static void initNvm() {
  int readMagic=0;
  
  int res = kv_get("/kv/magic", &readMagic, sizeof(int), NULL);
  if (res != MBED_SUCCESS || readMagic != nvmMagic) {
    Serial.println("res != MBED_SUCCESS || readMagic != nvmMagic, needs initialize");
    Serial.print("info: res= ");
    Serial.println(res, DEC);
    Serial.print("info: readMagic= ");
    Serial.println(readMagic, DEC);
    
    pwmValue = initialPwmValue;

    // Reset the nvmem storage when not yet initialized
    res = kv_reset("/kv/");
    res = kv_set("/kv/magic", &nvmMagic, sizeof(int), 0);
    res = kv_set("/kv/value", &pwmValue, sizeof(int), 0);

    Serial.print("Wrote PWM Value to kvstore: ");
    Serial.println(pwmValue, DEC);
  }
  else {
    kv_get("/kv/value", &pwmValue, sizeof(int), 0);
    Serial.print("Read PWM Value from kvstore: ");
    Serial.println(pwmValue, DEC);
  }
}


void setup() {
  Serial.begin(9600);
  //while (!Serial);

  initNvm();
  
  // set LED's pin to output mode
  pinMode(myled, OUTPUT);
  analogWrite(myled, pwmValue);
  
  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("Nano 33 BLE Sense");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);

  // add service
  BLE.addService(ledService);

  // set the initial value for the characteristic:
  switchCharacteristic.writeValue(pwmValue);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}

void loop() {
  // listen for Bluetooth® Low Energy peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, HIGH);            // turn on the LED to indicate the connection

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (switchCharacteristic.written()) {
        pwmValue = switchCharacteristic.value();
        analogWrite(myled, pwmValue);
      }
    }

    // meh, how to error handle?
    int res = kv_set("/kv/value", &pwmValue, sizeof(int), 0);
    Serial.print("Wrote PWM Value to kvstore: ");
    Serial.println(pwmValue, DEC);
    Serial.print("res: ");
    Serial.println(res, DEC);

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, LOW);         // when the central disconnects, turn off the LED

  }
}
