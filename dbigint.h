#ifndef DBIGINT_H
#define DBIGINT_H

#include <cstdlib>
#include <string>
#include <iostream>

class dbigint
{
public:

    typedef std::size_t size_type;
    // pre: none
    // post: creates a dbigint with given integer value (default 0)
    dbigint(int = 0);

    // pre: s[0] is +, - or a digits; s[1]...s[s.size()-1] are digits
    // post:creates a dbigint with the given digits and optional sign
    dbigint(const std::string & s);

    // pre: none
    // post: copy constructor
    dbigint(const dbigint &);


    // pre: none
    // post: returns dynamically allocated memory back to heap
    ~dbigint();

    // pre: none
    // post: makes this dbigint a copy of given dbigint
    void operator= (const dbigint &);

    // pre: none
    // post: returns the digit at given position (0 is least significant) or 0 if doesn't exist
    size_type operator [](size_type pos) const;

    // pre: none
    // post: returns 0, 1, or -1 if this dbigint is equal to, greater than, or less than given dbigint
    int compare(const dbigint &) const;


    // pre: none
    // post: returns a string of sign and digits of this dbigint
    std::string toStdString() const;

    // pre: none
    // post: returns the size of this dbigint
    size_type size() const;

    // pre: none
    // post: returns true iff this dbigint is negative
    bool is_negative() const;

    // pre: none
    // post: flips the sign of this dbigint
    void negate();

    void operator +=(const dbigint &ob);
    void operator -=(const dbigint &);

    // pre: digit < 10
    void operator *=(size_type digit);
    void operator *=(const dbigint &);

    /* NEWLY ADDED */
    // pre: none
    // post: if new_capacity <= _capacity then do nothing (cannot )
    //       else allocate a new block with size new_capacity
    //       copy
    void reserve(size_type new_capacity);

    void shiftleft();

    private:
        size_type *_data;
        size_type _capacity;
        bool _neg;

        // INVARIANTS
        //  _data holds the address of a dynamic array of _capacity size_type variables
        //  _data[i] holds the digit at position i (or 0 if not used)
        //  _data[0] holds the units digit
        // _neg = true if this dbigint is negative, false otherwise
    };

    bool operator <(const dbigint &, const dbigint &);
    bool operator <=(const dbigint &, const dbigint &);
    bool operator ==(const dbigint &, const dbigint &);
    bool operator !=(const dbigint &, const dbigint &);
    bool operator >=(const dbigint &, const dbigint &);
    bool operator >(const dbigint &, const dbigint &);

    dbigint operator +(const dbigint &, const dbigint &);
    dbigint operator -(const dbigint &, const dbigint &);
    dbigint operator -(const dbigint &);
    dbigint operator *(const dbigint &, dbigint::size_type);
    dbigint operator *(const dbigint &, const dbigint &);

    std::ostream & operator <<(std::ostream &, const dbigint &);
    std::istream & operator >>(std::istream &, dbigint &);

    #endif // DBIGINT_H
