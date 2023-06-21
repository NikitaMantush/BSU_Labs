#include <iostream>
#include <windows.h>
#include <vector>

using namespace std;

CRITICAL_SECTION cs;
HANDLE event;
int* finalArray;
int multResult = 1;
unsigned int A, B;

struct Information
{
    int arraySize;
    int* originalArray;
    unsigned int A, B;
};

DWORD WINAPI workThread(void* obj) {

    EnterCriticalSection(&cs);


    Information* info = (Information*)obj;
    int* f_arr = info->originalArray;
    int result[sizeof(f_arr)] = { 0 };
    int index = 0;

    cout << "--- workThread is started" << endl;

    int restInterval;
    cout << "Enter time interval to rest after preparing each element: ";
    cin >> restInterval;

    finalArray = new int[info->arraySize];
    for (unsigned int i = 0; i < info->arraySize; i++)
    {
        if (f_arr[i] >= A && f_arr[i] <= B)
        {
            result[index++] = f_arr[i];
        }
        else
        {
            result[i] = 0;
        }
        Sleep(restInterval);
    }

    for (int j = 0; j < info->arraySize; j++)
    {
        finalArray[j] = result[j];
    }

    cout << "Final array elements: ";
    for (int i = 0; i < info->arraySize; i++) {
        cout << finalArray[i] << " ";
    }
    cout << endl;
    cout << "--- workThread is finished" << endl << endl;
    LeaveCriticalSection(&cs);


    return 0;
}

DWORD WINAPI multElementThread(void* obj) {


    EnterCriticalSection(&cs);

    cout << "--- multElementThread is started" << endl;

    Information* info = (Information*)obj;

    //int multResult = 1;
    for (int i = 0; i < info->arraySize; i++)
    {
        if (finalArray[i] >= info->A && finalArray[i] <= info->B)
        {
            multResult *= finalArray[i];
        }
    }
    cout << "Multiplication result: " << multResult << endl;

    LeaveCriticalSection(&cs);

    cout << "--- multElementThread is finished" << endl;

    SetEvent(event);
    return 0;
}

int main() {
    InitializeCriticalSection(&cs);
    event = CreateEvent(NULL, FALSE, FALSE, NULL);

    int arraySize;
    cout << "Enter array size: ";
    cin >> arraySize;

    cout << "Enter array elements: ";
    int* originalArray = new int[arraySize];

    for (int i = 0; i < arraySize; i++) {
        std::cin >> originalArray[i];
    }

    // PRINT INPUT DATA
    cout << "Original array size: " << arraySize << std::endl;

    cout << "Original array elements: ";
    for (int i = 0; i < arraySize; i++) {
        cout << originalArray[i] << " ";
    }
    cout << endl;
    cout << endl;
    cout << "Enter A: ";
    cin >> A;

    cout << "Enter B: ";
    cin >> B;
    cout << endl;

    // INFORMATION
    Information* info = new Information();
    info->originalArray = originalArray;
    info->arraySize = arraySize;
    info->A = A;
    info->B = B;

    HANDLE hThread1 = CreateThread(NULL, 0, workThread, (void*)info, 0, NULL);
    WaitForSingleObject(hThread1, INFINITE);
    HANDLE hThread2 = CreateThread(NULL, 0, multElementThread, (void*)info, 0, NULL);


    WaitForSingleObject(event, INFINITE);
    cout << endl << "Result of MultElement: " << multResult << endl;
    WaitForSingleObject(hThread2, INFINITE);


    CloseHandle(hThread1);
    CloseHandle(hThread2);

    DeleteCriticalSection(&cs);
    CloseHandle(event);

    return 0;
}
