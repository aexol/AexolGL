/**************************************************************
 *                                                            * 
 *      Filename:   endian.cpp                                * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 wrz 24 16:21:24                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#include <iostream>

enum class endian_t{
    e_LITTLE_ENDIAN,
    e_BIG_ENDIAN,
    e_MID_ENDIAN,
    e_UNKNOWN
};
struct Endian{
    private:
        static CONSTEXPR const uint32_t _value = 0x01020304;
        static CONSTEXPR const uint8_t& _first = static_cast<const uint8_t&>(_value);
    public:
        static CONSTEXPR endian_t get_endianess()
        {
            return _first == 0x01 ? endian_t::e_BIG_ENDIAN : _first == 0x04 ? endian_t::e_LITTLE_ENDIAN : _first == 0x02 ? endian_t::e_MID_ENDIAN : endian_t::e_UNKNOWN;
        }
};
static CONSTEXPR endian_t value = Endian::get_endianess();
static CONSTEXPR const char* _big_endian = "BIG_ENDIAN";
static CONSTEXPR const char* _little_endian = "LITTLE_ENDIAN";
static CONSTEXPR const char* _mid_endian = "MID_ENDIAN";
static CONSTEXPR const char* _unknown_endian = "UNKNOWN_ENDIAN";

template<endian_t e = value> CONSTEXPR const char* print_endianness()
{
    return _unknown_endian;
}

template<> CONSTEXPR const char* print_endianness<endian_t::e_BIG_ENDIAN>()
{
    return _big_endian;
}

template<> CONSTEXPR const char* print_endianness<endian_t::e_LITTLE_ENDIAN>()
{
    return _little_endian;
}

template<> CONSTEXPR const char* print_endianness<endian_t::e_MID_ENDIAN>()
{
    return _mid_endian;
}

int main(int argc, char **argv)
{
    std::cout<< "Is big endian? " << print_endianness() << std::endl;
    return 0;
}
