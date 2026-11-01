#ifndef PET_H
#define PET_H

#define MAX_NAME 12

typedef enum { PET_NONE, PET_CAT, PET_DOG, PET_RABBIT, PET_NANDA } PetType;

typedef struct {
    PetType type;
    char name[MAX_NAME + 1];
    int hunger;
    int happiness;
    int energy;
} Pet;

#endif
