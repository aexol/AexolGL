/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LODManager.hpp
 * Author: rafa
 *
 * Created on June 13, 2016, 4:26 PM
 */

#ifndef LODMANAGER_HPP
#define LODMANAGER_HPP

#include <aex/Common.hpp>

namespace aex {

    class QuaternionCamera;
    class Aex;
    class AexMeshPart;
    class CombinedMesh;
    
    struct AexMeshPair
    {
        
        AexMeshPart* aex;
        CombinedMesh* mesh;
        AexMeshPair(AexMeshPart* caex, CombinedMesh* cmesh);

    };
    
    class AEX_PUBLIC_API LODManager 
    {
    private:
        QuaternionCamera* m_quatCamera;
        std::vector<AexMeshPair> m_alexList;
        std::vector<float> m_distanceList;
        
    protected:

    public:
        
        LODManager();
        virtual ~LODManager();
        
        void update();
        
        void addAex(AexMeshPart* newaex, CombinedMesh* mesh);
        void removeAex(AexMeshPart* newaex);
        
        void addDistance(float distance);
        
        
        
    };


}

#endif /* LODMANAGER_HPP */

