#include "dbigint.h"
#include <cassert>
#include <algorithm>
#include <vector>

dbigint::dbigint(int n)
{
    if (n == 0 || n == -0)
    {
        _data = new size_type[1];
        _data[0] = 0;
        _capacity = 1;
        _neg = false;
        return;
    }

    else if (n < 0)
    {
        _neg = true;
        n = -n;
    }
    else
        _neg = false;

    int copy(n);
    _capacity = 0;
    while (copy > 0)
    {
        copy /= 10;
        ++_capacity;
    }

    _data = new size_type[_capacity];
    std::fill(_data, _data+_capacity, 0);
    size_type i(0);
    while (n > 0)
    {
        _data[i++] = n % 10;
        n /= 10;
    }
}


dbigint::dbigint(const dbigint & b)
{
    _capacity = b._capacity;
    _data = new size_type [_capacity];
    _neg = b._neg;
    std::copy(b._data, b._data + _capacity, _data);
}

dbigint::dbigint(const std::string &s)
{
    _capacity = s.size() + 1;
    _data = new size_type [_capacity];
    std::fill(_data, _data + _capacity, 0);
    _neg = false;

    if (s.size() == 0)
        return;

   char c = s[0];
   int start(0);
   assert (c == '+' || c == '-' || isdigit(c));

   if (c == '-') {
       _neg = true;
       start = 1;
   }
   else if (c == '+')
       start = 1;

   size_type j = 0;
   for (int i = s.size() - 1; i >= start; --i) {
       assert(isdigit(s[i]));
       _data[j++] = s[i] - '0'; //converts to number by subtracting ascii code of 0
   }

   if (std::count(_data, _data+_capacity, 0) == _capacity)
       _neg = false;


}




dbigint::size_type dbigint::operator[](size_type pos) const
{
    if (pos >= _capacity)
        return 0;
    return _data[pos];
}



dbigint::size_type dbigint::size() const
{
    return _capacity;
}



bool dbigint::is_negative() const
{
    return _neg;
}


dbigint::~dbigint()
{
    delete [] _data;
}



std::ostream & operator <<(std::ostream &os, const dbigint &b)
{
    if (b < 0)
        os << "-";

   int i = b.size() - 1;
   while(i>0 && b[i]==0)
       i--;

    for (i; i >= 0; --i)
        os << b[i];

    return os;
}


void dbigint::operator =(const dbigint &b)
{
    if (this == &b)
        return;

    if (_capacity != b._capacity) {
        delete [] _data;
        _capacity = b._capacity;
        _data = new size_type[_capacity];  // MISSING LINE
    }
    std::copy(_data, b._data + _capacity, _data);
    _neg = b._neg;

}

void dbigint::reserve(size_type new_capacity)
{
    if (new_capacity > _capacity)
    {
        //allocate new block (every time you allocate a new block, you need to use a pointer)
        size_type *new_data = new size_type[new_capacity];

        //fill rest of block with 0s
        std::fill(new_data, new_data + new_capacity, 0);

        //copy information over
        std::copy(_data, _data + _capacity, new_data);

        delete [] _data;
        _data = new_data;
        _capacity = new_capacity;

    }
}


void dbigint::operator *=(size_type digit)
{
    assert(digit < 10);
    reserve(_capacity + 1);

    int sum, carry(0);
    for (int i = 0; i < _capacity; ++i)
    {
        sum = _data[i] * digit + carry;
        _data[i] = sum % 10;
        carry = sum / 10;
    }
}


void dbigint::operator *=(dbigint const&ob)
{
    size_type new_capacity = 2 * std::max(_capacity, ob._capacity);
    reserve(new_capacity);

    dbigint b(ob); //can't change ob because it was passed in w "const"
    b.reserve(new_capacity);

    int i = new_capacity - 1;
    while(i > 0 && _data[i] == 0)
        i--;

    dbigint ans(0);
    while(i >= 0)
    {
        ans.shiftleft();
        b *= _data[i];
        ans += b;
        i--;
    }
    std::copy(ans._data, ans._data +ans._capacity, _data);

}




void dbigint::operator +=(const dbigint &ob)
{
   size_type new_capacity = 1 + std::max(_capacity, ob._capacity);
   reserve(new_capacity);

   dbigint b(ob); //can't change ob because it was passed in w "const"
   b.reserve(new_capacity);

   // rest of code is the same as it was for bigint
   // with CAPACITY replaced by _capacity

   if (_neg == b._neg) {
       size_type carry(0), sum;

       for (int i = 0; i < _capacity; ++i) {
           sum = carry + _data[i] + b._data[i];
           _data[i] = sum % 10;
           carry = sum / 10;
       }
   }

   else {
       negate();
       *this -= b;
       negate();
   }
}

dbigint operator +(const dbigint &a, const dbigint &b)
{
    dbigint temp = a;
    temp += b;
    return temp;
}


dbigint operator -(const dbigint &a, const dbigint &b)
{
    dbigint temp = a;
    temp -= b;
    return temp;
}

dbigint operator -(const dbigint &a)
{
    dbigint temp = a;
    temp.negate();
    return temp;
}

dbigint operator *(const dbigint &a, const dbigint &b)
{
    dbigint temp = a;
    temp *= b;
    return temp;
}


void dbigint::operator -=(const dbigint &ob)
{
    size_type new_capacity = 1 + std::max(_capacity, ob._capacity);
    reserve(new_capacity);

    dbigint b(ob); //can't change ob because it was passed in w "const"
    b.reserve(new_capacity);

    if (compare(b) == 0)
        for (int i = 0; i < _capacity; i++)
            _data[i] = 0;

    else if ((!_neg && b._neg) || (_neg && !b._neg)) {
        negate();
        *this += b;
        negate();
        }

    else if ((!_neg && !b._neg && *this > b) || (_neg && !b._neg && *this < b)) {
        int borrow(0), dif;
        for (int i = 0; i < _capacity; ++i) {
            dif = _data[i] - b._data[i] - borrow;
            _data[i] =  (dif + 10) % 10;
            borrow = (dif < 0? 1 : 0);
         }
     }

     else if ((_neg && b._neg && *this > b) || (!_neg && !b._neg && *this < b)) {
         negate();
         int borrow(0), dif;
         for (int i = 0; i < _capacity; ++i) {
             dif = b._data[i] - _data[i] - borrow;
             _data[i] = (dif + 10) % 10;
             borrow = (dif < 0? 1 : 0);
          }
     }
}



void dbigint::shiftleft()
{
    reserve(_capacity + 1);
    for (int i = _capacity - 1; i > 0; --i)
        _data[i] = _data[i-1];

    _data[0] = 0;
}




void dbigint::negate()
{
    _neg = !_neg;
}



int dbigint::compare (const dbigint &b) const {
    if (_neg && !b._neg)
        return -1;
    if (!_neg && b._neg)
        return 1;

    for (int i = _capacity - 1; i >= 0; --i)
       {
           if (_data[i] > b._data[i])
           {
               if (!_neg)
                   return 1;
               else
                   return -1;
           }
           else if (_data[i] < b._data[i])
           {
               if (!_neg)
                   return -1;
               else
                   return 1;
           }
       }
       return 0;
}



bool operator < (const dbigint & b1, const dbigint & b2)
{
    return (b1.compare(b2) == -1);
}

bool operator <= (const dbigint & b1, const dbigint & b2)
{
    return (b1.compare(b2) <= 0);
}

bool operator == (const dbigint & b1, const dbigint & b2)
{
    return (b1.compare(b2) == 0);
}

bool operator != (const dbigint & b1, const dbigint & b2)
{
    return (b1.compare(b2) != 0);
}


bool operator > (const dbigint & b1, const dbigint & b2)
{
    return (b1.compare(b2) == 1);
}

bool operator >= (const dbigint & b1, const dbigint & b2)
{
    return (b1.compare(b2) >= 0);
}








//std::vector<int> v{1,2,3,4};

//for (vector<int>::interator i = v.begin(); i != v.end(); ++i)
//{
//    cout << *i << endl;
//}
