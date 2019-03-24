#ifndef BIGINT_H
#define BIGINT_H
#include <iostream>
#include <cstring> /// strlen
#include <cstdlib> /// abs
#include <limits>
#include <conio.h>


class BigInt
{
    char *number;
    int length;

public:
    BigInt();
    BigInt(int value);
    BigInt(const char string[]);
    BigInt(const BigInt & that);
    ~BigInt();


    bool isPositive;

    BigInt & operator=(const BigInt & that);

    BigInt & operator+=(const BigInt & that);
    BigInt operator+(const BigInt & that) const;

    BigInt & operator-=(const BigInt & that);
    BigInt operator-(const BigInt & that) const;

    bool operator<(const BigInt & that) const;
    bool operator<=(const BigInt & that) const;
    bool operator>(const BigInt & that) const;
    bool operator>=(const BigInt & that) const;


    friend std::ostream & operator<<(std::ostream & os, const BigInt & obj);
    friend std::istream & operator>>(std::istream & is, BigInt & obj);

    void paritate();
    void palindrom();
    int digitSum();

private:

    void init();
    void copy(const BigInt & that);
    void clear();

    int countDigits(unsigned int);

    void add(const BigInt & that);
    void subtract(const BigInt & that);

};

#endif // BIGINT_H
