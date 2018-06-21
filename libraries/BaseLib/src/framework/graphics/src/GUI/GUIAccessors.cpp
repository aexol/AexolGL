#include "aex/gui/GUIAccessors.hpp"

namespace aex
{
  namespace gui
  {
    void ComponentAccessor::getValues(const Component* obj, int typeID, std::vector<float>& retValues)
    {
      switch (typeID) {
          case POSITION_XY:
              retValues[0] = obj->getArea().p().x();
              retValues[1] = obj->getArea().p().y();
              break;
          default:
              break;
      }
    }

    void ComponentAccessor::setValues(Component* obj, int typeID, const std::vector<float>& srcValues)
    {
      switch (typeID) {
          case POSITION_XY:
              obj->setPosition(Position(srcValues[0], srcValues[1]));
              break;
          default:
              break;
      }
    }
  }
}
