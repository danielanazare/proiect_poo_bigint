#include "BigInt.h"


///Constructor fara parametri: sir nul
BigInt::BigInt()
{
    init();
}

///Constructor ce primeste un int si il stocheaza intr-un tablou unidimensional de tip char
///Va fi stocat inversul numarului, fara a se pastra semnul pentru usurinta in operatiile aritmcetice
BigInt::BigInt(int value)
{
    init();
    if(value < 0)
    {
        isPositive = false;
        //value = value *(-1); ///Exception if value = INT_MIN
    }
    unsigned int aux = abs(value);
    length = countDigits(aux);
    number = (char*) malloc ((length+1)* sizeof(char));
    int i;
    for(i = 0; i < length; i++, aux /= 10)
        *(number + i) = (aux % 10) + '0';
    *(number + length) = '\0';
}

///Constructor cu parametrii ce primeste un sir de caractere
BigInt::BigInt(const char string[])
{
    init();
    length = strlen(string);
    int index = 0;
    if(string[0] == '-')
    {
        isPositive = false;
        length--;
        index++; ///Daca numarul e negativ, sarim peste primul caracter ('-') cu ajutorul lui index
    }

    number = (char*) malloc((length + 1) * sizeof(char));
    for(int i = 0; i < length; i++)
        *(number + i) = string[length - i - 1 + index];
    *(number + length) = '\0';
}

///Constructor de copiere
BigInt::BigInt(const BigInt & that)
{
    init();
    copy(that);
}

///Deconstructor
BigInt::~BigInt()
{
    clear();
}

///Metoda ce intializeaza un obiect
void BigInt::init()
{
    number = NULL;
    isPositive = true;
    length = 0;
}

///Numara cifrele unui int
int BigInt::countDigits(unsigned int x)
{
    int k = 0;
    while(x > 0)
    {
        k++;
        x /= 10;
    }
    return k;
}

///Metoda de copiere
void BigInt::copy(const BigInt & that)
{
    clear();
    length = that.length;
    isPositive = that.isPositive;

    number = (char*) malloc ((length + 1) * sizeof(char));
    memcpy(number, that.number, length + 1);
}
///Sterge un obiect
void BigInt::clear()
{
    delete number;
    length = 0;
    isPositive = true;
}

///Supraincare operator citire
std::ostream & operator<<(std::ostream & os, const BigInt & obj)
{
    if(!obj.isPositive)
    {
        os<<'-';
    }

    int i = obj.length - 1;
    do
    {
        os << *(obj.number + i);
        i--;
    }
    while(i >= 0);

    return os;
}

std::istream & operator>>(std::istream & is, const BigInt & obj)
{
    // exemplu
}

///Supraincarcarea operatorului =, atat pentru atribuirea unui sir de caractere, a unui int sau a unui alt obiect
BigInt & BigInt::operator=(const BigInt & that)
{
    if (this != &that)
    {
        copy(that);
    }
    return *this;
}

///
BigInt BigInt::operator+(const BigInt & that) const
{
    return BigInt(*this) += that;
}

BigInt BigInt::operator-(const BigInt & that) const
{
    return BigInt(*this) -= that;
}

/// In fucnite de semnele operanzilor se apeleaza add() sau substract()
BigInt & BigInt::operator+=(const BigInt & that)
{
    if (isPositive && that.isPositive)
    {
        add(that);
    }
    if (!isPositive && that.isPositive) ///a<0, b>0 , -a+b = -(a-b)
    {
       isPositive = true;
       subtract(that);
       //isPositive = false;
    }
    if (isPositive && !that.isPositive)
    {
        BigInt placeholder(that);
        placeholder.isPositive = true;
        subtract(placeholder);
    }

    if(!isPositive && !isPositive) ///cand ambele sunt negative
    {
        add(that);
        isPositive = false;
    }

    return *this;
}

/// Scaderea a - b e privita ca o adunare cu opusul: a + (-b)
BigInt & BigInt::operator-=(const BigInt & that)
{
    BigInt placeholder(that);
    placeholder.isPositive = !placeholder.isPositive;
    return *this += placeholder;
}

/// Adunarea a doua numere mari.
void BigInt::add(const BigInt & that)
{
    BigInt placeholder(*this);
    clear();

    int sum, carry = 0, i;
    int commonLength = std::min(placeholder.length, that.length);
    number = (char*) malloc((commonLength) * sizeof(char));
    length = commonLength;

    for(i = 0; i < commonLength; i++)
    {
        sum = (*(placeholder.number + i) - '0') + (*(that.number + i) - '0') + carry;
        carry = sum / 10;
        *(number + i) = (sum % 10) + '0';
    }

    if(placeholder.length > commonLength)
    {
        number = (char*) realloc(number, (placeholder.length + 1) * sizeof(char));
        for(i = commonLength; i < placeholder.length; i++)
        {
            sum = (*(placeholder.number + i) - '0') + carry;
            carry = sum/10;
            *(number + i) = char((sum % 10) + '0');
        }
        length = placeholder.length;
    }
     if(that.length > commonLength)
    {
        number = (char*) realloc(number, (that.length + 1) * sizeof(char));
        for(i = commonLength; i < that.length; i++)
        {
            sum = (*(that.number + i) - '0') + carry;
            carry = sum/10;
            *(number + i) = char((sum % 10) +'0');
        }
        length = that.length;
    }

    if(carry > 0)
    {
        length++;
        number = (char*) realloc(number, (length + 1) * sizeof(char));
        *(number + length - 1) = char(carry + '0');
    }
    *(number + length ) = '\0';

}

///Scadere
void BigInt::subtract(const BigInt & that)
{
    ///First, we check if left < right. If this is the case, replace (left - right) with -(right - left)
    if(*this < that)
    {
        BigInt placeholder(that);
        placeholder.subtract(*this);
        copy(placeholder);
        isPositive = false;
    }
    else
    {
        BigInt placeholder(*this);
        clear();

        int result, borrow = 0, i;
        int commonLength = std::min(placeholder.length, that.length);
        number = (char*) malloc((commonLength + 1) * sizeof(char));
        for(i = 0; i < commonLength; i++)
        {
            result = (*(placeholder.number + i) - '0') - (*(that.number + i) - '0') - borrow;
            if(result < 0)
            {
                borrow = 1;
                result += 10;
            }
            *(number + i) = char(result + '0');
        }
        if(placeholder.length> commonLength)
        {
            number = (char*) realloc(number, (placeholder.length + 1) * sizeof(char));
            length = placeholder.length;

            for(i = commonLength; i < placeholder.length; i++)
            {
                result = (*(placeholder.number + i) - '0') - borrow;
                if(result < 0)
                {
                    borrow = 1;
                    result +=10;
                }
                *(number + i) = char(result + '0');
            }

            if(*(number + length - 1) - '0' == 0)
                length--;
        }
        *(number + length) = '0';

    }
}
/// a < b :
///caz 1: a<0, b>0 => a<b => true
///caz 2: a>0, b<0 => a>b =>false
///caz 3: a,b<0 => comparam lungimile; daca sunt egale comparam cifra cu cifra incepand cu cifra cea mai semnificativa
///                 a[i]>b[i] => true
///caz 4: a,b>0 => la fel la ca caz3, cu diferenta ca a[i]<b[i] =>true
bool BigInt::operator<(const BigInt & that) const
{
    if(!isPositive && that.isPositive)
        return true;
    if(isPositive && !that.isPositive)
        return false;
    if(!isPositive && !that.isPositive)
    {
        if(length > that.length) /// a.length > b.length => a<b =>true
            return true;
        if(length < that.length)
            return false;
        ///daca a.length == b.length
        for(int i = length - 1; i >=0 ;i--)
            if(*(number + i) - '0' > *(that.number + i) - '0')
                return true;
            else if(*(number + i) - '0' < *(that.number + i) - '0')
                return false;
        ///daca a==b
        return false;
    }
    if(isPositive && that.isPositive)
    {
        if(length < that.length)///a.length < b.length => a<b
            return true;
        if(length > that.length)
            return false;
        ///daca a.length == b.length
        for(int i = length - 1; i >= 0; i--)
            if(*(number + i) - '0' < *(that.number + i) - '0')
                    return true;
            else if(*(number + i) - '0' > *(that.number + i) - '0')
                return false;
        ///daca a==b
        return false;
    }
}

void BigInt::paritate()
{
    if(number == NULL)
    {
        std::cout << "Obiect neinstantiat" << std::endl;
        return;
    }
    if((*(number) - '0') % 2 == 0)
        std::cout << *this << " este par. " << std::endl;
    else
        std::cout << *this << " este impar. " << std::endl;
}
///Metoda ce determinca daca un numar este palindrom.
///Folosim 2 indecsi, i pleca de la stanga spre dreapta si j de la dreapta spre stanga
void BigInt::palindrom()
{
    int i = 0, j = length - 1;
    while(i <= j)
    {
        if(*(number + i) != *(number + j))
        {
            std::cout << *this << "  nu e palindrom. " << std::endl;
            return;
        }
        i++;
        j--;
    }
    std::cout << *this << " este numar palindrom. " << std::endl;

}
///Metoda ce returneaza suma cifrelor
int BigInt::digitSum()
{
    int sum = 0;
    for(int i = 0; i < length; i++)
        sum += *(number + i) - '0';
    return sum;
}


