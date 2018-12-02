//
//  types.c
//  Final Project
//
//  Created by Created by Shant Haik, Gavin Neises, Jordan Wheeler
//

#ifndef types_h
#define types_h

typedef enum
{
    EMPTY,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    BOMB,
    FLAG,
    POWERUP,
    POWERDOWN
} Piece;

typedef enum
{
    PENDING,
    WON,
    LOST
} Status;

typedef enum { // Used in ApplyPowerDown
    ADDBOMB,
    REFRESH,
} PowerDown;

typedef enum { // Used in ApplyPowerUp
    REMOVEBOMB,
    FLAGBOMB,
} PowerUp;

typedef struct {
    Piece elements[24][24];
} Board;

typedef struct {
    char * m;
} Message;

typedef struct {
    int x;
    int y;
} Map;

#endif /* types_h */