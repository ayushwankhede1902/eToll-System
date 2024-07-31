#include <BitBool.h>
#include <OnewireKeypad.h>
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;

// Pin definitions
#define sensorPin1 2
#define sensorPin2 6
#define RST_PIN 8
#define SS_PIN 10

// Card balances
int card1Balance = 2000;
int card2Balance = 400;

// Variables
int servoPos = 0;
int senVal1 = 0;
int senVal2 = 0;
int state = 0;
char Key;
bool recharge = true;

// RFID setup
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Keypad setup
#define num 7
char Data[num];
byte data_count = 0;
String num1, num2, card;
char KEYS[] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};
OnewireKeypad<Print, 16> KP2(Serial, KEYS, 4, 4, A0, 4700, 1000);

void setup() {
    lcd.init();
    lcd.backlight();
    Serial.begin(9600);
    servo.attach(9);
    servo.write(90);
    pinMode(sensorPin1, INPUT);
    pinMode(sensorPin2, INPUT);
    KP2.SetKeypadVoltage(5.0);
    SPI.begin();
    mfrc522.PCD_Init();
    lcd.setCursor(0, 0);
    lcd.print("Automatic toll");
    lcd.setCursor(0, 1);
    lcd.print("Collection Sys.");
    delay(3000);
    lcd.clear();
}

void loop() {
    if (recharge == 0) {
        reCharge();
    } else {
        lcd.setCursor(0, 0);
        lcd.print("Welcome to RCOEM");
        sensorRead();
        rfid();
        KeyPad();
        if (senVal1 == 0) {
            servoDown();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Vehicle Detected");
            delay(1000);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Put your card");
            lcd.setCursor(0, 1);
            lcd.print("for Payment");
            delay(2000);
            lcd.clear();
        } else if (senVal2 == 0 && state == 1) {
            servoUp();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Have a safe");
            lcd.setCursor(0, 1);
            lcd.print("journey");
            delay(1000);
            lcd.clear();
            state = 0;
        }
    }
}

void servoDown() {
    for (servoPos = 90; servoPos <= 180; servoPos += 1) {
        servo.write(servoPos);
        delay(5);
    }
}

void servoUp() {
    for (servoPos = 180; servoPos >= 90; servoPos -= 1) {
        servo.write(servoPos);
        delay(5);
    }
}

void sensorRead() {
    senVal1 = digitalRead(sensorPin1);
    senVal2 = digitalRead(sensorPin2);
}

void rfid() {
    if (!mfrc522.PICC_IsNewCardPresent()) return;
    if (!mfrc522.PICC_ReadCardSerial()) return;

    String content = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    content.toUpperCase();

    if (content.substring(1) == "E3 78 B3 94") {
        if (card1Balance >= 500) {
            lcdPrint();
            card1Balance -= 500;
            lcd.setCursor(9, 1);
            lcd.print(card1Balance);
            delay(2000);
            lcd.clear();
            state = 1;
        } else {
            card = content.substring(1);
            LcdPrint();
            lcd.setCursor(9, 1);
            lcd.print(card1Balance);
            lcd.print(" Tk");
            delay(2000);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Please Recharge");
            delay(1000);
            lcd.clear();
            state = 0;
        }
    } else if (content.substring(1) == "67 0B 9B 4A") {
        if (card2Balance >= 500) {
            lcdPrint();
            card2Balance -= 500;
            lcd.setCursor(9, 1);
            lcd.print(card2Balance);
            delay(2000);
            lcd.clear();
            state = 1;
        } else {
            card = content.substring(1);
            LcdPrint();
            lcd.setCursor(9, 1);
            lcd.print(card2Balance);
            lcd.print(" Tk");
            delay(2000);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Please Recharge");
            lcd.clear();
            delay(1000);
            state = 0;
        }
    } else {
        lcd.setCursor(0, 0);
        lcd.print("Unknown Vehicle");
        lcd.setCursor(0, 1);
        lcd.print("Access denied");
        delay(1500);
        lcd.clear();
    }
}

void KeyPad() {
    byte KState = KP2.Key_State();
    if (KState == PRESSED) {
        Key = KP2.Getkey();
        if (Key) {
            if (Key == 'A') {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Recharging Mode.");
                lcd.setCursor(0, 1);
                lcd.print("................");
                delay(1500);
                lcd.clear();
                recharge = 0;
            }
        }
    }
}

void clearData() {
    while (data_count != 0) {
        Data[data_count--] = 0;
    }
}

void reCharge() {
    lcd.setCursor(0, 0);
    lcd.print("Enter the amount");
    byte KState = KP2.Key_State();
    if (KState == PRESSED) {
        Key = KP2.Getkey();
        if (Key) {
            if (Key == '*') {
                if (card == "E3 78 B3 94") {
                    num1 = Data;
                    card1Balance = num1.toInt() + card1Balance;
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Your current");
                    lcd.setCursor(0, 1);
                    lcd.print("balance: ");
                    lcd.setCursor(9, 1);
                    lcd.print(card1Balance);
                    lcd.print(" Tk");
                    delay(3000);
                    clearData();
                    lcd.clear();
                    recharge = 1;
                } else if (card == "67 0B 9B 4A") {
                    num2 = Data;
                    card2Balance = num2.toInt() + card2Balance;
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Your current");
                    lcd.setCursor(0, 1);
                    lcd.print("balance: ");
                    lcd.setCursor(9, 1);
                    lcd.print(card2Balance);
                    lcd.print(" Tk");
                    delay(3000);
                    clearData();
                    lcd.clear();
                    recharge = 1;
                }
            } else {
                Data[data_count] = Key;
                lcd.setCursor(data_count, 1);
                lcd.print(Data[data_count]);
                data_count++;
            }
        }
    }
}

void lcdPrint() {
    delay(200);
    delay(100);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Successfully");
    lcd.setCursor(0, 1);
    lcd.print(" paid your bill");
    delay(1500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Your Remaining");
    lcd.setCursor(0, 1);
    lcd.print("balance: ");
}

void LcdPrint() {
    delay(200);
    delay(100);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Your balance");
    lcd.setCursor(0, 1);
    lcd.print(" is insufficent");
    delay(1500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Your Remaining");
    lcd.setCursor(0, 1);
    lcd.print("balance: ");
}
