/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2022/09/01
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/** Modified and ported to Arduino by Mitsumine Suzu (C) 2024 */

#include <cstring>

#include <stdarg.h>


extern "C" {
  void dbg_printf(const char* format, ...);
}

/** Workaround implementation of printf() for debug
 *  In the code from WCH that is "C" world, DUG_PRINTF() calls printf() directly which is not worked correctly.
 *  So this C function is needed from C to C++.
 */
void dbg_printf(const char* format, ...) {
    va_list args;
    char    buffer[256] = { 0 };

    // retrieve the variable arguments
    va_start( args, format );

    vsnprintf( buffer, 255, format, args ); // C4996
    serial_write_str(buffer);

    va_end( args );
}

void serial_write_char(char ch) {
    Serial.print(ch);
}

void serial_write_str(const char* str) {
    for (size_t i = 0; i < strlen(str); i++) {
        serial_write_char(str[i]);
    }
}

#include "USBHost_Keyboard.h"

USBHostKeyboard USBKeyboard;

void setup() {
    Delay_Init( );
    Serial.begin(115200);
    Serial.println("Initializing...");
    Serial.println("USBHost Keyboard demo");
    Serial.printf("SystemCoreClock = %u\r\n", SystemCoreClock);

    pinMode(PA5, OUTPUT);
    for (int i = 0; i < 4; i++) {
        digitalWrite(PA5, HIGH);
        delay(250);
        digitalWrite(PA5, LOW);
        delay(250);
    }

    USBKeyboard.init();

    Serial.println("Ready.");
}

void loop() {
  static char prev_key = 0;
  USBKeyboard.update();
  
  if (USBKeyboard.available()) {
      int ch = USBKeyboard.get_key();

      if (ch == 0 && prev_key > 0) {
        Serial.printf("<<<< Released 0x%02x\r\n", prev_key);
        prev_key = 0;

      } else if (ch > 0 && ch != prev_key) {
        Serial.printf(">>>> Pressed 0x%02x\r\n", ch);
        prev_key = (char)ch;
      }
  }
}
