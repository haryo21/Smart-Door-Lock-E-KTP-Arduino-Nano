#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define LED_G 3     // define green LED pin
#define LED_R 2     // define red LED pin
#define RELAY 4     // relay pin
#define BUZZER 6    // buzzer pin
#define ACCESS_DELAY 10000
#define DENIED_DELAY 1000

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

void setup() {
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();          // Initiate SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  digitalWrite(RELAY, HIGH);

  Serial.println("Tempelkan Kartu E-KTP yang terdaftar!!");
  Serial.println();

  // Cek versi software MFRC555
  byte versi = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  if (versi == 0x00 || versi == 0xFF) {
    while (true); // Abaikan perintah-2 berikutnya
  }
}

void loop() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();

  if (content.substring(1) == "05 82 C7 20 4C A1 00") // UID TERDAFTAR 1
  {
    Serial.println("Akses diterima!");
    Serial.println();
    delay(500);
    digitalWrite(RELAY, LOW);
    digitalWrite(LED_G, HIGH);
    delay(ACCESS_DELAY);
    digitalWrite(RELAY, HIGH);
    digitalWrite(LED_G, LOW);
  }
  else if (content.substring(1) == "05 81 C7 20 4C A1 00") // UID TERDAFTAR 2
  {
    Serial.println("Akses diterima!");
    Serial.println();
    delay(500);
    digitalWrite(RELAY, LOW);
    digitalWrite(LED_G, HIGH);
    delay(ACCESS_DELAY);
    digitalWrite(RELAY, HIGH);
    digitalWrite(LED_G, LOW);
  }
  else {
    Serial.println("Akses ditolak!");
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 300);
    delay(DENIED_DELAY);
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);
  }
  mfrc522.PICC_HaltA();
}
