#ifndef ARMING_STATE_H
#define ARMING_STATE_H

#include "Arduino.h"

#include "error_blink.h"
#include "receiver/Receiver.hpp"

// TRANSITION PASS_THROUGH FAILSAFE, CONFIG
typedef enum { DISARMED, ARMING, ARMING_STANDBY, ARMED, DISARMING, DISARMING_STANDBY } state_t;

class ArmingState {
    private:
        const uint16_t DISARM_TIMEOUT_MS = 2500;
        const uint16_t ARM_TIMEOUT_MS = 1000;
        /* find appropriate polling interval */
        const uint32_t INTERVAL_US = 500000;

        state_t internal_state = DISARMED;
        IntervalTimer state_change_timer;
        int16_t *channels;
        uint64_t state_change_time = 0;

        friend void update_state();

    public:
        explicit ArmingState(channels_t channels);
        const state_t get_state();
};
#endif // ARMING_STATE_H
