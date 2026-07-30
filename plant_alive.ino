#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// display setting
#define OLED_ADDR   0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// soil moisture setting
#define MOISTURE_PIN 27
#define AIR_VALUE 3800 // lower bound
#define WATER_VALUE 1200 // upper bound

// temp and humid setting
#define DHTPIN 18
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);

// pump setting
#define PUMP_PIN 23
#define PUMP_ON_LEVEL -20 
#define PUMP_OFF_LEVEL 45  
bool pumpRunning = false;

// light setting
#define LIGHT_PIN 34


void setup() {
  Serial.begin(115200);
  dht.begin();
  Wire.begin(21, 22);

  pinMode(MOISTURE_PIN, INPUT);
  pinMode(LIGHT_PIN, INPUT);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(20, 20);
  display.println("Hello");
  display.display();

  delay(2000);
  
  display.clearDisplay();
}

void loop() {

  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(", Humidity: ");
  Serial.print(humidity);
  Serial.println(" % ");

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Temp: ");
  display.println(temp);
  display.print(", Humidity: ");
  display.print(humidity);
  display.println(" % ");

  display.display();

  delay(2000);

  int moisture_raw = analogRead(MOISTURE_PIN);
  float value_range = (float)(AIR_VALUE - WATER_VALUE);
  float reading_adjusted = (float)(AIR_VALUE - moisture_raw);
  float percent = (reading_adjusted / value_range) * 100.0;
  // percent = constrain(percent, 0.0, 100.0);

  Serial.print("Moisture: ");
  Serial.print(percent);
  Serial.println(" % ");

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Soil Moisture: ");
  display.print(percent);
  display.println(" % ");

  display.display();

  int moisturePercent = map(
    moisture_raw,
    AIR_VALUE,
    WATER_VALUE,
    0,
    100
  );

  moisturePercent = constrain(moisturePercent, 0, 100);

  if (!pumpRunning && moisturePercent < PUMP_ON_LEVEL) {
    digitalWrite(PUMP_PIN, HIGH);
    pumpRunning = true;
  }

  if (pumpRunning && moisturePercent >= PUMP_OFF_LEVEL) {
    digitalWrite(PUMP_PIN, LOW);
    pumpRunning = false;
  }

  int lightRaw = analogRead(LIGHT_PIN);

  Serial.print("Light: ");
  Serial.println(lightRaw);

  delay(2000);

}