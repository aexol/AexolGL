//
//  Aex.h
//  aexolGL
//
//  Created by Kamil Matysiewicz on 25.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#ifndef __aexolGL__Aex__
#define __aexolGL__Aex__

#include <aex/ASceneNode.hpp>
#include <aex/Vector3.hpp>
#include <aex/Quaternion.hpp>
#include <aex/Uniform.hpp>
#include <aex/TAex.hpp>
#include <aex/AABB.hpp>
#include <atomic>
#include <vector>
#include <memory>
#include <string>
#include <list>


namespace aex
{
    class AEX_PUBLIC_API Aex : public TAex<TRSNode>
    {
        public:
            typedef aex::math::Vector3 PositionType;
            typedef aex::math::Quaternion RotationType;
            typedef aex::math::Vector3 ScaleType;
            typedef aex::shared_ptr<Uniform> Uniform_ptr;
            /**
             *	@brief	Default Ctor
             */
            Aex();

            /**
             * @brief Constructor.
             *
             * This constructor takes parent scene node as paremeter. All transformations of this aex will be relative to parent.
             *
             * @param ASceneNode* Parent node.
             */
            Aex(ASceneNode* parentAex);

            /**
             * @brief Constructor.
             *
             * This constructor sets initial position, rotation and scale. If parent scene node is provided all transformations of this aex will be relative to parent.
             *
             * @param PositionType Initial position.
             * @param RotationType Initial rotation.
             * @param ScaleType Initial scale.
             * @param ASceneNode* Parent node.
             */
            Aex(PositionType position, RotationType rotation, ScaleType scale, ASceneNode* parentAex = nullptr);

            /**
             * @brief Draw aex.
             */
            virtual void draw(AexRenderer *gl);
            virtual void finishdraw(AexRenderer* gl);
            /**
             * @brief Get unique ID.
             *
             * @returns uint32_t Unique ID.
             */
            uint32_t getAexId();
        protected:
            UniformFloat3 m_posUniform;
    };

} // end of namespace aex.

#endif /* defined(__aexolGL__Aex__) */
