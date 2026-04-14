/*
 * ESP32 Mini Web Server - IoT Monitoring System
 * 
 * Ilmiy maqola loyihasi
 * Muallif: Ibodullayev Jaxongir Maxammadi o'g'li
 * Muassasa: Muhammad al-Xorazmiy nomidagi Toshkent axborot texnologiyalari universiteti
 * Fakultet: "Kompyuter injiniringi"
 * Kafedra: "Kompyuter tizimlarini loyihalash"
 * Kurs: 1-kurs magistranti
 * Email: jahon18gir@gmail.com
 * 
 * Xususiyatlar:
 * - DHT22 harorat va namlik sensori
 * - Wi-Fi orqali ma'lumotlarni uzatish
 * - Web-server bilan foydalanuvchi interfeysi
 * - GPIO boshqaruv (4 ta relay)
 * - Real vaqt rejimida monitoring
 */

#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <ArduinoJson.h>

// WiFi configuration - O'zingizning WiFi ma'lumotlaringizni kiriting
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// Pin configurations
#define DHT_PIN 4
#define DHT_TYPE DHT22
#define LED_PIN 2
#define RELAY1_PIN 12
#define RELAY2_PIN 13
#define RELAY3_PIN 14
#define RELAY4_PIN 15

// Initialize components
DHT dht(DHT_PIN, DHT_TYPE);
WebServer server(80);

// Global variables
struct SensorData {
  float temperature;
  float humidity;
  unsigned long timestamp;
} sensorData;

// Relay states
bool relayStates[4] = {false, false, false, false};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting ESP32 Web Server...");
  
  // Initialize pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  pinMode(RELAY4_PIN, OUTPUT);
  
  // Initialize DHT sensor
  dht.begin();
  
  // Initialize sensor data
  sensorData.temperature = 0;
  sensorData.humidity = 0;
  sensorData.timestamp = 0;
  
  // Connect to WiFi
  connectWiFi();
  
  // Setup web server routes
  setupRoutes();
  
  // Start server
  server.begin();
  Serial.println("HTTP server started");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();
  
  // Update sensor data every 2 seconds
  static unsigned long lastSensorRead = 0;
  if (millis() - lastSensorRead > 2000) {
    readSensor();
    lastSensorRead = millis();
  }
}

void connectWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int timeout = 20;
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {
    Serial.print(".");
    delay(1000);
    timeout--;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi");
  }
}

void setupRoutes() {
  // Serve main page
  server.on("/", HTTP_GET, handleIndex);
  
  // Serve sensor data
  server.on("/data", HTTP_GET, handleSensorData);
  
  // Handle GPIO control
  server.on("/gpio/0/on", HTTP_POST, []() { handleGPIO(0, true); });
  server.on("/gpio/0/off", HTTP_POST, []() { handleGPIO(0, false); });
  server.on("/gpio/1/on", HTTP_POST, []() { handleGPIO(1, true); });
  server.on("/gpio/1/off", HTTP_POST, []() { handleGPIO(1, false); });
  server.on("/gpio/2/on", HTTP_POST, []() { handleGPIO(2, true); });
  server.on("/gpio/2/off", HTTP_POST, []() { handleGPIO(2, false); });
  server.on("/gpio/3/on", HTTP_POST, []() { handleGPIO(3, true); });
  server.on("/gpio/3/off", HTTP_POST, []() { handleGPIO(3, false); });
  
  // Handle LED toggle
  server.on("/toggle", HTTP_POST, handleToggle);
  
  // CORS preflight
  server.on("/gpio", HTTP_OPTIONS, handleCORS);
  server.on("/toggle", HTTP_OPTIONS, handleCORS);
}

void handleIndex() {
  String html = getIndexHTML();
  server.send(200, "text/html", html);
}

void handleSensorData() {
  readSensor();
  
  DynamicJsonDocument doc(1024);
  doc["temperature"] = sensorData.temperature;
  doc["humidity"] = sensorData.humidity;
  doc["timestamp"] = sensorData.timestamp;
  
  String response;
  serializeJson(doc, response);
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

void handleGPIO(int gpioNum, bool state) {
  if (gpioNum < 0 || gpioNum >= 4) {
    server.send(400, "text/plain", "Invalid GPIO number");
    return;
  }
  
  relayStates[gpioNum] = state;
  
  // Set corresponding relay pin
  switch (gpioNum) {
    case 0: digitalWrite(RELAY1_PIN, state ? HIGH : LOW); break;
    case 1: digitalWrite(RELAY2_PIN, state ? HIGH : LOW); break;
    case 2: digitalWrite(RELAY3_PIN, state ? HIGH : LOW); break;
    case 3: digitalWrite(RELAY4_PIN, state ? HIGH : LOW); break;
  }
  
  DynamicJsonDocument doc(512);
  doc["gpio"] = gpioNum;
  doc["state"] = state ? "on" : "off";
  
  String response;
  serializeJson(doc, response);
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

void handleToggle() {
  static bool ledState = false;
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  
  DynamicJsonDocument doc(256);
  doc["led"] = ledState ? "on" : "off";
  
  String response;
  serializeJson(doc, response);
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

void handleCORS() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(200, "text/plain", "");
}

void readSensor() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  
  if (!isnan(temp) && !isnan(hum)) {
    sensorData.temperature = temp;
    sensorData.humidity = hum;
    sensorData.timestamp = millis();
    
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print("°C, Humidity: ");
    Serial.print(hum);
    Serial.println("%");
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }
}

String getIndexHTML() {
  return R"(
<!DOCTYPE html>
<html lang="uz">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 Mini Server — IoT Monitoring Panel</title>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #0f2027, #2c5364);
            color: white;
            min-height: 100vh;
            padding: 20px;
        }

        .container {
            max-width: 1200px;
            margin: 0 auto;
        }

        .header {
            background: rgba(255, 255, 255, 0.1);
            backdrop-filter: blur(10px);
            padding: 20px;
            border-radius: 15px;
            margin-bottom: 20px;
            text-align: center;
        }

        .header h1 {
            font-size: 2rem;
            margin-bottom: 10px;
            color: #4dd0e1;
        }

        .wifi-status {
            font-size: 0.9rem;
            color: #81c784;
            margin-bottom: 5px;
        }

        .clock {
            font-size: 1.2rem;
            color: #ffd54f;
        }

        .dashboard {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
            gap: 20px;
            margin-bottom: 20px;
        }

        .card {
            background: rgba(255, 255, 255, 0.1);
            backdrop-filter: blur(10px);
            padding: 20px;
            border-radius: 15px;
            border: 1px solid rgba(255, 255, 255, 0.2);
        }

        .card h2 {
            color: #4dd0e1;
            margin-bottom: 15px;
            font-size: 1.3rem;
        }

        .sensor-value {
            font-size: 2.5rem;
            font-weight: bold;
            margin: 10px 0;
        }

        .temperature {
            color: #ff6b6b;
        }

        .humidity {
            color: #4ecdc4;
        }

        .chart-container {
            position: relative;
            height: 200px;
            margin-top: 15px;
        }

        .gpio-controls {
            display: grid;
            grid-template-columns: repeat(2, 1fr);
            gap: 10px;
        }

        .gpio-btn {
            background: linear-gradient(45deg, #2196F3, #21CBF3);
            color: white;
            border: none;
            padding: 15px;
            border-radius: 10px;
            cursor: pointer;
            font-size: 1rem;
            transition: all 0.3s ease;
        }

        .gpio-btn:hover {
            transform: translateY(-2px);
            box-shadow: 0 5px 15px rgba(33, 150, 243, 0.3);
        }

        .gpio-btn.on {
            background: linear-gradient(45deg, #4CAF50, #45a049);
        }

        .gpio-btn.off {
            background: linear-gradient(45deg, #f44336, #da190b);
        }

        .status-indicator {
            display: inline-block;
            width: 10px;
            height: 10px;
            border-radius: 50%;
            margin-right: 5px;
        }

        .status-online {
            background-color: #4CAF50;
            animation: blink 1s infinite;
        }

        .status-offline {
            background-color: #f44336;
        }

        @keyframes blink {
            0%, 50% { opacity: 1; }
            51%, 100% { opacity: 0.3; }
        }

        .led-control {
            text-align: center;
            margin-top: 15px;
        }

        .led-btn {
            background: linear-gradient(45deg, #FFC107, #FF9800);
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 20px;
            cursor: pointer;
            font-size: 1rem;
            transition: all 0.3s ease;
        }

        .led-btn:hover {
            transform: scale(1.05);
            box-shadow: 0 5px 15px rgba(255, 193, 7, 0.3);
        }

        @media (max-width: 768px) {
            .dashboard {
                grid-template-columns: 1fr;
            }
            
            .gpio-controls {
                grid-template-columns: 1fr;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>ESP32 Mini Server</h1>
            <div class="wifi-status">
                <span class="status-indicator status-online"></span>
                <span id="wifiStatus">Connected</span>
            </div>
            <div class="clock" id="clock"></div>
        </div>

        <div class="dashboard">
            <div class="card">
                <h2>Temperature Sensor</h2>
                <div class="sensor-value temperature" id="temperature">--°C</div>
                <div class="chart-container">
                    <canvas id="tempChart"></canvas>
                </div>
            </div>

            <div class="card">
                <h2>Humidity Sensor</h2>
                <div class="sensor-value humidity" id="humidity">--%</div>
                <div class="chart-container">
                    <canvas id="humidChart"></canvas>
                </div>
            </div>

            <div class="card">
                <h2>Combined Data</h2>
                <div class="chart-container">
                    <canvas id="combinedChart"></canvas>
                </div>
            </div>

            <div class="card">
                <h2>GPIO Control</h2>
                <div class="gpio-controls">
                    <button class="gpio-btn" id="gpio0" onclick="toggleGPIO(0)">GPIO 0: OFF</button>
                    <button class="gpio-btn" id="gpio1" onclick="toggleGPIO(1)">GPIO 1: OFF</button>
                    <button class="gpio-btn" id="gpio2" onclick="toggleGPIO(2)">GPIO 2: OFF</button>
                    <button class="gpio-btn" id="gpio3" onclick="toggleGPIO(3)">GPIO 3: OFF</button>
                </div>
                <div class="led-control">
                    <button class="led-btn" onclick="toggleLED()">Toggle LED</button>
                </div>
            </div>
        </div>
    </div>

    <script>
        // Global variables
        let gpioStates = [false, false, false, false];
        let tempData = [];
        let humidData = [];
        let timeLabels = [];
        const maxDataPoints = 20;

        // Initialize charts
        const tempChart = new Chart(document.getElementById('tempChart'), {
            type: 'line',
            data: {
                labels: timeLabels,
                datasets: [{
                    label: 'Temperature (°C)',
                    data: tempData,
                    borderColor: '#ff6b6b',
                    backgroundColor: 'rgba(255, 107, 107, 0.1)',
                    tension: 0.4
                }]
            },
            options: {
                responsive: true,
                maintainAspectRatio: false,
                scales: {
                    y: {
                        beginAtZero: false,
                        grid: { color: 'rgba(255, 255, 255, 0.1)' },
                        ticks: { color: 'white' }
                    },
                    x: {
                        grid: { color: 'rgba(255, 255, 255, 0.1)' },
                        ticks: { color: 'white' }
                    }
                },
                plugins: {
                    legend: { labels: { color: 'white' } }
                }
            }
        });

        const humidChart = new Chart(document.getElementById('humidChart'), {
            type: 'line',
            data: {
                labels: timeLabels,
                datasets: [{
                    label: 'Humidity (%)',
                    data: humidData,
                    borderColor: '#4ecdc4',
                    backgroundColor: 'rgba(78, 205, 196, 0.1)',
                    tension: 0.4
                }]
            },
            options: {
                responsive: true,
                maintainAspectRatio: false,
                scales: {
                    y: {
                        beginAtZero: true,
                        max: 100,
                        grid: { color: 'rgba(255, 255, 255, 0.1)' },
                        ticks: { color: 'white' }
                    },
                    x: {
                        grid: { color: 'rgba(255, 255, 255, 0.1)' },
                        ticks: { color: 'white' }
                    }
                },
                plugins: {
                    legend: { labels: { color: 'white' } }
                }
            }
        });

        const combinedChart = new Chart(document.getElementById('combinedChart'), {
            type: 'line',
            data: {
                labels: timeLabels,
                datasets: [{
                    label: 'Temperature (°C)',
                    data: tempData,
                    borderColor: '#ff6b6b',
                    backgroundColor: 'rgba(255, 107, 107, 0.1)',
                    tension: 0.4,
                    yAxisID: 'y'
                }, {
                    label: 'Humidity (%)',
                    data: humidData,
                    borderColor: '#4ecdc4',
                    backgroundColor: 'rgba(78, 205, 196, 0.1)',
                    tension: 0.4,
                    yAxisID: 'y1'
                }]
            },
            options: {
                responsive: true,
                maintainAspectRatio: false,
                interaction: {
                    mode: 'index',
                    intersect: false,
                },
                scales: {
                    y: {
                        type: 'linear',
                        display: true,
                        position: 'left',
                        grid: { color: 'rgba(255, 255, 255, 0.1)' },
                        ticks: { color: 'white' }
                    },
                    y1: {
                        type: 'linear',
                        display: true,
                        position: 'right',
                        grid: { drawOnChartArea: false },
                        ticks: { color: 'white' }
                    },
                    x: {
                        grid: { color: 'rgba(255, 255, 255, 0.1)' },
                        ticks: { color: 'white' }
                    }
                },
                plugins: {
                    legend: { labels: { color: 'white' } }
                }
            }
        });

        // Update clock
        function updateClock() {
            const now = new Date();
            document.getElementById('clock').textContent = now.toLocaleTimeString();
        }

        // Fetch sensor data
        async function fetchSensorData() {
            try {
                const response = await fetch('/data');
                const data = await response.json();
                
                document.getElementById('temperature').textContent = data.temperature.toFixed(1) + '°C';
                document.getElementById('humidity').textContent = data.humidity.toFixed(1) + '%';
                
                // Update chart data
                const now = new Date().toLocaleTimeString();
                timeLabels.push(now);
                tempData.push(data.temperature);
                humidData.push(data.humidity);
                
                // Keep only last maxDataPoints
                if (timeLabels.length > maxDataPoints) {
                    timeLabels.shift();
                    tempData.shift();
                    humidData.shift();
                }
                
                // Update charts
                tempChart.update();
                humidChart.update();
                combinedChart.update();
                
            } catch (error) {
                console.error('Error fetching sensor data:', error);
                document.getElementById('wifiStatus').textContent = 'Connection Error';
            }
        }

        // Toggle GPIO
        async function toggleGPIO(gpioNum) {
            const currentState = gpioStates[gpioNum];
            const newState = !currentState;
            
            try {
                const response = await fetch(`/gpio/${gpioNum}/${newState ? 'on' : 'off'}`, {
                    method: 'POST'
                });
                
                if (response.ok) {
                    gpioStates[gpioNum] = newState;
                    updateGPIOButton(gpioNum);
                }
            } catch (error) {
                console.error('Error toggling GPIO:', error);
            }
        }

        // Update GPIO button
        function updateGPIOButton(gpioNum) {
            const button = document.getElementById(`gpio${gpioNum}`);
            const state = gpioStates[gpioNum];
            button.textContent = `GPIO ${gpioNum}: ${state ? 'ON' : 'OFF'}`;
            button.className = `gpio-btn ${state ? 'on' : 'off'}`;
        }

        // Toggle LED
        async function toggleLED() {
            try {
                const response = await fetch('/toggle', {
                    method: 'POST'
                });
                
                if (response.ok) {
                    const data = await response.json();
                    console.log('LED toggled:', data.led);
                }
            } catch (error) {
                console.error('Error toggling LED:', error);
            }
        }

        // Initialize
        setInterval(updateClock, 1000);
        setInterval(fetchSensorData, 2000);
        updateClock();
        fetchSensorData();
        
        // Initialize GPIO buttons
        for (let i = 0; i < 4; i++) {
            updateGPIOButton(i);
        }
    </script>
</body>
</html>
)";
}
