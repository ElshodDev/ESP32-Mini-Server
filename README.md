# ESP32 Mini Web Server — IoT Monitoring Panel

ESP32 mikrokontrolleri asosida yaratilgan mini web server. IoT (Internet of Things) konsepsiyasi doirasida arzon, energiya tejamkor va yuqori samaradorlikka ega platforma.

## Live Demo

🌐 **GitHub Pages:** https://elshoddev.github.io/ESP32-Mini-Server

📁 **GitHub Repository:** https://github.com/ElshodDev/ESP32-Mini-Server

## Xususiyatlar

- 🌡️ Real vaqt harorat va namlik monitoringi
- 📊 3 xil grafik turi (chiziqli, ustunli, heatmap)
- 🔊 GPIO boshqaruv (Buzzer va Relay)
- 📱 Responsive dizayn (mobil qurilmalar uchun)
- ⚡ Real vaqt yangilanishlar
- 📝 Serial monitor loglari
- 🎛️ Sensor simulyatsiya sliderlari

## O'rnatish va ishlatish

### Variant 1: Brauzer simulyatsiyasi

1. Loyihani kompyuteringizga yuklab oling:
```bash
git clone https://github.com/ElshodDev/ESP32-Mini-Server.git
cd ESP32-Mini-Server
```

2. `index.html` faylini brauzeringizda oching

### Variant 2: Haqiqiy ESP32 qurilmasi

1. Arduino IDE yoki PlatformIO o'rnating
2. ESP32 board support qo'shing
3. Kerakli kutubxonalarni o'rnating:
   - DHT sensor library (Adafruit)
   - ArduinoJson (Benoit Blanchon)
4. `wokwi_sketch/wokwi_sketch.ino` faylini oching
5. WiFi ma'lumotlarini kiriting va ESP32 ga yuklang

## Fayllar tavsifi

| Fayl | Tavsif |
|------|--------|
| `index.html` | Professional dashboard (brauzerda oching) |
| `main.py` | MicroPython versiyasi |
| `wokwi_sketch/wokwi_sketch.ino` | Arduino C++ kodi |

## Grafiklar

1. **Rasm 1**: Chiziqli grafik - Vaqt bo'yicha harorat va namlik o'zgarishi
2. **Rasm 2**: Ustunli grafik - Joriy sensor qiymatlari
3. **Rasm 3**: Heatmap - Sensor ma'lumotlarining 2D tasviri

## Texnologiyalar

- **HTML5** - Strukturaviy belgilash
- **CSS3** - Modern dizayn va animatsiyalar
- **JavaScript** - Interaktiv funksiyalar
- **Canvas API** - Grafiklar va heatmap

## Litsenziya

MIT License - istalgan maqsadda foydalanishingiz mumkin.
