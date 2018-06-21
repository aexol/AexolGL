#include <aex/MeshImporter.hpp>
#include <aex/Aex.hpp>
#include <aex/QuaternionCamera.hpp>
#include <aex/Logger.hpp>
#include <aex/Exceptions.hpp>
#include <aex/Math.hpp>
#include <aex/Vector3.hpp>
#include <aex/Quaternion.hpp>
#include <map>

#ifdef USING_ASSIMP

namespace aex
{
    /******************TransformationImporter********************/
    

    TransformationImporter::TransformationImporter(const ImporterBase& ib) : ImporterBase(ib)
    {
    }

    TransformationImporter::TransformationImporter(const aex::string& fileName)
    {
      m_isValid = prepare(fileName);
    }

    /*int
    TransformationImporter::importTransformation(Aex* aex)
    {
    
    }*/


    
    
    void aiTransformToAex(aiMatrix4x4 aiTransform, Aex* aex)
    {
        aiVector3D scaling, position;
        aiQuaternion orientation;
        
        aiTransform.Decompose(scaling, orientation, position);
        
        aex::math::Vector3 aexScaling = aex::math::Vector3(scaling.x, scaling.y, scaling.z);
        aex::math::Vector3 aexPosition = aex::math::Vector3(position.x, position.y, position.z);
        aex::math::Quaternion aexOrientation = aex::math::Quaternion(orientation.x, orientation.y, orientation.z, orientation.w);
         
         
        aex->setPosition(aexPosition);
        aex->setOrientation(aexOrientation);
        aex->setSize(aexScaling);
        
       
        
    }
    
    void aiTransformToAexNode(aiMatrix4x4 aiTransform, AexNode& aexNode)
    {
        aiVector3D scaling, position;
        aiQuaternion orientation;
        
        aiTransform.Decompose(scaling, orientation, position);
        
        aexNode.scale = aex::math::Vector3(scaling.x, scaling.y, scaling.z);
        aexNode.position = aex::math::Vector3(position.x, position.y, position.z);
        aexNode.orientation  = aex::math::Quaternion(orientation.x, orientation.y, orientation.z, orientation.w);
    }
    
    void aiNodesToAexNodes(aiNode* assimpNode, std::vector<AexNode>& nodes, int index)
    {
        
        nodes.push_back(AexNode());
        
        AexNode& aexNode = nodes.back();
        aiTransformToAexNode(assimpNode->mTransformation, aexNode);
        aexNode.nodename=aex::string(assimpNode->mName.C_Str());;
        //LOG_DEBUG("nodes.size(): ", nodes.size());
        aexNode.nodeNumber=nodes.size()-1;
        if(index!=-1)
        {
            aexNode.parentInt=index;
            //LOG_DEBUG(index, " : ",aexNode.nodeNumber);
            nodes[index].childrenInt.push_back(aexNode.nodeNumber);
        }
        
        for (uint i = 0; i < assimpNode->mNumMeshes; ++i)
        {
            aexNode.meshlist.push_back(assimpNode->mMeshes[i]);
            
        }
        
        
        if(assimpNode->mNumChildren>0)
        {
            int back = aexNode.nodeNumber;
            //LOG_DEBUG(aexNode.nodeNumber,": assimpNode->mNumChildren: ",assimpNode->mNumChildren);
            for(uint i=0; i<assimpNode->mNumChildren ;++i)
            {
                aiNodesToAexNodes(assimpNode->mChildren[i],  nodes, back);
            }
        }
        else
        {
            aexNode.isLeaf=true;
        }
    }
    
    std::vector<AexNode> getAexNodes(aiNode* assimpNode ,const aiScene* scene)
    {
        std::vector<AexNode> ret;
        aiNodesToAexNodes(assimpNode,ret,-1);
        
        for(size_t i = 0; i< ret.size();++i)
        {
            for(size_t m = 0; m <ret[i].meshlist.size(); ++m)
            {
                uint index = scene->mMeshes[ret[i].meshlist[m]]->mMaterialIndex;
                ret[i].materialList.push_back(index);
                
                if(index<scene->mNumMaterials)
                {
                    aiMaterial* mat = scene->mMaterials[index];
                    aiString name;
                    if(AI_SUCCESS != aiGetMaterialString(mat,AI_MATKEY_NAME,&name)) {
                        LOG_ERR("Material has no name!");
                    }
                    else
                    {
                        std::string aexName = std::string(name.C_Str());
                        ret[i].materialNameList.push_back(aexName);
                    }
                }
                //materialNameList
            }
        }
        
        return ret;
    }
    
    std::vector<AexNode> TransformationImporter::getNodes(const aex::string& fileName)
    {
        const aiScene* scene = getSceneFromBase(this, fileName);
        
        return getAexNodes(scene->mRootNode, scene);
    }
    
    std::vector<AexNode> TransformationImporter::getNodes( )
    {
        const aiScene* scene = getSceneFromBase(this);
        
        return getAexNodes(scene->mRootNode, scene);
    }
    

    AexMesh::AexMesh(aex::Aex_ptr caex, aex::Mesh_ptr cmesh ): aex(caex), mesh(cmesh) 
    {
             
    }
    
    AexMesh::AexMesh(aex::Aex_ptr caex, aex::Mesh_ptr cmesh, std::vector<std::string> cmaterialNameList): 
    aex(caex),
    mesh(cmesh),
    materialNameList(cmaterialNameList)
    {
             
    }
  
    
    
    std::map<aex::string , std::vector<AexMesh>> 
    TransformationImporter::importMap(const aex::string& fileName)
    {
        
       std::map<aex::string ,  std::vector<AexMesh>> ret;
        
       aex::MeshImporter  mimporter;
       aex::TransformationImporter timporter;
       
       std::vector<aex::Mesh> meshes;
       
       mimporter.importAll(meshes,fileName);
       std::vector<aex::AexNode> nodes = timporter.getNodes(fileName);
       std::vector<aex::Aex_ptr> aexList;
              
       
       for(size_t i=0 ; i<nodes.size(); ++i)
       {
           aex::AexNode& node  =  nodes[i];
           aex::Aex* parent = nullptr;
           
           if(node.parentInt!=-1)
           {
               parent=aexList[node.parentInt].get();           
           }
           else{
               
           }
           
            aex::Aex_ptr newAex = aex::make_shared<aex::Aex>(node.position, node.orientation, node.scale, parent);
           
       
            std::vector<AexMesh> newVec;

            for (int i : node.meshlist) 
            {
                aex::Mesh_ptr newMesh = aex::make_shared<aex::Mesh>( meshes[i]);

                AexMesh newPair(newAex, newMesh, node.materialNameList);
                newVec.push_back(newPair);
            }
            
            if(newVec.size()==0)
            {
                AexMesh newPair(newAex, nullptr, node.materialNameList);
                
                if(i==0)
                {
                    newPair.isRoot=true;
                }
                newVec.push_back(newPair);
            }

            ret.emplace(node.nodename, newVec);
       

           
           aexList.push_back(newAex);
       }
       
       return ret;
       
    }
   
}
#endif
