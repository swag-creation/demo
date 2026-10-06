#include "test.h"

#include <iostream>
#include <windows.h>

void threadAverage(const int *array, int size, volatile int *averageOut) {
    int sum{};
    for (int i = 0; i < size; i++) {
        sum += array[i];
        Sleep(12);
    }
    *averageOut = sum / size;
    std::cout << "Average: " << *averageOut << std::endl;
}
