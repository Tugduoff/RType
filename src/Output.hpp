#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <fstream>  // For std::ofstream
#include <iostream>
#include <ostream>


// Declare the global ofstream variable
extern std::ofstream outputFile;

void openOutputFile();

#endif  // OUTPUT_HPP
