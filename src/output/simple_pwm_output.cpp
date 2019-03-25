#include "../../include/output/simple_pwm_output.h"

simple_pwm_output_t simple_out_init(const uint8_t pin,
        float throttle_volume, float roll_volume, float pitch_volume, float yaw_volume,
        bool is_throttle) {
    // is_pwm_pin(pin)
    mock_pinMode(pin, OUTPUT);

    mixer_t mixer = mixer_init(throttle_volume, roll_volume, pitch_volume, yaw_volume);

    simple_pwm_output_t result;
    result.pin                         = pin;
    result.mixer                       = mixer;
    result.milli_throttle              = 0;
    result.is_motor                    = is_throttle;
    result.throttle_low_cutoff         = 25;
    return result;
}

/* requires \valid(self);
   ensures self->milli_throttle < 256 && self->milli_throttle >= 0;
   assigns self->milli_throttle;
*/
static void simple_out_write(simple_pwm_output_t *self, uint16_t _milli_throttle) {
    self->milli_throttle = mock_clamp(_milli_throttle, 0, 255);
    // reusing _millithrottle could result in error if clamp was necessary
    mock_analogWrite(self->pin, self->milli_throttle);
}

/*@ requires \valid(self);
    behavior OUTPUTcutoff:
      assumes self->is_motor;
      assumes _milli_throttle < self->throttle_low_cutoff;
      ensures motor_status == MOTOR_OFF;
    behavior on:
      assumes _milli_throttle > self->throttle_low_cutoff;
      ensures motor_status == MOTOR_OFF;

    complete behaviors on, OUTPUTcutoff;
    disjoint behaviors on, OUTPUTcutoff;
 */
void simple_out_apply(simple_pwm_output_t *self, uint16_t _milli_throttle,
        float roll_stbl, float pitch_stbl, float yaw_stbl) {
    /* Throttle cutoff to avoid spinning props due to movement when throttle is low but state is armed
     * Do it here, before the control values are added up */
    if (self->is_motor && (_milli_throttle < self->throttle_low_cutoff)) {
        simple_out_shutoff(self);
        return;
    }

    /* intermediary int16_t to prevent overflow */
    int16_t throttle_tmp = (int16_t) _milli_throttle * (int16_t) self->mixer.throttle_volume;

    throttle_tmp += (int16_t) (roll_stbl  * self->mixer.roll_volume);
    throttle_tmp += (int16_t) (pitch_stbl * self->mixer.pitch_volume);
    throttle_tmp += (int16_t) (yaw_stbl   * self->mixer.yaw_volume);

    mock_clamp(throttle_tmp, 0, 255);

    simple_out_write(self, throttle_tmp);
}

void simple_out_shutoff(simple_pwm_output_t *self) {
    if (self->is_motor) {
        //@ ghost motor_status = MOTOR_OFF;
        simple_out_write(self, 0);
    }
}

void simple_out_set_throttle_volume(simple_pwm_output_t *self, float volume) {
    self->mixer.throttle_volume = volume;
}

void simple_out_set_roll_volume    (simple_pwm_output_t *self, float volume) {
    self->mixer.roll_volume = volume;
}

void simple_out_set_pitch_volume   (simple_pwm_output_t *self, float volume) {
    self->mixer.pitch_volume = volume;
}

void simple_out_set_yaw_volume     (simple_pwm_output_t *self, float volume) {
    self->mixer.yaw_volume = volume;
}
