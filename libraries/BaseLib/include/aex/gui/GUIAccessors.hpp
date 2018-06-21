#ifndef GUIACCESSORS_GUI_HEADER
#define GUIACCESSORS_GUI_HEADER

#include <aex/tween/TweenAccessor.hpp>
#include <aex/gui/Component.hpp>

namespace aex
{
  namespace gui
  {
    class ComponentAccessor : public aex::tween::TweenAccessor<Component>
    {
    public:
      static const int POSITION_XY = 0;
      virtual void getValues(const Component* obj, int typeID, std::vector<float>& retValues);
      virtual void setValues(Component* obj, int typeID, const std::vector<float>& srcValues);
    };
  }
}

#endif
