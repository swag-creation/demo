#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>
#include <windows.h>
volatile std::pair<int, int> minMax;
volatile std::pair<int, int> indxMinMax;
volatile int average;
void threadMinMax(std::vector<int> &array) {
    indxMinMax.first = 0;
    indxMinMax.second = 0;
    minMax.first = array[0];
    minMax.second = array[0];
    int size = array.size();
    for (int i = 1; i < size; i++) {
        if (array[i] > minMax.first) {
            minMax.first = array[i];
            indxMinMax.first = i;
            Sleep(7);
        } else {
            if (array[i] < minMax.second) {
                minMax.second = array[i];
                indxMinMax.second = i;
                Sleep(7);
            }
        }
    }
    std::cout << "Max: " << minMax.first << std::endl << "Min: " << minMax.second << std::endl;
}
typedef void (*ThreadAverageFunc)(const int *, int, volatile int *);

void threadAverage(std::vector<int> &array) {
    HMODULE hLib = LoadLibraryA("test.dll");
    if (hLib == NULL) {
        std::cerr << "Error loading test.dll" << std::endl;
        return;
    }
    ThreadAverageFunc func = (ThreadAverageFunc) GetProcAddress(hLib, "threadAverage");
    if (func == NULL) {
        std::cerr << "Error loading function threadAverage" << std::endl;
        FreeLibrary(hLib);
        return;
    }
    func(array.data(), (int) array.size(), &average);
    FreeLibrary(hLib);
}
int main() {
    int size{};
    std::cout << "Enter size: ";
    std::cin >> size;
    std::vector<int> array(size);
    for (int i = 0; i < size; i++) {
        std::cout << "Enter value " << i + 1 << ": ";
        std::cin >> array[i];
    }
    HANDLE hThread;
    hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadMinMax, &array, 0, NULL);
    if (hThread == NULL) {
        std::cerr << "Error creating thread" << std::endl;
    }
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadAverage, &array, 0, NULL);
    if (hThread == NULL) {
        std::cerr << "Error creating thread" << std::endl;
    }
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    std::cout << "Array before changes" << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << array[i] << ' ';
    }
    array[indxMinMax.first] = average;
    array[indxMinMax.second] = average;
    std::cout << '\n' <<"Array after changes" << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << array[i] << ' ';
    }
    std::cout << std::endl;
    return 0;
}