#pragma once

#include <mutex>
#include <string>
#include <vector>

class MarkerArray {
public:
    explicit MarkerArray(int size) : cells(size, 0) {
    }

    int size() const {
        return (int) cells.size();
    }

    bool isFree(int index) const {
        std::lock_guard<std::mutex> lock(mtx);
        return cells[index] == 0;
    }

    bool tryMark(int index, int markerNumber) {
        std::lock_guard<std::mutex> lock(mtx);
        if (cells[index] != 0) {
            return false;
        }
        cells[index] = markerNumber;
        return true;
    }

    int countMarkedBy(int markerNumber) const {
        std::lock_guard<std::mutex> lock(mtx);
        int count = 0;
        for (int value: cells) {
            if (value == markerNumber) {
                count++;
            }
        }
        return count;
    }

    int countMarked() const {
        std::lock_guard<std::mutex> lock(mtx);
        int count = 0;
        for (int value: cells) {
            if (value != 0) {
                count++;
            }
        }
        return count;
    }

    void clearMarksBy(int markerNumber) {
        std::lock_guard<std::mutex> lock(mtx);
        for (int &value: cells) {
            if (value == markerNumber) {
                value = 0;
            }
        }
    }

    std::string toString() const {
        std::lock_guard<std::mutex> lock(mtx);
        std::string result;
        for (int i = 0; i < (int) cells.size(); i++) {
            if (i > 0) {
                result += ' ';
            }
            result += std::to_string(cells[i]);
        }
        return result;
    }

private:
    std::vector<int> cells;
    mutable std::mutex mtx;
};
