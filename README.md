# ESP32 Mini Web Server — IoT Monitoring Panel

ESP32 mikrokontrolleri asosida yaratilgan mini web server. IoT (Internet of Things) konsepsiyasi doirasida arzon, energiya tejamkor va yuqori samaradorlikka ega platforma.

**Ilmiy maqola loyihasi**  
**Muallif:** Ibodullayev Jaxongir Maxammadi o'g'li  
**Muassasa:** Muhammad al-Xorazmiy nomidagi Toshkent axborot texnologiyalari universiteti  
**Fakultet:** "Kompyuter injiniringi"  
**Kafedra:** "Kompyuter tizimlarini loyihalash"  
**Kurs:** 1-kurs magistranti  
**Email:** jahon18gir@gmail.com  
**Tel:** (95)225-50-60
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

### Variant 1: Wokwi Simulyatori (Tavsiya etiladi)

1. [Wokwi.com](https://wokwi.com) saytiga kiring
2. "ESP32" qidiring yoki yangi loyiha yarating
3. `wokwi_sketch` papkasidagi fayllarni simulyatorga yuklang:
   - `wokwi_sketch.ino` - Asosiy Arduino kod
   - `diagram.json` - Sxema konfiguratsiyasi
   - `libraries.txt` - Kerakli kutubxonalar
4. "Start Simulation" tugmasini bosing

### Variant 2: Brauzer simulyatsiyasi

1. Loyihani kompyuteringizga yuklab oling:
```bash
git clone https://github.com/username/ESP32-Dashboard.git
cd ESP32-Dashboard
```

2. `index.html` faylini brauzeringizda oching:
```
Double-click on index.html
```

### Variant 3: Haqiqiy ESP32 qurilmasi

1. Arduino IDE yoki PlatformIO o'rnating
2. ESP32 board support qo'shing
3. Kerakli kutubxonalarni o'rnating:
   - DHT sensor library (Adafruit)
   - ArduinoJson (Benoit Blanchon)
4. `wokwi_sketch.ino` faylini oching
5. WiFi ma'lumotlarini kiriting:
```cpp
const char* WIFI_SSID = "Sizning_WiFi_Nomingiz";
const char* WIFI_PASSWORD = "Sizning_WiFi_Parolingiz";
```
6. ESP32 ga kodni yuklang
7. Serial monitor da IP manzilni ko'ring va brauzeringizda oching

## Funktsiyalar

### Sensor kartalari
- **Harorat**: Real vaqt harorat ko'rsatkichi (-10°C dan 50°C gacha)
- **Namlik**: Real vaqt namlik ko'rsatkichi (0% dan 100% gacha)
- Har 2 soniyada avtomatik yangilanish

### GPIO boshqaruv
- **Buzzer (GPIO 26)**: Yoqish/O'chirish tugmasi
- **Relay (GPIO 27)**: Yoqish/O'chirish tugmasi
- Tugma holatlari vizual indikator bilan ko'rsatiladi

### Grafiklar

1. **Chiziqli grafik**: Vaqt bo'yicha harorat va namlik o'zgarishi
2. **Ustunli grafik**: Joriy sensor qiymatlari
3. **Heatmap**: 10x10 grid da sensor ma'lumotlarining 2D tasviri

### Serial Monitor
- Real vaqt loglari
- WiFi ulanish holati
- Sensor o'qish natijalari
- GPIO holat o'zgarishlari

## Texnologiyalar

- **HTML5** - Strukturaviy belgilash
- **CSS3** - Modern dizayn va animatsiyalar
- **JavaScript** - Interaktiv funksiyalar
- **Chart.js** - Grafiklar kutubxonasi
- **Canvas API** - Heatmap chizish

## Brauzer mosligi

- Chrome 60+
- Firefox 55+
- Safari 12+
- Edge 79+

## Mobil qurilmalar

Loyiha to'liq responsive va quyidagi mobil qurilmalarda optimal ishlaydi:
- iOS 12+
- Android 8+

## Maqola uchun foydalanish

Bu loyiha ilmiy maqolalarda quyidagilarni namoyish qilish uchun mo'ljallangan:
- IoT tizimlarining web interfeysi
- Real vaqt ma'lumotlari vizualizatsiyasi
- ESP32 mikrokontrollerining imkoniyatlari

## Rang sxemasi

- **Fon**: Chuqur ko'k-yashil gradient
- **Harorat**: Orange (#ff8a65)
- **Namlik**: Teal (#4dd0e1)
- **Buzzer (yoqilgan)**: Yashil (#4caf50)
- **Relay (yoqilgan)**: Ko'k (#2196f3)

## Litsenziya

MIT License - istalgan maqsadda foydalanishingiz mumkin.

## Maqola maqsadi

Ushbu loyiha quyidagi ilmiy maqsadlarni ko'zlaydi:
- ESP32 asosida veb-server qurish
- Wi-Fi orqali ma'lumotlarni uzatish va qabul qilish
- Sensorlardan kelayotgan ma'lumotlarni qayta ishlash
- Real vaqt rejimida sensor ma'lumotlarini yig'ish va vizuallashtirish
- Aqlli uy va ekologik monitoring tizimlarida qo'llash

## Muallif

**Ibodullayev Jaxongir Maxammadi o'g'li**  
Muhammad al-Xorazmiy nomidagi Toshkent axborot texnologiyalari universiteti  
"Kompyuter injiniringi" fakulteti, "Kompyuter tizimlarini loyihalash" kafedrasi  
1-kurs magistranti  
Email: jahon18gir@gmail.com  
Tel: (95)225-50-60

## Adabiyotlar

1. Espressif Systems. (2023). ESP32 Technical Reference Manual.
2. Kolban N. (2019). Kolban's Book on ESP32.
3. Banzi M., Shiloh M. (2014). Getting Started with Arduino. Maker Media.
4. Al Fayez F., et al. (2022). IoT Based Smart Home Automation Using ESP32.
5. Shilov A. (2021). Low-cost IoT Servers Based on ESP32.
6. Sharma R., et al. (2022). Data Visualization in IoT Applications Using Python and ESP32.
7. Kurniawan A. (2018). Internet of Things Projects with ESP32. Packt Publishing.
8. Lee J., et al. (2020). Cloud Integration for IoT Devices Using ESP32.
9. Al-Haddad S., et al. (2021). Real-Time Data Monitoring Systems Based on ESP32.
10. Espressif Systems. (2024). ESP-IDF Programming Guide.

---

**Eslatma:** Bu loyiha Wokwi simulyatorida ham, haqiqiy ESP32 qurilmasida ham ishlatilishi mumkin. Wokwi variantida hech qanday hardware talab qilinmaydi.
