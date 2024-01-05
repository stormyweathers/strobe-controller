#ifndef PULSE_TRAIN_H
#define PULSE_TRAIN_H

#include <vector>
#include <string>
#include "LinkedList.h"

class PulseTrain : public LinkedList {
public:
    bool verbose;
    int total_duration;

    PulseTrain(const std::vector<int>& data, bool verbose = false);

    void VPrint(const std::string& message);

    void AddPulse(int t_start, int t_width);

    // Add any additional methods specific to PulseTrain here
};

#endif // PULSE_TRAIN_H
