#include <iostream>
#include <chrono>
#include <cmath>
int main(void)
{

    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    int sum = 0;
    for (int i = 0; i < 999999999; i++)
        sum += i;
    std::chrono::duration<double>sec = std::chrono::system_clock::now() - start;
    std::cout << "for���� �����µ� �ɸ��� �ð�(��) : " << sec.count() << "seconds" << std::endl;



}