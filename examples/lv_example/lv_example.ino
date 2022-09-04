#include "lv_helper.h"

lv_obj_t* arc;
lv_obj_t* slider;
lv_obj_t* arcValueLabel;

// Arc
void lv_example_arc_1(void) {
  arc = lv_arc_create(lv_scr_act());
  lv_obj_set_size(arc, 100, 100);
  lv_arc_set_rotation(arc, 135);
  lv_arc_set_bg_angles(arc, 0, 270);
  lv_arc_set_value(arc, 0);
  lv_arc_set_range(arc, 0, 100);
  lv_obj_align(arc, LV_ALIGN_TOP_MID, 0, 10);
  arcValueLabel = lv_label_create(arc);
  lv_label_set_text(arcValueLabel, String(0).c_str());
  lv_obj_center(arcValueLabel);
  lv_obj_add_event_cb(arc, arcValueChanged, LV_EVENT_VALUE_CHANGED, NULL);
}
static void arcValueChanged(lv_event_t* e){
  lv_obj_t* obj = lv_event_get_target(e);
  int value = (int)lv_arc_get_value(obj);
  lv_label_set_text(arcValueLabel, String(value).c_str());
  lv_bar_set_value(slider, value, LV_ANIM_OFF);
}

// Slider
void lv_example_slider_1(void) {
  slider = lv_slider_create(lv_scr_act());
  lv_obj_align(slider, LV_ALIGN_TOP_MID, 0, 130);
  lv_obj_add_event_cb(slider, sliderValueChanged, LV_EVENT_VALUE_CHANGED, NULL);
}
static void sliderValueChanged(lv_event_t* e){
  lv_obj_t* obj = lv_event_get_target(e);
  int value = (int)lv_slider_get_value(obj);
  lv_label_set_text(arcValueLabel, String(value).c_str());
  lv_arc_set_value(arc, value);
}

// Button and toggle
void lv_example_btn_1(void){
  lv_obj_t * label;

  lv_obj_t * btn1 = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
  lv_obj_align(btn1, LV_ALIGN_TOP_MID, 80, 170);

  label = lv_label_create(btn1);
  lv_label_set_text(label, "Button");
  lv_obj_center(label);

  lv_obj_t * btn2 = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(btn2, event_handler, LV_EVENT_ALL, NULL);
  lv_obj_align(btn2, LV_ALIGN_TOP_MID, -80, 170);
  lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_set_height(btn2, LV_SIZE_CONTENT);

  label = lv_label_create(btn2);
  lv_label_set_text(label, "Toggle");
  lv_obj_center(label);
}
static void event_handler(lv_event_t* e){
  lv_event_code_t code = lv_event_get_code(e);
  if(code == LV_EVENT_CLICKED) {
      Serial.println("Clicked");
  }
  else if(code == LV_EVENT_VALUE_CHANGED) {
      Serial.println("Toggled");
  }
}

void lv_example_checkbox_1(void) {
  lv_obj_t * checkboxs = lv_obj_create(lv_scr_act());
  lv_obj_set_flex_flow(checkboxs, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(checkboxs, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
  lv_obj_align(checkboxs, LV_ALIGN_TOP_MID, 0, 220);
  lv_obj_set_size(checkboxs, 200, 160);

  lv_obj_t * cb;
  cb = lv_checkbox_create(checkboxs);
  lv_checkbox_set_text(cb, "Apple");
  lv_obj_add_event_cb(cb, checkbox_event_handler, LV_EVENT_ALL, NULL);

  cb = lv_checkbox_create(checkboxs);
  lv_checkbox_set_text(cb, "Banana");
  lv_obj_add_state(cb, LV_STATE_CHECKED);
  lv_obj_add_event_cb(cb, checkbox_event_handler, LV_EVENT_ALL, NULL);

  cb = lv_checkbox_create(checkboxs);
  lv_checkbox_set_text(cb, "Lemon");
  lv_obj_add_event_cb(cb, checkbox_event_handler, LV_EVENT_ALL, NULL);

  cb = lv_checkbox_create(checkboxs);
  lv_checkbox_set_text(cb, "Melon\nand a new line");
  lv_obj_add_event_cb(cb, checkbox_event_handler, LV_EVENT_ALL, NULL);

  lv_obj_update_layout(cb);
}
static void checkbox_event_handler(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = lv_event_get_target(e);
  if(code == LV_EVENT_VALUE_CHANGED) {
    const char * txt = lv_checkbox_get_text(obj);
    const char * state = lv_obj_get_state(obj) & LV_STATE_CHECKED ? "Checked" : "Unchecked";
    Serial.printf("%s: %s\n", txt, state);
    Serial.flush();
  }
}

// Dropdown
void lv_example_dropdown_1(void) {
    lv_obj_t * dd = lv_dropdown_create(lv_scr_act());
    lv_dropdown_set_options(dd, "Apple\n"
                                "Banana\n"
                                "Orange\n"
                                "Cherry\n"
                                "Grape\n"
                                "Raspberry\n"
                                "Melon\n"
                                "Orange\n"
                                "Lemon\n"
                                "Nuts");

    lv_obj_align(dd, LV_ALIGN_TOP_MID, 0, 400);
    lv_obj_add_event_cb(dd, dropdown_event_handler, LV_EVENT_ALL, NULL);
}
static void dropdown_event_handler(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = lv_event_get_target(e);
  if(code == LV_EVENT_VALUE_CHANGED) {
    char buf[32];
    lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
    Serial.printf("Option: %s\n", buf);
    Serial.flush();
  }
}

// Roller
void lv_example_roller_1(void) {
  lv_obj_t *roller1 = lv_roller_create(lv_scr_act());
  lv_roller_set_options(
    roller1,
    "January\n"
    "February\n"
    "March\n"
    "April\n"
    "May\n"
    "June\n"
    "July\n"
    "August\n"
    "September\n"
    "October\n"
    "November\n"
    "December",
    LV_ROLLER_MODE_INFINITE
  );

  lv_roller_set_visible_row_count(roller1, 4);
  lv_obj_align(roller1, LV_ALIGN_TOP_MID, 0, 460);
  lv_obj_add_event_cb(roller1, roller_event_handler, LV_EVENT_ALL, NULL);
}
static void roller_event_handler(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = lv_event_get_target(e);
  if(code == LV_EVENT_VALUE_CHANGED) {
    char buf[32];
    lv_roller_get_selected_str(obj, buf, sizeof(buf));
    Serial.printf("Selected month: %s\n", buf);
    Serial.flush();
  }
}

// Switches
void lv_example_switch_1(void) {
  lv_obj_t * li;

  lv_obj_t * sw;
  lv_obj_t * label;

  li = lv_obj_create(lv_scr_act());
  lv_obj_clear_flag(li, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_size(li, 310, 50);
  lv_obj_align(li, LV_ALIGN_TOP_MID, 0, 590);
  sw = lv_switch_create(li);
  lv_obj_align(sw, LV_ALIGN_RIGHT_MID, -10, 0);
  lv_obj_add_event_cb(sw, switch_event_handler, LV_EVENT_ALL, NULL);
  label = lv_label_create(li);
  lv_label_set_text(label, "Switch 1");
  lv_obj_align(label, LV_ALIGN_LEFT_MID, 10, 0);

  li = lv_obj_create(lv_scr_act());
  lv_obj_clear_flag(li, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_size(li, 310, 50);
  lv_obj_align(li, LV_ALIGN_TOP_MID, 0, 650);
  sw = lv_switch_create(li);
  lv_obj_add_state(sw, LV_STATE_CHECKED);
  lv_obj_align(sw, LV_ALIGN_RIGHT_MID, -10, 0);
  lv_obj_add_event_cb(sw, switch_event_handler, LV_EVENT_ALL, NULL);
  label = lv_label_create(li);
  lv_label_set_text(label, "Switch 2");
  lv_obj_align(label, LV_ALIGN_LEFT_MID, 10, 0);
}
static void switch_event_handler(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = lv_event_get_target(e);
  if(code == LV_EVENT_VALUE_CHANGED) {
    Serial.printf("State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
    Serial.flush();
  }
}

// Calender
void lv_example_calendar_1(void) {
  lv_obj_t* calendar = lv_calendar_create(lv_scr_act());
  lv_obj_set_size(calendar, 300, 200);
  lv_obj_align(calendar, LV_ALIGN_TOP_MID, 0, 710);
  lv_obj_add_event_cb(calendar, calendar_event_handler, LV_EVENT_ALL, NULL);

  lv_calendar_set_today_date(calendar, 2022, 8, 9);
  lv_calendar_set_showed_date(calendar, 2022, 8);

  /*Highlight a few days*/
  static lv_calendar_date_t highlighted_days[3];       /*Only its pointer will be saved so should be static*/
  highlighted_days[0].year = 2022;
  highlighted_days[0].month = 8;
  highlighted_days[0].day = 7;

  highlighted_days[1].year = 2022;
  highlighted_days[1].month = 2;
  highlighted_days[1].day = 11;

  highlighted_days[2].year = 2022;
  highlighted_days[2].month = 2;
  highlighted_days[2].day = 22;

  lv_calendar_set_highlighted_dates(calendar, highlighted_days, 3);

  lv_calendar_header_dropdown_create(calendar);
  // lv_calendar_header_arrow_create(calendar);
  // lv_calendar_set_showed_date(calendar, 2021, 10);
}
static void calendar_event_handler(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = lv_event_get_current_target(e);

  if(code == LV_EVENT_VALUE_CHANGED) {
    lv_calendar_date_t date;
    if(lv_calendar_get_pressed_date(obj, &date)) {
      Serial.printf("Clicked date: %02d.%02d.%d\n", date.day, date.month, date.year);
      Serial.flush();
    }
  }
}

void lv_example_chart_1(void) {
  /*Create a chart*/
  lv_obj_t * chart;
  chart = lv_chart_create(lv_scr_act());
  lv_obj_set_size(chart, 200, 150);
  lv_obj_align(chart, LV_ALIGN_TOP_MID, 0, 920);
  lv_chart_set_type(chart, LV_CHART_TYPE_LINE);   /*Show lines and points too*/

  /*Add two data series*/
  lv_chart_series_t * ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
  lv_chart_series_t * ser2 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_SECONDARY_Y);

  /*Set the next points on 'ser1'*/
  lv_chart_set_next_value(chart, ser1, 10);
  lv_chart_set_next_value(chart, ser1, 10);
  lv_chart_set_next_value(chart, ser1, 10);
  lv_chart_set_next_value(chart, ser1, 10);
  lv_chart_set_next_value(chart, ser1, 10);
  lv_chart_set_next_value(chart, ser1, 10);
  lv_chart_set_next_value(chart, ser1, 10);
  lv_chart_set_next_value(chart, ser1, 30);
  lv_chart_set_next_value(chart, ser1, 70);
  lv_chart_set_next_value(chart, ser1, 90);

  /*Directly set points on 'ser2'*/
  ser2->y_points[0] = 90;
  ser2->y_points[1] = 70;
  ser2->y_points[2] = 65;
  ser2->y_points[3] = 65;
  ser2->y_points[4] = 65;
  ser2->y_points[5] = 65;
  ser2->y_points[6] = 65;
  ser2->y_points[7] = 65;
  ser2->y_points[8] = 65;
  ser2->y_points[9] = 65;

  lv_chart_refresh(chart); /*Required after direct set*/
}

void setup() {
  Serial.begin(115200);
  lh_init(DISPLAY_LANDSCAPE);
  Serial.println("LVGL Example: Ready");

  lv_example_arc_1();
  lv_example_slider_1();
  lv_example_btn_1();
  lv_example_checkbox_1();
  lv_example_dropdown_1();
  lv_example_roller_1();
  lv_example_switch_1();
  lv_example_calendar_1();
  lv_example_chart_1();
}

void loop() {
  lv_timer_handler();
}