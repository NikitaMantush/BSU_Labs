#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    vector <double> array;
    int n = atoi(argv[1]);
    cout << "n = " << n << endl;
    int k = atoi(argv[2]);
    cout << "k = " << k << endl;
    for (int i = 3; i < argc; i++) {
        array.push_back(atof(argv[i]));
    }
    cout << "array:\n";
    for (int i = 0; i < array.size(); i++) {
        cout << array[i] << " ";
    }
    double sum = 0;
    for (int i = 0; i < k; i++) {
        sum += array[i];
    }
    cout << endl;
    cout << "new resulting array:\n";
    cout << sum << " ";
    for (int i = k; i < array.size(); i++) {
        cout << array[i] << " ";
    }
    system("pause");
}