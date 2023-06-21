#include <iostream>
#include <windows.h>
using namespace std;

void main(int argc, char** argv) {
    srand(time(0));
    int size1;
    cout << "-Server-\n Enter array1 size: ";
    cin >> size1;
    char* array1 = new char[size1];
    cout << "Enter array1 elements: ";
    for (int i = 0; i < size1; ++i) {
        cin >> array1[i];
    }
    cout << endl;
    int size2;
    cout << "-Server-\n Enter array2 size: ";
    cin >> size2;
    int8_t* array2 = new int8_t[size2];
    cout << "Enter array2 elements: ";
    for (int i = 0; i < size2; i++) {
        cin >> array2[i];
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    HANDLE hWritePipe, hReadPipe, hEvent;
    SECURITY_ATTRIBUTES sa;

    hEvent = CreateEvent(nullptr, FALSE, FALSE, L"Event");
    if (!hEvent) {
        cout << "-Server-\n Event was not created";
        return;
    }

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = nullptr;
    sa.bInheritHandle = FALSE;

    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        cout << "-Server-\n Pipe was not created";
        return;
    }

    HANDLE hDuplicateReadPipe, hDuplicateWritePipe;
    if (!DuplicateHandle(GetCurrentProcess(), hReadPipe, GetCurrentProcess(), &hDuplicateReadPipe, 0, TRUE, DUPLICATE_SAME_ACCESS)) {
        cout << "-Server-\n Duplicate handle failed";
        return;
    }
    if (!DuplicateHandle(GetCurrentProcess(), hWritePipe, GetCurrentProcess(), &hDuplicateWritePipe, 0, TRUE, DUPLICATE_SAME_ACCESS)) {
        cout << "-Server-\n Duplicate handle failed";
        return;
    }

    ZeroMemory(&si, sizeof(STARTUPINFO));
    wchar_t* wstr = new wchar_t[80];
    wsprintf(wstr, L"\"Searh.exe\" %d %d %d", (int)hDuplicateWritePipe, (int)hDuplicateReadPipe, (int)size1);
    if (!CreateProcess(nullptr, wstr, nullptr, nullptr, TRUE, CREATE_NEW_CONSOLE, nullptr, nullptr, &si, &pi)) {
        cout << "-Server-\n sum creation failed";
        return;
    }

    DWORD dwBytesWritten;
    if (!WriteFile(hWritePipe, &size1, sizeof(int), &dwBytesWritten, nullptr)) {
        cout << "-Server-\n info was not written";
        return;
    }
    if (!WriteFile(hWritePipe, &size2, sizeof(int), &dwBytesWritten, nullptr)) {
        cout << "-Server-\n info was not written";
        return;
    }
    if (!WriteFile(hWritePipe, array1, sizeof(char) * size1, &dwBytesWritten, nullptr)) {
        cout << "-Server-\n info was not written";
        return;
    }
    if (!WriteFile(hWritePipe, array2, sizeof(int8_t) * size2, &dwBytesWritten, nullptr)) {
        cout << "-Server-\n info was not written";
        return;
    }

    WaitForSingleObject(hEvent, INFINITE);

    DWORD dwBytesRead;
    int arr_size;

    if (!ReadFile(hReadPipe, &arr_size, sizeof(int), &dwBytesRead, nullptr)) {
        cout << "-Server-\n arr was not read";
        return;
    }
    char* new_arr = new char[arr_size];
    if (!ReadFile(hReadPipe, new_arr, sizeof(char) * arr_size, &dwBytesRead, nullptr)) {
        cout << "-Server-\n arr was not read";
        return;
    }

    cout << "-Server-\n";

    cout << "size1: " << size1 << "\n";
    cout << "Elements: ";
    for (int i = 0; i < size1; ++i) {
        cout << array1[i] << " ";
    } cout << "\n\n";

    cout << "size2: " << size2 << "\n";
    cout << "Elements: ";
    for (int i = 0; i < size2; ++i) {
        cout << array2[i] << " ";
    } cout << "\n\n";

    cout << "-Searh-\n";
    cout << " elements: ";
    for (int i = 0; i < arr_size; ++i) {
        cout << new_arr[i] << " ";
    } cout << "\n";

    CloseHandle(hWritePipe);
    CloseHandle(hReadPipe);
    CloseHandle(hEvent);
}