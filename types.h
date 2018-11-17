#ifndef types_h
#define types_h


typedef struct {
   int elements[16][16];
} Board;

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

#endif /* types_h */