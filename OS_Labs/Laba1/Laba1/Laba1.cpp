#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <Windows.h>

using namespace std;

// Finds the maximum negative element in an array
int findMaxNegative(int* arr, int size) {
    int max = INT_MIN;
    for (int i = 0; i < size; i++) {
        if (arr[i] < 0 && arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

// Worker thread function
unsigned int __stdcall workerThread(void* data) {
    int* arr = (int*)data;
    int size = *(arr - 1);
    int max = findMaxNegative(arr, size);
    cout << "Max negative element: " << max << endl;
    Sleep(100);
    return 0;
}

int main() {
    srand(time(0));
    int size;
    cout << "Enter array size: ";
    cin >> size;
    int* arr = new int[size];
    cout << "Generating random elements for the array..." << endl;
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100 - 50;
    }
    cout << "Array: ";
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    int stopTime;
    cout << "Enter stop time (in milliseconds): ";
    cin >> stopTime;
    unsigned int threadID;
    HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, workerThread, arr + 1, 0, &threadID);
    SuspendThread(hThread);
    Sleep(stopTime);
    ResumeThread(hThread);
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    delete[] arr;
    return 0;
}