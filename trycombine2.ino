#include "Adafruit_Fingerprint.h" //fingerprint library header file
#include<EEPROM.h> //command for storing data
#include <LiquidCrystal_I2C.h> //lcd header file
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#define DS1307_I2C_ADDRESS 0x68
int H = 0;
int M = 0;
int S = 0;
const int enroll = A0;
const int del = A1;
const int up = A2;
const int down = A3;
const int records = 4;  // 5 for 5 user
uint8_t id;
tmElements_t tm;
int user1, user2, user3, user4, user5;
LiquidCrystal_I2C lcd(0x27, 20, 4);
SoftwareSerial fingerPrint(68, 69);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerPrint);


void setup()
{
delay(1000);
lcd.init();//after edit
lcd.backlight();//after edit

Wire.begin();
delay(100);
lcd.begin(20, 4);
Serial.begin(9600);
pinMode(enroll, INPUT_PULLUP);
pinMode(up, INPUT_PULLUP);
pinMode(down, INPUT_PULLUP);
pinMode(del, INPUT_PULLUP);

if (digitalRead(enroll) == 0)
{
lcd.clear();
lcd.print("Please wait");
lcd.setCursor(0, 1);
lcd.print("Downloding Data");
Serial.println("Please wait");
Serial.println("Downloding Data..");
Serial.println();
Serial.print("S.No.         ");
for (int i = 0; i < records; i++)
{

Serial.print("         User ID");
Serial.print(i + 1);
Serial.print("                   ");
}
Serial.println();
int eepIndex = 0;
for (int i = 0; i < 30; i++)
{
if (i + 1 < 10)
Serial.print('0');
Serial.print(i + 1);
Serial.print("          ");
eepIndex = (i * 7);
download(eepIndex);
eepIndex = (i * 7) + 210;
download(eepIndex);
eepIndex = (i * 7) + 420;
download(eepIndex);
eepIndex = (i * 7) + 630;
download(eepIndex);
Serial.println();
}
}
if (digitalRead(del) == 0)
{
lcd.clear();
lcd.print("Please Wait");
lcd.setCursor(0, 1);
lcd.print("Reseting.....");
for (int i = 1000; i < 1005; i++)
EEPROM.write(i, 0);
for (int i = 0; i < 841; i++)
EEPROM.write(i, 0xff);
lcd.clear();
lcd.print("System Reset");
delay(1000);
}

lcd.clear();
lcd.print(" Student  Attendance   ");
lcd.setCursor(0, 1);
lcd.print("     System     ");
delay(2000);
lcd.clear();
lcd.print("By Jessica Alarapon");
lcd.setCursor(0, 1);
lcd.print("final year thesis");
delay(2000);
for (int i = 1000; i < 1000 + records; i++)
{
if (EEPROM.read(i) == 0xff)
EEPROM.write(i, 0);
}
finger.begin(57600);
Serial.begin(9600);
lcd.clear();
lcd.print("Finding Module");
lcd.setCursor(0, 1);
delay(1000);
if (finger.verifyPassword())
{
Serial.println("Found fingerprint sensor!");
lcd.clear();
lcd.print("Found Module ");
delay(1000);
}
else
{
Serial.println("Did not find fingerprint sensor :(");
lcd.clear();
lcd.print("module not Found");
lcd.setCursor(0, 1);
lcd.print("Check Connections");
while (1);
}
user1 = EEPROM.read(1000);
user2 = EEPROM.read(1001);
user3 = EEPROM.read(1002);
user4 = EEPROM.read(1003);
user5 = EEPROM.read(1004);
lcd.clear();

}

void loop()
{
setTimes();
detectfinger();
checkKeys();
delay(300);
}


void setTimes() {

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
void detectfinger() {
int result = getFingerprintIDez();
if (result > 0)
{


lcd.clear();
lcd.print("ID:");
lcd.print(result);
lcd.setCursor(0, 1);
lcd.print("Please Wait....");
delay(1000);
attendance(result);
lcd.clear();
lcd.print("Attendance ");
lcd.setCursor(0, 1);
lcd.print("Registered");
delay(1000);

return;
}
}

//     dmyyhms - 7 bytes
void attendance(int id)
{
int user = 0, eepLoc = 0;
if (id == 1)
{
eepLoc = 0;
user = user1++;
}
else if (id == 2)
{
eepLoc = 210;
user = user2++;
}
else if (id == 3)
{
eepLoc = 420;
user = user3++;
}
else if (id == 4)
{
eepLoc = 630;
user = user4++;
}
else
return;

int eepIndex = (user * 7) + eepLoc;
EEPROM.write(eepIndex++, (tm.Hour));
EEPROM.write(eepIndex++, (tm.Minute));
EEPROM.write(eepIndex++, (tm.Second));
EEPROM.write(eepIndex++, (tm.Day));
EEPROM.write(eepIndex++, (tm.Month));
EEPROM.write(eepIndex++, (tm.Year) >> 8 );
EEPROM.write(eepIndex++, (tm.Year));
EEPROM.write(1000, user1);
EEPROM.write(1001, user2);
EEPROM.write(1002, user3);
EEPROM.write(1003, user4);
}
void checkKeys()
{
if (digitalRead(enroll) == 0)
{
lcd.clear();
lcd.print("Please Wait");
delay(1000);
while (digitalRead(enroll) == 0);
Enroll();
}
else if (digitalRead(del) == 0)
{
lcd.clear();
lcd.print("Please Wait");
delay(1000);
delet();
}
}
void Enroll()
{
int count = 1;
lcd.clear();
lcd.print("Enter Finger ID:");
while (1)
{
lcd.setCursor(0, 1);
lcd.print(count);
if (digitalRead(up) == 0)
{
count++;
if (count > records)
count = 1;
delay(500);
}
else if (digitalRead(down) == 0)
{
count--;
if (count < 1)
count = records;
delay(500);
}
else if (digitalRead(del) == 0)
{
id = count;
getFingerprintEnroll();
for (int i = 0; i < records; i++)
{
if (EEPROM.read(i) != 0xff)
{
  EEPROM.write(i, id);
  break;
}
}
return;
}
else if (digitalRead(enroll) == 0)
{
return;
}
}
}
void delet()
{
int count = 1;
lcd.clear();
lcd.print("Enter Finger ID");
while (1)
{
lcd.setCursor(0, 1);
lcd.print(count);
if (digitalRead(up) == 0)
{
count++;
if (count > records)
count = 1;
delay(500);
}
else if (digitalRead(down) == 0)
{
count--;
if (count < 1)
count = records;
delay(500);
}
else if (digitalRead(del) == 0)
{
id = count;
deleteFingerprint(id);
for (int i = 0; i < records; i++)
{
if (EEPROM.read(i) == id)
{
  EEPROM.write(i, 0xff);
  break;
}
}
return;
}
else if (digitalRead(enroll) == 0)
{
return;
}
}
}
uint8_t getFingerprintEnroll()
{
int p = -1;
lcd.clear();
lcd.print("finger ID:");
lcd.print(id);
lcd.setCursor(0, 1);
lcd.print("Place Finger");
delay(2000);
while (p != FINGERPRINT_OK)
{
p = finger.getImage();
switch (p)
{
case FINGERPRINT_OK:
Serial.println("Image taken");
lcd.clear();
lcd.print("Image taken");
break;
case FINGERPRINT_NOFINGER:
Serial.println("No Finger");
lcd.clear();
lcd.print("No Finger");
break;
case FINGERPRINT_PACKETRECIEVEERR:
Serial.println("Communication error");
lcd.clear();
lcd.print("Comm Error");
break;
case FINGERPRINT_IMAGEFAIL:
Serial.println("Imaging error");
lcd.clear();
lcd.print("Imaging Error");
break;
default:
Serial.println("Unknown error");
lcd.clear();
lcd.print("Unknown Error");
break;
}
}
// OK success!
p = finger.image2Tz(1);
switch (p) {
case FINGERPRINT_OK:
Serial.println("Image converted");
lcd.clear();
lcd.print("Image converted");
break;
case FINGERPRINT_IMAGEMESS:
Serial.println("Image too messy");
lcd.clear();
lcd.print("Image too messy");
return p;
case FINGERPRINT_PACKETRECIEVEERR:
Serial.println("Communication error");
lcd.clear();
lcd.print("Comm Error");
return p;
case FINGERPRINT_FEATUREFAIL:
Serial.println("Could not find fingerprint features");
lcd.clear();
lcd.print("Feature Not Found");
return p;
case FINGERPRINT_INVALIDIMAGE:
Serial.println("Could not find fingerprint features");
lcd.clear();
lcd.print("Feature Not Found");
return p;
default:
Serial.println("Unknown error");
lcd.clear();
lcd.print("Unknown Error");
return p;
}
Serial.println("Remove finger");
lcd.clear();
lcd.print("Remove Finger");
delay(2000);
p = 0;
while (p != FINGERPRINT_NOFINGER) {
p = finger.getImage();
}
Serial.print("ID "); Serial.println(id);
p = -1;
Serial.println("Place same finger again");
lcd.clear();
lcd.print("Place Finger");
lcd.setCursor(0, 1);
lcd.print("   Again");
while (p != FINGERPRINT_OK) {
p = finger.getImage();
switch (p) {
case FINGERPRINT_OK:
Serial.println("Image taken");
break;
case FINGERPRINT_NOFINGER:
Serial.print(".");
break;
case FINGERPRINT_PACKETRECIEVEERR:
Serial.println("Communication error");
break;
case FINGERPRINT_IMAGEFAIL:
Serial.println("Imaging error");
break;
default:
Serial.println("Unknown error");
return;
}
}
// OK success!
p = finger.image2Tz(2);
switch (p) {
case FINGERPRINT_OK:
Serial.println("Image converted");
break;
case FINGERPRINT_IMAGEMESS:
Serial.println("Image too messy");
return p;
case FINGERPRINT_PACKETRECIEVEERR:
Serial.println("Communication error");
return p;
case FINGERPRINT_FEATUREFAIL:
Serial.println("Could not find fingerprint features");
return p;
case FINGERPRINT_INVALIDIMAGE:
Serial.println("Could not find fingerprint features");
return p;
default:
Serial.println("Unknown error");
return p;
}
// OK converted!
Serial.print("Creating model for #");  Serial.println(id);
p = finger.createModel();
if (p == FINGERPRINT_OK) {
Serial.println("Prints matched!");
} else if (p == FINGERPRINT_PACKETRECIEVEERR) {
Serial.println("Communication error");
return p;
} else if (p == FINGERPRINT_ENROLLMISMATCH) {
Serial.println("Fingerprints did not match");
return p;
} else {
Serial.println("Unknown error");
return p;
}
Serial.print("ID "); Serial.println(id);
p = finger.storeModel(id);
if (p == FINGERPRINT_OK) {
Serial.println("Stored!");
lcd.clear();
lcd.print("Stored!");
delay(2000);
} else if (p == FINGERPRINT_PACKETRECIEVEERR) {
Serial.println("Communication error");
return p;
} else if (p == FINGERPRINT_BADLOCATION) {
Serial.println("Could not store in that location");
return p;
} else if (p == FINGERPRINT_FLASHERR) {
Serial.println("Error writing to flash");
return p;
}
else {
Serial.println("Unknown error");
return p;
}
}
int getFingerprintIDez()
{
uint8_t p = finger.getImage();
if (p != FINGERPRINT_OK)
return -1;
p = finger.image2Tz();
if (p != FINGERPRINT_OK)
return -1;
p = finger.fingerFastSearch();
if (p != FINGERPRINT_OK)
{
lcd.clear();
lcd.print("Finger Not Found");
lcd.setCursor(0, 1);
lcd.print("Try Later");
delay(2000);
return -1;
}
// found a match!
Serial.print("Found ID #");
Serial.print(finger.fingerID);
return finger.fingerID;
}
uint8_t deleteFingerprint(uint8_t id)
{
uint8_t p = -1;
lcd.clear();
lcd.print("Please wait");
p = finger.deleteModel(id);
if (p == FINGERPRINT_OK)
{
Serial.println("Deleted!");
lcd.clear();
lcd.print("Finger Deleted");
lcd.setCursor(0, 1);
lcd.print("Successfully");
delay(1000);
}
else
{
Serial.print("Something Wrong");
lcd.clear();
lcd.print("Something Wrong");
lcd.setCursor(0, 1);
lcd.print("Try Again Later");
delay(2000);
return p;
}
}

void print2digits(int number) {
if (number >= 0 && number < 10) {
lcd.write('0');
}
lcd.print(number);
}
void download(int eepIndex)
{
if(EEPROM.read(eepIndex) != 0xff)
{
Serial.print("T->");
if(EEPROM.read(eepIndex)<10)
Serial.print('0');
Serial.print(EEPROM.read(eepIndex++));
Serial.print(':');
if(EEPROM.read(eepIndex)<10)
Serial.print('0');
Serial.print(EEPROM.read(eepIndex++));
Serial.print(':');
if(EEPROM.read(eepIndex)<10)
Serial.print('0');
Serial.print(EEPROM.read(eepIndex++));
Serial.print(" D->");
if(EEPROM.read(eepIndex)<10)
Serial.print('0');
Serial.print(EEPROM.read(eepIndex++));
Serial.print('/');
if(EEPROM.read(eepIndex)<10)
Serial.print('0');
Serial.print(EEPROM.read(eepIndex++));
Serial.print('/');
Serial.print(EEPROM.read(eepIndex++)<<8 | EEPROM.read(eepIndex++));
}
else
{
Serial.print("---------------------------");
}
Serial.print(" ");
}
//-------------------------Program developed JESSICA------------------//
