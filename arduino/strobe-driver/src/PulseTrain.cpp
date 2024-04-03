#include "PulseTrain.h"
#include <cmath>
#include <cstdio>
#include "PulseTrainTest.h"

PulseTrain::PulseTrain(const std::vector<int32_t>* data, uint8_t _PinNum, bool _verbose)
: verbose(_verbose), PinNum(_PinNum) {
    for (int datum : *data) {
        if (datum != 0) {
            this->InsertAtEnd(datum);
        }
    }
};

void PulseTrain::VPrint(const std::string& message) {
    if (this->verbose) {
        Serial.println(message.c_str());
    }
}

void PulseTrain::AddPulseAtEnd(int32_t t_delay, int32_t t_width){
    this->VPrint("Adding new pulse to end of train");

    //Possibly some residual delay before new pulse starts
    if (t_delay>0){
        this->InsertAtEnd(-1*t_delay);
    }
    if (t_delay<0){
        Serial.printf("AddPulseAtEnd: t_delay=%i<0, something went wrong\n",t_delay);
    }
    this->InsertAtEnd(t_width);
    return;
}

void PulseTrain::AddPulse(int32_t t_start, int32_t t_width) {

    //Only nonnegative t_width makes sense
    if ( 0 > t_width ){
        Serial.printf("AddPulse: t_width = %i < 0, skipping",t_width);
        return;
    }

    //Only nonnegative t_start makes sense
    if ( 0 > t_start ){
        Serial.printf("AddPulse: t_start = %i < 0, skipping",t_start);
        return;
    }

    int idx = 0;
    Node* current_node = this->head;

    //Walk along the linked list until the proper place to add the pulse
    //End once all the time in t_width has been added to the pulse train
    while (t_width > 0) {

        //Reached the end of the pulse train,
        if (nullptr == current_node)
        {
            AddPulseAtEnd(t_start, t_width);
            return;
        }

        this->VPrint("idx=" + std::to_string(idx) + ", data=" + std::to_string(current_node->data) +
                      ", t_start=" + std::to_string(t_start) + ", t_width=" + std::to_string(t_width));

        int32_t current_node_duration = abs(current_node->data);
        // Time between the end of the current node and the start of the new pulse
        // delay>=0 means the new pulse begins AFTER the curent node
        // delat<0 means the new pulse begins DURING the current node
        int32_t delay = t_start - current_node_duration;

        if ( (delay >= 0) or (0 == current_node->data) ) {
            this->VPrint("Skipping pulse, new pulse doesn't start till after");
            t_start += -current_node_duration;
            idx++;
            current_node = current_node->next;
            continue;
        } 
        
        //From here on out, delay < 0
        if (current_node->data > 0) {
            this->VPrint("Positive pulse overlap of " + std::to_string(delay));
            t_width += delay;
            t_start += -current_node_duration;
            t_start = max(0, t_start);
            current_node = current_node->next;
            idx++;
            continue;
        }

        //only get here if current_node->data < 0
        if ( ( 0 == t_start ) && (t_width < current_node_duration) ) {
            //Pulse begins exactly at the same time as current_node, and terminates within
            this->VPrint("Adding ++- pulse");
            this->UpdateNode(t_width, idx);
            this->InsertAtIndex(t_width - current_node_duration, idx + 1);
            return;
        } else if ( ( 0 == t_start) && (t_width >= current_node_duration)) {
            //Pulse begins exactly at the same time as current_node, but extends at least as long
            this->VPrint("Adding +++ pulse");
            this->UpdateNode(current_node_duration, idx);
            t_width += -current_node_duration;
            idx++;
            current_node = current_node->next;
        } else if ( (t_start > 0) && (t_width < -delay) ) {
            //pulse begins within current_node, and terminates within
            this->VPrint("Adding -+- pulse");
            this->UpdateNode(-t_start, idx);
            this->InsertAtIndex(t_width, idx + 1);
            this->InsertAtIndex(delay + t_width, idx + 2);
            return;
        } else if ( (t_start > 0) && (t_width >= -delay) ) {
            //pulse begins within current_node, but extends at least as long
            this->VPrint("Adding -++ pulse");
            this->UpdateNode(-t_start, idx);
            this->InsertAtIndex(current_node_duration - t_start, idx + 1);
            t_width += -(current_node_duration - t_start);
            idx += 2;
            t_start = 0;
            current_node = current_node->next;
            current_node = current_node->next;
        } else {
            Serial.print("Unhandled pulse type. What happened?");
            return;
        }   
    }
}

bool PulseTrain::PerformTests(bool verbose=false){
    using namespace PulseTrainTest;
    int cases_passed = 0;
    for (int i =0; i< num_test_cases; i++)
    {
        if(verbose){
            Serial.printf("Starting case %i\n", i);
        }
        //Convert array to vec type
        //std::vector<int32_t> vec_data(init_pulse_train_data, *(&init_pulse_train_data+1) );
        std::vector<int32_t> vec_data(&init_pulse_train_data[0], &init_pulse_train_data[0]+len_init_pulse_train);
        //Initialize the starting pulse train
        PulseTrain test_train(&vec_data, verbose);
        //test_train.PrintList();
        
        // Add the test pulse
        test_train.AddPulse(t_start_arr[i], t_width_arr[i]);
        
        

        //Construct the desired final pulse train
        std::vector<int32_t> vec_data_result(new_train_ptrs[i], new_train_ptrs[i] + new_train_length_arr[i]);
        PulseTrain result_train(&vec_data_result ,verbose);

        if  ( test_train.IsEquivalent(&result_train) ){
           cases_passed++;
        }
        else{
            Serial.printf("Failed on case %i", i);
            Serial.printf("\t   constructed seq:\n", i);
            test_train.PrintList();
            Serial.printf("\t known correct seq:\n", i);
            result_train.PrintList();
        }
    }
    if(verbose){
        Serial.printf("Passed %i out of %i test cases!\n",cases_passed,num_test_cases);
    }
    return (cases_passed == num_test_cases);
}

void PulseTrain::ClockTick(){
    //@ 1us tick interval, this overflows every 01:11:34.8
    this->tick_counter++;

    if (nullptr == this->head){ return;}

    //Decrement the abs-value of the head-node's data
    this->head->data = this->sign(this->head->data) * (abs(this->head->data)-1);

    // When the data ticks down to zero, shift to the next node
    if (0 == this->head->data){
        this->temp_node_ptr = this->head->next;
        free(this->head);
        this->head = this->temp_node_ptr;
        digitalWriteFast(this->PinNum, (this->head->data >0) );
    }
}
