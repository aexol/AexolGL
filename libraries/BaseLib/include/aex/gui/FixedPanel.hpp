#ifndef FIXED_PANEL_AEX_GUI_HEADER
#define FIXED_PANEL_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "Panel.hpp"

namespace aex {
    namespace gui {
        /**
         * @brief Panel in fixed position.
         */
        class AEX_PUBLIC_API FixedPanel : public Panel {
        public:
            FixedPanel();
            FixedPanel(const Area& area);
            virtual ~FixedPanel();

            /**
             * @brief Add new component to panel.
             *
             * @details Add new component to the panel. Component is
             * in position [0, 0] relative to the the panel inner area.
             *
             * @param component New component.
             */
            virtual void addComponent(ComponentPtr component);

            /**
             * @brief Remove component from panel.
             *
             * @param component Component to remove.
             */
            virtual void removeComponent(ComponentPtr component);

            /**
             * @brief Add new component to panel.
             *
             * @details Add new component to the panel. Component 
             * position is relative to the the panel inner area.
             *
             * @param component New component.
             * @param relativePosition New position.
             */
            virtual void addComponent(ComponentPtr component,
                    const Position& relativePosition);

            /**
             * @brief Add or move component.
             *
             * @details If component is already in the panel move it's position.
             * Otherwise add this component to the panel.
             *
             * @param component Component.
             * @param relativePosition New position.
             */
            void set(ComponentPtr component, const Position& relativePosition);

            /**
             * @brief Add new component to panel.
             *
             * @details Add new component to the panel. Component 
             * position is relative to the pivot point.
             *
             * @param component New component.
             * @param relativePosition New position.
             * @param pivot Pivot point.
             */
            virtual void addComponent(ComponentPtr component,
                    const Position& relativePosition,
                    const Pivot& pivot);

            /**
             * @brief Add or move component.
             *
             * @details If component is already in the panel move it's position.
             * Otherwise add this component to the panel. Relative to pivot point.
             *
             * @param component Component.
             * @param relativePosition New position.
             * @param pivot Pivot point.
             */
            void set(ComponentPtr component,
                    const Position& relativePosition,
                    const Pivot& pivot);
        protected:
            virtual void refresh(int flags);
            Position getPosDeltaFor(ComponentPtr component, const Pivot& pivot) const;
            std::map<ComponentPtr, Position> m_positions;
        };
    }
}

#endif
