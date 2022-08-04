#include "lv_helper.h"

// Set Display Rotation
// 0: Portrait, 1: Landscape, 2: Portrait Reverse, 3: Landscape Reverse
#define DISPLAY_ROTATION 0

#define OPERATER_NONE     0
#define OPERATER_PLUS     1
#define OPERATER_MINUS    2
#define OPERATER_MULTIPLY 3
#define OPERATER_DIVIDE   4
#define OPERATER_PERCENT  5

#define STATUS_VALUE1 0
#define STATUS_VALUE2 1
#define STATUS_RESULT 2

lv_obj_t *textarea_obj;
lv_obj_t *keyboard_obj;
int currentOperate = -1;

String value1;
String value2;
String currentValue = "";
int currentOperater = -1;
int currentStatus = STATUS_VALUE1;

void draw_main();
void handleButton(const char* btn);
void calculate();


static const char * btnm_map[] = {
  "AC", "+/-", "%", "/", "\n",
  "7", "8", "9", "*", "\n",
  "4", "5", "6", "-", "\n",
  "1", "2", "3", "+", "\n",
  "0", ".", "=", "", "\n"
};

void setup(void) {
  Serial.begin(115200);
  Serial.println("Calculator Starts!");
  lh_init(DISPLAY_ROTATION);
  draw_main();
}

void loop() {
  lh_handler();
}

void draw_main() {
  lv_disp_set_bg_color(NULL, lv_color_hex(0x000000));
  textarea_obj = lv_textarea_create(lv_scr_act());
  lv_textarea_set_one_line(textarea_obj, true);
  lv_textarea_set_cursor_click_pos(textarea_obj, true);
  lv_obj_set_width(textarea_obj, 240);
  lv_obj_set_align(textarea_obj, LV_ALIGN_TOP_MID);
  lv_obj_set_pos(textarea_obj, 0, 0);
  // lv_obj_add_event_cb(textarea_obj, ta_event_cb, LV_EVENT_ALL, NULL);

  keyboard_obj = lv_btnmatrix_create(lv_scr_act());
  lv_btnmatrix_set_map(keyboard_obj, btnm_map);
  lv_obj_set_size(keyboard_obj, 240, 286);
  lv_btnmatrix_set_btn_width(keyboard_obj, 16, 2);        /*Make "Action1" twice as wide as "Action2"*/
  lv_btnmatrix_set_btn_ctrl(keyboard_obj, 3, LV_BTNMATRIX_CTRL_CHECKABLE);
  lv_btnmatrix_set_btn_ctrl(keyboard_obj, 7, LV_BTNMATRIX_CTRL_CHECKABLE);
  lv_btnmatrix_set_btn_ctrl(keyboard_obj, 11, LV_BTNMATRIX_CTRL_CHECKABLE);
  lv_btnmatrix_set_btn_ctrl(keyboard_obj, 15, LV_BTNMATRIX_CTRL_CHECKABLE);
  lv_btnmatrix_set_one_checked(keyboard_obj, true);
  lv_obj_align(keyboard_obj, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_add_event_cb(keyboard_obj, event_handler, LV_EVENT_ALL, NULL);
}

static void event_handler(lv_event_t * e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  if(code == LV_EVENT_VALUE_CHANGED) {
    uint32_t id = lv_btnmatrix_get_selected_btn(obj);
    const char * txt = lv_btnmatrix_get_btn_text(obj, id);

    Serial.printf("%s was pressed\n", txt);
    handleButton(txt);
  }
  if(code == LV_EVENT_DRAW_PART_BEGIN) {
    lv_obj_draw_part_dsc_t * dsc = (lv_obj_draw_part_dsc_t *)lv_event_get_param(e);

    dsc->rect_dsc->radius = LV_RADIUS_CIRCLE;
    /*Change the right column color*/
    if(dsc->id % 4 == 3 || dsc->id == 18) {
      if(lv_btnmatrix_get_selected_btn(obj) == dsc->id) {
        dsc->rect_dsc->bg_color = lv_color_white();
        dsc->label_dsc->color = lv_palette_main(LV_PALETTE_AMBER);
      }
      else {
        dsc->rect_dsc->bg_color = lv_palette_main(LV_PALETTE_AMBER);
        dsc->label_dsc->color = lv_color_white();
      }
    }
    /*Change the top left three button color to gray*/
    else if(dsc->id <= 2) {
      if(lv_btnmatrix_get_selected_btn(obj) == dsc->id) {
        dsc->rect_dsc->bg_color = lv_palette_main(LV_PALETTE_GREY);
      } else {
        dsc->rect_dsc->bg_color = lv_palette_lighten(LV_PALETTE_GREY, 2);
      }
    } else {
      if(lv_btnmatrix_get_selected_btn(obj) == dsc->id) {
        dsc->rect_dsc->bg_color = lv_palette_darken(LV_PALETTE_GREY, 4);
      } else {
        dsc->rect_dsc->bg_color = lv_palette_darken(LV_PALETTE_GREY, 2);
      }
      dsc->label_dsc->color = lv_color_white();
    }
  }
}

void handleButton(const char* btn) {
  if (String(btn) == "AC" ) {
    value1 = "";
    value2 = "";
    currentValue = "";
    currentOperater = OPERATER_NONE;
    currentStatus = STATUS_VALUE1;
  } else if (String(btn) == "+/-") {
    if (currentValue.startsWith("-")){
      currentValue = currentValue.substring(1);
    } else {
      currentValue = "-" + currentValue;
    }
  } else if (String(btn) == "%") {
    double temp = currentValue.toDouble();
    temp = temp / 100;
    currentValue = String(temp);
  } else if (String(btn) == "/") {
    calculate();
    currentOperater = OPERATER_DIVIDE;
    currentStatus = STATUS_VALUE2;
    currentValue = "";
  } else if (String(btn) == "*") {
    calculate();
    currentOperater = OPERATER_MULTIPLY;
    currentStatus = STATUS_VALUE2;
    currentValue = "";
  } else if (String(btn) == "-") {
    calculate();
    currentOperater = OPERATER_MINUS;
    currentStatus = STATUS_VALUE2;
    currentValue = "";
  } else if (String(btn) == "+") {
    calculate();
    currentOperater = OPERATER_PLUS;
    currentStatus = STATUS_VALUE2;
    currentValue = "";
  } else if (String(btn) == "=") {
    calculate();
    currentStatus = STATUS_RESULT;
    currentOperater = OPERATER_NONE;
    currentValue = value1;
    lv_btnmatrix_clear_btn_ctrl_all(keyboard_obj, LV_BTNMATRIX_CTRL_CHECKED);
  } else if (String(btn) == ".") {
    if (currentStatus == STATUS_RESULT) {
      currentValue = "";
      currentStatus = STATUS_VALUE1;
    }
    if (currentValue.indexOf(".") < 0) {
      currentValue += ".";
    } else if (currentValue.length() == 0){
      currentValue = "0.";
    }
  } else {
    if (currentStatus == STATUS_RESULT) {
      currentValue = "";
      currentStatus = STATUS_VALUE1;
    }
    currentValue += btn;
  }
  if (currentStatus == STATUS_VALUE1) {
    value1 = currentValue;
  } else {
    value2 = currentValue;
  }
  if (currentOperater != OPERATER_NONE && currentStatus == STATUS_VALUE2) {
    lv_textarea_set_text(textarea_obj, value2.c_str());
  } else {
    lv_textarea_set_text(textarea_obj, value1.c_str());
  }
}

void calculate() {
  if (value2 != "") {
    double v1 = value1.toDouble();
    double v2 = value2.toDouble();
    double result;
    switch (currentOperater) {
      case OPERATER_PLUS:
        result = v1 + v2;
        break;
      case OPERATER_MINUS:
        result = v1 - v2;
        break;
      case OPERATER_MULTIPLY:
        result = v1 * v2;
        break;
      case OPERATER_DIVIDE:
        result = v1 / v2;
        break;
    }
    currentValue = String(result);
  }
  value1 = currentValue;
  value2 = "";
  currentOperater = OPERATER_NONE;
}
