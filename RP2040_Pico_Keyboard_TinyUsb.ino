/*********************************************************************
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 Copyright (c) 2019 Ha Thach for Adafruit Industries
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

#include "Adafruit_TinyUSB.h"

// HID report descriptor using TinyUSB's template
// Single Report (no ID) descriptor

uint8_t const desc_hid_report[] = {
    TUD_HID_REPORT_DESC_KEYBOARD()
};

// USB HID object. For ESP32 these values cannot be changed after this declaration
// desc report, desc len, protocol, interval, use out endpoint

Adafruit_USBD_HID usb_hid;

//------------- Input Pins -------------//
#define PIN_BUTTON0 0  
#define PIN_BUTTON1 1
#define PIN_BUTTON2 2
#define PIN_BUTTON3 3
#define PIN_BUTTON4 4
#define PIN_BUTTON5 5
#define PIN_BUTTON6 6
#define PIN_BUTTON7 7
#define PIN_BUTTON8 8
#define PIN_BUTTON9 9
#define PIN_BUTTON10 10
#define PIN_BUTTON11 11
#define PIN_BUTTON12 12
#define PIN_BUTTON13 13
#define PIN_BUTTON14 14
#define PIN_BUTTON15 15
#define PIN_BUTTON16 16
#define PIN_BUTTON17 17
#define PIN_BUTTON18 18
#define PIN_BUTTON19 19
#define PIN_BUTTON20 20
#define PIN_BUTTON21 21
#define PIN_BUTTON22 22
#define PIN_BUTTON26 26
#define PIN_BUTTON27 27
#define PIN_BUTTON28 28


// 26 GPIO pins  0 -> 22 , 26 -> 28

const uint8_t pincount = 26;

uint8_t pins[pincount] = {
    PIN_BUTTON0, PIN_BUTTON1, PIN_BUTTON2, PIN_BUTTON3, 
    PIN_BUTTON4, PIN_BUTTON5, PIN_BUTTON6, 
    PIN_BUTTON7, PIN_BUTTON8, 
    PIN_BUTTON9, PIN_BUTTON10, 
    
    PIN_BUTTON11, PIN_BUTTON12, PIN_BUTTON13, 
    PIN_BUTTON14, PIN_BUTTON15, PIN_BUTTON16,
    PIN_BUTTON17, PIN_BUTTON18, PIN_BUTTON19,
    
    PIN_BUTTON20,
    PIN_BUTTON21,
    PIN_BUTTON22, PIN_BUTTON26, PIN_BUTTON27, 
    PIN_BUTTON28
};

// Map to HID keycodes  /*   HID_KEY_KEYPAD_DECIMAL  HID_KEY_KEYPAD_COLON    */
uint8_t hidcode[pincount] = {    
    HID_KEY_ARROW_UP,         HID_KEY_ARROW_DOWN,     HID_KEY_ARROW_LEFT,  HID_KEY_ARROW_RIGHT,
    HID_KEY_CONTROL_RIGHT,    HID_KEY_SHIFT_RIGHT,    HID_KEY_ENTER,   
    HID_KEY_HOME,             HID_KEY_END,   
    HID_KEY_KEYPAD_0,         HID_KEY_KEYPAD_DECIMAL,
    
    HID_KEY_KEYPAD_1,         HID_KEY_KEYPAD_2,       HID_KEY_KEYPAD_3,       
    HID_KEY_KEYPAD_4,         HID_KEY_KEYPAD_5,       HID_KEY_SEMICOLON,     /* HID_KEY_M,  */     
    HID_KEY_KEYPAD_7,         HID_KEY_KEYPAD_8,       HID_KEY_L,     
    
    HID_KEY_SPACE,      
    HID_KEY_ESCAPE,
    HID_KEY_F1,               HID_KEY_F2,           HID_KEY_F3,     
    HID_KEY_KEYPAD_ADD    
};

#if defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(ARDUINO_NRF52840_CIRCUITPLAY) || defined(ARDUINO_FUNHOUSE_ESP32S2)
bool activeState = true;
#else
bool activeState = false;
#endif

// the setup function runs once when you press reset or power the board
void setup() {
  // Manual begin() is required on core without built-in support e.g. mbed rp2040
  if (!TinyUSBDevice.isInitialized()) {
    TinyUSBDevice.begin(0);
  }

  // Setup HID
  usb_hid.setBootProtocol(HID_ITF_PROTOCOL_KEYBOARD);
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.setStringDescriptor("TinyUSB Keyboard");

  // Set up output report (on control endpoint) for Capslock indicator
  usb_hid.setReportCallback(NULL, hid_report_callback);

  usb_hid.begin();

  // If already enumerated, additional class driverr begin() e.g msc, hid, midi won't take effect until re-enumeration
  if (TinyUSBDevice.mounted()) {
    TinyUSBDevice.detach();
    delay(10);
    TinyUSBDevice.attach();
  }

  // led pin
#ifdef LED_BUILTIN
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
#endif


  // Set up pin as input
  for (uint8_t i = 0; i < pincount; i++) {
    pinMode(pins[i], activeState ? INPUT_PULLDOWN : INPUT_PULLUP);
  }
}

void process_hid() {
  // used to avoid send multiple consecutive zero report for keyboard
  static bool keyPressedPreviously = false;

  uint8_t count = 0;
  uint8_t keycode[6] = {0};

  // scan normal key and send report
  for (uint8_t i = 0; i < pincount; i++) {
    if (activeState == digitalRead(pins[i])) {
      // if pin is active (low), add its hid code to key report
      keycode[count++] = hidcode[i];

      // 6 is max keycode per report
      if (count == 6) break;
    }
  }

  if (TinyUSBDevice.suspended() && count) {
    // Wake up host if we are in suspend mode
    // and REMOTE_WAKEUP feature is enabled by host
    TinyUSBDevice.remoteWakeup();
  }

  // skip if hid is not ready e.g still transferring previous report
  if (!usb_hid.ready()) return;

  if (count) {
    // Send report if there is key pressed
    uint8_t const report_id = 0;
    uint8_t const modifier = 0;

    keyPressedPreviously = true;
    usb_hid.keyboardReport(report_id, modifier, keycode);
  } else {
    // Send All-zero report to indicate there is no keys pressed
    // Most of the time, it is, though we don't need to send zero report
    // every loop(), only a key is pressed in previous loop()
    if (keyPressedPreviously) {
      keyPressedPreviously = false;
      usb_hid.keyboardRelease(0);
    }
  }
}

void loop() {
  #ifdef TINYUSB_NEED_POLLING_TASK
  // Manual call tud_task since it isn't called by Core's background
  TinyUSBDevice.task();
  #endif

  // not enumerated()/mounted() yet: nothing to do
  if (!TinyUSBDevice.mounted()) {
    return;
  }

  // poll gpio once each 2 ms
  static uint32_t ms = 0;
  if (millis() - ms > 2) {
    ms = millis();
    process_hid();
  }
}

// Output report callback for LED indicator such as Caplocks
void hid_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
  (void) report_id;
  (void) bufsize;

  // LED indicator is output report with only 1 byte length
  if (report_type != HID_REPORT_TYPE_OUTPUT) return;

  // The LED bit map is as follows: (also defined by KEYBOARD_LED_* )
  // Kana (4) | Compose (3) | ScrollLock (2) | CapsLock (1) | Numlock (0)
  uint8_t ledIndicator = buffer[0];

#ifdef LED_BUILTIN
  // turn on LED if capslock is set
  digitalWrite(LED_BUILTIN, ledIndicator & KEYBOARD_LED_CAPSLOCK);
#endif
}
