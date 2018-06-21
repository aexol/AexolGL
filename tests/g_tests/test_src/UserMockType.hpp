#ifndef _USERMOCKTYPE_HPP
#define _USERMOCKTYPE_HPP

#include <aex/Common.hpp>

#include <limits>
#include <cmath>
#include <iostream>

struct UserSimulationTypeMock
{
    long long a;
    double b;
    static CONSTEXPR UserSimulationTypeMock max();
    static CONSTEXPR UserSimulationTypeMock min();
    static CONSTEXPR UserSimulationTypeMock zero();
    UserSimulationTypeMock& operator=(long long a);
    UserSimulationTypeMock& operator=(double a);
    void forward(const UserSimulationTypeMock& rhs, const UserSimulationTypeMock& range);
    void rewind(const UserSimulationTypeMock& rhs, const UserSimulationTypeMock& range);
};

struct UserSimulationTypeMockWithFB : public UserSimulationTypeMock
{
    static CONSTEXPR UserSimulationTypeMockWithFB max();
    static CONSTEXPR UserSimulationTypeMockWithFB min();
    static CONSTEXPR UserSimulationTypeMockWithFB zero();
    UserSimulationTypeMockWithFB() = default;
    CONSTEXPR UserSimulationTypeMockWithFB(long long a, double b) : UserSimulationTypeMock{a, b}{}
    UserSimulationTypeMockWithFB& forward(const UserSimulationTypeMockWithFB& rhs, const UserSimulationTypeMockWithFB& range);
    UserSimulationTypeMockWithFB& rewind(const UserSimulationTypeMockWithFB& rhs, const UserSimulationTypeMockWithFB& range);
};

inline CONSTEXPR UserSimulationTypeMock UserSimulationTypeMock::max()
{
    return UserSimulationTypeMock{
        std::numeric_limits<long long>::max(),
        std::numeric_limits<double>::max()
    };
}

inline CONSTEXPR UserSimulationTypeMock UserSimulationTypeMock::min()
{
    return UserSimulationTypeMock{
        std::numeric_limits<long long>::min(),
        std::numeric_limits<double>::min()
    };
}

inline CONSTEXPR UserSimulationTypeMock UserSimulationTypeMock::zero()
{
    return UserSimulationTypeMock{0, 0.0};
}

inline CONSTEXPR UserSimulationTypeMockWithFB UserSimulationTypeMockWithFB::max()
{
    return UserSimulationTypeMockWithFB{
        std::numeric_limits<long long>::max(),
        std::numeric_limits<double>::max()
    };
}

inline CONSTEXPR UserSimulationTypeMockWithFB UserSimulationTypeMockWithFB::min()
{
    return UserSimulationTypeMockWithFB{
        std::numeric_limits<long long>::min(),
        std::numeric_limits<double>::min()
    };
}

inline CONSTEXPR UserSimulationTypeMockWithFB UserSimulationTypeMockWithFB::zero()
{
    return UserSimulationTypeMockWithFB{0, 0.0};
}

inline UserSimulationTypeMock& 
UserSimulationTypeMock::operator=(long long a)
{
    this->a = a;
    return *this;
}

inline UserSimulationTypeMock& 
UserSimulationTypeMock::operator=(double b)
{
    this->b = b;
    return *this;
}

inline void
UserSimulationTypeMock::forward(const UserSimulationTypeMock& rhs, const UserSimulationTypeMock& range)
{
    this->a = 0; 
    this->b = 0.0; 
}

inline void
UserSimulationTypeMock::rewind(const UserSimulationTypeMock& rhs, const UserSimulationTypeMock& range)
{
    this->a = 0; 
    this->b = 0.0; 
}

inline UserSimulationTypeMockWithFB&
UserSimulationTypeMockWithFB::forward(const UserSimulationTypeMockWithFB& rhs, const UserSimulationTypeMockWithFB& range)
{
    this->a = 5; 
    this->b = 5.0; 
    return *this;
}

inline UserSimulationTypeMockWithFB&
UserSimulationTypeMockWithFB::rewind(const UserSimulationTypeMockWithFB& rhs, const UserSimulationTypeMockWithFB& range)
{
    this->a = 5; 
    this->b = 5.0; 
    return *this;
}

inline UserSimulationTypeMock operator+(const UserSimulationTypeMock& lhs, const UserSimulationTypeMock& rhs)
{
    return {lhs.a+rhs.a, lhs.b+rhs.b};
}

inline UserSimulationTypeMock operator+(const UserSimulationTypeMock& lhs, double rhs)
{
    return {lhs.a, lhs.b+rhs};
}

inline UserSimulationTypeMock operator+(const UserSimulationTypeMock& lhs, long long rhs)
{
    return {lhs.a+rhs, lhs.b};
}

inline UserSimulationTypeMock operator-(const UserSimulationTypeMock& lhs, const UserSimulationTypeMock& rhs)
{
    return {lhs.a-rhs.a, lhs.b-rhs.b};
}

inline UserSimulationTypeMock operator-(const UserSimulationTypeMock& lhs, double rhs)
{
    return {lhs.a, lhs.b-rhs};
}

inline UserSimulationTypeMock operator-(const UserSimulationTypeMock& lhs, long long rhs)
{
    return {lhs.a-rhs, lhs.b};
}

inline UserSimulationTypeMock operator%(const UserSimulationTypeMock& lhs, const UserSimulationTypeMock& rhs)
{
    return {lhs.a%rhs.a, std::fmod(lhs.b, rhs.b)};
}

inline UserSimulationTypeMock operator%(const UserSimulationTypeMock& lhs, double rhs)
{
    return {lhs.a, std::fmod(lhs.b, rhs)};
}

inline UserSimulationTypeMock operator%(const UserSimulationTypeMock& lhs, long long rhs)
{
    return {lhs.a%rhs, lhs.b};
}

inline UserSimulationTypeMockWithFB operator+(const UserSimulationTypeMockWithFB& lhs, const UserSimulationTypeMockWithFB& rhs)
{
    return {lhs.a+rhs.a, lhs.b+rhs.b};
}

inline UserSimulationTypeMockWithFB operator+(const UserSimulationTypeMockWithFB& lhs, double rhs)
{
    return {lhs.a, lhs.b+rhs};
}

inline UserSimulationTypeMockWithFB operator+(const UserSimulationTypeMockWithFB& lhs, long long rhs)
{
    return {lhs.a+rhs, lhs.b};
}

inline UserSimulationTypeMockWithFB operator-(const UserSimulationTypeMockWithFB& lhs, const UserSimulationTypeMockWithFB& rhs)
{
    return {lhs.a-rhs.a, lhs.b-rhs.b};
}

inline UserSimulationTypeMockWithFB operator-(const UserSimulationTypeMockWithFB& lhs, double rhs)
{
    return {lhs.a, lhs.b-rhs};
}

inline UserSimulationTypeMockWithFB operator-(const UserSimulationTypeMockWithFB& lhs, long long rhs)
{
    return {lhs.a-rhs, lhs.b};
}

inline UserSimulationTypeMockWithFB operator%(const UserSimulationTypeMockWithFB& lhs, const UserSimulationTypeMockWithFB& rhs)
{
    return {lhs.a%rhs.a, std::fmod(lhs.b, rhs.b)};
}

inline UserSimulationTypeMockWithFB operator%(const UserSimulationTypeMockWithFB& lhs, double rhs)
{
    return {lhs.a, std::fmod(lhs.b, rhs)};
}

inline UserSimulationTypeMockWithFB operator%(const UserSimulationTypeMockWithFB& lhs, long long rhs)
{
    return {lhs.a%rhs, lhs.b};
}

inline bool operator<(const UserSimulationTypeMock& lhs, const UserSimulationTypeMock& rhs)
{
    return lhs.a<rhs.a;
}

inline bool operator<(const UserSimulationTypeMock& lhs, double rhs)
{
    return lhs.b<rhs;
}

inline bool operator<(const UserSimulationTypeMock& lhs, long long rhs)
{
    return lhs.a<rhs;
}

inline bool operator==(const UserSimulationTypeMock& lhs, const UserSimulationTypeMock& rhs)
{
    return lhs.a==rhs.a  && lhs.b==rhs.b;
}

inline bool operator!=(const UserSimulationTypeMock& lhs, const UserSimulationTypeMock& rhs)
{
    return !(lhs == rhs);
}

inline bool operator==(const UserSimulationTypeMock& lhs, double rhs)
{
    return lhs.b==rhs;
}

bool operator==(const UserSimulationTypeMock& lhs, long long rhs)
{
    return lhs.a==rhs;
}

inline bool operator>(const UserSimulationTypeMock& lhs, const UserSimulationTypeMock& rhs)
{
    return lhs.a>rhs.a;
}

inline bool operator>(const UserSimulationTypeMock& lhs, double rhs)
{
    return lhs.b>rhs;
}

inline bool operator>(const UserSimulationTypeMock& lhs, long long rhs)
{
    return lhs.a>rhs;
}

inline std::ostream& operator<<(std::ostream& os, const UserSimulationTypeMock& obj)
{
    os << "{a: " << obj.a <<", b: "<<obj.b<<"}";
    return os;
}

#endif // _USERMOCKTYPE_HPP
