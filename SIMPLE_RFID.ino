#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
unsigned long uidDec, uidDecTemp;
LiquidCrystal_I2C lcd(0x27, 16, 2);
int ID1 = 0;
int ID2 = 0;
boolean ok = false;
const int list = A0;
const int CLM = A1;
const int yes = A2;
const int no = A3;
int H = 0;
int M = 0;
int S = 0;
int i = 0;

char Name1[] = "JESSICA ";
char Name2[] = "LISA";
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(yes, INPUT);
  pinMode(no, INPUT);
  pinMode(list, INPUT);
  pinMode(CLM, INPUT);
  digitalWrite(CLM, HIGH);
  digitalWrite(yes, HIGH);
  digitalWrite(no, HIGH);
  digitalWrite(list, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(list) == LOW)
  {
    Read_data();
  }
  if (digitalRead(CLM) == LOW)
  {
    clear_Memory();
  }
  seTime();
  rfid_start();
  delay(1000);
}
void seTime() {


  tmElements_t tm;
  if (RTC.read(tm))
  {
    lcd.clear();
    H = tm.Hour;
    M = tm.Minute;
    S = tm.Second;
    lcd.setCursor(0, 0);
    lcd.print("TIME:");
    print2digits(tm.Hour);
    lcd.print(":");
    print2digits(tm.Minute);
    lcd.print(":");
    print2digits(tm.Second);
    lcd.setCursor(0, 1);
    lcd.print("DATE:");
    lcd.print(tm.Day);
    lcd.print("/");
    lcd.print(tm.Month);
    lcd.print("/");
    lcd.print(tmYearToCalendar(tm.Year));
    delay(1000);
  } else {
    if (RTC.chipPresent())
    {
      lcd.setCursor(0, 0);
      lcd.print("RTC stopped!!!");
      lcd.setCursor(0, 1);
      lcd.print("Run SetTime code");
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Read error!");
      lcd.setCursor(0, 1);
      lcd.print("Check circuitry!");
    }
  }

}

void rfid_start() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Выбор метки
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  uidDec = 0;

  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    uidDecTemp = mfrc522.uid.uidByte[i];
    uidDec = uidDec * 256 + uidDecTemp;
  }
  Serial.println("Card UID: ");
  Serial.println(uidDec);
  if (uidDec == 2470351136)
  {
     ID1 = ID1 + 1;
    EEPROM.write(1, ID1);
    ID1 = EEPROM.read(1);
   
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Welcome");
     lcd.setCursor(0, 1);
    lcd.print(Name1);
    delay(3000);

  }
 if (uidDec == 1355872115)
  {
     ID2 = ID2 + 1;
    EEPROM.write(2, ID2);
    ID2 = EEPROM.read(2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Welcome");
     lcd.setCursor(0, 1);
    lcd.print(Name2);
delay(3000);

  }
  else
  {
    }
}
void Read_data()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(Name1);
  lcd.print(":");
  lcd.print(EEPROM.read(1));
  lcd.setCursor(0, 1);
  lcd.print(Name2);
  lcd.print(":");
  lcd.print(EEPROM.read(2));
  delay(2000);

}
void clear_Memory()
{
  lcd.clear();
  lcd.print(0, 0);
  lcd.print(F("Clear All Data?"));
  lcd.setCursor(0, 1);
  lcd.print(F("Long press: Y/N"));
  delay(2500);
  Serial.print("YES");
  if (digitalRead(yes) == LOW)
  {
    EEPROM.write(1, 0);
    EEPROM.write(2, 0);
    EEPROM.write(3, 0);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("All Data Cleared"));
    lcd.setCursor(0, 1);
    lcd.print(F("****************"));
    delay(1500);
  }
  if (digitalRead(no) == LOW);
  {
    return;
  }
}
void print2digits(int number) {
  if (number >= 0 && number < 10) {
    lcd.write('0');
  }
  lcd.print(number);
}
//
