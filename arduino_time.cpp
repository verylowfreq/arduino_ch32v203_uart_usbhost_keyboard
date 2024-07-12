#include <Arduino.h>
#include "arduino_time.h"

/* NOTE:
delayMicroseconds() in Arduino world is "inline" function.
We need it as symbol but couldn't, so make the wrapper function.
 */

void delay_microseconds(uint32_t delay_us) {
  delayMicroseconds(delay_us);
}
