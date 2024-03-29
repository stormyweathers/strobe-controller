#include "PulseTrain.h"
#include <cmath>
#include <cstdio>

PulseTrain::PulseTrain(const std::vector<int32_t>* data, bool verbose) : verbose(verbose), total_duration(0) {
    for (int datum : *data) {
        if (datum != 0) {
            this->InsertAtEnd(datum);
            this->total_duration += abs(datum);
        }
    }
};

void PulseTrain::VPrint(const std::string& message) {
    if (this->verbose) {
        Serial.print(message.c_str());
    }
}

void PulseTrain::AddPulse(uint32_t t_start, uint32_t t_width) {


    if (this->total_duration < t_start) {
        this->VPrint("Adding new pulse to end of train");
        this->InsertAtEnd(this->total_duration - t_start);
        this->InsertAtEnd(t_width);
        this->total_duration = t_width + t_start;
        return;
    } else if (this->total_duration == t_start) {
        this->VPrint("Adding new pulse snug up to end of train");
        this->InsertAtEnd(t_width);
        this->total_duration = t_width + t_start;
        return;
    }

    int idx = 0;
    Node* current_node = this->head;

    while (t_width > 0) {
       // this->VPrint("idx=" + std::to_string(idx) + ", data=" + std::to_string(current_node->data) +
       //               ", t_start=" + std::to_string(t_start) + ", t_width=" + std::to_string(t_width));

        uint32_t duration = abs(current_node->data);
        uint32_t delay = t_start - duration;

        if (delay >= 0) {
            //this->VPrint("Skipping pulse, new pulse doesn't start till after");
            t_start += -duration;
            if (nullptr == current_node->next)
            {
                this->InsertAtEnd(t_width);
                t_width=0;
            }
            idx++;
            current_node = current_node->next;
        } else if (delay < 0) {
            if (current_node->data > 0) {
              //  this->VPrint("Positive pulse overlap of " + std::to_string(delay));
                t_width += delay;
                t_start += -duration;
                t_start = max(static_cast<uint32_t>(0), t_start);
                current_node = current_node->next;
                idx++;
            } else if (current_node->data < 0) {
                if ( ( 0== t_start ) && (t_width < duration) ) {
                 //   this->VPrint("Adding ++- pulse");
                    this->UpdateNode(t_width, idx);
                    this->InsertAtIndex(t_width - duration, idx + 1);
                    return;
                } else if ( (t_start > 0) && (t_width < -delay) ) {
                   // this->VPrint("Adding -+- pulse");
                    this->UpdateNode(-t_start, idx);
                    this->InsertAtIndex(t_width, idx + 1);
                    this->InsertAtIndex(delay + t_width, idx + 2);
                    return;
                } else if ( ( 0 == t_start) && (t_width >= duration)) {
                   // this->VPrint("Adding +++ pulse");
                    this->UpdateNode(duration, idx);
                    t_width += -duration;
                    idx++;
                    current_node = current_node->next;
                } else if ( (t_start > 0) && (t_width >= -delay) ) {
                   // this->VPrint("Adding -++ pulse");
                    this->UpdateNode(-t_start, idx);
                    this->InsertAtIndex(duration - t_start, idx + 1);
                    t_width += -(duration - t_start);
                    idx += 2;
                    t_start = 0;
                    current_node = current_node->next;
                    current_node = current_node->next;
                } else {
                    Serial.print("Unhandled pulse type. What happened?");
                }
            } else {
                Serial.print("Unhandled main case. What happened?");
            }
        }

        if (t_width > 0) {
            // Handle residual pulse time remaining at the end of the train
            // (Note: You need to implement this part)
        }
    }
}
