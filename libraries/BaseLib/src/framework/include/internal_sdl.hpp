/**************************************************************
 *                                                            * 
 *      Filename:   internal_sdl.hpp                          * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2016 lut 26 11:50:39                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#ifndef AEX_INTERNAL_SDL_HPP
#define AEX_INTERNAL_SDL_HPP 

// Suppress warning in SDL on MSVC.
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic warning  "-Wmacro-redefined"
#pragma clang diagnostic ignored  "-Wmacro-redefined"
#pragma clang diagnostic warning  "-Wunknown-pragmas"
#pragma clang diagnostic ignored  "-Wunknown-pragmas"
#elif defined(_MSC_VER)
#pragma warning(push, 0)
#endif // defined(__clang__)
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif // defined(__clang__)

#endif /* AEX_INTERNAL_SDL_HPP */
