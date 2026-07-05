#include <Arduino.h>
#include <esp_display_panel.hpp>

#include <lvgl.h>
#include "lvgl_v8_port.h"

using namespace esp_panel::drivers;
using namespace esp_panel::board;

// ==========================
// Vehicle status data structure
// This is updated by the main program / CAN parser
// UI only reads from this structure
// ==========================
struct VehicleStatus {
    bool canConnected;

    bool batteryValid;
    int battery;              // percent

    bool driveModeValid;
    bool manualMode;          // true = Manual, false = Automatic

    bool steeringValid;
    int steering;             // 0=Forward, 1=Backward, 2=Left, 3=Right

    bool speedValid;
    int leftSpeed;            // percent
    int rightSpeed;           // percent

    bool estopValid;
    bool emergencyReleased;   // true = Released, false = Pressed
};

// Default values before real CAN data is received
static VehicleStatus vehicleStatus = {
    false,    // canConnected

    false,    // batteryValid
    0,        // battery

    false,    // driveModeValid
    true,     // manualMode

    false,    // steeringValid
    0,        // steering

    false,    // speedValid
    0,        // leftSpeed
    0,        // rightSpeed

    false,    // estopValid
    true      // emergencyReleased
};

// ==========================
// HEGE UI label objects
// ==========================
static lv_obj_t *label_can;
static lv_obj_t *label_battery;
static lv_obj_t *label_drive;
static lv_obj_t *label_steering;
static lv_obj_t *label_speed;
static lv_obj_t *label_estop;

// ==========================
// Create HEGE Control Panel UI
// ==========================
void create_hege_ui()
{
    lv_obj_clean(lv_scr_act());

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);

    lv_obj_t *title = lv_label_create(lv_scr_act());
    lv_label_set_text(title, "HEGE CONTROL PANEL");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_30, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 35);

    lv_obj_t *line = lv_obj_create(lv_scr_act());
    lv_obj_set_size(line, 650, 3);
    lv_obj_set_style_bg_color(line, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_opa(line, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(line, 0, 0);
    lv_obj_align(line, LV_ALIGN_TOP_MID, 0, 95);

    label_can = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(label_can, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(label_can, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(label_can, LV_ALIGN_LEFT_MID, 100, -115);

    label_battery = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(label_battery, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(label_battery, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(label_battery, LV_ALIGN_LEFT_MID, 100, -70);

    label_drive = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(label_drive, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(label_drive, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(label_drive, LV_ALIGN_LEFT_MID, 100, -25);

    label_steering = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(label_steering, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(label_steering, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(label_steering, LV_ALIGN_LEFT_MID, 100, 20);

    label_speed = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(label_speed, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(label_speed, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(label_speed, LV_ALIGN_LEFT_MID, 100, 65);

    label_estop = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(label_estop, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(label_estop, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(label_estop, LV_ALIGN_LEFT_MID, 100, 110);
}

// ==========================
// Update UI from VehicleStatus
// No simulated data here
// ==========================
void update_hege_ui(lv_timer_t *timer)
{
    char buffer[100];

    snprintf(buffer, sizeof(buffer), "CAN Status: %s",
             vehicleStatus.canConnected ? "Connected" : "ERROR");
    lv_label_set_text(label_can, buffer);

    if (vehicleStatus.batteryValid) {
        snprintf(buffer, sizeof(buffer), "Battery Level: %d %%", vehicleStatus.battery);
    } else {
        snprintf(buffer, sizeof(buffer), "Battery Level: NULL");
    }
    lv_label_set_text(label_battery, buffer);

    if (vehicleStatus.driveModeValid) {
        snprintf(buffer, sizeof(buffer), "Drive Mode: %s",
                 vehicleStatus.manualMode ? "Manual" : "Automatic");
    } else {
        snprintf(buffer, sizeof(buffer), "Drive Mode: NULL");
    }
    lv_label_set_text(label_drive, buffer);

    if (vehicleStatus.steeringValid) {
        if (vehicleStatus.steering == 0) {
            snprintf(buffer, sizeof(buffer), "Steering: Forward");
        } else if (vehicleStatus.steering == 1) {
            snprintf(buffer, sizeof(buffer), "Steering: Backward");
        } else if (vehicleStatus.steering == 2) {
            snprintf(buffer, sizeof(buffer), "Steering: Left");
        } else if (vehicleStatus.steering == 3) {
            snprintf(buffer, sizeof(buffer), "Steering: Right");
        } else {
            snprintf(buffer, sizeof(buffer), "Steering: NULL");
        }
    } else {
        snprintf(buffer, sizeof(buffer), "Steering: NULL");
    }
    lv_label_set_text(label_steering, buffer);

    if (vehicleStatus.speedValid) {
    snprintf(buffer, sizeof(buffer), "Speed: L %d%% | R %d%%",
             vehicleStatus.leftSpeed, vehicleStatus.rightSpeed);
    } else {
    snprintf(buffer, sizeof(buffer), "Speed: L NULL | R NULL");
    }
    lv_label_set_text(label_speed, buffer);

    if (vehicleStatus.estopValid) {
        snprintf(buffer, sizeof(buffer), "Emergency Stop: %s",
                 vehicleStatus.emergencyReleased ? "Released" : "Pressed");
    } else {
        snprintf(buffer, sizeof(buffer), "Emergency Stop: NULL");
    }
    lv_label_set_text(label_estop, buffer);

    lv_obj_invalidate(lv_scr_act());
}

void setup()
{
    String title = "HEGE Control Panel UI";

    Serial.begin(115200);

    Serial.println("Initializing board");
    Board *board = new Board();
    board->init();

#if LVGL_PORT_AVOID_TEARING_MODE
    auto lcd = board->getLCD();
    lcd->configFrameBufferNumber(LVGL_PORT_DISP_BUFFER_NUM);

#if ESP_PANEL_DRIVERS_BUS_ENABLE_RGB && CONFIG_IDF_TARGET_ESP32S3
    auto lcd_bus = lcd->getBus();
    if (lcd_bus->getBasicAttributes().type == ESP_PANEL_BUS_TYPE_RGB) {
        static_cast<BusRGB *>(lcd_bus)->configRGB_BounceBufferSize(lcd->getFrameWidth() * 10);
    }
#endif
#endif

    assert(board->begin());

    Serial.println("Initializing LVGL");
    lvgl_port_init(board->getLCD(), board->getTouch());

    Serial.println("Creating HEGE Control Panel UI");

    lvgl_port_lock(-1);

    create_hege_ui();

    // First update immediately
    update_hege_ui(NULL);

    // UI refresh every 200 ms
    lv_timer_create(update_hege_ui, 200, NULL);

    lvgl_port_unlock();

    Serial.println("HEGE UI started");
}

void loop()
{
    delay(10);
}