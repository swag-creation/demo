#include <iostream>
#include <vector>
#include <windows.h>

#include "marker_array.h"

static MarkerArray *g_array = nullptr;
static HANDLE *g_startEvents = nullptr;
static HANDLE *g_stopEvents = nullptr;
static HANDLE *g_blockedEvents = nullptr;
static CRITICAL_SECTION g_printLock;

DWORD WINAPI MarkerProc(LPVOID param) {
    int number = (int) (INT_PTR) param;
    int id = number - 1;

    WaitForSingleObject(g_startEvents[id], INFINITE);
    srand(number);

    while (true) {
        int index = rand() % g_array->size();

        if (g_array->isFree(index)) {
            Sleep(5);
            if (g_array->tryMark(index, number)) {
                Sleep(5);
                continue;
            }
        }

        EnterCriticalSection(&g_printLock);
        std::cout << "Marker " << number
                << ": marked " << g_array->countMarkedBy(number)
                << " elements, cannot mark index " << index << std::endl;
        LeaveCriticalSection(&g_printLock);

        SetEvent(g_blockedEvents[id]);

        HANDLE waitHandles[2] = {g_startEvents[id], g_stopEvents[id]};
        DWORD result = WaitForMultipleObjects(2, waitHandles, FALSE, INFINITE);

        if (result == WAIT_OBJECT_0 + 1) {
            g_array->clearMarksBy(number);
            return 0;
        }
    }
}

int main() {
    int size;
    std::cout << "Enter array size: ";
    std::cin >> size;
    if (size < 1) {
        std::cout << "Size must be positive." << std::endl;
        return 1;
    }

    MarkerArray array(size);
    g_array = &array;

    int markerCount;
    std::cout << "Enter markers count: ";
    std::cin >> markerCount;
    if (markerCount < 1) {
        std::cout << "Markers count must be positive." << std::endl;
        return 1;
    }

    InitializeCriticalSection(&g_printLock);

    g_startEvents = new HANDLE[markerCount];
    g_stopEvents = new HANDLE[markerCount];
    g_blockedEvents = new HANDLE[markerCount];
    std::vector<HANDLE> threads(markerCount);
    std::vector<bool> active(markerCount, true);

    for (int i = 0; i < markerCount; i++) {
        g_startEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
        g_stopEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
        g_blockedEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        threads[i] = CreateThread(NULL, 0, MarkerProc, (LPVOID) (INT_PTR) (i + 1), 0, NULL);
    }

    for (int i = 0; i < markerCount; i++) {
        SetEvent(g_startEvents[i]);
    }

    int remaining = markerCount;

    while (remaining > 0) {
        std::vector<HANDLE> blocked;
        for (int i = 0; i < markerCount; i++) {
            if (active[i]) {
                blocked.push_back(g_blockedEvents[i]);
            }
        }
        WaitForMultipleObjects((DWORD) blocked.size(), blocked.data(), TRUE, INFINITE);

        std::cout << "Array: " << array.toString() << std::endl;

        int number;
        do {
            std::cout << "Enter marker number to terminate: ";
            std::cin >> number;
        } while (number < 1 || number > markerCount || !active[number - 1]);

        int id = number - 1;
        SetEvent(g_stopEvents[id]);
        WaitForSingleObject(threads[id], INFINITE);
        CloseHandle(threads[id]);
        active[id] = false;
        remaining--;

        std::cout << "Array: " << array.toString() << std::endl;

        for (int i = 0; i < markerCount; i++) {
            if (active[i]) {
                ResetEvent(g_blockedEvents[i]);
                SetEvent(g_startEvents[i]);
            }
        }
    }

    for (int i = 0; i < markerCount; i++) {
        CloseHandle(g_startEvents[i]);
        CloseHandle(g_stopEvents[i]);
        CloseHandle(g_blockedEvents[i]);
    }
    delete[] g_startEvents;
    delete[] g_stopEvents;
    delete[] g_blockedEvents;
    DeleteCriticalSection(&g_printLock);

    std::cout << "All markers finished." << std::endl;
    return 0;
}
