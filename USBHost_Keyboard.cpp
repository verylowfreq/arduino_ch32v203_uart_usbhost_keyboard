#include "USBHost_Keyboard.h"

#include "src/USB_Host/ch32v20x_usbfs_host.h"
#include "src/USB_Host/app_km.h"
#include "src/USB_Host/usb_host_config.h"

void USBHostKeyboard::init() {
    is_usbkeyboard_connected = false;
    memset(global_hid_keyboard_report, 0x00, 8);

    Delay_Init();
    TIM3_Init( 9, SystemCoreClock / 10000 - 1 );
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
