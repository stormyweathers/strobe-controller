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
    uint32_t tick_counter = 0;
    uint8_t PinNum;

    PulseTrain(const std::vector<int32_t>* data, uint8_t _PinNum, bool _verbose = false);

    void VPrint(const std::string& message);

    //Append new pulse at end of train
    void AddPulseAtEnd(int32_t t_delay, int32_t t_width);

    //Add new pulse anywhere within the current train
    void AddPulse(int32_t t_start, int32_t t_width);
    void AddPulse(double t_start, int32_t t_width);
    void AddPulse(double t_start, double t_width);
    void AddPulse(int32_t t_start, double t_width);

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

    Node * temp_node_ptr;

    //Scratch space for converting floats to ints in callback funcs
    int32_t * int_ptr, int_ptr2;
};

#endif // PULSE_TRAIN_H
