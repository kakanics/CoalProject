#include <WiFi.h>
#include <PubSubClient.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include "ThingSpeak.h"

// Wi-Fi credentials
const char *ssid = "";
const char *password = "";

// MQTT configuration
const char *mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char *mqttClientId = "";
const char *outputTopic = "";
WiFiClient espClient;

// ThingSpeak configuration
unsigned long myChannelNumber = 0;
const char *myWriteAPIKey = "";
String myStatus = "";

// Firebase configuration
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;

static unsigned long lastMillis = 0;
PubSubClient client(espClient);

void setup()
{
    Serial.begin(115200);
    Serial2.begin(9600);
    WifiSetup();

    // configure cloud services
    fireBaseSetup();
    ThingSpeak.begin(espClient);
    ThingSpeak.setStatus(myStatus);

    // Configure MQTT
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callBack);
    connectToMQTT();
}

void WifiSetup()
{
    // Connect to Wi-Fi
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    // keep retrying until successful connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void fireBaseSetup()
{
    // configure firebase
    config.api_key = "";
    config.database_url = "";

    // try to connect
    if (Firebase.signUp(&config, &auth, "", ""))
    {
        Serial.println("ok");
        signupOK = true;
    }
    else
    {
        Serial.printf("%s\n", config.signer.signupError.message.c_str());
    }
    config.token_status_callback = tokenStatusCallback;

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
}

void callBack(char *inputTopic, byte *message, unsigned int length)
{
    // Callback function for MQTT messages
    Serial.print("Message arrived on topic: ");
    Serial.print(inputTopic);
    Serial.print(". Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }
    Serial.println();
}

void loop()
{
    int data = -1; //-1 for data=not read

    // reconnect to wifi if disconnected
    if (WiFi.status() != WL_CONNECTED)
    {
        WifiSetup();
    }
    // Handle MQTT events
    if (client.connected() == false)
    {
        connectToMQTT();
    }
    client.loop();

    // read data from arduino
    if (Serial2.available() > 0)
    {
        data = Serial2.read();
        Serial.println(data);
        // clear the buffer in case there are too many messages
        while (Serial2.available())
        {
            Serial2.read();
        }
    }

    // Publish a message every 5 seconds
    if (millis() - lastMillis > 1000 && data != -1)
    {
        char message[50];
        snprintf(message, sizeof(message), "Light Intensity: %d", data);
        publishMessage(message);
        Serial.println(message);

        // publish data to cloud
        ThingSpeakPublishData(1, data);
        firebasePublishData("test/int", data);
    }
    delay(100);
}

void ThingSpeakPublishData(int field, int data)
{
    // Publish data to ThingSpeak
    ThingSpeak.setField(1, data);
    int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (httpCode == 200)
    {
        Serial.println("Channel update successful.");
    }
    else
    {
        Serial.println("Problem updating channel. HTTP error code " + String(httpCode));
    }
}

void firebasePublishData(String address, int data)
{
    // Publish data to Firebase
    if (Firebase.ready() && signupOK)
    {
        if (Firebase.RTDB.setInt(&fbdo, address, data))
        {
            Serial.println("PASSED");
            Serial.println("PATH: " + fbdo.dataPath());
            Serial.println("TYPE: " + fbdo.dataType());
        }
        else
        {
            Serial.println("FAILED");
            Serial.println("REASON: " + fbdo.errorReason());
        }
    }
}

void connectToMQTT()
{
    while (!client.connected())
    {
        Serial.println("Connecting to MQTT...");
        if (client.connect(mqttClientId))
        {
            Serial.println("Connected to MQTT");
        }
        else
        {
            Serial.print("Failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
}

void publishMessage(const char *message)
{
    if (client.connected())
    {
        client.publish(outputTopic, message);
        Serial.println("Message Published");
    }
}