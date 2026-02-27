#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SDA_PIN 21
#define SCL_PIN 22

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int ppgPin = 34;
const int threshold = 1400;

const int windowSize = 10;
int samples[windowSize];
int indexSample = 0;
int totalSample = 0;
int avgSample = 0;

bool peakDetected = false;
unsigned long lastBeatTime = 0;
float bpm = 0;

unsigned long peakTimes[10];
int peakIndex = 0;
bool bufferFilled = false;

unsigned long lastLCDUpdate = 0;

void setup() {
  Serial.begin(921600); // Tera Term-compatible baudrate
  pinMode(ppgPin, INPUT);

  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Heart Rate:");
  lcd.setCursor(0, 1);
  lcd.print("masukkan jari...");

  for (int i = 0; i < windowSize; i++) {
    samples[i] = 2000;
    totalSample += 2000;
  }

  for (int i = 0; i < 10; i++) {
    peakTimes[i] = 0;
  }
}

void loop() {
  unsigned long now = micros();

  // Sampling rate: 1000 Hz (1 ms)
  static unsigned long lastSampleTime = 0;
  if (now - lastSampleTime >= 1000) {
    lastSampleTime = now;

    int sample = analogRead(ppgPin);

    // Moving average
    totalSample -= samples[indexSample];
    samples[indexSample] = sample;
    totalSample += sample;
    indexSample = (indexSample + 1) % windowSize;
    avgSample = totalSample / windowSize;

    // Deteksi puncak
    if (sample > threshold && !peakDetected) {
      peakDetected = true;
      peakTimes[peakIndex] = millis();
      peakIndex = (peakIndex + 1) % 10;
      if (peakIndex == 0) bufferFilled = true;

      if (bufferFilled) {
        float totalInterval = 0;
        int validIntervals = 0;
        for (int i = 0; i < 9; i++) {
          unsigned long diff = peakTimes[(i + 1) % 10] - peakTimes[i];
          if (diff > 300 && diff < 2000) {
            totalInterval += diff;
            validIntervals++;
          }
        }
        if (validIntervals > 0) {
          float avgInterval = totalInterval / validIntervals;
          bpm = 60000.0 / avgInterval;
        }
      }

      lastBeatTime = millis();
    } else if (sample < threshold - 100) {
      peakDetected = false;
    }

    // Reset BPM jika tidak ada deteksi dalam 2 detik
    if (millis() - lastBeatTime > 2000) {
      bpm = 0;
    }

    // Kirim data ke Serial Monitor (BPM dan nilai sensor)
    Serial.print((int)bpm);
    Serial.print(", ");
    Serial.println(sample);
  }

  // Update LCD setiap 500 ms
  if (millis() - lastLCDUpdate >= 500) {
    lastLCDUpdate = millis();

    lcd.setCursor(0, 0);
    lcd.print("Heart Rate:      ");
    lcd.setCursor(0, 0);
    lcd.print("Heart Rate:");

    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);

    if (bpm > 0) {
      lcd.print((int)bpm);
      lcd.print(" BPM <3");
    } else {
      lcd.print("masukkan jari...");
    }
  }
}
