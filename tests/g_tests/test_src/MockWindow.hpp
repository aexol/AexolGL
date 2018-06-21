/**************************************************************
 *                                                            * 
 *      Filename:   MockWindow.hpp                            * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 paź 06 15:33:46                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#ifndef MOCKWINDOW_HPP
#define MOCKWINDOW_HPP 
#include <cstddef>
#include <aex/AWindow.hpp>

class MockWindow : public aex::AWindow
{
    public:
        static MockWindow createWindow(std::size_t w, std::size_t h);
    protected:
        void makeWindow(std::size_t w, std::size_t h);
};

#endif /* MOCKWINDOW_HPP */
