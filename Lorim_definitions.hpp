#pragma once

#define GEM_DISABLE_GLCD
#define GEM_DISABLE_ADAFRUIT_GFX
#define U8G2_DISPLAY_TYPE U8G2_ST7565_JLX12864_F_4W_SW_SPI

#define CH450_INT D0

// Task IDs (this is not even close to elegance)
#define DEFAULT_TASK ID_TASKGEM
#define ID_NONE    0
#define ID_TASKGEM 1
