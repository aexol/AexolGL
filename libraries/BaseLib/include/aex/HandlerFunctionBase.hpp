#ifndef HANDLERFUNCTIONBASE_H
#define HANDLERFUNCTIONBASE_H

#include <aex/GameEvent.hpp>

namespace aex
{

//!
//! Base class needed to group all inheriting
//! specialized function templates in one
//! container.
//!

    class AEX_PUBLIC_API HandlerFunctionBase
    {
    public:
        virtual ~HandlerFunctionBase() {}
        virtual bool isSameObject(void* object) = 0;
        void exec(const GameEvent* e)
        {
            call(e);
        }
    private:
        virtual void call(const GameEvent*) = 0;
    };
}

#endif
