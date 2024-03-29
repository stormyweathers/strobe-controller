#ifndef PULSE_TRAIN_H
#define PULSE_TRAIN_H

#include <vector>
#include <string>
#include "LinkedList.h"
#include <Arduino.h>

class PulseTrain : public LinkedList {
public:
    bool verbose;
    uint32_t total_duration;

    PulseTrain(const std::vector<uint32_t>& data, bool verbose = false);

    void VPrint(const std::string& message);

    void AddPulse(uint32_t t_start, uint32_t t_width);

    // Add any additional methods specific to PulseTrain here
};

#endif // PULSE_TRAIN_H
