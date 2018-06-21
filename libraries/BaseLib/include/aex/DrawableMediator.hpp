/**************************************************************
 *                                                            * 
 *      Filename:   DrawableMediator.hpp                      * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 wrz 30 11:47:49                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#ifndef DRAWABLEMEDIATOR_HPP
#define DRAWABLEMEDIATOR_HPP 

#include <aex/AexDrawable.hpp>
namespace aex
{
    class AEX_PUBLIC_API AexRenderer;
    template<class MediatorType>
    class Drawable : public DrawObject
    {
        public:
            using mediator_type = MediatorType;
            explicit Drawable (mediator_type* med = nullptr);
            virtual ~Drawable ();

            void draw(AexRenderer* gl);
            void finishdraw(AexRenderer* gl);
            void needsUpdate();
            operator bool();
    
        private:
            mediator_type* m_mediator;
    };
    
    template <class DrawableType>
    class DrawableMediator
    {
        public:
            using drawable_type = DrawableType;
            DrawableMediator() : m_drawInterface(this)
            {}
            explicit DrawableMediator(const drawable_type& drawable) : DrawableMediator()
            {
                *this = drawable;
            }
            
            DrawableMediator& operator=(const drawable_type& drawable)
            {
                if(&m_drawable != &drawable)
                {
                    m_drawable = drawable;
                }
                return *this;
            }
            
            explicit DrawableMediator(drawable_type&& drawable) : DrawableMediator()
            {
                *this = std::move(drawable);
            }
            
            DrawableMediator& operator=(drawable_type&& drawable)
            {
                if(&m_drawable != &drawable)
                {
                    m_drawable = std::move(drawable);
                }
                return *this;
            }

            DrawObject& getDrawable()
            {
                return m_drawInterface;
            }

            drawable_type& getDrawableImplementation()
            {
                return m_drawable;
            }
        private:
            friend Drawable<DrawableMediator>;

            void draw(AexRenderer* gl)
            {
                m_drawable.draw(gl);
            }
            void finishdraw(AexRenderer* gl)
            {
                m_drawable.finishdraw(gl);
            }
            void needsUpdate()
            {
                m_drawable.needsUpdate();
            }
    
            Drawable<DrawableMediator> m_drawInterface;
            drawable_type m_drawable;
    };
} /* namespace aex */

#include "details/DrawableMediator.ipp"
#endif /* DRAWABLEMEDIATOR_HPP */
