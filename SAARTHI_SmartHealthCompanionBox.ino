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
 *
 *  NOTE: This file mirrors the source pasted into chat. The
 *  handleButtonAndBuzzer() function was cut off mid-body when
 *  it was pasted here — paste the remaining lines (buzzer
 *  toggle + closing braces) back in before compiling, or share
 *  the rest of the file and it can be completed.
 *
 *  SECURITY NOTE: this file contains a live Blynk auth token
 *  and WiFi credentials in plaintext. Before pushing to GitHub,
 *  move these into a gitignored `secrets.h` (see README) so the
 *  real token/password never lands in a public commit history.
 * ============================================================
 */

#define BLYNK_TEMPLATE_ID "TMPL3OIgi2atY"
#define BLYNK_TEMPLATE_NAME "Medical Box"
#define BLYNK_AUTH_TOKEN "5EtXNwBNyT8UMiQVreMEbruhev3DELQh"
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <time.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define BUZZER_PIN D5
#define BUTTON_PIN D6
#define SERVO_PIN  D7
int light =D8;
Servo medServo;

bool servoOpen = false;
bool lastButtonState = HIGH;

bool medicineTaken = false;

unsigned long buzzerTimer = 0;
bool buzzerState = false;
String nextMedicineName = "";
String nextMedicineTime = "";

bool lcdPage = false;

char ssid[] = "vkkyy";
char pass[] = "12345678";

BlynkTimer timer;

String medNames = "";
String medTimes = "";
String medCounts = "";

int lidStatus = 0;

String medicine[4];
String medTime[4];
String medCount[4];

bool alarmActive = false;
unsigned long alarmStartMillis = 0;
int currentAlarmIndex = -1;

BLYNK_WRITE(V0)
{
  medNames = param.asString();
}

BLYNK_WRITE(V1)
{
  medTimes = param.asString();
}

BLYNK_WRITE(V2)
{
  medCounts = param.asString();
}

BLYNK_WRITE(V3)
{
  lidStatus = param.asInt();
}

void splitData()
{
  int start = 0;
  int end = 0;

  for (int i = 0; i < 4; i++)
  {
    end = medNames.indexOf(',', start);

    if (end == -1)
      end = medNames.length();

    medicine[i] = medNames.substring(start, end);

    start = end + 1;
  }

  start = 0;

  for (int i = 0; i < 4; i++)
  {
    end = medTimes.indexOf(',', start);

    if (end == -1)
      end = medTimes.length();

    medTime[i] = medTimes.substring(start, end);

    start = end + 1;
  }

  start = 0;

  for (int i = 0; i < 4; i++)
  {
    end = medCounts.indexOf(',', start);

    if (end == -1)
      end = medCounts.length();

    medCount[i] = medCounts.substring(start, end);

    start = end + 1;
  }
}

void decrementMedicineCount(int index)
{
  if (index < 0 || index > 3)
    return;

  int count = medCount[index].toInt();

  if (count > 0)
    count = count - 1;

  medCount[index] = String(count);

  // Rebuild the comma separated string from the 4 counts
  String updatedCounts = medCount[0] + "," +
                          medCount[1] + "," +
                          medCount[2] + "," +
                          medCount[3];

  medCounts = updatedCounts;

  // Push the updated count back to the Blynk app (Terminal/Value/Slider on V2)
  Blynk.virtualWrite(V2, medCounts);

  Serial.print("Updated count for ");
  Serial.print(medicine[index]);
  Serial.print(" : ");
  Serial.println(medCount[index]);
}

void printTable()
{
  splitData();

  time_t now = time(nullptr);
  struct tm *t = localtime(&now);

  int currentMinutes =
      t->tm_hour * 60 +
      t->tm_min;

  int nearestDiff = 9999;
  int nextIndex = -1;

  for (int i = 0; i < 4; i++)
  {
    int hh = medTime[i].substring(0, 2).toInt();
    int mm = medTime[i].substring(3, 5).toInt();

    int medMinutes =
        hh * 60 + mm;

    int diff =
        medMinutes -
        currentMinutes;

    if (diff >= 0 && diff < nearestDiff)
    {
      nearestDiff = diff;
      nextIndex = i;
    }
  }

  Serial.println();
  Serial.println("===============================================");
  Serial.println("             HEALTH COMPANION BOX");
  Serial.println("===============================================");

  char timeBuffer[30];

  strftime(timeBuffer,
           sizeof(timeBuffer),
           "%d-%m-%Y %H:%M:%S",
           t);

  Serial.print("Current Time : ");
  Serial.println(timeBuffer);

  Serial.println();

  Serial.println("Medicine\tTime\tCount");

  for (int i = 0; i < 4; i++)
  {
    Serial.print(medicine[i]);
    Serial.print("\t");
    Serial.print(medTime[i]);
    Serial.print("\t");
    Serial.println(medCount[i]);
  }

  Serial.println();

  if (nextIndex >= 0)
  {
    Serial.print("Next Medicine : ");
    Serial.println(medicine[nextIndex]);

    Serial.print("Time Remaining : ");

    int hr = nearestDiff / 60;
    int mn = nearestDiff % 60;
  Serial.print(hr);
    Serial.print(" hr ");
    Serial.print(mn);
    Serial.println(" min");
        // Send next medicine name to Blynk
nextMedicineName = medicine[nextIndex];
nextMedicineTime = medTime[nextIndex];

Blynk.virtualWrite(V4, nextMedicineName);
    // Send remaining time to Blynk
    String remainingTime =
        String(hr) + " hr " +
        String(mn) + " min";

    Blynk.virtualWrite(V5, remainingTime);
  }

  Serial.print("Lid Status : ");

  if (lidStatus)
    Serial.println("OPEN");
  else
    Serial.println("CLOSED");

  Serial.println("===============================================");
}

void checkMedicineTime()
{
  splitData();

  time_t now = time(nullptr);
  struct tm *t = localtime(&now);

  char currentTime[6];

  sprintf(currentTime,
          "%02d:%02d",
          t->tm_hour,
          t->tm_min);

  for (int i = 0; i < 4; i++)
  {
    if (medTime[i] == currentTime)
    {
      if (!alarmActive)
      {
alarmActive = true;
alarmStartMillis = millis();
currentAlarmIndex = i;

medicineTaken = false;
Blynk.virtualWrite(V6, 0);
digitalWrite(BUZZER_PIN, LOW);

        Serial.println();
        Serial.println("#################################");
        Serial.println("MEDICINE TIME STARTED");
        Serial.println("#################################");
      }
    }
  }

  if (alarmActive)
  {
    if (millis() - alarmStartMillis < 120000)
    {
      Serial.println();
      Serial.println("* MEDICINE TIME *");

      Serial.print("Take : ");
      Serial.println(medicine[currentAlarmIndex]);

      Serial.println("********");
    }
    else
{
    alarmActive = false;

    if (!medicineTaken)
    {
        Blynk.virtualWrite(V6, 1);

        Serial.println();
        Serial.println("MEDICINE MISSED");
    }
    else
    {
        Blynk.virtualWrite(V6, 0);
    }

    digitalWrite(BUZZER_PIN, LOW);

    Serial.println();
    Serial.println("Medicine Reminder Ended");
}
}}
void updateLCD()
{
  time_t now = time(nullptr);
  struct tm *t = localtime(&now);

  char dayDate[17];
  char clockTime[17];

  strftime(dayDate,
           sizeof(dayDate),
           "%a %d/%m/%Y",
           t);

  strftime(clockTime,
           sizeof(clockTime),
           "%H:%M:%S",
           t);

  lcd.clear();

  if (!lcdPage)
  {
    lcd.setCursor(0, 0);
    lcd.print(dayDate);

    lcd.setCursor(0, 1);
    lcd.print(clockTime);
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("Next:");

    String med =
        nextMedicineName.substring(
            0,
            min((int)nextMedicineName.length(), 10));

    lcd.print(med);

    lcd.setCursor(0, 1);
    lcd.print("Time:");
    lcd.print(nextMedicineTime);
  }

  lcdPage = !lcdPage;
}

void handleButtonAndBuzzer()
{
  bool buttonState = digitalRead(BUTTON_PIN);

  // Detect button press
  if (lastButtonState == HIGH &&
      buttonState == LOW)
  {
    if (alarmActive)
    {
      // Only run this ONCE per alarm - the first press that acknowledges the dose
      if (!medicineTaken)
      {
        medicineTaken = true;
        Blynk.virtualWrite(V6, 0);

        // Stop the buzzer immediately on press
        digitalWrite(BUZZER_PIN, LOW);

        // Reduce the remaining quantity for this medicine and update the app
        decrementMedicineCount(currentAlarmIndex);
      }

      // Lid toggle happens every press, regardless of count logic
      if (!servoOpen)
      {
        medServo.write(180);
        digitalWrite(light, HIGH);
        servoOpen = true;

        Serial.println("Servo Opened");
        delay(1000);
      }
      else
      {
        medServo.write(0);
        servoOpen = false;
        digitalWrite(light, LOW);

        Serial.println("Servo Closed");
        delay(1000);
      }
    }
  }

  lastButtonState = buttonState;

  // No medicine time
  if (!alarmActive)
  {
    digitalWrite(BUZZER_PIN, LOW);
    return;
  }

  // Medicine time active
  if (!medicineTaken)
  {
    // Blink buzzer every 500 ms
    if (millis() - buzzerTimer > 500)
    {
      buzzerTimer = millis();
      // TODO: this is where the pasted source cut off.
      // Likely intent (confirm before compiling):
      //   buzzerState = !buzzerState;
      //   digitalWrite(BUZZER_PIN, buzzerState);
    }
  }
}
// TODO: handleButtonAndBuzzer() may have more code after this
// point in your original file — the closing brace above was
// added here just to make the file compile-checkable; verify
// against your source before trusting it.

// ============================================================
// TODO: setup() and loop() were not included in the pasted
// source. Add them here, e.g.:
//
// void setup() {
//   Serial.begin(115200);
//   pinMode(BUZZER_PIN, OUTPUT);
//   pinMode(BUTTON_PIN, INPUT_PULLUP);
//   pinMode(light, OUTPUT);
//   medServo.attach(SERVO_PIN);
//   lcd.init();
//   lcd.backlight();
//   WiFi.begin(ssid, pass);
//   Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
//   configTime(19800, 0, "pool.ntp.org"); // IST offset, adjust as needed
//   timer.setInterval(1000L, updateLCD);
//   timer.setInterval(1000L, checkMedicineTime);
//   timer.setInterval(5000L, printTable);
// }
//
// void loop() {
//   Blynk.run();
//   timer.run();
//   handleButtonAndBuzzer();
// }
// ============================================================
