//
//  Instance.h
//  aexolGL
//
//  Created by Kamil Matysiewicz on 30.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#ifndef __aexolGL__Instance__
#define __aexolGL__Instance__

#include <aex/Vector3.hpp>
#include <aex/Quaternion.hpp>

namespace aex
{
    class ASceneNode;    
    template<class T> class TAex;
    template<class T>
    class AEX_PUBLIC_API TInstance : public T
    {
    private:
        typedef aex::math::Vector3      Vector3;
        typedef aex::math::Quaternion   Quaternion;
        static_assert(std::is_convertible<T*, ASceneNode*>::value, "Invalid template parameter.");
        TAex<T>* m_parentAex;
    public:

        /**
         * @brief Create Instance without any parent aex.
         */
        TInstance();

        /**
         * @brief Create instance assigned to aex.
         *
         * Constructor will copy aex current position for itself.
         *
         * @param TAex<T> Parent aex.
         */
        TInstance(TAex<T>& aex);

        /**
         * @brief Set new parent aex for this instance object.
         */
        void setParent(TAex<T>* aex);

        /**
         * @brief Break binding with aex.
         */
        void reset(TAex<T>* parent = nullptr);

        /**
         * @brief Destructor.
         *
         * Will clean after aex<->instance relationship.
         */
        ~TInstance();
    };
}
#include <aex/Instance.ipp>
#endif /* defined(__aexolGL__Instance__) */
