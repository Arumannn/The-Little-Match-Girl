#ifndef MINIGAME_H
#define MINIGAME_H

typedef struct QTE *address_List;
typedef char info;
typedef struct{
    info karakter;
    address_List next;
}QTE;

typedef struct {
    address_List Root;
} List;

#endif