#include <ESP8266WiFi.h>
#include <ESP8266Firebase.h>
#include <DHT.h>
#include <Servo.h>

// Firebase configuration
#define _SSID "Pixel 7a"            // Your WiFi SSID
#define _PASSWORD "87654321"        // Your WiFi Password
#define REFERENCE_URL "https://gas-detection-iot-default-rtdb.firebaseio.com"  // Your Firebase project reference URL

// DHT sensor configuration
#define DHTPIN D1        // DHT11 Data pin connected to D1 (GPIO5)
#define DHTTYPE DHT11    // DHT 11 sensor
DHT dht(DHTPIN, DHTTYPE);

// MQ-2 sensor pin configuration
#define MQ2_PIN A0       // MQ-2 Analog pin connected to A0
#define GAS_THRESHOLD 1000  // Threshold value for gas detection

// Buzzer and motor configuration
#define BUZZER_PIN D6    // Buzzer pin connected to D6 (GPIO12)
#define FAN_PIN D7       // Exhaust fan pin connected to D7 (GPIO13)
#define SERVO_PIN D2     // Servo motor pin connected to D2 (GPIO4)

Servo gasServo;

// Initialize Firebase
Firebase firebase(REFERENCE_URL);

void setup() {
  Serial.begin(115200);
  
  // Setup Wi-Fi connection
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
  }

  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize DHT sensor
  dht.begin();

  // Initialize servo motor
  gasServo.attach(SERVO_PIN);
  gasServo.write(0);  // Set servo to initial position

  // Set pin modes
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
}

void loop() {
  // Read temperature and humidity values from DHT11
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Read temperature in Celsius

  // Read MQ-2 gas sensor value (analog value)
  float gasValue = analogRead(MQ2_PIN);
  //float gasPercentage = map(gasValue, 0, 1023, 0, 100); // Convert to percentage

  // Print sensor values on Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Gas Level: ");
  Serial.print(gasValue);
  Serial.println(" %");

  // Send data to Firebase (under 'gas_detector' path)
  if (isnan(humidity) || isnan(temperature)) {
    firebase.setString("gas_detector/temperature", "Sensor not working");
    firebase.setString("gas_detector/humidity", "Sensor not working");
  } else {
    firebase.setFloat("gas_detector/temperature", temperature);
    firebase.setFloat("gas_detector/humidity", humidity);
  }

  firebase.setFloat("gas_detector/gas_level", gasValue);

  // Check gas level and take action if it exceeds the threshold
  if (gasValue >= GAS_THRESHOLD) {
    // Activate buzzer and fan
    tone(BUZZER_PIN, 3000);  // Send 1 kHz tone signal to buzzer
    digitalWrite(FAN_PIN, HIGH);
    
    // Rotate servo motor to close gas regulator
    gasServo.write(110);

    Serial.println("Warning: Gas detected! Taking action...");
  } else {
    // Turn off buzzer and fan
    noTone(BUZZER_PIN);      // Stop the tone
    digitalWrite(FAN_PIN, LOW);

    // Rotate servo motor back to original position
    gasServo.write(0);
    
    Serial.println("Gas level normal.");
  }

  // Delay to avoid flooding Firebase with updates
  delay(1000); // Update every 1 second
}
