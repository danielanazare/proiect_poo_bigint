#include <iostream>
#include "BigInt.h"
///russian peasant for multiplication
using namespace std;

int main()
{

    BigInt a, b, c, d;

    a = 1234;
    b = "-789";
    c = "45";
    d = -99;

    std::cout << a << " + " << c << " = " << a + c << std::endl; ///pozitiv + pozitiv
    std::cout << a << " + " << b << " = " << a + b << std::endl; /// pozitiv + negativ
    std::cout << b << " + " << d << " = " << b + d << std::endl; ///negativ + negativ

    BigInt e, f;
    e = "12321";
    f = 12347321;
    e.palindrom();
    f.palindrom();

    int sum = e.digitSum();
    //cout<<sum;

   /**
   * Inca nu functioneaza
    BigInt x;
    cin >> x;
    cout << x;
    */


    return 0;
}
