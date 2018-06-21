#ifndef PANEL_AEX_GUI_HEADER
#define PANEL_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "Component.hpp"

namespace aex {
    namespace gui {

        class AEX_PUBLIC_API Panel : public Component {
        private:
            struct CompPrior {

                CompPrior(ComponentPtr comp, int p) : component(comp), priority(p) {
                }
                ComponentPtr component;
                int priority;
            };
        public:
            friend class Frame;
            friend class Component;
            static const int DEFAULT_PRIORITY = 50;
 
            Panel();
            virtual ~Panel();
            virtual void addComponent(ComponentPtr component);
            virtual void removeComponent(ComponentPtr component);
            bool contains(ComponentPtr component) const;
            void clearKeyboard();
            void setPriority(ComponentPtr component, int priority);
            void setDebugRect(DEBUG_RECT type, bool visible);
            void setDebugColor(DEBUG_RECT type, const Color& color);
            void invalidate();

            // DrawObject Interface
            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual void needsUpdate();

            const std::vector<ComponentPtr>& getComponents() const;
            void modifyComponents(std::function<void(ComponentPtr) > action);
            void fitToContent();
            Area getFitContentArea() const;
            void clear();

        protected:
            virtual ComponentPtr setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe);
            void removeDrawPriorityComponent(ComponentPtr component);
            void putWithPriority(ComponentPtr component, int priority);

        protected:
            std::vector<ComponentPtr> m_components;
            std::vector<CompPrior> m_drawOrder;
            BoundingRectUPtr m_debugRect;
            std::map<DEBUG_RECT, bool> m_debugSettings;
            std::map<DEBUG_RECT, Color> m_debugColors;
        };
    }
}

#endif
