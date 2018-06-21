#include <aex/Mesh.hpp>

namespace aex
{
    MeshPartInfo::MeshPartInfo()
    {
        
    }
    
    MeshPartInfo::MeshPartInfo(Mesh::indices_type cOffset, Mesh::indices_type cSize):offset(cOffset) , size(cSize)
    {
        
    }
    
    CombinedMesh::CombinedMesh()
    {
        
    }
    
    uint32_t CombinedMesh::addMesh(Mesh& mesh)
    {
		return addMesh(&mesh);
    }
    
    uint32_t CombinedMesh::addMesh(Mesh* mesh)
    {

        size_t cSize = 0;
        size_t oldInd = 0;
        if(m_meshMap.size()!=0)
        {
            Mesh::vertex_array_type& cVertices = getVertices();
            cSize = cVertices.size()/3;
            oldInd =  getIndices().size();
        }
        
        Mesh::index_array_type& aIndices = mesh->getIndices();
        Mesh::vertex_array_type& aVertices = mesh->getVertices();
        Mesh::vertex_array_type* aNormals = mesh->getNormals();
        Mesh::vertex_array_type* aCoords= mesh->getCoords();
        
        size_t aSize = aIndices.size();
        
        Mesh::index_array_type cpyaIndices;
        cpyaIndices.resize(aIndices.size());
        
        if (m_meshMap.size() != 0) {
            for (size_t i = 0; i < aIndices.size(); ++i) {
                cpyaIndices[i] = aIndices[i] + cSize;
               
            }
        }

        if (m_meshMap.size() == 0) {
            setTriangles(aIndices);
            setVertices(aVertices);

            
            if (aNormals) {
                setNormals(*aNormals);
            }

            if (aCoords) {
                setCoords(*aCoords);
            }
        } else {
            Mesh::index_array_type& cIndices = this->getIndices();
            cIndices.insert(cIndices.end(), cpyaIndices.begin(), cpyaIndices.end());


          
            Mesh::vertex_array_type& cVertices = this->getVertices();
            Mesh::vertex_array_type* cNormals = this->getNormals();
            Mesh::vertex_array_type* cCoords = this->getCoords();

            cVertices.insert(cVertices.end(),aVertices.begin(),aVertices.end());

            if (aNormals) {
                cNormals->insert(cNormals->end(), aNormals->begin(), aNormals->end());
            }

            if (aCoords) {
                cCoords->insert(cCoords->end(), aCoords->begin(), aCoords->end());
            }

        }
        
        m_meshMap.emplace(m_meshMap.size(),MeshPartInfo(oldInd,aSize));
        LOG_DEBUG("MeshPartInfo: offset: ",oldInd, ", size:", aSize );
        m_compiled = false;
	return cSize;
    }
    
    
    MeshPartInfo CombinedMesh::getMeshInfo(size_t infonumber)
    {
        if(m_meshMap.find( infonumber)!= m_meshMap.end())
        {
            return m_meshMap[infonumber];
        }else
        {
            if(infonumber>=m_meshMap.size())
            {
                return m_meshMap[m_meshMap.size()-1];
            }
            else
            {
                return m_meshMap[infonumber];   
            }
        }        
    }
    
}
