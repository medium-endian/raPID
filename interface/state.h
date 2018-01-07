#ifndef STATE_H
#define STATE_H

#include "../interface/receiver.h"

// TRANSITION
// PASS_THROUGH
typedef enum {
    ARMED, DISARMING, DISARMED, ARMING, CONFIG
} state_t;

void disarm_init();
void arm_init();

bool disarming_input(uint16_t rx_input[NUM_CHANNELS]);
bool arming_input(uint16_t rx_input[NUM_CHANNELS]);

bool disarming_complete();
bool arming_complete();

void arm();

void disarm();

void config();

#endif // STATE_H
