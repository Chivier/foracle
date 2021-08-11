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
    std::uniform_int_distribution<unsigned> distrib(1, 100);

    ofstream output_file;
    output_file.open("file.txt");
    output_file << length << std::endl;
    for (unsigned long j = 0; j < length; ++j) {
        output_file << distrib(gen) << ' ';
    }

    output_file << '\n';
    output_file.close();
    return 0;
}
