#ifndef TWEEN_ACCESSOR_AEX_TWEEN_HEADER
#define TWEEN_ACCESSOR_AEX_TWEEN_HEADER

#include <vector>

namespace aex {
    namespace tween {
        template <typename T> class TweenAccessor {
            public:
                TweenAccessor() {}
                virtual ~TweenAccessor() {}

                virtual void getValues(const T* obj, int typeID, std::vector<float>& retValues) = 0;
                virtual void setValues(T* obj, int typeID, const std::vector<float>& srcValues) = 0;
        };
    }
}


#endif
