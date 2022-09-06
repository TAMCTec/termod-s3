#if ARDUINO_USB_MODE
#warning This sketch should be used when USB is in OTG mode
void setup(){}
void loop(){}
#else

#include "lv_helper.h"
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDConsumerControl.h"

USBHIDConsumerControl ConsumerControl;
USBHIDKeyboard Keyboard;

#define CONSUMER_CONTROL_INTERNET_BROWSER 0x0196

LV_IMG_DECLARE(calculator_icon);
LV_IMG_DECLARE(kicad_icon);
LV_IMG_DECLARE(arduino_icon);
LV_IMG_DECLARE(vscode_icon);

#define KEYBOARD_LAYOUT_MAC 0
#define KEYBOARD_LAYOUT_WINDOWS 1
// If you are using a Mac, set this to KEYBOARD_LAYOUT_MAC
#define KEYBOARD_LAYOUT KEYBOARD_LAYOUT_WINDOWS

#define LAYOUT_WIDTH 4
#define LAYOUT_HEIGHT 3
#define PADDING 2
#define BUTTON_WIDTH 320 / LAYOUT_WIDTH - (2 * PADDING)
#define BUTTON_HEIGHT 240 / LAYOUT_HEIGHT - (2 * PADDING)

static lv_style_t pressedStyle;

lv_obj_t* createButton(int x, int y, void (*onPressed)(_lv_event_t*), void (*onReleased)(_lv_event_t*), void (*onTap)(_lv_event_t*));
void createTextButton(char* text, int x, int y, void (*onPressed)(_lv_event_t*), void (*onReleased)(_lv_event_t*), void (*onTap)(_lv_event_t*));
void createIconButton(const lv_img_dsc_t *image, int x, int y, void (*onPressed)(_lv_event_t*), void (*onReleased)(_lv_event_t*), void (*onTap)(_lv_event_t*));


void setup() {
  Serial.begin(115200);
  lh_init(DISPLAY_LANDSCAPE);
  
  // Create button style, when button is pressed, glow it
  lv_style_init(&pressedStyle);
  lv_style_set_border_color(&pressedStyle, lv_color_hex(0x33dddd));
  lv_style_set_shadow_color(&pressedStyle, lv_color_hex(0x33dddd));
  lv_style_set_shadow_width(&pressedStyle, 2);
  Keyboard.begin();
  ConsumerControl.begin();
  USB.begin();


  createIconButton(&calculator_icon, 0, 0, NULL, NULL, openCalculator);
  createIconButton(&kicad_icon, 1, 0, NULL, NULL, openKicad);
  createIconButton(&arduino_icon, 2, 0, NULL, NULL, openArduino);
  createIconButton(&vscode_icon, 3, 0, NULL, NULL, openVSCode);
  createTextButton(LV_SYMBOL_VOLUME_MID, 0, 1, volumeDownPressed, volumeDownReleased, NULL);
  createTextButton(LV_SYMBOL_VOLUME_MAX, 1, 1, volumeUpPressed, volumeUpReleased, NULL);
  createTextButton(LV_SYMBOL_MUTE, 2, 1, NULL, NULL, mute);
  createTextButton(LV_SYMBOL_HOME, 3, 1, NULL, NULL, home);
  createTextButton(LV_SYMBOL_COPY, 0, 2, NULL, NULL, copy);
  createTextButton(LV_SYMBOL_PASTE, 1, 2, NULL, NULL, paste);
  createTextButton(LV_SYMBOL_LEFT, 2, 2, NULL, NULL, leftDesktop);
  createTextButton(LV_SYMBOL_RIGHT, 3, 2, NULL, NULL, rightDesktop);
}

void loop() {
  lv_timer_handler();
}

void openCalculator(_lv_event_t* event) {
  ConsumerControl.press(CONSUMER_CONTROL_CALCULATOR);
  ConsumerControl.release();
}

void openKicad(_lv_event_t* event) {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('1');
  Keyboard.releaseAll();
}

void openArduino(_lv_event_t* event) {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('2');
  Keyboard.releaseAll();
}

void openVSCode(_lv_event_t* event) {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('3');
  Keyboard.releaseAll();
}

void volumeDownPressed(_lv_event_t* event) {
  ConsumerControl.press(CONSUMER_CONTROL_VOLUME_DECREMENT);
}
void volumeDownReleased(_lv_event_t* event) {
  ConsumerControl.release();
}
void volumeUpPressed(_lv_event_t* event) {
  ConsumerControl.press(CONSUMER_CONTROL_VOLUME_INCREMENT);
}
void volumeUpReleased(_lv_event_t* event) {
  ConsumerControl.release();
}
void mute(_lv_event_t* event) {
  ConsumerControl.press(CONSUMER_CONTROL_MUTE);
  ConsumerControl.release();
}
void copy(_lv_event_t* event) {
  #if KEYBOARD_LAYOUT == KEYBOARD_LAYOUT_WINDOWS
  Keyboard.press(KEY_LEFT_CTRL);
  #else
  Keyboard.press(KEY_LEFT_GUI);
  #endif
  Keyboard.press('c');
  Keyboard.releaseAll();
}

void paste(_lv_event_t* event) {
  #if KEYBOARD_LAYOUT == KEYBOARD_LAYOUT_WINDOWS
  Keyboard.press(KEY_LEFT_CTRL);
  #else
  Keyboard.press(KEY_LEFT_GUI);
  #endif
  Keyboard.press('v');
  Keyboard.releaseAll();
}

void home(_lv_event_t* event) {
  #if KEYBOARD_LAYOUT == KEYBOARD_LAYOUT_WINDOWS
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('d');
  #endif
  Keyboard.releaseAll();
}

void leftDesktop(_lv_event_t* event) {
  Keyboard.press(KEY_LEFT_CTRL);
  #if KEYBOARD_LAYOUT == KEYBOARD_LAYOUT_WINDOWS
  Keyboard.press(KEY_LEFT_GUI);
  #endif
  Keyboard.press(KEY_LEFT_ARROW);
  Keyboard.releaseAll();
}

void rightDesktop(_lv_event_t* event) {
  Keyboard.press(KEY_LEFT_CTRL);
  #if KEYBOARD_LAYOUT == KEYBOARD_LAYOUT_WINDOWS
  Keyboard.press(KEY_LEFT_GUI);
  #endif
  Keyboard.press(KEY_RIGHT_ARROW);
  Keyboard.releaseAll();
}

// create a button
lv_obj_t* createButton(int x, int y, void (*onPressed)(_lv_event_t*), void (*onReleased)(_lv_event_t*), void (*onTap)(_lv_event_t*)) {
  int top = x * 80 + PADDING;
  int left = y * 80 + PADDING;

  lv_obj_t* btn = lv_obj_create(lv_scr_act());
  lv_obj_set_size(btn, BUTTON_WIDTH, BUTTON_HEIGHT);
  lv_obj_align(btn, LV_ALIGN_TOP_LEFT, top, left);
  lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_style(btn, &pressedStyle, LV_STATE_PRESSED);
  if (onPressed != NULL) {
    lv_obj_add_event_cb(btn, onPressed, LV_EVENT_PRESSED, NULL);
  }
  if (onReleased != NULL) {
    lv_obj_add_event_cb(btn, onReleased, LV_EVENT_RELEASED, NULL);
  }
  if (onTap != NULL) {
    lv_obj_add_event_cb(btn, onTap, LV_EVENT_CLICKED, NULL);
  }

  return btn;
}

void createTextButton(char* text, int x, int y, void (*onPressed)(_lv_event_t*), void (*onReleased)(_lv_event_t*), void (*onTap)(_lv_event_t*)) {
  lv_obj_t* btn = createButton(x, y, onPressed, onReleased, onTap);
  lv_obj_t* label = lv_label_create(btn);
  lv_label_set_text(label, text);
  lv_obj_set_style_text_font(label, &lv_font_montserrat_22, 0);
  lv_obj_center(label);
}

void createIconButton(const lv_img_dsc_t *image, int x, int y, void (*onPressed)(_lv_event_t*), void (*onReleased)(_lv_event_t*), void (*onTap)(_lv_event_t*)){
  lv_obj_t* btn = createButton(x, y, onPressed, onReleased, onTap);
  lv_obj_t* img = lv_img_create(btn);
  lv_img_set_src(img, image);
  lv_obj_center(img);
}

#endif /* ARDUINO_USB_MODE */