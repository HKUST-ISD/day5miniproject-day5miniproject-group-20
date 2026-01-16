/* Project 2: Humidity and Temperature Alarm (温度阈值27°C) */
#include <Arduino.h>
#include <DHT.h>
#include "NewPing.h"
#include <ESP32Servo.h>

// define DHT11 sensor pin
#define DHT_PIN   15

#define DHT_TYPE DHT11
DHT dht11(DHT_PIN, DHT_TYPE);

// Maximum distance we want to ping for (in centimeters).
#define MAX_DISTANCE 400

//define pins
#define LED_PIN 2
#define TRIG_PIN 5        
#define ECHO_PIN 16       
#define BUZZER_PIN 35     
#define SERVO_PIN 7

// NewPing setup of pins and maximum distance.
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 

// 创建舵机对象
Servo servo;

void setup() {
  // Initialize Serial Monitor 
  Serial.begin(115200);    
  // initialize the DHT11 sensor
  dht11.begin();
  //attach servo pin
  servo.attach(SERVO_PIN);
  // config the pin mode of each pin
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // 初始化servo停止
  servo.write(90);
  
  Serial.println("==========================================");
  Serial.println("Project 2: Humidity and Temperature Alarm");
  Serial.println("Temperature threshold: 27°C");
  Serial.println("==========================================");
}

void loop() {
  // 读取超声波距离
  float distance = sonar.ping_cm();
  delay(50);
  
  // read humidity and temperature
  float humi  = dht11.readHumidity();
  float tempC = dht11.readTemperature();

  // 始终打印传感器读数
  Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.print("°C | Humi: ");
  Serial.print(humi);
  Serial.print("% | Dist: ");
  Serial.print(distance);
  Serial.println(" cm");

  // 根据流程图逻辑（温度阈值改为27°C）
  if (isnan(tempC) || isnan(humi)) {
    Serial.println("Failed to read from DHT11 sensor!");
    
  } else if (tempC <= 27) {  // 改为27°C
    // 温度 ≤ 27°C → Safe
    Serial.println("Safe");
    servo.write(90);        // 风扇停止
    noTone(BUZZER_PIN);    // 蜂鸣器关闭
    digitalWrite(LED_PIN, LOW);
    
  } else {
    // 温度 > 27°C
    if (humi >= 45) {
      // 湿度 ≥ 45% → Caution!!!
      Serial.println("Caution!!!");
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");
      
      servo.write(90);        // 风扇停止
      tone(BUZZER_PIN, 1000); // 蜂鸣器持续响
      digitalWrite(LED_PIN, HIGH);
      
    } else {
      // 湿度 < 45% → Dangerous!!!!
      Serial.println("Dangerous!!!!");
      
      servo.write(180);      // 风扇开启（舵机带螺旋桨）
      
      // 蜂鸣器间歇响
      tone(BUZZER_PIN, 800, 500);
      digitalWrite(LED_PIN, HIGH);
      delay(1000);
      noTone(BUZZER_PIN);
      digitalWrite(LED_PIN, LOW);
      delay(500);
    }
  }

  Serial.println("------------------------");
  
  // wait 2 seconds
  delay(2000);
}