/**************************************************************
 *                                                            * 
 *      Filename:   KeyboardEvent.hpp                         * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2016 lut 19 14:58:43                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#ifndef KEYBOARDEVENT_HPP
#define KEYBOARDEVENT_HPP 

#include <aex/Common.hpp>
#include "../include/internal_sdl.hpp"
#include <aex/KeyboardEvent.hpp>

namespace aex
{

    AEX_PUBLIC_API uint32_t extractScancode(SDL_Event* e);
    AEX_PUBLIC_API NKeyData extractFromEvent(SDL_Event* e);
}

#endif /* KEYBOARDEVENT_HPP */
