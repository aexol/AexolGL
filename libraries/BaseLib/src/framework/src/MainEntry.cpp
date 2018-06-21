/**************************************************************
 *                                                            *
 *      Filename:   MainEntry.cpp                             *
 *                                                            *
 *   Description:                                             *
 *                                                            *
 *       Version:   1.0                                       *
 *       Created:   2017 maj 15 10:19:58                      *
 *                                                            *
 *        Author:   Kamil Matysiewicz                         *
 *          Mail:   kamil@aexol.com                           *
 *  Organization:   Aexol                                     *
 *                                                            *
 **************************************************************/
#include <aex/AexolGL.hpp>
#if __TARGET_PLATFORM__ == __APPLE_IOS__ || \
    __TARGET_PLATFORM__ == __AEX_WIN32__ || \
    __TARGET_PLATFORM__ == __ANDROID_OS__
#       define SDL_MAIN_NEEDED
#endif
#undef main
#ifdef SDL_MAIN_NEEDED
extern "C" int SDL_main(int argc, char* argv[]);
#define main SDL_main
#endif
#include "aex/cds_support.hpp"
#include "../graphics/api/include/GLContext.hpp"

int main(int argc, char* argv[]) {
    aex::cds_guard cg;
    cds::threading::Manager::attachThread();
    aex::GLContextGC gc;
    return AEXMain(argc, argv);
}
