#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// display setting
#define OLED_ADDR   0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_DELAY_MS 2000
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// soil moisture setting
#define MOISTURE_PIN 27
#define AIR_VALUE 4095 // lower bound
#define WATER_VALUE 1270 // upper bound

// temp and humid setting
#define DHTPIN 18
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);

// pump setting
#define PUMP_PIN 23
#define PUMP_ON_LEVEL 10
#define PUMP_OFF_LEVEL 45  
bool pumpRunning = false;

// light setting
#define LIGHT_PIN 34


void setup() {
  // initiate components 
  Serial.begin(115200);
  dht.begin();
  Wire.begin(21, 22);

  pinMode(MOISTURE_PIN, INPUT);
  pinMode(LIGHT_PIN, INPUT);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);

  // initiate display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(4, 24);
  display.println("PlantAlive");
  display.display();

  delay(SCREEN_DELAY_MS);
  display.clearDisplay();
}

void loop() {
  // read DHT sensor data
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  bool dhtValid = !isnan(temp) && !isnan(humidity);

  if (dhtValid) {
    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.print(" C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  } else {
    Serial.println("DHT read failed");
  }


  // display the data
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);

  display.println("Temp");
  if (dhtValid) {
    display.print(temp);
    display.println(" C");
  } else {
    display.println("Error");
  }

  display.display();
  delay(SCREEN_DELAY_MS);

  display.clearDisplay();
  display.setCursor(0, 0);

  display.println("Humidity");
  if (dhtValid) {
    display.print(humidity);
    display.println(" %");
  } else {
    display.println("Error");
  }
  display.display();
  delay(SCREEN_DELAY_MS);

  // read moisture sensor data
  int moistureRaw = analogRead(MOISTURE_PIN);

  Serial.print("Raw: ");
  Serial.print(moistureRaw);

  // calibrate 
  int moisturePercent = map(moistureRaw, AIR_VALUE, WATER_VALUE, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);

  Serial.print("Moisture: ");
  Serial.print(moisturePercent);
  Serial.println(" % ");

  // display data
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);

  display.println("Soil Moist");
  display.print(moisturePercent);
  display.println(" % ");

  display.display();
  delay(SCREEN_DELAY_MS);

  // pump control logic
  if (!pumpRunning && moisturePercent < PUMP_ON_LEVEL) {
    digitalWrite(PUMP_PIN, HIGH);
    pumpRunning = true;
    Serial.print("Pump on");
  }

  if (pumpRunning && moisturePercent >= PUMP_OFF_LEVEL) {
    digitalWrite(PUMP_PIN, LOW);
    pumpRunning = false;
    Serial.print("pump off");
  }

  // read light data 
  int lightRaw = analogRead(LIGHT_PIN);

  Serial.print("Light: ");
  Serial.println(lightRaw);

  // display light data 
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);

  display.println("Light");
  display.println(lightRaw);

  display.display();
  delay(SCREEN_DELAY_MS);
}