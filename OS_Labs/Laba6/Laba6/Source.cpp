#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <algorithm>

std::vector<unsigned int> arr;
std::vector<unsigned int> final_arr;
std::vector<unsigned int> prime_arr;
std::vector<unsigned int> other_arr;
unsigned int k;
unsigned int arr_size = 0;
std::mutex mtx1;
std::mutex mtx2;
std::condition_variable cv_work_start;
std::condition_variable cv_sum_start;
bool work_started = false;
bool sum_started = false;

void work_thread()
{
    std::cout << "Enter sleep interval (in milliseconds): ";
    int sleep_interval;
    std::cin >> sleep_interval;

    prime_arr.reserve(arr_size);
    other_arr.reserve(arr_size);

    for (unsigned int i = 0; i < arr_size; i++)
    {
        bool is_prime = true;
        unsigned int num = arr[i];

        if (num > 1)
        {
            for (unsigned int j = 2; j <= num / 2; ++j)
            {
                if (num % j == 0)
                {
                    is_prime = false;
                    break;
                }
            }
        }
        else
        {
            is_prime = false;
        }

        if (is_prime)
        {
            prime_arr.push_back(num);
        }
        else
        {
            other_arr.push_back(num);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_interval));
    }
    final_arr.reserve(prime_arr.size() + other_arr.size());
    final_arr.insert(final_arr.end(), prime_arr.begin(), prime_arr.end());
    final_arr.insert(final_arr.end(), other_arr.begin(), other_arr.end());
    {
        std::lock_guard<std::mutex> lock(mtx1);
        work_started = true;
        cv_work_start.notify_one(); // Сигнализируем о завершении работы
    }
}

void sum_elem_thread()
{
    {
        std::unique_lock<std::mutex> lock(mtx1);
        cv_work_start.wait(lock, [] { return work_started; }); // Ждем завершения работы
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(3)); // Приостанавливаем работу на 3 мс

    unsigned long long sum = 0;
    {
        std::lock_guard<std::mutex> lock(mtx2);
        sum_started = true;
        cv_sum_start.notify_one(); // Сигнализируем о начале суммирования
        for (unsigned int i = 0; i < k; i++)
        {
            sum += final_arr[i];
        }
    }

    std::cout << "Sum of first " << k << " elements: " << sum << std::endl;
}

int main()
{
    std::cout << "Enter the size of the array: ";
    std::cin >> arr_size;

    std::cout << "Enter array elements: ";
    for (unsigned int i = 0; i < arr_size; i++)
    {
        unsigned int num;
        std::cin >> num;
        arr.push_back(num);
    }

    std::cout << "Array size: " << arr_size << std::endl;
    std::cout << "Array elements: ";
    for (unsigned int i = 0; i < arr_size; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "\nEnter k (number of elements to sum): ";
    std::cin >> k;

    std::thread work(work_thread);
    {
        std::unique_lock<std::mutex> lock(mtx1);
        cv_work_start.wait(lock, [] { return work_started; }); // Ждем завершения работы
    }

    std::cout << "Final array: ";
    for (unsigned int i = 0; i < final_arr.size(); i++)
    {
        std::cout << final_arr[i] << " ";
    }
    std::cout << std::endl;

    std::thread sum_elem(sum_elem_thread);

    work.join();
    sum_elem.join();

    return 0;
}
