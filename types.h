//
//  types.c
//
//  Created by Jordan Wheeler
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

typedef struct {
   piece elements[24][24];
} Board;

#endif /* types_h */