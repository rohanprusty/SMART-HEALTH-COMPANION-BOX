/*
 * ============================================================
 *  SAARTHI (सारथि) — Smart Health Companion Box
 *  Automated Medication Reminder & Dispensing System
 * ------------------------------------------------------------
 *  Board       : NodeMCU ESP8266
 *  Platform    : Arduino / Blynk IoT
 *  Peripherals : 16x2 I2C LCD, SG90 Servo, Push Button,
 *                Active Buzzer, LED Strip
 *
 *  Team        : Rohan (24BEC104), Aftab (24BEC076),
 *                Dhruv (24BEC032), Prachetus (24BEC093),
 *                Samarth (24BEC107)
 *  Institute   : IIIT Jabalpur
 * ============================================================
 */

#define BLYNK_TEMPLATE_ID   "TMPL3OIgi2atY"
#define BLYNK_TEMPLATE_NAME "Medical Box"
#define BLYNK_AUTH_TOKEN    "5EtXNwBNyT8UMiQVreMEbruhev3DELQh"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include <time.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- Pin Definitions ---
#define BUZZER_PIN  D5
#define BUTTON_PIN  D6
#define SERVO_PIN   D7
#define LED_PIN     D8

// --- Peripherals & Objects ---
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo medServo;
BlynkTimer timer;

// --- Network Credentials ---
char ssid[] = "WIFI_SSID";
char pass[] = "WIFI_PASSWORD";

// --- Global State Variables ---
String medNames = "";
String medTimes = "";
String medCounts = "";
int lidStatus = 0;

String medicine[4];
String medTime[4];
String medCount[4];

bool servoOpen = false;
bool lastButtonState = HIGH;
bool medicineTaken = false;
bool alarmActive = false;
unsigned long alarmStartMillis = 0;
int currentAlarmIndex = -1;

unsigned long buzzerTimer = 0;
bool buzzerToggle = false;

String nextMedicineName = "";
String nextMedicineTime = "";
bool lcdPage = false;

// --- Blynk Virtual Pin Handlers ---
BLYNK_WRITE(V0) { medNames = param.asString(); }
BLYNK_WRITE(V1) { medTimes = param.asString(); }
BLYNK_WRITE(V2) { medCounts = param.asString(); }
BLYNK_WRITE(V3) { lidStatus = param.asInt(); }

// --- String Parsing Utility ---
void splitData() {
  int start = 0, end = 0;
  for (int i = 0; i < 4; i++) {
    end = medNames.indexOf(',', start);
    if (end == -1) end = medNames.length();
    medicine[i] = medNames.substring(start, end);
    start = end + 1;
  }

  start = 0;
  for (int i = 0; i < 4; i++) {
    end = medTimes.indexOf(',', start);
    if (end == -1) end = medTimes.length();
    medTime[i] = medTimes.substring(start, end);
    start = end + 1;
  }

  start = 0;
  for (int i = 0; i < 4; i++) {
    end = medCounts.indexOf(',', start);
    if (end == -1) end = medCounts.length();
    medCount[i] = medCounts.substring(start, end);
    start = end + 1;
  }
}

// --- Decrement Stock on Consumption ---
void decrementMedicineCount(int index) {
  if (index < 0 || index > 3) return;

  int count = medCount[index].toInt();
  if (count > 0) count -= 1;
  medCount[index] = String(count);

  String updatedCounts = medCount[0] + "," + medCount[1] + "," + medCount[2] + "," + medCount[3];
  medCounts = updatedCounts;

  Blynk.virtualWrite(V2, medCounts);
  Serial.printf("Updated count for %s: %s\n", medicine[index].c_str(), medCount[index].c_str());
}

// --- Status Table & Next Dose Calculation ---
void printTable() {
  splitData();

  time_t now = time(nullptr);
  struct tm *t = localtime(&now);
  int currentMinutes = t->tm_hour * 60 + t->tm_min;

  int nearestDiff = 9999;
  int nextIndex = -1;

  for (int i = 0; i < 4; i++) {
    if (medTime[i].length() < 5) continue;
    int hh = medTime[i].substring(0, 2).toInt();
    int mm = medTime[i].substring(3, 5).toInt();
    int diff = (hh * 60 + mm) - currentMinutes;

    if (diff >= 0 && diff < nearestDiff) {
      nearestDiff = diff;
      nextIndex = i;
    }
  }

  if (nextIndex >= 0) {
    nextMedicineName = medicine[nextIndex];
    nextMedicineTime = medTime[nextIndex];
    int hr = nearestDiff / 60;
    int mn = nearestDiff % 60;

    Blynk.virtualWrite(V4, nextMedicineName);
    Blynk.virtualWrite(V5, String(hr) + " hr " + String(mn) + " min");
  }
}

// --- Scheduled Medicine Check Routine ---
void checkMedicineTime() {
  splitData();

  time_t now = time(nullptr);
  struct tm *t = localtime(&now);
  char currentTime[6];
  sprintf(currentTime, "%02d:%02d", t->tm_hour, t->tm_min);

  for (int i = 0; i < 4; i++) {
    if (medTime[i] == String(currentTime)) {
      if (!alarmActive) {
        alarmActive = true;
        alarmStartMillis = millis();
        currentAlarmIndex = i;
        medicineTaken = false;
        Blynk.virtualWrite(V6, 0);
        digitalWrite(BUZZER_PIN, LOW);
      }
    }
  }

  if (alarmActive) {
    if (millis() - alarmStartMillis >= 120000) { // 2-minute alarm timeout
      alarmActive = false;
      if (!medicineTaken) {
        Blynk.virtualWrite(V6, 1); // Missed Dose Alert
        Serial.println("ALERT: Medicine Dose Missed!");
      }
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}

// --- LCD Display Rotation ---
void updateLCD() {
  time_t now = time(nullptr);
  struct tm *t = localtime(&now);

  char dayDate[17];
  char clockTime[17];
  strftime(dayDate, sizeof(dayDate), "%a %d/%m/%Y", t);
  strftime(clockTime, sizeof(clockTime), "%H:%M:%S", t);

  lcd.clear();
  if (!lcdPage) {
    lcd.setCursor(0, 0);
    lcd.print(dayDate);
    lcd.setCursor(0, 1);
    lcd.print(clockTime);
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Next: ");
    lcd.print(nextMedicineName.substring(0, 10));
    lcd.setCursor(0, 1);
    lcd.print("Time: ");
    lcd.print(nextMedicineTime);
  }
  lcdPage = !lcdPage;
}

// --- Hardware Button & Buzzer Control ---
void handleButtonAndBuzzer() {
  bool buttonState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && buttonState == LOW) {
    if (alarmActive && !medicineTaken) {
      medicineTaken = true;
      Blynk.virtualWrite(V6, 0);
      digitalWrite(BUZZER_PIN, LOW);
      decrementMedicineCount(currentAlarmIndex);
    }

    // Toggle servo latch & LED
    if (!servoOpen) {
      medServo.write(180);
      digitalWrite(LED_PIN, HIGH);
      servoOpen = true;
    } else {
      medServo.write(0);
      digitalWrite(LED_PIN, LOW);
      servoOpen = false;
    }
    delay(300); // Debounce delay
  }
  lastButtonState = buttonState;

  // Active buzzer pulsing during alarm window
  if (alarmActive && !medicineTaken) {
    if (millis() - buzzerTimer > 500) {
      buzzerTimer = millis();
      buzzerToggle = !buzzerToggle;
      digitalWrite(BUZZER_PIN, buzzerToggle ? HIGH : LOW);
    }
  } else if (!alarmActive) {
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void setup() {
  Serial.begin(115200);

  Wire.begin(D2, D1); // SDA = D2, SCL = D1
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("SAARTHI Box");
  lcd.setCursor(0, 1);
  lcd.print("Connecting Wi-Fi");

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  medServo.attach(SERVO_PIN);
  medServo.write(0);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Sync NTP Time (IST = UTC + 5:30 -> 19800 sec offset)
  configTime(19800, 0, "pool.ntp.org", "time.nist.gov");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Syncing Time...");

  // Critical Bug Fix: Wait for NTP to sync before continuing
  while (time(nullptr) < 100000) {
    delay(500);
    Serial.print(".");
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready!");
  delay(1500);

  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V3);

  timer.setInterval(5000L, printTable);
  timer.setInterval(1000L, checkMedicineTime);
  timer.setInterval(3000L, updateLCD);
}

void loop() {
  Blynk.run();
  timer.run();
  handleButtonAndBuzzer();
}
