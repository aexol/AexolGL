//
//  Aex.h
//  aexolGL
//
//  Created by Kamil Matysiewicz on 25.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#ifndef __aexolGL__AexMeshPart__
#define __aexolGL__AexMeshPart__
#include <aex/Aex.hpp>
#include <aex/ASceneNode.hpp>
#include <aex/Mesh.hpp>
#include <atomic>
#include <vector>
#include <memory>
#include <string>
#include <list>


namespace aex
{
    class AEX_PUBLIC_API AexMeshPart : public Aex
    {
        public:
            typedef aex::math::Vector3 PositionType;
            typedef aex::math::Quaternion RotationType;
            typedef aex::math::Vector3 ScaleType;
            typedef aex::shared_ptr<Uniform> Uniform_ptr;
            /**
             *	@brief	Default Ctor
             */
            AexMeshPart();

            /**
             * @brief Constructor.
             *
             * This constructor takes parent scene node as paremeter. All transformations of this aex will be relative to parent.
             *
             * @param ASceneNode* Parent node.
             */
            AexMeshPart(ASceneNode* parentAex);

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
            AexMeshPart(PositionType position, RotationType rotation, ScaleType scale, ASceneNode* parentAex = nullptr);

            /**
             * @brief Draw aex.
             */
            void draw(AexRenderer *gl);
            void finishdraw(AexRenderer *gl);
            
            void setPartToDraw(MeshPartInfo partToDraw);
            MeshPartInfo getPartToDraw() const;

            void setPartOffset(int offset);
            void setPartSize(int size);
            
            std::vector<float>& getDistanceList();
            void addDistance(float distance);
            
            /**
             * @brief Get unique ID.
             *
             * @returns uint32_t Unique ID.
             */

        private:
            MeshPartInfo m_partToDraw;
            std::vector<float> m_distanceList;
            
    };

} // end of namespace aex.tt

#endif /* defined(__aexolGL__Aex__) */
