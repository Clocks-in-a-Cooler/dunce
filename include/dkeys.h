#pragma once

#define DKEYS_VERSION 1.1

#include <keypadc.h>
#include <tice.h>
#include <stdlib.h>
#include <string.h>

#define DKEYS_SEEN     1
#define DKEYS_PRESSED  2
#define DKEYS_HELD     3
#define DKEYS_RELEASED 4

extern bool dkeys_enter_latch;
#define dkeys_EnterLatch dkeys_enter_latch

void dkeys_init();
void dkeys_update();

// for other people, if they're foolish enough to use this library
#define dkeys_Init() dkeys_init()
#define dkeys_Update() dkeys_update()

short dkeys_check(kb_lkey_t kb_lcode);

#define dkeys_Check(kb_lcode) dkeys_check(kb_lcode)

#define dkeys_check_pressed(kb_l_code) dkeys_check(kb_l_code) & DKEYS_PRESSED
#define dkeys_check_held(kb_l_code) dkeys_check(kb_l_code) & DKEYS_HELD
#define dkeys_check_released(kb_l_code) dkeys_check(kb_l_code) & DKEYS_RELEASED

#define dkeys_CheckPressed(kb_l_code) dkeys_check(kb_l_code) & DKEYS_PRESSED
#define dkeys_CheckHeld(kb_l_code) dkeys_check(kb_l_code) & DKEYS_HELD
#define dkeys_CheckReleased(kb_l_code) dkeys_check(kb_l_code) & DKEYS_RELEASED

// assumes that update_keypad() is already called. you have been warned!
kb_lkey_t dkeys_get_next();
#define dkeys_GetNext() dkeys_get_next();

// helpers

kb_lkey_t sk_key_t_to_kb_lkey_t(sk_key_t sk_code);
sk_key_t kb_lkey_t_to_sk_key_t(kb_lkey_t kb_lcode);

extern char** DKEYS_NAMES;

// these must be extern now, or the LTO will complain
extern char DKEYS_UP_ARROW[2];
extern char DKEYS_DOWN_ARROW[2];
extern char DKEYS_LEFT_ARROW[2];
extern char DKEYS_RIGHT_ARROW[2];

#define KEY_NAME_LENGTH 9

extern char* DKEYS_EN_NAMES[];

extern char DKEYS_FR_RESOL[];
extern char DKEYS_FR_WINDOW[];
extern char* DKEYS_FR_NAMES[];

typedef enum {
    DKEYS_EN = 0,
    DKEYS_FR,
} Dkeys_language;

#define dkeys_get_name(kb_l_code) DKEYS_NAMES[kb_lkey_t_to_sk_key_t(kb_l_code)]
#define dkeys_GetName(kb_l_code) DKEYS_NAMES[kb_lkey_t_to_sk_key_t(kb_l_code)]
