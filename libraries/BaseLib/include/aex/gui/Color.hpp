#ifndef COLOR_AEX_GUI_HEADER
#define COLOR_AEX_GUI_HEADER

#include "GUIMath.hpp"

namespace aex {
    namespace gui {

        /**
         * @brief Representation of color for GUI,
         */
        class AEX_PUBLIC_API Color {
        public:
            static const int RANGE_CLAMPF = 0;
            static const int RANGE_0_255 = 1;

            /**
             * @brief New color. Black.
             */
            Color();

            /**
             * @brief New color.
             *
             * @details RGBA color with 0-255 ranges.
             *
             * @param r Red 
             * @param g Green
             * @param b Blue
             * @param a Alpha
             */
            Color(int r, int g, int b, int a = 255);
            virtual ~Color();

            /**
             * @brief Normalized red.
             *
             * @return Red.
             */
            float getClampfR() const;

            /**
             * @brief Normalized green.
             *
             * @return Green.
             */
            float getClampfG() const;

            /**
             * @brief Normalized blue.
             *
             * @return Blue.
             */
            float getClampfB() const;

            /**
             * @brief Normalized alpha.
             *
             * @return Alpha.
             */
            float getClampfA() const;

            /**
             * @brief Red in 0-255 range.
             *
             * @return Red.
             */
            int getR() const;

            /**
             * @brief Green in 0-255 range.
             *
             * @return Green.
             */
            int getG() const;

            /**
             * @brief Blue in 0-255 range.
             *
             * @return Blue.
             */
            int getB() const;

            /**
             * @brief Alpha in 0-255 range.
             *
             * @return Alpha.
             */
            int getA() const;

            /**
             * @brief Set red channel.
             *
             * @param value 0-255 red channel.
             */
            void setR(int value);

            /**
             * @brief Set green channel.
             *
             * @param value 0-255 green channel.
             */
            void setG(int value);

            /**
             * @brief Set blue channel.
             *
             * @param value 0-255 blue channel.
             */
            void setB(int value);

            /**
             * @brief Set alpha channel.
             *
             * @param value 0-255 alpha channel.
             */
            void setA(int value);

            /**
             * @brief Set red channel.
             *
             * @param value Normalized red.
             */
            void getClampfR(float value);

            /**
             * @brief Set green channel.
             *
             * @param value Normalized green.
             */
            void getClampfG(float value);

            /**
             * @brief Set blue channel.
             *
             * @param value Normalized blue.
             */
            void getClampfB(float value);

            /**
             * @brief Set alpha channel.
             *
             * @param value Normalized alpha.
             */
            void getClampfA(float value);
        protected:
            float m_r, m_g, m_b, m_a;
        };

        bool operator == (const Color& lhs, const Color& rhs);
        bool operator != (const Color& lhs, const Color& rhs);
    }
}

#endif
