#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h> // For LCD display

#define SS_PIN 10  // Slave select pin for RFID
#define RST_PIN 9  // Reset pin for RFID
MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of RFID reader

// Define LCD pins
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Product data (hardcoded for simplicity)
String products[] = {"Milk", "Bread", "Eggs", "Juice"};
float prices[] = {1.25, 0.85, 2.15, 1.50};

// For tracking the total price
float totalAmount = 0;

void setup() {
  SPI.begin();
  rfid.PCD_Init();
  
  lcd.begin(16, 2); // Start the LCD (16x2)
  lcd.print("Smart Trolley");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Check if a new card is present
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Read the UID (unique identifier) from the RFID tag
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i], HEX);
  }

  lcd.clear();
  lcd.print("Item Scanned!");

  // Match the UID to a product (example UIDs are hardcoded)
  if (uid == "ab12cd34") {  // UID of first RFID tag
    addItem(0);  // Index for Milk
  } else if (uid == "ef56gh78") {  // UID of second RFID tag
    addItem(1);  // Index for Bread
  } else if (uid == "ij90kl12") {  // UID of third RFID tag
    addItem(2);  // Index for Eggs
  } else if (uid == "mn34op56") {  // UID of fourth RFID tag
    addItem(3);  // Index for Juice
  } else {
    lcd.clear();
    lcd.print("Unknown Item");
    delay(2000);
  }

  rfid.PICC_HaltA();  // Stop reading the card
}

// Function to add item to the total and display it
void addItem(int index) {
  lcd.clear();
  lcd.print(products[index]);
  lcd.setCursor(0, 1);
  lcd.print("Price: $");
  lcd.print(prices[index]);

  totalAmount += prices[index];
  delay(2000);

  lcd.clear();
  lcd.print("Total: $");
  lcd.print(totalAmount);
}
