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

void Init_HardwareTimer_TIM3(uint16_t period/*, uint16_t prescaler*/) {
#if false
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = { 0 };
    NVIC_InitTypeDef NVIC_InitStructure = { 0 };

    /* Enable timer3 clock */
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );

    /* Initialize timer3 */
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure );

    /* Enable updating timer3 interrupt */
    TIM_ITConfig( TIM3, TIM_IT_Update, ENABLE );

    /* Configure timer3 interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );

    /* Enable timer3 */
    TIM_Cmd( TIM3, ENABLE );

    /* Enable timer3 interrupt */
    NVIC_EnableIRQ( TIM3_IRQn );
#endif
#if false
    prescaler /= 1000;
    Serial.printf("Init_HardwareTimer_TIM3(%u, %u)\n", period, prescaler);
    Timer3.pause();
    TRACE();
    Timer3.setPrescaleFactor(prescaler);
    TRACE();
    Timer3.setOverflow(period);
    TRACE();
    Timer3.refresh();

    TRACE();
    Serial.printf("Timer3 clock = %u[Hz]\n", Timer3.getTimerClkFreq());

    TRACE();
    Timer3.resume();
    TRACE();
    NVIC_InitTypeDef NVIC_InitStructure = { 0 };
    /* Configure timer3 interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );
    // Timer3.setInterruptPriority(4,4);
    TRACE();
    /* Enable timer3 interrupt */
    NVIC_EnableIRQ( TIM3_IRQn );
    // Timer3.attachInterrupt(TIM3_IRQHandler_);
    TRACE();
#endif
#if false
    // WORKAROUND:
    Timer3.attachInterrupt(TIM3_IRQHandler_);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = { 0 };
    NVIC_InitTypeDef NVIC_InitStructure = { 0 };

    /* Enable timer3 clock */
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );

    /* Initialize timer3 */
    TIM_TimeBaseStructure.TIM_Period = period;
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure );

    /* Enable updating timer3 interrupt */
    TIM_ITConfig( TIM3, TIM_IT_Update, ENABLE );

    /* Configure timer3 interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );

    /* Enable timer3 */
    TIM_Cmd( TIM3, ENABLE );

    /* Enable timer3 interrupt */
    NVIC_EnableIRQ( TIM3_IRQn );
#endif
#if true
    Timer3.pause();
    Timer3.setOverflow(period * 1000, MICROSEC_FORMAT);
    Timer3.attachInterrupt(TIM3_IRQHandler_);
    Timer3.refresh();
    Timer3.resume();
#endif
}

void USBHostKeyboard::init() {
    is_usbkeyboard_connected = false;
    memset((void*)global_hid_keyboard_report, 0x00, 8);

    Delay_Init();
    
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
