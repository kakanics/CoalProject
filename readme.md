# ESP32 IoT Project Documentation

## Overview

This code is designed for an ESP32 microcontroller to enable IoT functionalities. It integrates with Wi-Fi, MQTT, ThingSpeak, and Firebase services to facilitate communication and data storage. The primary goal is to read data from an Arduino device connected via Serial2 and publish it to both ThingSpeak and Firebase, while also providing real-time updates via MQTT.

## Dependencies

- **WiFi.h:** Library for connecting to Wi-Fi networks.
- **PubSubClient.h:** MQTT library for ESP32.
- **Firebase_ESP_Client.h:** Library for interfacing with Firebase Realtime Database.
- **ThingSpeak.h:** Library for sending data to ThingSpeak IoT platform.

## Configuration

### Wi-Fi Credentials

- **SSID:** HIDDEN
- **Password:** HIDDEN

### MQTT Configuration

- **Broker:** broker.hivemq.com
- **Port:** 1883
- **Client ID:** HIDDEN
- **Output Topic:** HIDDEN

### ThingSpeak Configuration

- **Channel Number:** HIDDEN
- **Write API Key:** HIDDEN

### Firebase Configuration

- **API Key:** HIDDEN
- **Database URL:** HIDDEN

## Setup

1. Initialize serial communication at a baud rate of 115200.
2. Begin Serial2 communication at 9600 baud for Arduino data transfer.
3. Configure Wi-Fi using the provided credentials.
4. Setup and configure Firebase and ThingSpeak services.
5. Configure MQTT, set the callback function, and establish a connection.

## Main Functions

### `WifiSetup()`

- Connects to the specified Wi-Fi network.
- Prints the IP address upon successful connection.

### `fireBaseSetup()`

- Configures Firebase with the provided API key and database URL.
- Attempts to sign up and prints status.
- Sets token status callback and initializes Firebase.

### `callBack(char *inputTopic, byte *message, unsigned int length)`

- Callback function for MQTT messages.
- Prints the arrived message and topic.

### `loop()`

- Continuously checks and handles Wi-Fi, MQTT, and Serial2 data.
- Publishes data to ThingSpeak and Firebase.

### `ThingSpeakPublishData(int field, int data)`

- Publishes data to ThingSpeak for the specified field.

### `firebasePublishData(string address, int data)`

- Publishes data to Firebase at the given address.

### `connectToMQTT()`

- Attempts to connect to the MQTT broker.

### `publishMessage(const char *message)`

- Publishes a message to the specified MQTT topic.

## Data Flow

1. Connect to Wi-Fi.
2. Setup Firebase and ThingSpeak.
3. Configure MQTT and establish a connection.
4. Continuously check for Serial2 data.
5. If data is available, publish it to ThingSpeak, Firebase, and MQTT.
6. Repeat the process every 5 seconds.

## Notes

- The system uses an MQTT broker for real-time communication.
- Data is read from an Arduino device connected via Serial2.
- Published data includes light intensity information.
- Real-time updates are provided through MQTT.
- Data is stored in both ThingSpeak and Firebase for historical reference.
- Wi-Fi reconnection is attempted if the connection is lost.

