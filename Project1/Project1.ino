/* replace ? and add code in ???*/
#include "pitches.h"
#include "Arduino.h"
#include "NewPing.h"

// define ultrasonic pins
#define TRIG_PIN   5       // GPIO 5
#define ECHO_PIN   16      // GPIO 18

// define buzzer pin
#define BUZZER_PIN 35      // GPIO 25

//Define Built-in LED pin
#define LED_PIN    2       // ESP32内置LED（通常GPIO 2）

// Maximum distance we want to ping for (in centimeters).
#define MAX_DISTANCE 400

// NewPing setup of pins and maximum distance.
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

//*** Define distance as a variable ***
float distance;  

//jingle bells high_tone
int melody_high[] = {
  NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4,
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4,
  NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4, NOTE_D4, NOTE_D4, NOTE_E4,
  NOTE_D4, NOTE_G4
};

int noteDurations_high[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8, 2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};

//jingle bells LOW
int melody_low[] = {
  NOTE_E3, NOTE_E3, NOTE_E3,
  NOTE_E3, NOTE_E3, NOTE_E3,
  NOTE_E3, NOTE_G3, NOTE_C3, NOTE_D3, NOTE_E3,
  NOTE_F3, NOTE_F3, NOTE_F3, NOTE_F3,
  NOTE_F3, NOTE_E3, NOTE_E3, NOTE_E3, NOTE_E3,
  NOTE_E3, NOTE_D3, NOTE_D3, NOTE_E3,
  NOTE_D3, NOTE_G3
};

int noteDurations_low[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8, 2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};

void setup() {
  // initialize serial monitor
  Serial.begin(115200);
  
  // set pins mode
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // 初始化LED关闭
  digitalWrite(LED_PIN, LOW);
}

void loop() { 
  // Get the distance from the Ultrasonic Sensor
  float distance = sonar.ping_cm();
  
  // 打印距离到串口监视器
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  if (distance > 10) {
    // turn on built in LED (距离较远，亮灯)
    digitalWrite(LED_PIN, HIGH);
    noTone(BUZZER_PIN); // 停止播放音乐
  } 
  else if (distance <= 10 && distance >= 5) {
    // turn off built in LED
    digitalWrite(LED_PIN, LOW);
    // play high tone Jingle-Bells Iterate over the notes of the melody_high:
    for (int i = 0; i < 26; i++) {
      int noteDuration = 1000 / noteDurations_high[i];
      tone(BUZZER_PIN, melody_high[i], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      noTone(BUZZER_PIN);
    }
  } 
  else if (distance < 5) {
    // turn off built in LED
    digitalWrite(LED_PIN, LOW);
    // play low tone Jingle-Bells Iterate over the notes of the melody_low
    for (int i = 0; i < 26; i++) {
      int noteDuration = 1000 / noteDurations_low[i];
      tone(BUZZER_PIN, melody_low[i], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      noTone(BUZZER_PIN);
    }
  }
  
  delay(100); // 短延时，避免过于频繁读取
}