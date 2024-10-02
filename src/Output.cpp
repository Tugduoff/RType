#include "Output.hpp"


std::ofstream outputFile;

void openOutputFile() {
    outputFile.open("output.txt");

    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output.txt" << std::endl;
    }
}