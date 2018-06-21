#include <aex/AexInstances.hpp>

namespace aex
{

   /* AexInstances::AexInstances()
    {

    }

    AexInstances::AexInstances(std::vector<Aex_ptr> instances) :
        m_instances(instances)
    {

    }

    void AexInstances::draw(AexRenderer* gl)
    {
        for(auto iter = m_instances.begin(); iter != m_instances.end(); ++iter)
        {
            (*iter)->draw(gl);
            (*iter)->finishdraw(gl);
        }
    }

    void AexInstances::finishdraw(AexRenderer* gl)
    {

    }

    void AexInstances::needsUpdate()
    {

    }

    void AexInstances::setInstances(std::vector<Aex_ptr> instances)
    {
        m_instances = instances;
    }

    std::vector<Aex_ptr>& AexInstances::getInstances()
    {
        return m_instances;
    }

    void AexInstances::addAex(Aex_ptr aex)
    {
        m_instances.push_back(aex);
    }

    void AexInstances::removeAex(Aex_ptr aex)
    {
        m_instances.erase(std::remove(m_instances.begin(), m_instances.end(), aex), m_instances.end());
    }
    
    void AexInstances::clear()
    {
        m_instances.clear();
    }*/
    
    ////////////////////////////////////////////////////////////////////////////////////////////// AexInstances
    /*AexInstancesRaw::AexInstancesRaw()
    {

    }

    AexInstancesRaw::AexInstancesRaw(std::vector<Aex*> instances) :
        m_instances(instances)
    {

    }

    void AexInstancesRaw::draw(AexRenderer* gl)
    {
        for(auto iter = m_instances.begin(); iter != m_instances.end(); ++iter)
        {
            (*iter)->draw(gl);
            (*iter)->finishdraw(gl);
        }
    }

    void AexInstancesRaw::finishdraw(AexRenderer* gl)
    {

    }

    void AexInstancesRaw::needsUpdate()
    {

    }

    void AexInstancesRaw::setInstances(std::vector<Aex*> instances)
    {
        m_instances = instances;
    }

    std::vector<Aex*>& AexInstancesRaw::getInstances()
    {
        return m_instances;
    }

    void AexInstancesRaw::addAex(Aex*  aex)
    {
        m_instances.push_back(aex);
    }

    void AexInstancesRaw::removeAex(Aex* aex)
    {
        m_instances.erase(std::remove(m_instances.begin(), m_instances.end(), aex), m_instances.end());
    }

    void AexInstancesRaw::clear()
    {
        m_instances.clear();
    }*/
    
}
