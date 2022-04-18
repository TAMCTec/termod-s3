#define ST7789_DRIVER
#define TFT_RGB_ORDER TFT_BGR
#define TFT_WIDTH  240
#define TFT_HEIGHT 320

#define TFT_MISO 13
#define TFT_MOSI 11
#define TFT_SCLK 12
#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST  14

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_FONT8N
#define LOAD_GFXFF

#define SMOOTH_FONT

#define SPI_FREQUENCY  27000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000

#define USE_HSPI_PORT