#ifndef _AEX_COORDINATE_SPACES_HPP
#define _AEX_COORDINATE_SPACES_HPP

namespace aex
{
    class CoordinateSpaceBase
    {
        public:
            ~CoordinateSpaceBase();
        private:
            void(*SetCoordinateSpace)();
            void(*Destruct)();
    }
    class CoordinateSpace
}

#endif // _AEX_COORDINATE_SPACES_HPP
