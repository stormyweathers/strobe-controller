#ifndef PULSE_TRAIN_H
#define PULSE_TRAIN_H

#include <vector>
#include <string>
#include "LinkedList.h"
#include <Arduino.h>
#include <TeensyTimerTool.h>

class PulseTrain : public LinkedList {
public:
    bool verbose;
    uint32_t total_duration;
    uint32_t tick_counter = 0;
    uint8_t PinNum=4;

    PulseTrain(const std::vector<int32_t>* data, bool verbose = false);

    void VPrint(const std::string& message);

    //Append new pulse at end of train
    void AddPulseAtEnd(int32_t t_delay, int32_t t_width);

    //Add new pulse anywhere within the current train
    void AddPulse(int32_t t_start, int32_t t_width);

    bool PerformTests(bool verbose=false);

    //Action for incrementing a single time step
    //Will be used as a timer callback function
    void ClockTick();


    // Add any additional methods specific to PulseTrain here

private:

    /*
    * Simple func to get the sign of an integer
    */
    int32_t sign(int32_t val){
        if (val > 0) return 1;
        if (val < 0) return -1;
        return 0;
    }
};

#endif // PULSE_TRAIN_H
