#ifndef SENTRAL_IMU_H
#define SENTRAL_IMU_H

#include <stdint.h>

#include "Arduino.h"

#include "HardwareIMU.hpp"

#include "../../include/error_blink.h"
#include "../../include/pins.h"
#include "../../include/settings.h"

class SENtralIMU/* : HardwareIMU */{
    private:
    public:
        SENtralIMU();
        void update_angular_rates(float *angular_rates)/* override*/;
        void update_attitude(float *attitude)/* override*/;
};

#endif // SENTRAL_IMU_H

