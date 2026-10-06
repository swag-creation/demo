#pragma once

extern "C" __declspec(dllexport) void threadAverage(const int *array, int size, volatile int *averageOut);
