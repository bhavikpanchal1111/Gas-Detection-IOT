# [Gas-Detection-IOT](https://bhavikpanchal1111.github.io/Gas-Detection-IOT/)

This code is for an IoT gas detection system that uses an ESP8266 microcontroller, an MQ-2 gas sensor, a DHT11 temperature and humidity sensor, a buzzer, a DC exhaust fan, and a servo motor to control a gas regulator. It also sends the sensor data to a Firebase Realtime Database for monitoring.

Here’s a breakdown of the code:

### 1. **Libraries and Definitions**
- **Libraries:**
  - `ESP8266WiFi.h`: Allows the ESP8266 to connect to Wi-Fi.
  - `ESP8266Firebase.h`: Enables communication with Firebase.
  - `DHT.h`: Provides functionality for the DHT11 temperature and humidity sensor.
  - `Servo.h`: Allows control of the servo motor.
  
- **Wi-Fi Configuration:**
  ```cpp
  #define _SSID "Pixel 7a"            // Your WiFi SSID
  #define _PASSWORD "87654321"        // Your WiFi Password
  ```
  These lines define the Wi-Fi network's SSID and password for connecting the ESP8266 to the internet.

- **Firebase Configuration:**
  ```cpp
  #define REFERENCE_URL "https://gas-detection-iot-default-rtdb.firebaseio.com"
  ```
  This specifies the Firebase Realtime Database URL where the sensor data will be sent.

- **Sensor and Pin Configurations:**
  - **DHT Sensor:**
    - DHT11 data pin is connected to pin `D1`.
    - The DHT11 sensor type is defined as `DHT11`.
  
  - **MQ-2 Gas Sensor:**
    - The MQ-2 gas sensor is connected to the analog pin `A0`.
    - The gas threshold is set to `1000`, which is the gas level that triggers the emergency response (buzzer, fan, and servo motor).
  
  - **Buzzer, Fan, and Servo Motor:**
    - The buzzer is connected to pin `D6`.
    - The exhaust fan is connected to pin `D7`.
    - The servo motor is connected to pin `D2`.

### 2. **Setup Function**
```cpp
void setup() 
  - Initialize serial communication for debugging.
  - Serial.begin(115200);
  - Setup Wi-Fi connection
  - Initialize DHT sensor
  - Initialize servo motor and set to its initial position (0 degrees).
  - Set buzzer and fan pin modes to OUTPUT

```
- **Wi-Fi Connection**: It connects to the Wi-Fi network using the provided SSID and password.
- **DHT Sensor Initialization**: Starts the DHT11 sensor for reading temperature and humidity.
- **Servo Initialization**: The servo motor is attached to `SERVO_PIN`, and its initial position is set to 0 (gas knob is open).
- **Pin Modes**: Sets the buzzer and fan pins as output.

### 3. **Main Loop Function**
```cpp
void loop() flow:
  - Read temperature and humidity from the DHT11 sensor
  - Read gas level from MQ-2 gas sensor
  - Print sensor data on the Serial Monitor
  - Send sensor data to Firebase
  - Take action if gas level exceeds threshold
  - Rotate the servo motor to turn on/off the gas regulator
  - Turn on/off buzzer and fan
  - Delay of 1 second before updating again

```
- **Sensor Data Reading:**
  - Reads the humidity and temperature using the DHT11 sensor.
  - Reads the gas level using the MQ-2 gas sensor (analog input).
  
- **Serial Monitor Output:**
  - Prints the temperature, humidity, and gas level values to the Serial Monitor for debugging.

- **Firebase Data Upload:**
  - Sends the temperature, humidity, and gas level values to the Firebase database under the path `gas_detector`.
  - If the DHT sensor values are invalid (e.g., sensor not working), it sends an error message to Firebase.
  
- **Gas Detection and Actions:**
  - If the gas level exceeds the defined threshold (`GAS_THRESHOLD`), the following actions occur:
    - The buzzer is activated with a tone of 3000 Hz (3 kHz).
    - The exhaust fan is turned on.
    - The servo motor rotates to 110 degrees to close the gas regulator (gas knob off).
  
  - If the gas level is below the threshold:
    - The buzzer and fan are turned off.
    - The servo motor rotates back to 0 degrees to open the gas regulator (gas knob on).

- **Delay:**
  - A 1-second delay is added to avoid spamming Firebase with updates too frequently.

### Key Functionalities:
1. **Gas Detection**: The MQ-2 sensor detects the presence of gas, and if it exceeds the threshold, actions are triggered to ensure safety.
2. **Real-time Data Logging**: Sensor data is sent to Firebase for real-time monitoring on a web dashboard.
3. **Emergency Response**: If a gas leak is detected, a buzzer is triggered, a fan is turned on, and a servo motor closes the gas regulator to prevent further leakage.
4. **Restoration**: Once the gas levels are back to normal, the system restores to its initial state (fan and buzzer off, gas knob open).

