#ifndef FLAGS_AEX_GUI_HEADER
#define FLAGS_AEX_GUI_HEADER

namespace aex {
    namespace gui {
        namespace flags {
            /**
             * Component state flags.
             */
            enum FLAG {
                // Area
                POSITION_CHANGED            = 1 << 0,
                SIZE_CHANGED                = 1 << 1,
                HITBOX_CHANGED              = 1 << 2,
                MARGIN_CHANGED              = 1 << 3,
                PADDING_CHANGED             = 1 << 4,

                // Text
                TEXT_CHANGED                = 1 << 5,
                LINE_WRAP_CHANGED           = 1 << 6,
                FONT_SIZE_CHANGED           = 1 << 7,
                KERNING_CHANGED             = 1 << 8,
                ALIGNING_CHANGED            = 1 << 9,
                FIT_TO_CONTENT_CHANGED      = 1 << 10,
                DISCARD_TRUNCATED_CHANGED   = 1 << 11,
                DISCARD_OUTED_CHANGED       = 1 << 12,
                REBUILD_TEXT                = 1 << 13,
                COLOR_CHANGED               = 1 << 14
            };

            void clearFlags(int* flags);
            void setFlag(int flag, int* inFlags);
            bool checkFlag(int flag, int inFlags);
            bool checkFlag(FLAG flag, int inFlags);
        }
    }
}

#endif
