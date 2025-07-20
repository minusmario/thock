#include QMK_KEYBOARD_H

#include "quantum.h"

enum layers {
  _BASE,
  _LOWER,
  _RAISE,
  _ADJUST,
  _FN
};

enum keycodes {
  MARCO_VIM_NEXT = SAFE_RANGE,
  MARCO_VIM_PREVIOUS,
  MARCO_VIM_YANK,
  MARCO_VIM_PASTE,
  DYNAMIC_MACRO_RANGE,
};

#include "dynamic_macro.h"

// Tap-Dance
enum {
    COMMA_DOT,
    QUOTE_DQT,
    MINUS_PLUS
};
// Tap Dance definitions
qk_tap_dance_action_t  tap_dance_actions[] = {
    [COMMA_DOT] = ACTION_TAP_DANCE_DOUBLE(KC_COMMA, KC_DOT),
    [QUOTE_DQT] = ACTION_TAP_DANCE_DOUBLE(KC_QUOTE, KC_DQT),
    [MINUS_PLUS] = ACTION_TAP_DANCE_DOUBLE(KC_MINUS, KC_PLUS)
};

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define FN MO(_FN)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_mit(
    KC_ESCAPE,     KC_Q,   KC_W,   KC_E,   KC_R, KC_T,    KC_Y,KC_U, KC_I,    KC_O,   KC_P,    KC_BSPC,     \
    LCTL_T(KC_TAB),KC_A,   KC_S,   KC_D,   KC_F, KC_G,    KC_H,KC_J, KC_K,    KC_L,   KC_SCLN, KC_QUOTE,     \
    KC_LSFT,       KC_Z,   KC_X,   KC_C,   KC_V, KC_B,    KC_N,KC_M, KC_COMMA,KC_DOT, KC_SLASH,RSFT_T(KC_ENTER), \
    FN,            KC_LCTL,KC_LGUI,KC_LALT,LOWER,KC_SPACE,     RAISE,KC_LEFT, KC_DOWN,KC_UP,   KC_RIGHT
  ),

  [_LOWER] = LAYOUT_mit(
    KC_TILD,KC_EXLM,KC_AT,  KC_HASH,KC_DLR, KC_PERC,LGUI(KC_D),KC_7,   KC_8,KC_9,         KC_LPRN,      KC_RPRN, \
    KC_TRNS,KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,     KC_4,   KC_5,KC_6,         KC_LBRC,      KC_RBRC, \
    KC_TRNS,KC_F7,  KC_F8,  KC_F9,  KC_F10, KC_F11, KC_F12,    KC_1,   KC_2,KC_3,         KC_LCBR,      KC_RCBR, \
    KC_NO,  KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,           KC_TRNS,KC_0,TD(COMMA_DOT),TD(QUOTE_DQT),TD(MINUS_PLUS)
  ),

  [_RAISE] = LAYOUT_mit(
    KC_GRAVE,KC_PAUSE,           KC_PSCR,             KC_INSERT,KC_HOME,KC_PGUP,KC_CIRC,KC_AMPR, KC_ASTR,            KC_UNDS,             KC_PLUS,        KC_PIPE, \
    KC_TRNS, LCTL(LGUI(KC_LEFT)),LCTL(LGUI(KC_RIGHT)),KC_DELETE,KC_END, KC_PGDN,KC_LEFT,KC_DOWN, KC_UP,              KC_RIGHT,            KC_EQUAL,       KC_BSLS, \
    KC_TRNS, KC_NUMLOCK,         KC_CAPS,             KC_NO,    KC_NO,  KC_NO,  KC_NO,  KC_MINUS,KC_NO,              KC_MEDIA_PLAY_PAUSE, KC_MUTE,        KC_NO, \
    KC_NO,   KC_TRNS,            KC_TRNS,             KC_TRNS,  KC_TRNS,KC_TRNS,        KC_TRNS, KC_MEDIA_PREV_TRACK,KC_AUDIO_VOL_DOWN,   KC_AUDIO_VOL_UP,KC_MEDIA_NEXT_TRACK
  ),

  [_ADJUST] = LAYOUT_mit(
    LALT(LCTL(KC_DELETE)),DYN_REC_START1, DYN_REC_START2, KC_NO,  KC_NO,  KC_NO,KC_NO,KC_NO,  KC_NO,KC_NO,KC_NO,KC_NO,             \
    KC_TRNS,              DYN_MACRO_PLAY1,DYN_MACRO_PLAY1,KC_NO,  KC_NO,  KC_NO,KC_NO,KC_NO,  KC_NO,KC_NO,KC_NO,KC_NO, \
    KC_TRNS,              KC_NO,          KC_NO,          KC_NO,  KC_NO,  KC_NO,KC_NO,KC_NO,  KC_NO,KC_NO,KC_NO,EEP_RST, \
    DYN_REC_STOP,         KC_TRNS,        KC_TRNS,        KC_TRNS,KC_TRNS,KC_NO,      KC_TRNS,KC_NO,KC_NO,KC_NO,RESET
  ),

  [_FN] = LAYOUT_mit(
    KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,KC_NO,MARCO_VIM_YANK,    KC_NO,KC_NO,  KC_NO,         MARCO_VIM_PASTE,KC_BSPC,\
    KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,KC_NO,MARCO_VIM_PREVIOUS,KC_NO,KC_NO,  MARCO_VIM_NEXT,KC_NO,          KC_NO,  \
    KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,KC_NO,KC_NO,             KC_NO,KC_NO,  KC_NO,         KC_NO,          KC_ENTER,\
    KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_NO,KC_TRNS,                 KC_NO,KC_LEFT,KC_DOWN,       KC_UP,          KC_RIGHT
  ),
};

uint32_t layer_state_set_user(uint32_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	if (!process_record_dynamic_macro(keycode, record)) {
		return false;
	}
    switch (keycode) {
        case MARCO_VIM_NEXT:
            if (record->event.pressed) {
                SEND_STRING("gt");
            }
            return false;
        case MARCO_VIM_PREVIOUS:
            if (record->event.pressed) {
                SEND_STRING("gT");
            }
            return false;
        case MARCO_VIM_YANK:
            if (record->event.pressed) {
                SEND_STRING(SS_LSFT(SS_TAP(X_QUOTE)) SS_LSFT(SS_TAP(X_EQUAL)) SS_TAP(X_Y));
            }
            return false;
        case MARCO_VIM_PASTE:
            if (record->event.pressed) {
                SEND_STRING(SS_LSFT(SS_TAP(X_QUOTE)) SS_LSFT(SS_TAP(X_EQUAL)) SS_TAP(X_P));
            }
            return false;
    }
    return true;
}
