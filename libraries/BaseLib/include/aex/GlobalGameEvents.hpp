/** @file GlobalGameEvents.hpp
 *  @brief Definition of GlobalGameEvents.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef AEXOLGL_GLOBALGAMEEVENTS
#define AEXOLGL_GLOBALGAMEEVENTS

#include <aex/EventHandler.hpp>
#include <aex/Common.hpp>

namespace aex
{

    /**
     *	@brief	Just a class to make life cleaner.
     *  @details This class doesn't really add anything to EventHandler, it's just made for the purpose of making "local" and "gloabl" communication more transparent.
     *  @note This one should be used for global.
     */
    class AEX_PUBLIC_API GlobalGameEvents : public EventHandler
    {
    public:
        typedef EventHandler::entry_reverse_iterator entry_reverse_iterator;
        /**
         *	@brief	Destructor.
         */
        virtual ~GlobalGameEvents();
    };

    /**
     *	@brief	Pointer to a GlobalGameEvents.
     */
    typedef GlobalGameEvents* GlobalGameEventsPtr;

    typedef aex::shared_ptr<GlobalGameEvents> GlobalGameEvents_ptr;
}

#endif
