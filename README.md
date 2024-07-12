# USB Host sample for CH32V203 in Arduino

Based on https://github.com/openwch/ch32v20x/tree/main/EVT/EXAM/USB/USBFS/HOST_KM

Tested on Suzuno32RV (CH32V203C8T6)

これは、Suzuno32RVでUSBキーボードの入力を受け取るサンプルコードです。Type-Aコネクタに接続されたキーボードの入力を受け取り、情報をシリアルポートに出力します。

Arduino IDE、arduino_core_ch32_sz 1.0.4+sz4 で動作確認しています。

シリアル通信はGPIOに引き出されたハードウェアシリアルも、TinyUSBによるUSBシリアルも、どちらも利用できます。
