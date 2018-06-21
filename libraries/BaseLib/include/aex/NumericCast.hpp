/**************************************************************
 *                                                            * 
 *      Filename:   NumericCast.hpp                           * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2017 maj 12 15:11:13                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#ifndef NUMERICCAST_HPP
#define NUMERICCAST_HPP 
#include <boost/numeric/conversion/cast.hpp>

namespace aex {
    using boost::numeric_cast;
    using boost::numeric::bad_numeric_cast;
    using boost::numeric::positive_overflow;
    using boost::numeric::negative_overflow;
}

#endif /* NUMERICCAST_HPP */
