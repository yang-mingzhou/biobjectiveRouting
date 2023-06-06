// module load gcc/11.3.0
// g++ -o pairs pairs.cpp && ./pairs

#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <fstream>

std::vector<std::pair<int, int>> generate_pairs(int range, int num_pairs, unsigned int seed) {
    std::srand(seed);
    std::vector<std::pair<int, int>> pairs;

    for (int i = 0; i < num_pairs; ++i) {
        int origin = std::rand() % range + 1;         // Origins and destinations start from 1 now
        int destination = std::rand() % range + 1;

        // Ensure origin and destination are different
        while (destination == origin) {
            destination = std::rand() % range + 1;
        }

        pairs.push_back(std::make_pair(origin, destination));
    }

    return pairs;
}

void write_to_file(const std::string& output_file_name, const std::vector<std::pair<int, int>>& pairs) {
    std::ofstream output_file(output_file_name);

    if (!output_file) {
        std::cerr << "Failed to open the output file.\n";
        return;
    }

    for (const auto& pair : pairs) {
        output_file << pair.first << " " << pair.second << "\n";
    }
}

int main() {
    int range, num_pairs;
    unsigned int seed;
    std::string output_file_name;

    std::cout << "Enter range: ";
    std::cin >> range;

    std::cout << "Enter number of pairs: ";
    std::cin >> num_pairs;

    std::cout << "Enter random seed: ";
    std::cin >> seed;

    std::cout << "Enter output file name: ";
    std::cin >> output_file_name;

    auto pairs = generate_pairs(range, num_pairs, seed);

    write_to_file(output_file_name, pairs);

    std::cout << "Pairs written to output file.\n";

    return 0;
}
