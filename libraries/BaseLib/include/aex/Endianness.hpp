/**************************************************************
 *                                                            * 
 *      Filename:   Endianness.hpp                            * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 wrz 29 14:38:33                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#ifndef ENDIANNESS_HPP
#define ENDIANNESS_HPP 

namespace aex
{
    enum class endian_t{
        e_LITTLE_ENDIAN,
        e_BIG_ENDIAN,
        e_MID_ENDIAN,
        e_UNKNOWN
    };
    struct AEX_PUBLIC_API Endian{
        private:
            static CONSTEXPR const uint32_t _value = 0x01020304;
            static CONSTEXPR const uint8_t& _first = static_cast<const uint8_t&>(_value);
        public:
            static CONSTEXPR endian_t get_endianess()
            {
                return _first == 0x01 ? endian_t::e_BIG_ENDIAN : _first == 0x04 ? endian_t::e_LITTLE_ENDIAN : _first == 0x02 ? endian_t::e_MID_ENDIAN : endian_t::e_UNKNOWN;
            }
    };
    static CONSTEXPR endian_t endianness = Endian::get_endianess();
} /* namespace aex */

#endif /* ENDIANNESS_HPP */
