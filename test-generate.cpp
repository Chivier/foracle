#include <chrono>
#include <fstream>
#include <iostream>
#include <random>

using namespace std;

int main() {
    int length;
    cout << "Input the test case length: " << endl;
    cin >> length;

    std::random_device rd;
    std::mt19937::result_type seed =
        rd() ^ ((std::mt19937::result_type)std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::system_clock::now().time_since_epoch())
                    .count() +
                (std::mt19937::result_type)std::chrono::duration_cast<std::chrono::microseconds>(
                    std::chrono::high_resolution_clock::now().time_since_epoch())
                    .count());

    std::mt19937 gen(seed);
    std::normal_distribution<double> distrib(30, 20);

    ofstream output_file;
    output_file.open("file.txt");
    output_file << length << std::endl;
    for (unsigned long j = 0; j < length; ++j) {
        int num = (int) distrib(gen);
        if (num <= 0) {
            num = 1;
        }
        if (num > 100) {
            num = 100;
        }
        output_file << num << ' ';
    }

    output_file << '\n';
    output_file.close();
    return 0;
}
