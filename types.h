//
//  types.c
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
} piece;

typedef enum {
    REMOVEBOMB,
    FLAGBOMB,
} powerUp;

typedef enum {
    ADDBOMB,
    RESTART,
} powerDown;

typedef struct {
    piece elements[24][24];
} Board;

typedef struct {
    char * m;
} Message;

#endif /* types_h */