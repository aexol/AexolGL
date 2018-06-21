//
//  BasicEvents.h
//  AexolGL
//
//  Created by Kamil Matysiewicz on 06.11.2013.
//
//

#ifndef AexolGL_BasicEvents_h
#define AexolGL_BasicEvents_h

#include <aex/GameEvent.hpp>

namespace aex
{
    enum class DRAW_STATUS
    {
        DRAW,
        NO_DRAW
    };

    class FRUSTUM_CULL_STATUS_CHANGED : public GameEvent
    {
    public:
        FRUSTUM_CULL_STATUS_CHANGED(DRAW_STATUS newStatus) : m_status(newStatus) {}
        DRAW_STATUS getStatus()
        {
            return m_status;
        }
    private:
        DRAW_STATUS m_status;
    };

    class AEX_PUBLIC_API NOT_DRAWN : public GameEvent
    {

    };

    class AEX_PUBLIC_API DRAWN : public GameEvent
    {

    };

    class AEX_PUBLIC_API FRAME_DONE : public GameEvent
    {

    };

}

#endif
