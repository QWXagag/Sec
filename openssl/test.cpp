#include <iostream>
#include <unordered_set>
using namespace std;


int func(int arr[], int arrLen)
{
    for (int i = 0; i < arrLen; i++)
    {
        while (arr[i] != i)
        {
            if (arr[arr[i]] == arr[i])
            {
                return arr[i];
            }
            int temp = arr[i];
            arr[i] = arr[temp];
            arr[temp] = temp;
        }

    }
    return -1;
}


int main()
{
    int arr[] = {1, 5, 3, 2, 3, 1, 0, 2, 5, 3};
    cout << sizeof(arr)/sizeof(int) << endl;
    cout << func(arr, sizeof(arr)/sizeof(int)) << endl;
    return 0;
}
