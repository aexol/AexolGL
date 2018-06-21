/**************************************************************
 *                                                            * 
 *      Filename:   MockObjectGuard.hpp                       * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 paź 06 15:20:33                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#ifndef MOCKOBJECTGUARD_HPP
#define MOCKOBJECTGUARD_HPP 

template<class T, class U>
class MockObjectGuard
{
    public:
        MockObjectGuard()
        {
            if(T::_instance == nullptr)
                T::_instance = &u;
        }
        ~MockObjectGuard()
        {
            if(T::_instance == &u)
                T::_instance = nullptr;
        }
    private:
        U u;
};

#endif /* MOCKOBJECTGUARD_HPP */
