// change this based on the name of your font
// look in the header for the exact define
#define SOURCESANSPRO_BOLD_IMPLEMENTATION
#define RXD2 32
#define TXD2 33
#define TFT_MOSI 19
#define TFT_SCLK 18
#define TFT_CS 5
#define TFT_DC 16
#define TFT_RST 23
#define TFT_BL 4
// change this to your header file for the font
#include "SourceSansPro_Bold.h"
#include <TinyGPSPlus.h>
// The TinyGPSPlus object
TinyGPSPlus gps;
int speed;

#if __has_include(<Arduino.h>)
#include <Arduino.h>
#else
#include <stdio.h>
#include <stdint.h>
#include "esp_lcd_panel_ops.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
extern "C" void app_main();
#endif
#include <lvgl.h>
#include "display.hpp"
#ifdef M5STACK_CORE2
#include <m5core2_power.hpp>
#endif

#ifdef M5STACK_CORE2
m5core2_power power;
#endif

lv_obj_t *label;
lv_font_t* label_font;
lv_style_t label_style;
lv_obj_t *satlabel;
lv_font_t* satlabel_font;
lv_style_t satlabel_style;
static uint32_t get_ms() {
#ifdef ARDUINO
    return millis();
#else
    return ((uint32_t)pdTICKS_TO_MS(xTaskGetTickCount()));
#endif
}

int ones;
int tens;
int hundreds;
int roundedSpeed;
int satellites;
long hexChars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9' 
};        // an array of pin numbers to which LEDs are attached
int charCount = 10;
unsigned long delaytime = 5;

#define TIME_HEADER "T"  // Header tag for serial time sync message
#define TIME_REQUEST 7   // ASCII bell character requests a time sync message

time_t prevDisplay = 0;  // when the digital clock was displayed


void processSyncMessage() {
    unsigned long pctime;
    const unsigned long DEFAULT_TIME = 1357041600;  // Jan 1 2013
}
void displayInfo() {
    // Serial.print(F("Location: "));
    if (gps.location.isValid()) {

    float speedFloat = gps.speed.kmph();
    int roundedSpeed = round(speedFloat);
    ones = (roundedSpeed % 10);
    tens = ((roundedSpeed / 10)%10);
    hundreds = ((roundedSpeed / 100)%10);

    Serial.print(F(" Sats: "));
    Serial.print(gps.satellites.value());
    Serial.print(F(" Speed: "));
    Serial.print(gps.speed.kmph());
    Serial.print(F(" SpeedFloat: "));
    Serial.print(speedFloat);
    Serial.print(F(" Rounded: "));
    Serial.print(roundedSpeed);
    Serial.print(F(" Hundreds: "));
    Serial.print(hundreds);
    Serial.print(F(" tens: "));
    Serial.print(tens);
    Serial.print(F(" ones: "));
    Serial.print(ones);

    Serial.println();
    }
}
void update_all() {
    ones = (roundedSpeed % 10);
    tens = ((roundedSpeed / 10)%10);
    hundreds = ((roundedSpeed / 100)%10);
    uint16_t time = millis();
    displayInfo();
    display_update();
}
void initialize_common() {
    display_init();
    puts("Booted");
    label = lv_label_create( lv_screen_active() );
    satlabel = lv_label_create( lv_screen_active() );
    lv_tiny_ttf_init();
    // change this to the name of your font
    label_font = lv_tiny_ttf_create_data(SourceSansPro_Bold,sizeof(SourceSansPro_Bold),115);
    satlabel_font = lv_tiny_ttf_create_data(SourceSansPro_Bold,sizeof(SourceSansPro_Bold),50);
    lv_style_init(&label_style);
    lv_style_set_text_font(&label_style,label_font);
    lv_obj_add_style(label,&label_style,0);
 // label_font = lv_tiny_ttf_create_data(SourceSansPro_Bold,sizeof(SourceSansPro_Bold),110);
    lv_style_init(&satlabel_style);
    lv_style_set_text_font(&satlabel_style,satlabel_font);
    lv_obj_add_style(satlabel,&satlabel_style,0);
 //   lv_label_set_text( label, "8-)" );
//char speed_buffer[32];
//itoa(roundedSpeed,speed_buffer,10);
//lv_label_set_text(label,speed_buffer);
    //lv_label_set_text( label, (speedFloat()));

    lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );
    lv_obj_align( satlabel, LV_ALIGN_OUT_LEFT_TOP, 0, 0 );


}
#ifdef ARDUINO
void setup() {
    Serial.begin(115200);
    Serial2.begin(38400, SERIAL_8N1, RXD2, TXD2);
    // enable the power pins, as necessary
#ifdef T_DISPLAY_S3
    pinMode(15, OUTPUT); 
    digitalWrite(15, HIGH);
#elif defined(S3_T_QT)
    pinMode(4, OUTPUT); 
    digitalWrite(4, HIGH);
#endif
#ifdef M5STACK_CORE2
    power.initialize();
#endif

    initialize_common();
Serial.println(F("DeviceExample.ino"));
Serial.println(F("A simple demonstration of TinyGPSPlus with an attached GPS module"));
Serial.print(F("Testing TinyGPSPlus library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
Serial.println(F("by Mikal Hart"));
Serial.println(F("TFT Initialized"));
Serial.println();
  // setSyncProvider(requestSync);  // set function to call when sync required
Serial.println("Waiting for sync message");
Serial.print(F(" Sats: "));
    Serial.println(gps.satellites.value());
}

void loop() {
    update_all();
    Serial.print(F(" Sats: "));
    Serial.println(gps.satellites.value());
        while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
    
    if (millis() > 5000 && gps.charsProcessed() < 10) {
        Serial.println(F("No GPS detected: check wiring."));
        while (true);
            
    }
   displayInfo();
   float speedFloat = gps.speed.kmph();
   float satFloat = gps.satellites.value();
    int roundedSpeed = round(speedFloat);
    int satellites = (satFloat);
    ones = (roundedSpeed % 10);
    tens = ((roundedSpeed / 10)%10);
    hundreds = ((roundedSpeed / 100)%10);

   char speed_buffer[32];
   char sat_buffer[32];
   itoa(roundedSpeed,speed_buffer,10);
   itoa(satellites, sat_buffer,10);
   lv_label_set_text(label,speed_buffer);
   lv_label_set_text(satlabel,sat_buffer);
    delay(200);
}
#else
void loop_task(void* state) {
    while(true) {
        update_all();
        vTaskDelay(1);
    }
}
void app_main() {
    // open stdin as binary
    freopen(NULL, "rb", stdin);
#ifdef T_DISPLAY_S3
    gpio_set_direction((gpio_num_t)15,GPIO_MODE_OUTPUT);
    gpio_set_level((gpio_num_t)15,1);
#elif defined(S3_T_QT)
    gpio_set_direction((gpio_num_t)4,GPIO_MODE_OUTPUT);
    gpio_set_level((gpio_num_t)4,1);
#endif
#ifdef M5STACK_CORE2
    power.initialize();
#endif
    initialize_common();
    TaskHandle_t htask = nullptr;
    xTaskCreate(loop_task,"loop_task",5000,nullptr,uxTaskPriorityGet(nullptr),&htask);
    if(htask==nullptr) {
        printf("Unable to create loop task\n");
    }
    vTaskSuspend(nullptr);
}
#endif
