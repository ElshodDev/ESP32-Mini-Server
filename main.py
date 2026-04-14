"""
ESP32 Mini Web Server - IoT Monitoring System (MicroPython)

Ilmiy maqola loyihasi
Muallif: Ibodullayev Jaxongir Maxammadi o'g'li
Muassasa: Muhammad al-Xorazmiy nomidagi Toshkent axborot texnologiyalari universiteti
Fakultet: "Kompyuter injiniringi"
Kafedra: "Kompyuter tizimlarini loyihalash"
Kurs: 1-kurs magistranti
Email: jahon18gir@gmail.com

Xususiyatlar:
- DHT22 harorat va namlik sensori
- Wi-Fi orqali ma'lumotlarni uzatish
- Web-server bilan foydalanuvchi interfeysi
- GPIO boshqaruv (relay)
- Real vaqt rejimida monitoring
"""

import network
import socket
import time
import machine
import dht
import json
from machine import Pin

# WiFi configuration - O'zingizning WiFi ma'lumotlaringizni kiriting
WIFI_SSID = "YOUR_WIFI_SSID"
WIFI_PASSWORD = "YOUR_WIFI_PASSWORD"

# Pin configurations
DHT_PIN = 4
LED_PIN = 2
RELAY_PINS = [12, 13, 14, 15]

# Initialize components
dht_sensor = dht.DHT22(Pin(DHT_PIN))
led = Pin(LED_PIN, Pin.OUT)
relays = [Pin(pin, Pin.OUT) for pin in RELAY_PINS]

# Global variables
sensor_data = {
    "temperature": 0,
    "humidity": 0,
    "timestamp": 0
}

def connect_wifi():
    """Connect to WiFi network"""
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    
    if not wlan.isconnected():
        print("Connecting to WiFi...")
        wlan.connect(WIFI_SSID, WIFI_PASSWORD)
        
        timeout = 20
        while not wlan.isconnected() and timeout > 0:
            print(".", end="")
            time.sleep(1)
            timeout -= 1
        
        if wlan.isconnected():
            print("\nWiFi connected!")
            print(f"IP Address: {wlan.ifconfig()[0]}")
        else:
            print("\nFailed to connect to WiFi")
            return None
    
    return wlan.ifconfig()[0]

def read_sensor():
    """Read DHT22 sensor data"""
    try:
        dht_sensor.measure()
        temp = dht_sensor.temperature()
        hum = dht_sensor.humidity()
        
        sensor_data["temperature"] = temp
        sensor_data["humidity"] = hum
        sensor_data["timestamp"] = time.time()
        
        return temp, hum
    except Exception as e:
        print(f"Error reading sensor: {e}")
        return None, None

def handle_request(request):
    """Handle HTTP requests"""
    request = request.decode('utf-8')
    
    # Parse request
    lines = request.split('\n')
    if len(lines) == 0:
        return None
    
    first_line = lines[0]
    method, path, _ = first_line.split(' ')
    
    # Route handling
    if path == '/':
        return serve_index()
    elif path == '/data':
        return serve_sensor_data()
    elif path.startswith('/gpio/'):
        return handle_gpio(path, method)
    elif path == '/toggle':
        return handle_toggle()
    else:
        return "404 Not Found", "text/plain", "Page not found"

def serve_index():
    """Serve the main HTML page"""
    try:
        with open('index.html', 'r') as f:
            html = f.read()
        return "200 OK", "text/html", html
    except:
        return "500 Internal Server Error", "text/plain", "Could not load index.html"

def serve_sensor_data():
    """Serve sensor data as JSON"""
    read_sensor()
    data = json.dumps(sensor_data)
    return "200 OK", "application/json", data

def handle_gpio(path, method):
    """Handle GPIO control requests"""
    if method != 'POST':
        return "405 Method Not Allowed", "text/plain", "Method not allowed"
    
    try:
        parts = path.split('/')
        if len(parts) < 4:
            return "400 Bad Request", "text/plain", "Invalid GPIO request"
        
        gpio_num = int(parts[2])
        state = parts[3].lower()
        
        if gpio_num < 0 or gpio_num >= len(relays):
            return "400 Bad Request", "text/plain", "Invalid GPIO number"
        
        if state == 'on':
            relays[gpio_num].value(1)
        elif state == 'off':
            relays[gpio_num].value(0)
        else:
            return "400 Bad Request", "text/plain", "Invalid state"
        
        response = {"gpio": gpio_num, "state": state}
        return "200 OK", "application/json", json.dumps(response)
        
    except Exception as e:
        return "500 Internal Server Error", "text/plain", f"Error: {str(e)}"

def handle_toggle():
    """Handle LED toggle"""
    led.value(not led.value())
    state = "on" if led.value() == 1 else "off"
    response = {"led": state}
    return "200 OK", "application/json", json.dumps(response)

def create_response(status, content_type, content):
    """Create HTTP response"""
    response = f"HTTP/1.1 {status}\r\n"
    response += f"Content-Type: {content_type}\r\n"
    response += f"Access-Control-Allow-Origin: *\r\n"
    response += f"Connection: close\r\n\r\n"
    response += content
    return response

def web_server():
    """Main web server function"""
    ip_address = connect_wifi()
    if not ip_address:
        print("Failed to start web server - no WiFi connection")
        return
    
    addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1]
    s = socket.socket()
    s.bind(addr)
    s.listen(1)
    
    print(f"Web server running on http://{ip_address}")
    print("Waiting for connections...")
    
    while True:
        try:
            conn, addr = s.accept()
            print(f"Connection from {addr}")
            
            request = conn.recv(1024)
            if request:
                status, content_type, content = handle_request(request)
                response = create_response(status, content_type, content)
                conn.send(response.encode('utf-8'))
            
            conn.close()
            
        except Exception as e:
            print(f"Error handling connection: {e}")
            try:
                conn.close()
            except:
                pass

# Sensor reading thread
def sensor_loop():
    """Continuously read sensor data"""
    while True:
        read_sensor()
        time.sleep(2)

# Main execution
if __name__ == "__main__":
    print("Starting ESP32 Web Server...")
    
    # Start sensor reading in background
    import _thread
    _thread.start_new_thread(sensor_loop, ())
    
    # Start web server
    web_server()
