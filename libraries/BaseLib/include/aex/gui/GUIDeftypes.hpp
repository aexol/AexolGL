#ifndef GUIDEFTYPES_AEX_GUI_HEADER
#define GUIDEFTYPES_AEX_GUI_HEADER

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <cmath>
#include <chrono>
#include <limits>
#include <algorithm>

#include <aex/Common.hpp>

#define RAD(stop)  ((stop*M_PI)/180.0f)
#define STRINGIFY(A)  #A /*// if u wanna macro he
re use \n before it. eg: \n#define MACRO 123*/

namespace aex {
    namespace gui {

        template <typename T> class GPosition2D;
        using Position = GPosition2D<float>;

        template <typename T> class GPosition2D;
        using Pivot = GPosition2D<float>;

        template <typename T> class GSize2D;
        using Size = GSize2D<float>;

        class Scale;

        template <typename T> class GArea2D;
        using Area = GArea2D<float>;

        template <typename T> class GInterspace2D;
        using Interspace = GInterspace2D<float>;

        class Frame;
        class Color;
        class Component;
        class Panel;
        class SidePanel;
        class FixedPanel;
        class SimplePanel;
        class RootPanel;
        class StretchPanel;
        class LabeledComponent;
        class ScrollView;
        class FixedLabel;
        class HudComponent;
        class ColorRectComponent;
        class BoundingRect;
        class TriangleBatch;


        typedef Component* ComponentPtr;
        typedef std::unique_ptr<Component> ComponentUPtr;
        typedef Panel* PanelPtr;
        typedef aex::unique_ptr<Panel> PanelUPtr;
        typedef ScrollView* ScrollViewPtr;
        typedef aex::unique_ptr<ScrollView> ScrollViewUPtr;
        typedef SidePanel* SidePanelPtr;
        typedef aex::unique_ptr<SidePanel> SidePanelUPtr;
        typedef HudComponent* HudComponentPtr;
        typedef aex::unique_ptr<HudComponent> HudComponentUPtr;
        typedef ColorRectComponent* ColorRectComponentPtr;
        typedef aex::unique_ptr<ColorRectComponent> ColorRectComponentUPtr;
        typedef BoundingRect* BoundingRectPtr;
        typedef aex::unique_ptr<BoundingRect> BoundingRectUPtr;
        typedef FixedLabel* FixedLabelPtr;
        typedef aex::unique_ptr<FixedLabel> FixedLabelUPtr;
        typedef LabeledComponent* LabeledComponentPtr;
        typedef aex::unique_ptr<LabeledComponent> LabeledComponentUPtr;
    }
}

#endif
