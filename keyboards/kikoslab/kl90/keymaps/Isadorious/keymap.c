/*
Copyright 2021 Swiftrax <swiftrax@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include QMK_KEYBOARD_H
#include <string.h>
#include "raw_hid.h"
#include "print.h"


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[0] = LAYOUT_all(
    KC_AUDIO_MUTE ,   KC_ESC ,   KC_F1  ,   KC_F2  ,   KC_F3  ,   KC_F4  ,   KC_F5  ,   KC_F6  ,   KC_F7  ,   KC_F8  ,   KC_F9  ,   KC_F10 ,   KC_F11 ,   KC_F12 ,   KC_PSCR,   KC_MPLY,
    KC_F13 ,   KC_GRV ,   KC_1   ,   KC_2   ,   KC_3   ,   KC_4   ,   KC_5   ,   KC_6   ,   KC_7   ,   KC_8   ,   KC_9   ,   KC_0   ,   KC_MINS,   KC_EQL ,   KC_BSPC,   KC_DEL ,
    KC_F14 ,   KC_TAB ,   KC_Q   ,   KC_W   ,   KC_E   ,   KC_R   ,   KC_T   ,   KC_Y   ,   KC_U   ,   KC_I   ,   KC_O   ,   KC_P   ,   KC_LBRC,   KC_RBRC,   KC_BSLS,   KC_PGUP ,
    KC_F15 ,   KC_CAPS,   KC_A   ,   KC_S   ,   KC_D   ,   KC_F   ,   KC_G   ,   KC_H   ,   KC_J   ,   KC_K   ,   KC_L   ,   KC_SCLN,   KC_QUOT,   KC_PIPE,   KC_ENT ,   KC_PGDN,
    KC_F16 ,   KC_LSFT,   KC_Z   ,   KC_X   ,   KC_C   ,   KC_V   ,   KC_B   ,   KC_N   ,   KC_M   ,   KC_COMM,   KC_DOT ,   KC_SLSH,   KC_RSFT,   KC_RSFT,   KC_UP  ,   KC_END,
    KC_F17 ,   KC_LCTL,   KC_LGUI,   KC_LALT,   KC_SPC ,   KC_SPC ,              KC_SPC ,   KC_SPC ,              KC_RALT,   KC_RCTL,   KC_RCTL,   KC_LEFT,   KC_DOWN,   KC_RGHT),

[1] = LAYOUT_all(
    _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,
    _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,
    _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,
    _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,
    _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,
    _______,   _______,   _______,   _______,   _______,   _______,              _______,   _______,              _______,   _______,   _______,   _______,   _______,   _______),

[2] = LAYOUT_all(
    _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,
    _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,
    _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,
    _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,
    _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,
    _______,   _______,   _______,   _______,   _______,   _______,              _______,   _______,              _______,   _______,   _______,   _______,   _______,   _______),
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    } else if (index == 1) { /* Second encoder */
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // If console is enabled, it will print the matrix position and status of each key pressed
#ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif 
  return true;
}

// Variables to store system information
static char cpu_percent[6];  // CPU usage percentage
static char ram_usage[6];    // RAM usage in GB
static char gpu_percent[6];  // GPU usage percentage
static char vram_usage[6];   // VRAM usage in GB
static uint32_t last_hid_msg_time = 0; // last time we recieved a message from the PC

static bool isPcConnected = false;

#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_0;
}

#define IDLE_FRAMES 5
#define IDLE_SPEED 10

#define ANIM_SIZE 600 // number of bytes in array, minimize for adequate firmware size, max is 1024
#define ANIM_FRAME_DURATION 200 //how long each frame lasts in ms

uint32_t anim_timer = 0;
uint32_t anim_sleep = 0;
uint8_t current_idle_frame = 0;

static void render_anim(void){
    static const char PROGMEM idle[IDLE_FRAMES][ANIM_SIZE] = {
        {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,192, 32, 16, 16,  8,  4,  3,  0,  0,  1,  2,  4,  4,  8,  8,  8,  8,  8,  8,  8, 16, 16, 16, 16, 16,  8,  8,  4,  2,  2,126,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 96, 16, 31,  0,  0,  0,  0, 12, 30, 30, 12,  0,  0,  8, 24, 24,  8,  0,  0, 24, 60, 60, 24,  0,  0,128, 64,  0,  0,  0,  0,  0,  0,  0, 15, 48, 96,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  7,  8, 16, 16, 16, 16,  8,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  7, 24, 32, 32, 32, 32, 16, 12,  4,  4,  4,  4,  4,  7,  4,  4, 12,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8, 24, 16, 16, 16, 16, 16, 16, 16, 16, 16, 48, 32, 32, 32, 32, 32, 32, 96, 64,
        64,
        },
        {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,192, 32, 16, 16,  8,  4,  3,  0,  0,  1,  2,  4,  4,  8,  8,  8,  8,  8,  8,  8, 16, 16, 16, 16, 16,  8,  8,  4,  2,  2,126,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 96, 16, 31,  0,  0,  0,  0, 12, 30, 30, 12,  0,  0, 16, 32, 32, 16,  0,  0, 24, 60, 60, 24,  0,  0,128, 64,  0,  0,  0,  0,  0,  0,  0, 15, 48, 96,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  7,  8, 16, 16, 16, 16,  8,  4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  7, 24, 32, 32, 32, 32, 16, 12,  4,  4,  4,  4,  4,  7,  4,  4, 12,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8, 24, 16, 16, 16, 16, 16, 16, 16, 16, 16, 48, 32, 32, 32, 32, 32, 32, 96, 64,
        64,
        },
        {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,192, 32, 16, 16,  8,  4,  3,  0,  0,  1,  2,  4,  4,  8,  8,  8,  8,  8,  8,  8, 16, 16, 16, 16, 16,  8,  8,  4,  2,  2,126,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 96, 16, 31,  0,  0,  0,  0,  8,  8,  8,  8,  0,  0, 16, 16, 16, 16,  0,  0,  8,  8,  8,  8,  0,  0,128, 64,  0,  0,  0,  0,  0,  0,  0, 15, 48, 96,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1, 17, 17,  9,  9,  9,193, 39,  8, 16, 16, 16, 16,  8, 36, 66,130,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 18, 18, 18, 18,146, 71, 24, 32, 32, 32, 32, 16, 12,  4, 36, 36, 68, 68,  7,  4,  4, 12,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8, 24, 16, 16, 16, 16, 16, 16, 16, 16, 16, 48, 32, 32, 32, 32, 32, 32, 96, 64,
        64,
        },
        {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,192, 32, 16, 16,  8,  4,  3,  0,  0,  1,  2,  4,  4,  8,  8,  8,  8,  8,  8,  8, 16, 16, 16, 16, 16,  8,  8,  4,  2,  2,126,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,  0,  0,  0,  0,  0,252,  6,  1,  1,  1,  2,  4, 15,  0,  0,  0,  0, 12, 30, 30, 12,  0,  0, 16, 56, 56, 16,  0,  0, 24, 60, 60, 24,  0,  0,128, 64,  0,  0,  0,  0,  0,  0,  0, 15, 48, 96,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  3,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 18, 18, 18, 18,130, 71, 24, 32, 32, 32, 32, 16, 12,  4, 36, 36, 68, 68,  7,  4,  4, 12,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8, 24, 16, 16, 16, 16, 16, 16, 16, 16, 16, 48, 32, 32, 32, 32, 32, 32, 96, 64,
        64,
        },
        {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,192, 32, 16, 16,  8,  4,  3,  0,  0,  1,  2,  4,  4,  8,  8,  8,  8,  8,  8,  8, 16, 16, 16, 16, 16,  8,  8,  4,  2,  2,126,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,  0,  0,  0,  0,  0,252,  6,  1,  1,  1,  2,  4, 15,  0,  0,  0,  0, 12, 30, 30, 12,  0,  0, 16, 48, 48, 16,  0,  0, 24, 60, 60, 24,  0,  0,248,  4,  4, 12, 16, 96,  0,  0,  0, 15, 48, 96,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  3,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  6,  7,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  7,  4,  4, 12,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8, 24, 16, 16, 16, 16, 16, 16, 16, 16, 16, 48, 32, 32, 32, 32, 32, 32, 96, 64,
        64,
        }
    };
    void animation_phase(void) {
        current_idle_frame = (current_idle_frame + 1) % IDLE_FRAMES;
        oled_write_raw_P(idle[abs((IDLE_FRAMES-1)-current_idle_frame)], ANIM_SIZE);
    }

    if(get_current_wpm() != 000) {
        oled_on(); // not essential but turns on animation OLED with any alpha keypress
        if(timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
            anim_timer = timer_read32();
            animation_phase();
        }
        anim_sleep = timer_read32();
    }
    else {
        if(timer_elapsed32(anim_sleep) > OLED_TIMEOUT)
            oled_off();
        else {
            if(timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
                anim_timer = timer_read32();
                animation_phase();
            }
        }
    }
}


void render_data(void) {

    if(timer_elapsed32(last_hid_msg_time) > 60000) {
        isPcConnected = false;
    }

    if(isPcConnected) {

        // want to reset oled buffer to start from 0
        oled_set_cursor(0, 0);

        // new layout CPU top left, RAM top right, GPU bottom left, VRAM bottom right
        oled_write("\n", false);
        oled_write("  CPU: ", false);
        oled_write(cpu_percent, false);
        oled_write("", false);

        oled_write("RAM: ", false);
        oled_write(ram_usage, false);
        oled_write("\n", false);

        oled_write("  GPU: ", false);
        oled_write(gpu_percent, false);
        oled_write("", false);

        oled_write("MEM: ", false);
        oled_write(vram_usage, false);

    } else {
        // oled_write("\n", false);
        // oled_write_ln("         PC Not\n", false);
        // oled_write_ln("       Connected", false);
        render_anim(); // bongo cat animation
    }
}

bool oled_task_user(void) {
    // block updates if the OLED buffer is dirty
    extern OLED_BLOCK_TYPE oled_dirty;
    if (oled_dirty) return false;

    render_data();
    return false;
}
#endif


#ifdef RAW_ENABLE
void raw_hid_receive(uint8_t *data, uint8_t length) {
    isPcConnected = true; // recieved a report so PC is connected

    #ifdef CONSOLE_ENABLE
    for(uint8_t i = 0; i < length; i++) {
        uprintf("Data at %d :: %d \n", i, data[i] );
    }
    #endif

    last_hid_msg_time = timer_read32(); // update last time we recieved a message

    // parse the data sent over, know length is always 32
    // first 5 bytes are CPU usage
    for(uint8_t i = 0; i < 5; i++) {
        cpu_percent[i] = data[i];
    }
    cpu_percent[5] = '\0';

    // next 5 bytes are RAM usage
    for(uint8_t i = 0; i < 5; i++) {
        ram_usage[i] = data[i+5];
    }
    ram_usage[5] = '\0';


    // next 5 bytes are GPU usage
    for(uint8_t i = 0; i < 5; i++) {
        gpu_percent[i] = data[i+10];
    }
    gpu_percent[5] = '\0';

    // next 5 bytes are VRAM usage
    for(uint8_t i = 0; i < 5; i++) {
        vram_usage[i] = data[i+15];
    }
    vram_usage[5] = '\0';

    // last 12 bytes are unused
}

#endif