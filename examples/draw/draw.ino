#include <TFT_eSPI.h>
#include <TAMC_FT62X6.h>
#include <Wire.h>

#define SDA 4
#define SCL 5

uint32_t currentColor = 0xF000;

TFT_eSPI tft = TFT_eSPI();
TAMC_FT62X6 tp = TAMC_FT62X6();

uint16_t colors[7] = {TFT_RED, TFT_ORANGE, TFT_YELLOW, TFT_GREEN, TFT_CYAN, TFT_BLUE, TFT_PURPLE};

void drawButtons() {
  tft.fillRect(0, 0, 40, 30, TFT_RED);
  tft.fillRect(0, 30, 40, 30, TFT_ORANGE);
  tft.fillRect(0, 60, 40, 30, TFT_YELLOW);
  tft.fillRect(0, 90, 40, 30, TFT_GREEN);
  tft.fillRect(0, 120, 40, 30, TFT_CYAN);
  tft.fillRect(0, 150, 40, 30, TFT_BLUE);
  tft.fillRect(0, 180, 40, 30, TFT_PURPLE);
  tft.fillRect(0, 210, 40, 30, TFT_BLACK);
  tft.setCursor(4, 222);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.println("Clear");
}

void setup() {
  Wire.begin(SDA, SCL);
  tft.begin();
  tp.begin();
  tp.setRotation(ROTATION_NORMAL);
  tft.setRotation(ROTATION_NORMAL);
  tft.fillScreen(TFT_WHITE);
  drawButtons();
}

void buttonPressed(int i) {
  drawButtons();
  if (i < 7) {
    tft.drawRect(0, i * 30, 40, 30, TFT_BLACK);
    currentColor = colors[i];
  } else {
    tft.fillRect(40, 0, 320, 240, TFT_WHITE);
  }
}

int lastX = -1;
int lastY = -1;
void loop() {
  int x = 0;
  int y = 0;
  tp.read();
  if (tp.isTouched) {
    x = tp.points[0].x;
    y = tp.points[0].y;
    // if touchstart
    if (lastX == -1) {
      if (x < 40) {
        buttonPressed(y / 30);
      }
    } else {
      if (x > 40) {
        tft.drawLine(lastX, lastY, x, y, currentColor);
      }
    }
    lastX = x;
    lastY = y;
  } else {
    lastX = -1;
    lastY = -1;
  }
}