/**************************************************************
 *                                                            * 
 *      Filename:   Simulation.ipp                            * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 paź 01 10:24:23                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/

#ifndef _AEX_SIMULATION_IPP
#define _AEX_SIMULATION_IPP
#include <aex/Simulation.hpp>
namespace aex
{

    /************* SimulationClock and SimulationClockVisitor ******/

    template<class T>
    SimulationClockVisitor<T>::SimulationClockVisitor(const AexTimer* clock) : m_clock(clock){}

    template<class T>
    void 
    SimulationClockVisitor<T>::visit(BaseSimpleSimulation<T>& sim)
    {
        sim.setData(m_clock->getElapsedTime());
    }
    
    template<class T>
    SimulationClockVisitor<T> SimulationClock::getVisitor() const
    {
        return SimulationClockVisitor<T>(&m_clock);
    }

    /***************** BaseSimpleSimulation ************/
    template<class SimulationDataTraits>
    BaseSimpleSimulation<SimulationDataTraits>::BaseSimpleSimulation():
    m_time(simulation_data_traits_type::min())
    {}

    template<class SimulationDataTraitsType>
    void
    BaseSimpleSimulation<SimulationDataTraitsType>::accept(ISimulatorVisitor<simulation_data_traits_type>& visitor)
    {
        visitor.visit(*this);
    }

    template<class SimulationDataTraitsType>
    void
    BaseSimpleSimulation<SimulationDataTraitsType>::accept(SimulationClockVisitor<simulation_data_traits_type>& clock)
    {
        clock.visit(*this);
    }
    
    template<class SimulationDataTraitsType>
    template<class U>
    void
    BaseSimpleSimulation<SimulationDataTraitsType>::setData(U&& u)
    {
        simulation_data_traits_type::assign(m_time, u);
    }

    template<class SimulationDataTraitsType>
    const typename BaseSimpleSimulation<SimulationDataTraitsType>::simulation_data_type&
    BaseSimpleSimulation<SimulationDataTraitsType>::getData() const
    {
        return m_time;
    }
}
#endif // _AEX_SIMULATION_IPP
