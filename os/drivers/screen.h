#ifndef DRIVERS_SCREEN_H
#define DRIVERS_SCREEN_H

#include <stdint.h>

#define BLUE_FOREGROUND 0x01
#define GREEN_FOREGROUND 0x02
#define CYAN_FOREGROUND 0x03
#define RED_ORANGE_FOREGROUND 0x04
#define MAGENTA_FOREGROUND 0x05
#define BROWN_FOREGROUND 0x06
#define GREY_WHITE_FOREGROUND 0x07

#define INTENSE_FOREGROUND 0x08

#define BLUE_BACKGROUND 0x10
#define GREEN_BACKGROUND 0x20
#define CYAN_BACKGROUND 0x30
#define RED_ORANGE_BACKGROUND 0x40
#define MAGENTA_BACKGROUND 0x50
#define BROWN_BACKGROUND 0x60
#define GREY_WHITE_BACKGROUND 0x70

#define INTENSE_BACKGROUND 0x80

// Note offset is bytes not chars
int32_t getCursor();
void setCursor(int32_t offset);

// Note strings must be null terminated. DEF_STYLE if attribute_byte is 0.
void printAt(char *message, int32_t col, int32_t row, char attribute_byte);
void print(char *message);
void printHeader(char *message, int32_t col, char attribute_byte);

void clearScreen();

#endif
