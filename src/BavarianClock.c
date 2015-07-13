#include <pebble.h>
#include "BavarianClock.h"

static Window *window;
//Array of textlayers
static TextLayer *bav_text_layer[4] ;
GBitmap *flag_bitmap, *bt_ok_bitmap, *bt_not_ok_bitmap;
BitmapLayer *flag_layer, *bt_layer;
bool hasbluetooth;
static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(window_get_root_layer(window));
}


//All Seconds
static void bav_update_proc(Layer *layer, GContext *ctx) {

  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  // Bavarian Time starts here
  int32_t minutes = t->tm_min;

  int fmin = (minutes / 5) ;
  //Minutes
  text_layer_set_text(bav_text_layer[0], bav_minutes[fmin]);
  //Hours
  int32_t hours = t->tm_hour ;
  int fhours = getHours(minutes, hours);
  text_layer_set_text(bav_text_layer[1], bav_hours[fhours]);


  static char time_buffer[] = "00:00:00";
  strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", t);
  text_layer_set_text(bav_text_layer[2], time_buffer);

  //Week day
  int32_t day = t->tm_wday ;
  int32_t mday = t->tm_mday;
  char *tag = malloc(sizeof(char) * 15);
  snprintf(tag,15,"%s da %d.",bav_days[day],((int)mday));
  text_layer_set_text(bav_text_layer[3], tag);

  //Buetooth?
  if (bluetooth_connection_service_peek()) {
      if (!hasbluetooth) {
        hasbluetooth=true;
        bitmap_layer_set_bitmap(bt_layer, bt_ok_bitmap);
      }
  } else {
      if (hasbluetooth) {
        hasbluetooth=false;
        bitmap_layer_set_bitmap(bt_layer, bt_not_ok_bitmap);
      }
  }
}
//End all Seconds

//Initial setup after loading window
static void window_load(Window *window) {

  Layer *window_layer = window_get_root_layer(window);

  GRect bounds = layer_get_bounds(window_layer);
  //Background
  flag_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BAV_FLAG);
  flag_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(flag_layer, flag_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(flag_layer));
  //Create layers
  int text_Size=30;
  int bold = 2;
  for (int i=0; i<4; i++) {
    bav_text_layer[i] = text_layer_create((GRect) { .origin = { 0, 24+text_Size*i }, .size = { bounds.size.w, text_Size } });
    #ifdef PBL_COLOR
      #define bgcol GColorClear
      #define vgcol GColorBlack
      #define vgboldcol GColorDukeBlue
    #else
      #define bgcol GColorClear
      #define vgcol GColorBlack
      #define vgboldcol GColorBlack
    #endif

    text_layer_set_background_color(bav_text_layer[i], bgcol);
    text_layer_set_text_color(bav_text_layer[i], bold==i ? vgboldcol : vgcol);
    text_layer_set_font(bav_text_layer[i], fonts_get_system_font(bold == i ? FONT_KEY_GOTHIC_28_BOLD : FONT_KEY_GOTHIC_24));
    text_layer_set_text(bav_text_layer[i], "Servus");
    text_layer_set_text_alignment(bav_text_layer[i], GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(bav_text_layer[i]));
  }
  //Bluetooth symbol

  hasbluetooth=false;
  bt_ok_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BT_OK);
  bt_not_ok_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BT_NOK);
  bt_layer = bitmap_layer_create(GRect(0, 0, 25, 26));
  bitmap_layer_set_bitmap(bt_layer, bt_not_ok_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bt_layer));
  layer_set_update_proc(window_layer, bav_update_proc);
}

static void window_unload(Window *window) {
}

static void init(void) {
  window = window_create();

  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  #ifdef PBL_COLOR
    window_set_background_color(window, GColorDukeBlue);
  #else
    window_set_background_color(window, GColorBlack);
  #endif
  window_stack_push(window, true);

  window_set_background_color(window, GColorBlack);
  //Update all seconds
  tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
}

static void deinit(void) {
  text_layer_destroy(bav_text_layer[0]);
  window_destroy(window);
}

int main(void) {
  init();
  //Generated
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
