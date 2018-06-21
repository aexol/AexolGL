
#include <aex/AexMeshPart.hpp>

namespace aex {

    AexMeshPart::AexMeshPart() : Aex::Aex() {

    }

    AexMeshPart::AexMeshPart(ASceneNode* parentAex) : Aex::Aex(parentAex) {

    }

    AexMeshPart::AexMeshPart(PositionType position, RotationType rotation, ScaleType scale, ASceneNode* parentAex) : Aex(position, rotation, scale, parentAex) {

    }

    void AexMeshPart::draw(AexRenderer *gl) {
        const ShaderDrw* shader = gl->getShader();

        TAex<TRSNode>::updateDrawUniforms(gl, shader);

        const PositionType& position = getPosition();
        m_posUniform.setValue(position.x, position.y, position.z);
        m_posUniform.setUniform(*shader);

        const auto& program = shader->GetProgram();
        //LOG_DEBUG("AexMeshPart::draw", m_partToDraw.size, " ",m_partToDraw.offset);
        program->drawPart(m_partToDraw.size, m_partToDraw.offset);
        

    }
    
    void AexMeshPart::finishdraw(AexRenderer *gl)
    {
        Aex::finishdraw(gl);
    }

    void AexMeshPart::setPartToDraw(MeshPartInfo partToDraw) {
        m_partToDraw = partToDraw;
    }

    MeshPartInfo AexMeshPart::getPartToDraw() const {
        return m_partToDraw;
    }

    void AexMeshPart::setPartOffset(int offset) {
        m_partToDraw.offset = offset;
    }

    void AexMeshPart::setPartSize(int size) {
        m_partToDraw.size = size;
    }

    std::vector<float>& AexMeshPart::getDistanceList() {
        return m_distanceList;
    }

    void AexMeshPart::addDistance(float distance) {
        m_distanceList.push_back(distance);

        if (m_distanceList.size() > 1) {
            std::sort(m_distanceList.begin(), m_distanceList.end());
        }
        
    }

}