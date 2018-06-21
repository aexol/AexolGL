/**************************************************************
 *                                                            *
 *      Filename:   cds_helpers.hpp                           *
 *                                                            *
 *   Description:                                             *
 *                                                            *
 *       Version:   1.0                                       *
 *       Created:   2017 maj 26 09:51:08                      *
 *                                                            *
 *        Author:   Kamil Matysiewicz                         *
 *          Mail:   kamil@aexol.com                           *
 *  Organization:   Aexol                                     *
 *                                                            *
 **************************************************************/
#ifndef CDS_HELPERS_HPP
#define CDS_HELPERS_HPP

#include <cds/algo/bit_reversal.h>
namespace aex {
    namespace _cds_details {
        // Workaround for error on type deduction
        // when size_t and uintN_t have the same size
        // but different integral type.
        template<size_t SIZE> struct cast_pick;
        template<> struct cast_pick<4> {
            using type = uint32_t;
        };
        template<> struct cast_pick<8> {
            using type = uint64_t;
        };
        struct lookup : public cds::algo::bit_reversal::lookup {
            using base = cds::algo::bit_reversal::lookup;
            uint32_t operator()( uint32_t x ) const
            {
                return base::operator()(x);
            }
            uint64_t operator()( uint64_t x ) const
            {
                return base::operator()(x);
            }
            template<class T>
            typename cast_pick<sizeof(T)>::type operator() ( T x ) const {
                static_assert(std::is_integral<T>::value && !std::is_signed<T>::value,
                        "T must be unsigned integer.");
                return (*this)(static_cast<typename cast_pick<sizeof(T)>::type>(x));
            }
        };
    }
}

#endif /* CDS_HELPERS_HPP */
