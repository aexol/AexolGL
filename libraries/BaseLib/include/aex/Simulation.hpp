/**************************************************************
 *                                                            * 
 *      Filename:   Simulation.hpp                            * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 paź 01 10:23:48                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/

#ifndef _AEX_SIMULATION_HPP
#define _AEX_SIMULATION_HPP

#include <aex/ISimulation.hpp>
#include <aex/Timer.hpp>
#include <aex/Traits.hpp>
#include <aex/ALoopNode.hpp>
#include <iterator>
#include <exception>

namespace aex
{
    /* Forward declarations */
    template<class SimulationDataType, class SimulationDataTraitsType = SimulationDataTraits<SimulationDataType>>

    class AEX_PUBLIC_API SimulationError : public std::exception
    {
        public:
            SimulationError();
            explicit SimulationError(const aex::string& err_msg);
            const char* what() const NOEXCEPT;
        private:
            const aex::string m_err;
    };

    template<class T>
    class SimulationClockVisitor final : public ISimulatorVisitor<T>
    {
        public:
            /*
             * @brief Default constructor.
             */
            explicit SimulationClockVisitor(const AexTimer* clock);

            /*
             * @brief Visit simulators and update them with new time.
             */
            void visit(BaseSimpleSimulation<T>& sim);

        private:
            const AexTimer* m_clock;
    };

    class AEX_PUBLIC_API SimulationClock : public ALoopNode
    {
        public:
            /*
             * @brief Default constructor.
             */
            SimulationClock();

            template<class T>
            SimulationClockVisitor<T> getVisitor() const;

        protected:

            /*
             * @brief Always evalutes to true.
             */
            bool init();

            /*
             * @brief No action.
             */

            void pause();

            /*
             * @brief No action.
             */

            void resume();

            /*
             * @brief Update clock with new time.
             */
            void update();

            /*
             * @brief No action.
             */
            void finish();

        private:
            AexTimer& m_clock;
    };

    template<class SimulationDataTraitsType>
    class BaseSimpleSimulation final : public ISimulator<SimulationDataTraitsType>
    {
        public:
            using simulation_data_traits_type = SimulationDataTraitsType;
            using simulation_data_type = typename simulation_data_traits_type::simulation_data_type;

            BaseSimpleSimulation();
            /*
             * @brief Accept polymorphic visitor.
             */
            void accept(ISimulatorVisitor<simulation_data_traits_type>& visitor);

            void accept(SimulationClockVisitor<simulation_data_traits_type>& visitor);

            template<class U>
            void setData(U&& u);

            const simulation_data_type& getData() const;
        private:
            simulation_data_type m_time;
    };
}

#include "details/Simulation.ipp"
#endif // _AEX_SIMULATION_HPP
