/*
 * @Author       : Chivier Humber
 * @Date         : 2021-08-11 22:52:25
 * @LastEditors  : Chivier Humber
 * @LastEditTime : 2021-08-11 22:55:49
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

const double kInterval = 0;

int gN;
int gM, gm;
std::array<int, kMaxN> gTask;
std::array<int, kMaxM> gResourcesPlan;
std::array<int, kMaxN> gPlan; // Resource Amount
std::array<double, kMaxN> gTime; // Start time

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

/**
 * @description: Arrange all the tasks
 * @param {vector<int>} split
 * @return {*}
 */
void Arrange(std::vector<int> split) {
    int split_size = split.size();
    vector<double> time_list(split_size, 0);

    double total_time = 0;
    // Give a guess on the next task

    // First tuen is the same as intrinsic method
    for (int index = 0; index < min(split_size, gN); ++index) {
        time_list[index] = 1.0 * gTask[index] / split[index] + kInterval;
        gTime[index] = 0;
        gPlan[index] = split[index];

        total_time += 1.0 * gTask[index] / split[index];
    }

    for (int index = min(split_size, gN); index < gN; ++index) {
        // Find the earliest ended task
        double time = std::numeric_limits<double>::max();
        int flag_index = -1;
        for (int sub_index = 0; sub_index < split_size; ++sub_index) {
            if (time > time_list[sub_index]) {
                time = time_list[sub_index];
                flag_index = sub_index;
            }
        }
        
        // Arrange the index-th task
        gTime[index] = time_list[flag_index];
        time_list[flag_index] += 1.0 * gTask[index] / split[flag_index] + kInterval;
        gPlan[index] = split[flag_index];
    }

    double final_time = 0;
    double usage_rate = 0;

    for (int index = 0; index < split_size; ++index) {
        if (time_list[index] > final_time) {
            final_time = time_list[index];
        }
    }

    for (int index = 0; index < split_size; ++index) {
        usage_rate += split[index] * (final_time - time_list[index]);
    }

    usage_rate = 1 - usage_rate / (gM * final_time);
    usage_rate *= 100;
    
    cout << "Final time = " << final_time << endl;
    cout << "Usage rate = " << usage_rate << " %" << endl;
}

int main() {
    ReadConfig();
    ReadTask();
    auto split = SplitResource();
    Arrange(split);
    return 0;
}