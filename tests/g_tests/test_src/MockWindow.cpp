/**************************************************************
 *                                                            * 
 *      Filename:   MockWindow.cpp                            * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 paź 06 15:34:36                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#include "MockWindow.hpp"
#include <aex/GPUApi.hpp>
#include <aex/Exceptions.hpp>


MockWindow
MockWindow::createWindow(std::size_t w, std::size_t h)
{
    MockWindow window;
    AWindow::initWindow(window, 0, 0);
    return window;
}
void 
MockWindow::makeWindow(std::size_t w, std::size_t h)
{
    try{
        InitAexContext(*this, nullptr);
    } catch (const aex::ContextError& e)
    {
    }
}
