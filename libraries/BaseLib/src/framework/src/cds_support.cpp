/**************************************************************
 *                                                            *
 *      Filename:   cds_support.cpp                           *
 *                                                            *
 *   Description:                                             *
 *                                                            *
 *       Version:   1.0                                       *
 *       Created:   2017 maj 16 10:04:33                      *
 *                                                            *
 *        Author:   Kamil Matysiewicz                         *
 *          Mail:   kamil@aexol.com                           *
 *  Organization:   Aexol                                     *
 *                                                            *
 **************************************************************/
#include <cds/init.h>
#include "aex/cds_support.hpp"

namespace aex{
    cds_init_guard::cds_init_guard() {
        cds::Initialize();
    }

    cds_init_guard::~cds_init_guard() {
        cds::Terminate();
    }

    cds_thread_guard::cds_thread_guard() {
        cds::threading::Manager::attachThread();
    }
    cds_thread_guard::~cds_thread_guard() {
        cds::threading::Manager::detachThread();
    }
}
