/*
 * This file is part of Cleanflight and Betaflight.
 *
 * Cleanflight and Betaflight are free software. You can redistribute
 * this software and/or modify this software under the terms of the
 * GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Cleanflight and Betaflight are distributed in the hope that they
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "common/time.h"
#include "hw.h"

extern uint32_t excute_time, excute_temp, excute_max, excute_count;

#if defined(USE_GPS) || defined(USE_MAG)
extern int16_t magHold;
#endif

typedef struct throttleCorrectionConfig_s {
    uint16_t throttle_correction_angle;     // the angle when the throttle correction is maximal. in 0.1 degres, ex 225 = 22.5 ,30.0, 450 = 45.0 deg
    uint8_t throttle_correction_value;      // the correction that will be applied at throttle_correction_angle.
} throttleCorrectionConfig_t;

extern throttleCorrectionConfig_t throttleCorrectionConfig;

typedef enum {
    LAUNCH_CONTROL_DISABLED = 0,
    LAUNCH_CONTROL_ACTIVE,
    LAUNCH_CONTROL_TRIGGERED,
} launchControlState_e;

typedef enum {
    LAUNCH_CONTROL_MODE_NORMAL = 0,
    LAUNCH_CONTROL_MODE_PITCHONLY,
    LAUNCH_CONTROL_MODE_FULL,
    LAUNCH_CONTROL_MODE_COUNT // must be the last element
} launchControlMode_e;

typedef enum {
    DISARM_REASON_ARMING_DISABLED   = 0,
    DISARM_REASON_FAILSAFE          = 1,
    DISARM_REASON_THROTTLE_TIMEOUT  = 2,
    DISARM_REASON_STICKS            = 3,
    DISARM_REASON_SWITCH            = 4,
    DISARM_REASON_CRASH_PROTECTION  = 5,
    DISARM_REASON_RUNAWAY_TAKEOFF   = 6,
    DISARM_REASON_GPS_RESCUE        = 7,
    DISARM_REASON_SERIAL_COMMAND    = 8,
#ifdef UNIT_TEST
    DISARM_REASON_SYSTEM            = 255,
#endif
} flightLogDisarmReason_e;

#ifdef USE_LAUNCH_CONTROL
#define LAUNCH_CONTROL_THROTTLE_TRIGGER_MAX 90
extern const char * const osdLaunchControlModeNames[LAUNCH_CONTROL_MODE_COUNT];
#endif

void throttleCorrectionConfig_Init(void);
void handleInflightCalibrationStickPosition(void);

void resetArmingDisabled(void);

void disarm(flightLogDisarmReason_e reason);
void tryArm(void);

bool processRx(uint32_t currentTimeUs);
void processRxModes(uint32_t currentTimeUs);
void updateArmingStatus(void);

void taskGyroSample(uint32_t currentTimeUs);
bool gyroFilterReady(void);
bool pidLoopReady(void);
void taskFiltering(uint32_t currentTimeUs);
void taskMainPidLoop(uint32_t currentTimeUs);

bool isFlipOverAfterCrashActive(void);
int8_t calculateThrottlePercent(void);
uint8_t calculateThrottlePercentAbs(void);
bool areSticksActive(uint8_t stickPercentLimit);
void runawayTakeoffTemporaryDisable(uint8_t disableFlag);
bool isAirmodeActivated();
uint32_t getLastDisarmTimeUs(void);
bool isTryingToArm();
void resetTryingToArm();

void subTaskTelemetryPollSensors(uint32_t currentTimeUs);

bool isLaunchControlActive(void);
