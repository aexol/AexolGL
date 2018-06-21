#ifndef _AEX_IONVIEWPROJECTIONUPDATE_HPP
#define _AEX_IONVIEWPROJECTIONUPDATE_HPP

#include <aex/Common.hpp>

namespace aex
{

    class AEX_PUBLIC_API IOnAexRendererUpdate{
            friend class AexRenderer;
        public:
            IOnAexRendererUpdate();
            IOnAexRendererUpdate(const IOnAexRendererUpdate&);
            IOnAexRendererUpdate& operator=(const IOnAexRendererUpdate&);
            IOnAexRendererUpdate(AexRenderer* gl);
            IOnAexRendererUpdate& operator=(AexRenderer*);
            void setAexRenderer(AexRenderer* gl);
            virtual ~IOnAexRendererUpdate();
        protected:
            virtual void onViewProjectionUpdate() = 0;
            virtual void onLightsUpdate() = 0;
            AexRenderer* gl() const;
        private:
            void reset(AexRenderer* gl = nullptr);
            AexRenderer* m_gl;
        friend AEX_PUBLIC_API bool operator==(const IOnAexRendererUpdate& lhs, AexRenderer* rhs);
    };
    AEX_PUBLIC_API bool operator==(const IOnAexRendererUpdate& lhs, AexRenderer* rhs);
	AEX_PUBLIC_API bool operator!=(const IOnAexRendererUpdate& lhs, AexRenderer* rhs);
}

#endif
