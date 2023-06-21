#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
using namespace std;

void main(int argc, char** argv) {
	HANDLE hWritePipe = (HANDLE)atoi(argv[1]);
	HANDLE hReadPipe = (HANDLE)atoi(argv[2]);
	//int size = atoi(argv[3]);
	//cout << size << "\n";
	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Event");
	if (!hEvent) {
		cout << "-Searh-\n event was not read";
	}
	int arr1_size, arr2_size;

	DWORD dwBytesRead;
	ReadFile(hReadPipe, &arr1_size, sizeof(int), NULL, NULL);
	ReadFile(hReadPipe, &arr2_size, sizeof(int), NULL, NULL);
	char* arr1 = new char[arr1_size];
	__int8* arr2 = new __int8[arr2_size];
	if (!ReadFile(hReadPipe, arr1, sizeof(char) * arr1_size, &dwBytesRead, NULL)) {
		cout << "-Searh-\n info was not read";
	}
	if (!ReadFile(hReadPipe, arr2, sizeof(int8_t) * arr2_size, &dwBytesRead, NULL)) {
		cout << "-Searh-\n info was not read";
	}

	// ������� �������� �������� �� �������
	cout << "Array 1: ";
	for (int i = 0; i < arr1_size; i++) {
		cout << arr1[i] << " ";
	}
	cout << endl;

	cout << "Array 2: ";
	for (int i = 0; i < arr2_size; i++) {
		cout << arr2[i] << " ";
	}
	cout << endl;

	// ������� ����� ������ � ��������� ��� ������������ ����������
	int new_arr_size = max(arr1_size, arr2_size); // ����� ������ �������
	char* new_arr = new char[new_arr_size];
	for (int i = 0; i < new_arr_size; i++) {
		new_arr[i] = '0'; // ��������� ����� ������ ������
	}
	int j = 0;
	for (int i = 0; i < arr1_size; i++) {
		if (isdigit(arr1[i])) {
			for (int k = 0; k < arr2_size; k++) {
				if (isdigit(arr2[k]) && arr1[i] == arr2[k]) {
					new_arr[j] = arr1[i];
					j++;
					break;
				}
			}
		}
	}

	// ������� ����� ������ �� �������
	cout << "New Array: ";
	for (int i = 0; i < j; i++) {
		cout << new_arr[i] << " ";
	}
	cout << endl;
	DWORD dwBytesWritten;
	if (!WriteFile(hWritePipe, &j, sizeof(int), &dwBytesWritten, NULL)) {
		cout << "-Searh-\n res was not written";
	}
	if (!WriteFile(hWritePipe, new_arr, j * sizeof(char), &dwBytesWritten, NULL)) {
		cout << "-Searh-\n res was not written";
	}

	SetEvent(hEvent);
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(hEvent);

	delete[] arr1;
	delete[] arr2;
	delete[] new_arr;

	while (_getch() != '0') {}
}