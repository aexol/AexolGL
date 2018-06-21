/**************************************************************
 *                                                            *
 *      Filename:   cds_support.hpp                           *
 *                                                            *
 *   Description:                                             *
 *                                                            *
 *       Version:   1.0                                       *
 *       Created:   2017 maj 16 10:03:46                      *
 *                                                            *
 *        Author:   Kamil Matysiewicz                         *
 *          Mail:   kamil@aexol.com                           *
 *  Organization:   Aexol                                     *
 *                                                            *
 **************************************************************/
#ifndef CDS_SUPPORT_HPP
#define CDS_SUPPORT_HPP
#include <cds/gc/hp.h>

namespace aex {
    class cds_init_guard {
        public:
            cds_init_guard();
            ~cds_init_guard();
    };
    class cds_guard {
        private:
            cds_init_guard m_guard;
            cds::gc::HP hpGC;
    };
    class cds_thread_guard{
        public:
            cds_thread_guard();
            ~cds_thread_guard();
    };
}

#endif /* CDS_SUPPORT_HPP */
