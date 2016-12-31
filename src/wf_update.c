#include <stddef.h>
#include "waveform.h"


void get_update_waveform_timings(int stage,
    uint32_t *ckv_high_delay_ns, uint32_t *ckv_low_delay_ns,
    uint32_t *stage_delay_us)
{
    if (stage < 0 || stage >= 29) {
        *ckv_high_delay_ns = 0;
        *ckv_low_delay_ns = 0;
        *stage_delay_us = 0;
        return;
    }

    *ckv_high_delay_ns = 60*15;
    *ckv_low_delay_ns = 60*40;
    *stage_delay_us = 0;
}

enum PIXEL_VALUE get_update_waveform_value(int stage,
    pixel_t old_pixel, pixel_t new_pixel)
{
    int diff = new_pixel - old_pixel;
    if (diff == 0) return PV_NEUTRAL;

    enum PIXEL_VALUE fwd = (diff > 0) ? PV_BLACK : PV_WHITE;
    enum PIXEL_VALUE back = (fwd == PV_BLACK) ? PV_WHITE : PV_BLACK;

    // get absolute value
    if (diff < 0) diff = -diff;

    // we want to drive it X=diff stages to the new value.
    // to maintain DC balance, we have to also do X stages in the other
    // direction. we also prefix an extra stage in the regular direction.
    // (making both Xs = diff-1, not diff)
    if (stage == 0) {
        return fwd;
    }
    --stage;

    if (stage < diff - 1) {
        return back;
    }
    stage -= (diff - 1);

    if (stage < diff - 1) {
        return fwd;
    }
    stage -= (diff - 1);

    return PV_NEUTRAL;
}