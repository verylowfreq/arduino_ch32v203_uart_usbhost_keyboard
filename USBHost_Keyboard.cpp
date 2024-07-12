#include "USBHost_Keyboard.h"

#include "ch32v20x_usbfs_host.h"
#include "app_km.h"
#include "usb_host_config.h"

#include "Arduino.h"
#include "HardwareTimer.h"

#define TRACE() do { Serial.printf("TRACE: LINE %d in %s()\n",  __LINE__, __func__); } while (false)

extern "C" {
  void TIM3_IRQHandler_(void);
}

HardwareTimer Timer3(TIM3);

void Init_HardwareTimer_TIM3(uint16_t period) {
    Timer3.pause();
    Timer3.setOverflow(period * 1000, MICROSEC_FORMAT);
    Timer3.attachInterrupt(TIM3_IRQHandler_);
    Timer3.refresh();
    Timer3.resume();
}

void USBHostKeyboard::init() {
    is_usbkeyboard_connected = false;
    memset((void*)global_hid_keyboard_report, 0x00, 8);

    // NOTE: Delay_Init(),Delay_Ms(),Delay_Us() are from MounRiver Studio SDK
    //       which cannot use delay(),delayMicroseconds() from Arduino at same time.
    // Delay_Init();
    
    // TIM3_Init( 9, SystemCoreClock / 10000 - 1 );
    Init_HardwareTimer_TIM3(10);

    USBFS_RCC_Init( );
    USBFS_Host_Init( ENABLE );
    memset( &RootHubDev.bStatus, 0, sizeof( ROOT_HUB_DEVICE ) );
    memset( &HostCtl[ DEF_USBFS_PORT_INDEX * DEF_ONE_USB_SUP_DEV_TOTAL ].InterfaceNum, 0, DEF_ONE_USB_SUP_DEV_TOTAL * sizeof( HOST_CTL ) );

}

void USBHostKeyboard::update() {
    USBH_MainDeal();
}

int USBHostKeyboard::available() {
    return is_usbkeyboard_connected;
}

int USBHostKeyboard::get_key() {
    return global_hid_keyboard_report[2];
}

size_t USBHostKeyboard::get_all_key(uint8_t* buf, size_t buflen) {
    size_t cur = 0;
    for (size_t i = 0; i < 6 && cur < buflen; i++) {
        uint8_t val = global_hid_keyboard_report[2 + i];
        if (val != 0x00) {
            buf[cur] = val;
            cur++;
        }
    }
    return cur;
}

volatile int is_usbkeyboard_connected;
volatile uint8_t global_hid_keyboard_report[8];
