#ifndef PET_H
#define PET_H

#define MAX_NAME 12

typedef enum { 
    PET_NONE, 
    PET_CAT, 
    PET_DOG, 
    PET_RABBIT, 
    PET_NANDA 
} PetType;

typedef struct {
    PetType type;
    char name[MAX_NAME + 1];
    int hunger;       // 0 - 100 (higher = full)
    int happiness;    // 0 - 100 (higher = happy)
    int energy;       // 0 - 100 (higher = rested)
    int cleanliness;  // 0 - 100 (higher = clean)
    int health;       // 0 - 100 (higher = healthy)
} Pet;

#endif
