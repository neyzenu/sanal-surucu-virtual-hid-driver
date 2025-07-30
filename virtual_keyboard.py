import os
import time

class VirtualKeyboard:
    """Python interface for the virtual keyboard kernel module"""
    
    # Linux input event keycodes
    KEY_CODES = {
        'ESC': 1, '1': 2, '2': 3, '3': 4, '4': 5, '5': 6, '6': 7, '7': 8, '8': 9,
        '9': 10, '0': 11, 'MINUS': 12, 'EQUAL': 13, 'BACKSPACE': 14, 'TAB': 15,
        'Q': 16, 'W': 17, 'E': 18, 'R': 19, 'T': 20, 'Y': 21, 'U': 22, 'I': 23,
        'O': 24, 'P': 25, 'LEFTBRACE': 26, 'RIGHTBRACE': 27, 'ENTER': 28,
        'LEFTCTRL': 29, 'A': 30, 'S': 31, 'D': 32, 'F': 33, 'G': 34, 'H': 35,
        'J': 36, 'K': 37, 'L': 38, 'SEMICOLON': 39, 'APOSTROPHE': 40, 'GRAVE': 41,
        'LEFTSHIFT': 42, 'BACKSLASH': 43, 'Z': 44, 'X': 45, 'C': 46, 'V': 47,
        'B': 48, 'N': 49, 'M': 50, 'COMMA': 51, 'DOT': 52, 'SLASH': 53,
        'RIGHTSHIFT': 54, 'KPASTERISK': 55, 'LEFTALT': 56, 'SPACE': 57,
        'CAPSLOCK': 58, 'F1': 59, 'F2': 60, 'F3': 61, 'F4': 62, 'F5': 63,
        'F6': 64, 'F7': 65, 'F8': 66, 'F9': 67, 'F10': 68, 'NUMLOCK': 69,
        'SCROLLLOCK': 70, 'KP7': 71, 'KP8': 72, 'KP9': 73, 'KPMINUS': 74,
        'KP4': 75, 'KP5': 76, 'KP6': 77, 'KPPLUS': 78, 'KP1': 79, 'KP2': 80,
        'KP3': 81, 'KP0': 82, 'KPDOT': 83, 'F11': 87, 'F12': 88, 'KPENTER': 96,
        'RIGHTCTRL': 97, 'KPSLASH': 98, 'RIGHTALT': 100, 'HOME': 102,
        'UP': 103, 'PAGEUP': 104, 'LEFT': 105, 'RIGHT': 106, 'END': 107,
        'DOWN': 108, 'PAGEDOWN': 109, 'INSERT': 110, 'DELETE': 111
    }
    
    def __init__(self, device_path="/dev/virtualkbd"):
        """Initialize the virtual keyboard"""
        self.device_path = device_path
        
    def press_key(self, key):
        """Press a key"""
        if isinstance(key, str):
            key = self.KEY_CODES.get(key.upper())
            if key is None:
                raise ValueError(f"Unknown key: {key}")
                
        with open(self.device_path, "w") as dev:
            dev.write(f"{key}:1")
            
    def release_key(self, key):
        """Release a key"""
        if isinstance(key, str):
            key = self.KEY_CODES.get(key.upper())
            if key is None:
                raise ValueError(f"Unknown key: {key}")
                
        with open(self.device_path, "w") as dev:
            dev.write(f"{key}:0")
            
    def tap_key(self, key, delay=0.1):
        """Press and release a key with delay in between"""
        self.press_key(key)
        time.sleep(delay)
        self.release_key(key)
        
    def type_string(self, text, delay=0.05):
        """Type a string by simulating keystrokes"""
        for char in text:
            # This is a simplified version, would need a proper char-to-keycode mapping
            if char.upper() in self.KEY_CODES:
                if char.isupper():
                    self.press_key('LEFTSHIFT')
                    self.tap_key(char.upper())
                    self.release_key('LEFTSHIFT')
                else:
                    self.tap_key(char.upper())
            elif char == ' ':
                self.tap_key('SPACE')
            time.sleep(delay)
