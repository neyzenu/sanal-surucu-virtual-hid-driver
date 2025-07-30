# sanal-surucu-virtual-hid-driver

# 🧠 Virtual Keyboard Linux Kernel Module / Sanal Klavye Kernel Modülü

Bu proje, bir Linux sanal klavye sürücüsünü kernel seviyesinde oluşturarak Python üzerinden kontrol etmenizi sağlar. Girdi olayları doğrudan sistem seviyesinde gönderilir — tıpkı fiziksel bir klavyeyle yazılmış gibi.

---

## 🇹🇷 Türkçe Dokümantasyon
./install.sh ile kolayca yükleyebilirsiniz
### 🚀 Özellikler
- Kernel seviyesinde çalışan sanal klavye
- `/dev/virtualkbd` dosyası üzerinden tuş komutları alır (`keycode:1` → bas, `keycode:0` → bırak)
- Python API ile kolay kontrol
- Sistem genelinde çalışır, uygulamaya özel değildir

---

### ⚙️ Kurulum

1. Modülü derleyin:
make
Modülü yükleyin:




sudo insmod virtual_keyboard.ko
Cihaz dosyasını kontrol edin:




ls -l /dev/virtualkbd
sudo chmod 666 /dev/virtualkbd
🐍 Python ile Kullanım
python

from virtual_keyboard import VirtualKeyboard

kb = VirtualKeyboard()
kb.type_string("Merhaba Dünya!")
kb.tap_key("ENTER")
kb.press_key("LEFTCTRL")
kb.tap_key("C")
kb.release_key("LEFTCTRL")
🧪 Demo Çalıştırma


python3 demo.py
🛠️ Sorun Giderme
Module not found: dmesg | tail ile hata loglarını kontrol et

Permission denied: sudo chmod 666 /dev/virtualkbd ile erişimi aç

Tuşlar çalışmıyor: cat /proc/bus/input/devices ile kayıtlı aygıtları kontrol et

❌ Modülü Kaldırma


sudo rmmod virtual_keyboard
🇬🇧 English Documentation
You can easily install it with ./install.sh
🚀 Features
Linux kernel module virtual keyboard

Accepts keystroke commands via /dev/virtualkbd in the format keycode:1 (press), keycode:0 (release)

Fully scriptable with Python

System-wide event injection via the Linux input subsystem

⚙️ Installation
Compile the module:




make
Load the module:



sudo insmod virtual_keyboard.ko
Set permissions on the device:


ls -l /dev/virtualkbd
sudo chmod 666 /dev/virtualkbd
🐍 Python Usage
python

from virtual_keyboard import VirtualKeyboard

kb = VirtualKeyboard()
kb.type_string("Hello World!")
kb.tap_key("ENTER")
kb.press_key("LEFTCTRL")
kb.tap_key("C")
kb.release_key("LEFTCTRL")
🧪 Run Demo


python3 demo.py
🛠️ Troubleshooting
Module won't load: Check logs with dmesg | tail

Permission denied: Use sudo chmod 666 /dev/virtualkbd

No key effect: Verify input registration with cat /proc/bus/input/devices

❌ Unload the Module


sudo rmmod virtual_keyboard
