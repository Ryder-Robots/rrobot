#ifndef MSPSENSORS_HPP
#define MSPSENSORS_HPP

#include <stdint.h>

namespace rrobot {
    /**
     * @enum MSPSENSOR_FLAGS
     * @brief
     * Binary flags that can be used by handlers to set what sensors are available.
     */
    enum MSPSENSOR_FLAGS : uint16_t {
        BARO  =0b000000000000001,  // Barometer
        MAG   =0b000000000000010,  // Magnometer
        GPS   =0b000000000000100,  // GPS
        SONAR =0b000000000001000,  // Ultrasonic
        CAMERA=0b000000000010000,  // Camera available
    };
}

#endif // MSPSENSORS_HPP
