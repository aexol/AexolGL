/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <aex/LODManager.hpp>
#include <aex/AexMeshPart.hpp>
#include <aex/QuaternionCamera.hpp>
#include <algorithm>
namespace aex {

     AexMeshPair::AexMeshPair(AexMeshPart* caex, CombinedMesh* cmesh):aex(caex), mesh(cmesh)
     {
         
     }
    
    LODManager::LODManager() {

    }

    LODManager::~LODManager() {

    }

    void LODManager::update() {
        for (size_t ii = 0; ii < m_alexList.size(); ++ii) {
            aex::math::Vector3 distvec = m_alexList[ii].aex->getCenterPoint() - m_quatCamera->GetPosition();
            float distance = distvec.squaredLength();
            for (size_t d = 0; m_distanceList.size(); ++d) {
                if (m_distanceList[d] > distance) {
                    m_alexList[ii].aex->setPartToDraw(m_alexList[ii].mesh->getMeshInfo(d));
                } else {
                    break;
                }
            }

        }
    }

    void LODManager::addAex(AexMeshPart* newaex, CombinedMesh *mesh) {
        AexMeshPair newpair = AexMeshPair(newaex,mesh);
        m_alexList.push_back(newpair);
    }

    void LODManager::removeAex(AexMeshPart* newaex) {
        
        m_alexList.erase(std::remove_if(m_alexList.begin(), m_alexList.end(), [&newaex](AexMeshPair aexmesh){return aexmesh.aex == newaex;}), m_alexList.end());

    }

    void LODManager::addDistance(float distance) {
        m_distanceList.push_back(distance);

        if (m_distanceList.size() > 1) {
            std::sort(m_distanceList.begin(), m_distanceList.end());
        }
    }

}
