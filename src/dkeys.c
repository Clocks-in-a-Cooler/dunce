#include <dkeys.h>

#define KEYS_N 57
#define to_sk_key_t(group, offset) (group << 3 | offset)

char DKEYS_UP_ARROW[2]    = { 0x18, '\0' };
char DKEYS_DOWN_ARROW[2]  = { 0x19, '\0' };
char DKEYS_LEFT_ARROW[2]  = { 0x1a, '\0' };
char DKEYS_RIGHT_ARROW[2] = { 0x1b, '\0' };

char* DKEYS_EN_NAMES[] = {
    "",
    // DOWN_ARROW, LEFT_ARROW, RIGHT_ARROW, UP_ARROW, "", "", "", "",
    "down",  "left",  "right",  "up",    "",      "",      "",      "",
    "enter", "+",     "-",      "*",     "/",     "^",     "clear", "",
    "(-)",   "3",     "6",      "9",     ")",     "tan",   "vars",  "",
    ".",     "2",     "5",      "8",     "(",     "cos",   "prgm",  "stat",
    "0",     "1",     "4",      "7",     ",",     "sin",   "apps",  "XT0n",
    "",      "sto",   "ln",     "log",   "x^2",   "x^-1",  "math",  "alpha",
    "graph", "table", "zoom",   "window","y=",    "2nd",   "mode",  "del"
};

char DKEYS_FR_RESOL[]  = { 'r', 0xe9, 's', 'o', 'l', '\0' }; // "résol"
char DKEYS_FR_WINDOW[] = { 'f', 'e', 'n', 0xea, 't', 'r', 'e', '\0' }; // "fenêtre"

char* DKEYS_FR_NAMES[] = {
    "",
    // DOWN_ARROW, LEFT_ARROW, RIGHT_ARROW, UP_ARROW, "", "", "", "",
    "bas",    "gauche",  "droit", "haut",          "",     "",             "",        "",
    "entrer", "+",       "-",     "*",             "/",    "^",            "annul",   "",
    "(-)",    "3",       "6",     "9",             ")",    "n/d",          "var",     "",
    ".",      "2",       "5",     "8",             "(",    DKEYS_FR_RESOL, "prgm",    "stats",
    "0",      "1",       "4",     "7",             ",",    "trig",         "matrice", "XT0n",
    "",       "sto",     "ln",    "log",           "x^2",  "< >",          "math",    "alpha",
    "graphe", "trace",   "zoom",  DKEYS_FR_WINDOW, "f(x)", "2nd",          "mode",    "del"
};

bool dkeys_enter_latch;

short keys[KEYS_N];

char** DKEYS_NAMES;

void dkeys_init() {
    memset((void*) keys, 0, KEYS_N);
    dkeys_enter_latch = kb_IsDown(kb_KeyEnter);

    DKEYS_NAMES = (os_GetSystemInfo())->hardwareType ? DKEYS_FR_NAMES : DKEYS_EN_NAMES;
}

void dkeys_update() {
    kb_Scan();
    for (int c = 1; c < KEYS_N; c++) {
        short* current_key = &(keys[c]);
        if (kb_IsDown(sk_key_t_to_kb_lkey_t(c))) {
            if (*current_key & DKEYS_HELD) {
                *current_key = *current_key & DKEYS_SEEN;
            } else {
                *current_key = DKEYS_PRESSED | DKEYS_SEEN;
            }
        } else {
            if (*current_key & DKEYS_HELD) {
                *current_key = DKEYS_RELEASED;
            } else {
                *current_key = 0;
            }
        }
    }
}

kb_lkey_t dkeys_get_next() {
    for (int c = 1; c < KEYS_N; c++) {
        if (keys[c] & DKEYS_PRESSED) return sk_key_t_to_kb_lkey_t(c);
    }
    return 0;
}

short dkeys_check(kb_lkey_t kb_lcode) {
    return keys[kb_lkey_t_to_sk_key_t(kb_lcode)];
}

kb_lkey_t sk_key_t_to_kb_lkey_t(sk_key_t sk_code) {
    sk_code--;
    uint8_t group  = 7 - (sk_code >> 3);
    uint8_t offset = sk_code & 7;

    return (kb_lkey_t)(group << 8 | 1 << offset);
}

sk_key_t kb_lkey_t_to_sk_key_t(kb_lkey_t kb_lcode) {
    if (!kb_lcode) return 0;

    uint8_t group      = (7 - (kb_lcode >> 8)) << 3;
    uint8_t offset_bit = (uint8_t) kb_lcode & 255;
    uint8_t offset     = 0;

    // find the offset from the offset bit
    while (!(offset_bit & 1)) {
        offset++;
        // thanks, commandblockguy, for catching this one... my sleepy fingers did it correctly in the Javascript, but not here
        offset_bit = offset_bit >> 1;
        if (offset > 8) return 0; // failsafe
    }

    return ((group | offset) + 1);
}