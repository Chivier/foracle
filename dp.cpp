/*
 * @Author       : Chivier Humber
 * @Date         : 2021-08-12 11:28:23
 * @LastEditors  : Chivier Humber
 * @LastEditTime : 2021-08-12 16:46:11
 * @Description  : file content
 */

#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include <array>
#include <algorithm>
#include <limits>
#include <cstddef>

using namespace std;

const int kMaxM = 1000;
const int kMaxN = 1000000;

const int kInterval = 0;

int gN;
int gM, gm;
int gFactor;
std::array<int, kMaxN> gTask;
std::array<int, kMaxM> gResourcesPlan;
std::array<int, kMaxN> gPlan; // Resource Amount
std::array<int, kMaxN> gTime; // Start time

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int lcm(int a, int b) {
    return a * b / gcd(a, b);
}

/**
 * @description: Read Config information
 * @param {*}
 * @return {*}
 */
void ReadConfig() {
    ifstream file("config.txt");
    if (file.is_open()) {
        file >> gM;
        file >> gm;
        for (int index = 0; index < gm; ++index) {
            file >> gResourcesPlan[index];
        }
    }
    file.close();
    sort(gResourcesPlan.begin(), gResourcesPlan.begin() + gm, std::greater<int>());
    gFactor = gResourcesPlan[0];
    for (int index = 1; index < gm; ++index) {
        gFactor = lcm(gFactor, gResourcesPlan[index]);
    }
}

/**
 * @description: Read Task information
 * @param {*}
 * @return {*}
 */
void ReadTask() {
    ifstream file("file.txt");
    if (file.is_open()) {
        file >> gN;
        for (int index = 0; index < gN; ++index) {
            file >> gTask[index];
            // Promise all the time interval is integer
            gTask[index] *= gFactor;
        }
    }
    file.close();
}

/**
 * @description: Split the resources in a greed method
 * @param {*}
 * @return {*}
 */
std::vector<int> SplitResource() {
    std:;vector<int> split;
    int resource_total = gM;
    for (int index = 0; index < gm; ++index) {
        int split_times = resource_total / gResourcesPlan[index];
        resource_total = resource_total % gResourcesPlan[index];

        for (int sub_index = 0; sub_index < split_times; ++sub_index) {
            split.push_back(gResourcesPlan[index]);
        }

        if (resource_total == 0) {
            // all the resources are used
            break;
        }
    }

    cout << "Intrinsic Plan:" << endl;
    for (auto const &num : split) {
        cout << num << " ";
    }
    cout << endl;

    return split;
}

void PrintTags(vector<int> const &tags) {
    for (auto item : tags) {
        cout << item << " ";
    }
    cout << endl;
}

/**
 * @description: Arrange all the tasks
 * @param {vector<int>} split
 * @return {*}
 */
void Arrange(std::vector<int> split) {
    int split_size = split.size();
    vector<int> time_list(split_size, 0);
    vector<int> arrange_tag(gM, -1);

    int total_time = 0;
    // Give a guess on the next task

    // First tuen is the same as intrinsic method
    int start_tag_position = 0;
    for (int index = 0; index < min(split_size, gN); ++index) {
        time_list[index] = gTask[index] / split[index] + kInterval;
        gTime[index] = 0;
        gPlan[index] = split[index];

        total_time += gTask[index] / split[index];
        // Place the tag on the corresponding places
        for (int sub_index = start_tag_position; sub_index < start_tag_position + split[index]; ++sub_index) {
            arrange_tag[sub_index] = index;
        }

        start_tag_position += split[index];
    }

    // PrintTags(arrange_tag);
    int index = min(split_size, gN);
    while (index < gN) {
        // If there is already resources available
        int empty_resources = 0;

        // Find the earliest ended task
        int time_select = std::numeric_limits<int>::max();
        int flag_index = -1;
        
        for (int sub_index = 0; sub_index < gM; ++sub_index) {
            int tag = arrange_tag[sub_index];
            if (tag == -1) {
                continue;
            }
            int end_time = gTime[tag] + gTask[tag] / gPlan[tag];
            if (end_time < time_select) {
                time_select = end_time;
                flag_index = tag;
            }
        }
        
        // Arrange the index-th task
        empty_resources = 0;
        for (int sub_index = 0; sub_index < gM; ++sub_index) {
            int tag = arrange_tag[sub_index];
            if (tag == -1) {
                continue;
            }
            if (gTime[tag] + gTask[tag] / gPlan[tag] == time_select) {
                arrange_tag[sub_index] = -1;
                empty_resources++;
            }
        }

        while (empty_resources) {
            // Use all the empty_resources
            // Guess the resources for the next task
            double guess_time = ((double) total_time) / (index - 1);
            double guess_resource = gTask[index] / guess_time;
            int resources_next = 0;
            for (int resource_index = gm - 1; resource_index >= 0; --resource_index) {
                if (gResourcesPlan[resource_index] <= guess_resource) {
                    resources_next = gResourcesPlan[resource_index];
                    break;
                }
            }
            // Promise the usage of resources is legal
            resources_next = min(resources_next, empty_resources);
            empty_resources -= resources_next;
            total_time += gTask[index] / resources_next;
            gTime[index] = time_select;
            gPlan[index] = resources_next;

            int tag_count = 0;
            for (int sub_index = 0; sub_index < gM; ++sub_index) {
                if (arrange_tag[sub_index] == -1) {
                    arrange_tag[sub_index] = index;
                    tag_count++;
                }
            }
            index++;

            if (index >= gN) {
                break;
            }
        }
    }

    int final_time = 0;
    int begin_time = std::numeric_limits<int>::max();
    double usage_rate = 0;

    for (int index = 0; index < gM; ++index) {
        int tag = arrange_tag[index];
        if (tag == -1) {
            continue;
        }
        int end_time = gTime[tag] + gTask[tag] / gPlan[tag];
        if (end_time > final_time) {
            final_time = end_time;
        }
        if (end_time < begin_time) {
            begin_time = end_time;
        }
    }

    for (int index = 0; index < gM; ++index) {
        int tag = arrange_tag[index];
        if (tag == -1) {
            usage_rate += (final_time - begin_time);
        }
        int end_time = gTime[tag] + gTask[tag] / gPlan[tag];
        usage_rate += (final_time - end_time);
    }


    usage_rate = 1 - usage_rate / (gM * final_time);
    usage_rate *= 100;
    
    cout << "Plan :" << endl;
    for (int index = 0; index < gN; ++index) {
        cout << index << " begin time = "<< (double)gTime[index] / gFactor << "   resources = " << gPlan[index] << endl;
    }
    cout << "Final time = " << ((double) final_time) / gFactor << endl;
    cout << "Usage rate = " << usage_rate << " %" << endl;
}

int main() {
    ReadConfig();
    ReadTask();
    auto split = SplitResource();
    Arrange(split);
    return 0;
}