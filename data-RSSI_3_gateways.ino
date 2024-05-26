#include <MKRWAN.h>

LoRaModem modem;

// Déclaration individuelle pour chaque App EUI et App Key
String appEui1 = "A84041FFFF26606C";
String appKey1 = "NNSXS.VUAXE5NF6VXHNJTOAOE6CV6FLG53ZMPNNELUQRA.5ZJAZVOIYETAE6DOGB4J4XI7D4UMIC7AF7XB7NCTNNZOYIKCCGWQ";

String appEui2 = "A84041FFFF27D838";
String appKey2 = "NNSXS.XUS5QB26P5OR6GKUZ6HTT7ZL3FC4BY6RK6XR76Q.4V3DTDKGNDEWOGJ3VGNZR4XHJJHI6RHVMVWDF4MV5ZR7SQU6HNWQ";

String appEui3 = "A84041FFFF2661DC";
String appKey3 = "NNSXS.IQILE5K5GOCZVUCVBUKMLHW2UAATWX3RNGRWARA.5UDT6NTX5HWXVO56TY32OJ4ESX3FWEJWNLV2NHPVVVLL7WIFRQCQ";

void setup() {
  Serial.begin(115200);
  while (!Serial);  // Attend que la console série soit prête

  // Configuration pour chaque réseau
  Serial.println("Configuring for network 1");
  configureNetwork(appEui1, appKey1);

  Serial.println("Configuring for network 2");
  configureNetwork(appEui2, appKey2);

  Serial.println("Configuring for network 3");
  configureNetwork(appEui3, appKey3);

  delay(1);
}

void configureNetwork(String eui, String key) {
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  }

  Serial.print("Module version: ");
  Serial.println(modem.version());
  Serial.print("Device EUI: ");
  Serial.println(modem.deviceEUI());
  modem.sendMask("ff000000f000ffff00020000");

  Serial.println("Attempting to join network...");
  if (modem.joinOTAA(eui, key)) {
    Serial.println("Connected.");
  } else {
    Serial.println("Failed to connect.");
  }
  modem.minPollInterval(60);  // Interval minimum de poll à 60 secondes
}

void loop() {
  String msg = "BEES";
  Serial.print("Sending: " + msg + " - ");
  printHex(msg);

  modem.beginPacket();
  modem.print(msg);
  modem.endPacket(false);

  handleDownlink();
  Serial.println("Waiting for next TX");
  delay(1); // Intervalles de 2 minutes
}

void printHex(String msg) {
  for (unsigned int i = 0; i < msg.length(); i++) {
    Serial.print(msg[i] >> 4, HEX);
    Serial.print(msg[i] & 0xF, HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void handleDownlink() {
  if (!modem.available()) {
    Serial.println("No downlink message received at this time.");
    return;
  }
  
  char rcv[64];
  int i = 0;
  while (modem.available()) {
    rcv[i++] = (char)modem.read();
  }
  Serial.print("Received: ");
  for (unsigned int j = 0; j < i; j++) {
    Serial.print(rcv[j] >> 4, HEX);
    Serial.print(rcv[j] & 0xF, HEX);
    Serial.print(" ");
  }
  Serial.println();
}
