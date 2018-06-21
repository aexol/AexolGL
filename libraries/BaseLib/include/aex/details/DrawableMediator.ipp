/**************************************************************
 *                                                            * 
 *      Filename:   DrawableMediator.ipp                      * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 wrz 30 11:52:25                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#ifndef DRAWABLEMEDIATOR_IPP
#define DRAWABLEMEDIATOR_IPP 

namespace aex
{
   template <class MediatorType>
   Drawable<MediatorType>::Drawable(mediator_type* mediator) : m_mediator(mediator)
   {} 

   template <class MediatorType>
   Drawable<MediatorType>::~Drawable()
   {}

   template <class MediatorType>
   void
   Drawable<MediatorType>::draw(AexRenderer* gl)
   {
       m_mediator->draw(gl);
   }

   template <class MediatorType>
   void
   Drawable<MediatorType>::finishdraw(AexRenderer* gl)
   {
       m_mediator->finishdraw(gl);
   }

   template <class MediatorType>
   void
   Drawable<MediatorType>::needsUpdate()
   {
       m_mediator->needsUpdate();
   }

   template <class MediatorType>
   Drawable<MediatorType>::operator bool()
   {
       return m_mediator != nullptr;
   }
} /* namespace aex */

#endif /* DRAWABLEMEDIATOR_IPP */
