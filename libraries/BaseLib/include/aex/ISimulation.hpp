#ifndef _AEX_ISIMULATION_HPP
#define _AEX_ISIMULATION_HPP
namespace aex
{
    /* Forward declarations */

    template<class> class BaseSimpleSimulation;

    template<class SimulationDataTraitsType>
    class ISimulatorVisitor
    {
        public:
            typedef SimulationDataTraitsType simulation_data_traits_type;
            typedef typename simulation_data_traits_type::simulation_data_type simulation_data_type;
            virtual void visit(BaseSimpleSimulation<simulation_data_traits_type>& sim) = 0;
            virtual ~ISimulatorVisitor() = 0;
    };
    
    template<class SimulationDataTraitsType>
    class ISimulator
    {
        public:
            typedef SimulationDataTraitsType simulation_data_traits_type;
            typedef typename simulation_data_traits_type::simulation_data_type simulation_data_type;
            virtual void accept(ISimulatorVisitor<simulation_data_traits_type>& visitor) = 0;
            virtual ~ISimulator() = 0;
    };
}

#include "details/ISimulation.ipp"
#endif // _AEX_ISIMULATION_HPP
