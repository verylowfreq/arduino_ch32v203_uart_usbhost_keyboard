#pragma once

#include <stddef.h>
#include <stdint.h>

class USBHostKeyboard {
public:
    bool is_keyboard_connected;

    void init();
    void update();
    int available();
    int get_key();
    size_t get_all_key(uint8_t* buf, size_t buflen);
};

extern "C" {
  extern volatile uint8_t global_hid_keyboard_report[8];
  extern volatile int is_usbkeyboard_connected;
}
