from virtual_keyboard import VirtualKeyboard
import time

def main():
    # Initialize the virtual keyboard
    kb = VirtualKeyboard()
    
    print("Starting virtual keyboard demo in 3 seconds...")
    time.sleep(3)
    
    # Type some text
    kb.type_string("Hello, this is a virtual keyboard test!")
    
    # Press Enter
    kb.tap_key('ENTER')
    
    # Perform a keyboard shortcut (Ctrl+A)
    kb.press_key('LEFTCTRL')
    kb.tap_key('A')
    kb.release_key('LEFTCTRL')
    
    print("Demo completed!")

if __name__ == "__main__":
    main()
