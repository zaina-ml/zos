#ifndef COLORS_H
#define COLORS_H

#define RED        0x00FF0000
#define GREEN      0x0000FF00
#define BLUE       0x000000FF
#define BLACK      0x00000000
#define WHITE      0x00FFFFFF
#define GRAY       0x00808080

#define RGB(r, g, b) ((((r) & 0xFF) << 16) | (((g) & 0xFF) << 8) | ((b) & 0xFF))
#endif