#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    cout << "enter array size: ";
    int arraySize;
    cin >> arraySize;
    double* array = new double[arraySize];
    int k;
    cout << "enter value of k: ";
    cin >> k;
    cout << "enter array elements: ";
    for (int i = 0; i < arraySize; i++) {
        cin >> array[i];
    }
 
    wstring str = L"\"child.exe\" ";
    str += to_wstring(arraySize) + L" ";
    str += to_wstring(k) + L" ";
    for (int i = 0; i < arraySize; i++) {
        str += to_wstring(array[i]) + L" ";
    }
    wchar_t* s = new wchar_t[str.length()];
    wcscpy_s(s, str.length() + 1, str.c_str());

    STARTUPINFO si;
    PROCESS_INFORMATION piApp;

    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.dwFlags = STARTF_USEFILLATTRIBUTE;
    si.dwFillAttribute = FOREGROUND_GREEN;

    if (CreateProcess(nullptr, s, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp)) {
        cout << "the new process is created.\n";
        WaitForSingleObject(piApp.hProcess, INFINITE);
    }
    else {
        cout << "the new process isn't created.\n";
    }
    CloseHandle(piApp.hThread);
    CloseHandle(piApp.hProcess);
}