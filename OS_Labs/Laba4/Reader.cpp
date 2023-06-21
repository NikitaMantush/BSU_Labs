#include <iostream>
#include <windows.h>

int main(int args, char* argv[]) {
	HANDLE semaphore;
	HANDLE events[2], events2[2];
	HANDLE signal_event_from_reader;
	const char* msgs[] = { "A", "B", "A1", "B1" };

	if (args != 2) {
		std::cerr << "Wrong parameters amount\n";
		system("pause");
		return GetLastError();
	}
	int n = atoi(argv[1]);

	if (n <= 0) {
		std::cerr << "Wrong messages amount\n";
		system("pause");
		return GetLastError();
	}

	semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"reader_semaphore");

	if (semaphore == NULL) {
		std::cerr << "Can't open semaphore\n";
		system("pause");
		return GetLastError();
	}

	events[0] = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"A_writer");
	events[1] = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"B_writer");
	events2[0] = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"A1_writer");
	events2[1] = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"B1_writer");

	signal_event_from_reader = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"signal_from_reader");

	for (size_t i = 0; i < 4; ++i) {
		if (events[i] == NULL) {
			for (size_t j = 0; j < 4; ++j)
				if (events[j] != NULL)
					CloseHandle(events[i]);
			std::cerr << "Can't open event\n";
			system("pause");
			CloseHandle(semaphore);
			return GetLastError();
		}
	}

	std::cout << "Messages:\n";

	for (size_t i = 0; i < n; ++i) {
		WaitForSingleObject(semaphore, INFINITE);
		std::cout << "active" << "\n";
		int index = WaitForMultipleObjects(2, events, FALSE, INFINITE) - WAIT_OBJECT_0;
		std::cout << msgs[index + 2] << std::endl;
		SetEvent(events2[index]);
		ReleaseSemaphore(semaphore, 1, NULL);
		std::cout << "Semaphore realised." << "\n";
	}

	std::cout << "I'm done!\n";
	system("pause");
	for (size_t i = 0; i < 4; ++i)
		CloseHandle(events[i]);
	CloseHandle(semaphore);

	SetEvent(signal_event_from_reader);
	return 0;
}