// 4.16.1 0xe0e66494
// Generated by imageconverter. Please, do not edit!

#include <BitmapDatabase.hpp>
#include <touchgfx/Bitmap.hpp>

extern const unsigned char image_blue_buttons_square_icon_button[]; // BITMAP_BLUE_BUTTONS_SQUARE_ICON_BUTTON_ID = 0, Size: 60x60 pixels
extern const unsigned char image_blue_buttons_square_icon_button_pressed[]; // BITMAP_BLUE_BUTTONS_SQUARE_ICON_BUTTON_PRESSED_ID = 1, Size: 60x60 pixels
extern const unsigned char image_red_led_circle_01[]; // BITMAP_RED_LED_CIRCLE_01_ID = 2, Size: 16x16 pixels
extern const unsigned char image_red_led_circle_02[]; // BITMAP_RED_LED_CIRCLE_02_ID = 3, Size: 16x16 pixels

const touchgfx::Bitmap::BitmapData bitmap_database[] =
{
    { image_blue_buttons_square_icon_button, 0, 60, 60, 5, 4, 50, (uint8_t)(touchgfx::Bitmap::ARGB8888) >> 3, 50, (uint8_t)(touchgfx::Bitmap::ARGB8888) & 0x7 },
    { image_blue_buttons_square_icon_button_pressed, 0, 60, 60, 5, 4, 50, (uint8_t)(touchgfx::Bitmap::ARGB8888) >> 3, 50, (uint8_t)(touchgfx::Bitmap::ARGB8888) & 0x7 },
    { image_red_led_circle_01, 0, 16, 16, 3, 2, 10, (uint8_t)(touchgfx::Bitmap::ARGB8888) >> 3, 12, (uint8_t)(touchgfx::Bitmap::ARGB8888) & 0x7 },
    { image_red_led_circle_02, 0, 16, 16, 3, 2, 10, (uint8_t)(touchgfx::Bitmap::ARGB8888) >> 3, 12, (uint8_t)(touchgfx::Bitmap::ARGB8888) & 0x7 }
};

namespace BitmapDatabase
{
const touchgfx::Bitmap::BitmapData* getInstance()
{
    return bitmap_database;
}

uint16_t getInstanceSize()
{
    return (uint16_t)(sizeof(bitmap_database) / sizeof(touchgfx::Bitmap::BitmapData));
}
}
