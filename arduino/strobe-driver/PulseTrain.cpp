#include "PulseTrain.h"
#include <cmath>

PulseTrain::PulseTrain(const std::vector<int>& data, bool verbose) : verbose(verbose), total_duration(0) {
    for (int datum : data) {
        if (datum != 0) {
            this->InsertAtEnd(datum);
            this->total_duration += std::abs(datum);
        }
    }
}

void PulseTrain::VPrint(const std::string& message) {
    if (this->verbose) {
        Serial.print(message.c_str());
    }
}

void PulseTrain::AddPulse(int t_start, int t_width) {
    assert(t_start >= 0);
    assert(t_width >= 0);

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
        this->VPrint("idx=" + std::to_string(idx) + ", data=" + std::to_string(current_node->data) +
                      ", t_start=" + std::to_string(t_start) + ", t_width=" + std::to_string(t_width));

        int duration = std::abs(current_node->data);
        int delay = t_start - duration;

        if (delay >= 0) {
            this->VPrint("Skipping pulse, new pulse doesn't start till after");
            t_start -= duration;
            idx++;
            current_node = current_node->next;
        } else if (delay < 0) {
            if (current_node->data > 0) {
                this->VPrint("Positive pulse overlap of " + std::to_string(delay));
                t_width += delay;
                t_start -= duration;
                t_start = std::max(0, t_start);
                current_node = current_node->next;
                idx++;
            } else if (current_node->data < 0) {
                if (t_start == 0 && t_width < duration) {
                    this->VPrint("Adding ++- pulse");
                    this->UpdateNode(t_width, idx);
                    this->InsertAtIndex(t_width - duration, idx + 1);
                    return;
                } else if (t_start > 0 && t_width < -delay) {
                    this->VPrint("Adding -+- pulse");
                    this->UpdateNode(-t_start, idx);
                    this->InsertAtIndex(t_width, idx + 1);
                    this->InsertAtIndex(delay + t_width, idx + 2);
                    return;
                } else if (t_start == 0 && t_width >= duration) {
                    this->VPrint("Adding +++ pulse");
                    this->UpdateNode(duration, idx);
                    t_width -= duration;
                    idx++;
                    current_node = current_node->next;
                } else if (t_start > 0 && t_width >= -delay) {
                    this->VPrint("Adding -++ pulse");
                    this->UpdateNode(-t_start, idx);
                    this->InsertAtIndex(duration - t_start, idx + 1);
                    t_width -= duration - t_start;
                    idx += 2;
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