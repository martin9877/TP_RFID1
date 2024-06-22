#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define SS_PIN 5  // Pin de selección del lector RFID
#define RST_PIN 4 // Pin de reset del lector RFID

const int amarillo  = 13;
const int rojo  = 25;
const int verde = 26 ; 


const char* ssid = "Martin"; // Cambia por tu SSID
const char* password = "11208dafff"; // Cambia por tu contraseña WiFi
const char* mqtt_server = "181.229.113.249"; // Cambia por la dirección IP del broker MQTT
const int mqtt_port = 1883; // Puerto MQTT predeterminado
//const char* mqtt_user = "TU_USUARIO"; // Cambia por el usuario del broker MQTT
//const char* mqtt_password = "TU_CONTRASEÑA"; // Cambia por la contraseña del broker MQTT

const char* RFIDCARD_topic = "RFIDCARD"; // Tópico MQTT para publicar el número de la tarjeta RFID
const char* RFIDRESP_topic = "RFIDRESP"; // Tópico MQTT para suscribirse a respuestas

WiFiClient espClient;
PubSubClient client(espClient);

MFRC522 mfrc522(SS_PIN, RST_PIN); // Crear instancia del lector RFID

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido ");
  Serial.print(topic);
  Serial.print("");
  String mensaje = "" ;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    mensaje += (char)payload[i];
  }
  Serial.println(mensaje);



if (mensaje == "[]") {
  digitalWrite(verde,LOW);
  digitalWrite(rojo,HIGH);
  digitalWrite(amarillo,LOW);
  delay(1100);
digitalWrite(verde,LOW);
  digitalWrite(rojo,LOW);
  digitalWrite(amarillo,HIGH);
}
else {
  digitalWrite(verde,HIGH);
  digitalWrite(rojo,LOW);
  digitalWrite(amarillo,LOW);
  delay(900);
  digitalWrite(verde,LOW);
  digitalWrite(rojo,LOW);
  digitalWrite(amarillo,HIGH);
}

}








void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado al broker MQTT");
      client.subscribe(RFIDRESP_topic);
    } else {
      Serial.print("falló, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  SPI.begin(); // Inicializar SPI bus
  mfrc522.PCD_Init(); // Inicializar MFRC522
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
pinMode(amarillo, OUTPUT);
pinMode(verde, OUTPUT);
pinMode(rojo, OUTPUT);

}

void loop() {
 digitalWrite(amarillo,HIGH);
  if (!client.connected()) {
    reconnect();
    
  }
  client.loop();

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String content = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    content.toUpperCase();
    Serial.print("Tarjeta RFID leída: ");
    Serial.println(content);

    // Publicar el número de tarjeta RFID en el tópico RFIDCARD
    if (client.publish(RFIDCARD_topic, content.c_str())) {
      Serial.println("Publicado en el tópico RFIDCARD");
    } else {
      Serial.println("Fallo al publicar en el tópico RFIDCARD");
    }
    delay(1000); // Evitar lecturas múltiples
  }
}
