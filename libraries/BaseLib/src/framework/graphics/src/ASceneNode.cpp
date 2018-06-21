#include <aex/ASceneNode.hpp>
#include <aex/Logger.hpp>
namespace aex {
    typedef aex::math::Matrix Matrix;
    typedef aex::math::Vector3 Vector3;
    typedef aex::math::Quaternion Quaternion;

    ASceneNode::ASceneNode(ASceneNode* parentNode) :
    m_parent(nullptr),
    m_transformChange(true),
    m_updateFinal(true) {
        setParent(parentNode);
    }

    ASceneNode::ASceneNode() :
    ASceneNode(nullptr) {
    }

    ASceneNode::ASceneNode(const ASceneNode& nd) :
    m_ModelMatrix(nd.m_ModelMatrix),
    m_finalModelMatrix(nd.m_finalModelMatrix),
    m_transformChange(nd.m_transformChange),
    m_updateFinal(nd.m_updateFinal) {
        setParent(nd.m_parent);
    }

    ASceneNode&
            ASceneNode::operator=(const ASceneNode& nd) {
        if (this != &nd) {
            setParent(nd.m_parent);
            m_ModelMatrix = nd.m_ModelMatrix;
            m_finalModelMatrix = nd.m_finalModelMatrix;
            m_transformChange = nd.m_transformChange;
            m_updateFinal = nd.m_updateFinal;
        }
        return *this;
    }

    ASceneNode::ASceneNode(ASceneNode&& nd) :
    m_ModelMatrix(std::move(nd.m_ModelMatrix)),
    m_finalModelMatrix(std::move(nd.m_finalModelMatrix)),
    m_transformChange(std::move(nd.m_transformChange)),
    m_updateFinal(std::move(nd.m_updateFinal)) {
        setParent(nd.m_parent);
        nd.releaseParent();
    }

    ASceneNode&
            ASceneNode::operator=(ASceneNode&& nd) {
        if (this != &nd) {
            setParent(nd.m_parent);
            nd.releaseParent();
            m_ModelMatrix = std::move(nd.m_ModelMatrix);
            m_finalModelMatrix = std::move(nd.m_finalModelMatrix);
            m_transformChange = std::move(nd.m_transformChange);
            m_updateFinal = std::move(nd.m_updateFinal);
        }
        return *this;
    }

    void
    ASceneNode::setParent(ASceneNode& parent) {
        setParent(&parent);
    }

    void
    ASceneNode::setParent(ASceneNode* parent) {
        if (parent) {
            parent->addChild(this);
        } else {
            _setParent(parent);
        }
    }

    void ASceneNode::unsetParent() {
        setParent(nullptr);
    }

    void
    ASceneNode::addChild(ASceneNode& child) {
        addChild(&child);
    }

    void
    ASceneNode::addChild(ASceneNode* child) {
        m_children.push_back(child);
        child->_setParent(this);
    }

    void
    ASceneNode::removeChild(ASceneNode* child) {
        auto it = std::find(m_children.begin(), m_children.end(), child);
        if (it != m_children.end())
            m_children.erase(it);
    }

    Matrix
    ASceneNode::getModelMatrix() const {
        if (transformNeedsUpdate()) {
            Matrix modelMatrix;
            modelMatrix = computeModelMatrix();
            return modelMatrix;
        }
        return m_ModelMatrix;
    }

    Matrix&
    ASceneNode::getModelMatrix() {
        if (transformNeedsUpdate()) {
            const ASceneNode& self = *this;
            const Matrix& modelMatrix = self.getModelMatrix();
            m_ModelMatrix = modelMatrix;
            updateFinal();
            m_transformChange = false;
        }
        return m_ModelMatrix;
    }

    Matrix
    ASceneNode::getFinalModelMatrix() const {
        if (finalNeedsUpdate()) {
            const Matrix& modelMatrix = getModelMatrix();
            Matrix finalMatrix;
            if (m_parent != nullptr) {
                finalMatrix = getFinalParentMatrix() * modelMatrix;
            } else {
                finalMatrix = modelMatrix;
            }
            return finalMatrix;
        }
        return m_finalModelMatrix;
    }

    Matrix&
    ASceneNode::getFinalModelMatrix() {
        if (finalNeedsUpdate()) {
            const Matrix& modelMatrix = getModelMatrix();
            if (m_parent != nullptr) {
                m_finalModelMatrix = getFinalParentMatrix() * modelMatrix;
            } else {
                m_finalModelMatrix = modelMatrix;
            }
            m_updateFinal = false;
        }
        return m_finalModelMatrix;
    }

    Matrix
    ASceneNode::getParentMatrix() const {
        return m_parent->getModelMatrix();
    }

    const Matrix&
    ASceneNode::getParentMatrix() {
        return m_parent->getModelMatrix();
    }

    Matrix
    ASceneNode::getFinalParentMatrix() const {
        return m_parent->getFinalModelMatrix();
    }

    const Matrix&
    ASceneNode::getFinalParentMatrix() {
        return m_parent->getFinalModelMatrix();
    }

    ASceneNode::~ASceneNode() {
        releaseParent();
        ChildVec children;
        children.swap(m_children);
        for (auto child : children)
            child->setParent(nullptr);
    }

    bool ASceneNode::isUpdateAABB() const {
        return m_updateAABB;
    }

    void
    ASceneNode::_setParent(ASceneNode* parent) {
        releaseParent();
        m_parent = parent;
    }

    void
    ASceneNode::transformChanged() {
        m_transformChange = true;
        updateFinal();
    }

    void
    ASceneNode::updateFinal() {
        m_updateFinal = true;
        m_updateDraw = true;
        m_updateAABB = true;
        for (auto child : m_children)
            child->updateFinal();
    }

    bool
    ASceneNode::transformNeedsUpdate() const {
        return m_transformChange;
    }

    bool
    ASceneNode::finalNeedsUpdate() const {
        /*bool needsUpdate = m_updateFinal;
        if (!m_updateFinal) {
            const Matrix& modelMatrix = getModelMatrix();
            needsUpdate = m_ModelMatrix != modelMatrix;
        }
        return needsUpdate;*/
        
        return m_updateFinal;
    }
    
    bool
    ASceneNode::finalNeedsUpdate() {
        return m_updateFinal;
    }

    bool ASceneNode::drawNeedsUpdate() {
        return m_updateDraw;
    }

    void ASceneNode::drawUpdated() {
        m_updateDraw = false;
    }
    
    bool ASceneNode::aabbNeedsUpdate() {
        return m_updateAABB;
    }
    
    void ASceneNode::aabbUpdated() {
        m_updateAABB = false;
    }

    bool ASceneNode::hasParent() const
    {
        return m_parent!=nullptr;
    }
    
    ASceneNode* ASceneNode::getParent() const
    {
        return m_parent;
    }
    
    void
    ASceneNode::releaseParent() {
        if (m_parent)
            m_parent->removeChild(this);
        updateFinal();
    }

    TRSNode::TRSNode(Vector3 position, Quaternion rotation, Vector3 scale, ASceneNode* parentNode) :
    ASceneNode(parentNode),
    m_size(scale),
    m_position(position),
    m_orientation(rotation) {
    }

    TRSNode::TRSNode(ASceneNode* parentNode):
    TRSNode(Vector3(0.0f, 0.0f, 0.0f), aex::math::getIdentityQuaternion(), Vector3(1.0f, 1.0f, 1.0f), parentNode) {
    }

    TRSNode::TRSNode() :
    TRSNode(nullptr) {
    }

    void
    TRSNode::scale(float x, float y, float z, bool r) {
        if (r) {
            m_size = m_size * Vector3(x, y, z);
        } else {
            m_size = Vector3(x, y, z);
        }

        transformChanged();
    }

    void
    TRSNode::scale(Vector3 scaleVec, bool r) {
        scale(scaleVec.x, scaleVec.y, scaleVec.z, r);
    }

    void
    TRSNode::scale(float f, bool r) {
        scale(f, f, f, r);
    }

    void
    TRSNode::move(float x, float y, float z, bool r) {
        if (r) {
            m_position = m_position + Vector3(x, y, z);
        } else {
            m_position = Vector3(x, y, z);
        }
        transformChanged();
    }

    void
    TRSNode::move(Vector3 moveVec, bool r) {
        move(moveVec.x, moveVec.y, moveVec.z, r);
    }

    void 
    TRSNode::rotate(const Quaternion& rot, bool r)
    { 
        if (r) {
            m_orientation *= rot;
        } else {
            m_orientation = rot;
        }
        transformChanged();
    }
    
    void
    TRSNode::rotate(float x, float y, float z, bool r) {
        rotate(aex::math::getQuaternionFromEulerDeg(x, y, z),r);
    }

    void
    TRSNode::rotate(const Vector3 & rotVec, bool r) {
        rotate(rotVec.x, rotVec.y, rotVec.z, r);
    }

    Vector3
    TRSNode::getPosition() const {
        return m_position;
    }
    
    void 
    TRSNode::setMatrix(Matrix mat)
    {
        Vector3 scale = aex::math::extract_scale_from_matrix(mat);
        Vector3 pos = aex::math::extract_translation_from_matrix(mat);
        Quaternion quat = aex::math::extract_rotation_from_matrix(mat);
        m_position= pos;
        m_size = scale; 
        m_orientation = quat;
        
        transformChanged();
    }

    void
    TRSNode::setPosition(Vector3 position) {
        m_position = position;
        transformChanged();
    }

    void
    TRSNode::setPositionX(float value) {
        m_position.x = value;
        transformChanged();
    }

    void
    TRSNode::setPositionY(float value) {
        m_position.y = value;
        transformChanged();
    }

    void
    TRSNode::setPositionZ(float value) {
        m_position.z = value;
        transformChanged();
    }

    void
    TRSNode::setOrientation(Quaternion orientation) {
        m_orientation = orientation;
        transformChanged();
    }

    Quaternion
    TRSNode::getOrientation() const {
        return m_orientation;
    }

    void
    TRSNode::setSize(Vector3 size) {
        m_size = size;
        transformChanged();
    }

    Vector3
    TRSNode::getSize() const {
        return m_size;
    }

    Vector3 
    TRSNode::getPivot() const {
        if (hasParent()) {
            return getFinalParentMatrix().transformPoint(aex::math::Vector3());
        } else {
            return m_position;
        }
    }
    
    Vector3 
    TRSNode::getCenterPoint() const
    {
        return getFinalModelMatrix().transformPoint(aex::math::Vector3());
    }
    
    Quaternion TRSNode::getFinalRotation() const
    {
        return aex::math::getQuaternionFromMatrix(getFinalModelMatrix());
    }

    void 
    TRSNode::setAabb(AABB aabb) {
        m_aabb = aabb;
    }
    
    void 
    TRSNode::addAabb(AABB aabb)
    {
        m_aabb = AABB(aabb, m_aabb);

    }

    AABB 
    TRSNode::getOabb()  {

        if(aabbNeedsUpdate())
        {
            Matrix mat = getFinalModelMatrix();
        
            Vector3 center =  mat.transformPoint(m_aabb.center());
            Vector3 scl=aex::math::extract_scale_from_matrix(mat);
            Vector3 halfdist = Vector3 (m_aabb.halfdist().x * scl.x, m_aabb.halfdist().y * scl.y, m_aabb.halfdist().z * scl.z );
            m_oabb.setCenter(center);
            m_oabb.setHalfdist(halfdist);
            aabbUpdated();
        }
        
        return m_oabb;
    }

    Matrix
    TRSNode::computeModelMatrix() const {
        Matrix modelMatrix;
        modelMatrix = aex::math::identity();
        modelMatrix.translate(m_position.x, m_position.y, m_position.z);
        modelMatrix.rotate(m_orientation);
        modelMatrix.scale(m_size.x, m_size.y, m_size.z);
        return modelMatrix;
    }

} // end of namespace aex.
