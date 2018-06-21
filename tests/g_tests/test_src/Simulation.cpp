#include "UserMockType.hpp"
#include <aex/Traits.hpp>
#include <aex/Simulation.hpp>
#include <aex/ALoop.hpp>
#include "gmock/gmock.h"
#include <chrono>
#include <thread>

typedef aex::SimulationDataTraits<float> primitive_simulation_traits_type;
typedef aex::SimulationDataTraits<std::chrono::milliseconds> duration_simulation_traits_type;
typedef aex::SimulationDataTraits<UserSimulationTypeMock> user_simulation_traits_type;
typedef aex::SimulationDataTraits<aex::aexTimeUnit> aex_time_unit_simulation_traits_type;

template<class T>
class MockSimulatorVisitor : public aex::ISimulatorVisitor<T>
{
    public:
        MOCK_METHOD1_T(visit, void(aex::BaseSimpleSimulation<T>&));
};


TEST(BaseSimpleSimulationTest, CallsVisit)
{
    MockSimulatorVisitor<primitive_simulation_traits_type> pVisitor;
    MockSimulatorVisitor<duration_simulation_traits_type> dVisitor;
    MockSimulatorVisitor<user_simulation_traits_type> uVisitor;
    EXPECT_CALL(pVisitor, visit(testing::_)).Times(1);
    EXPECT_CALL(dVisitor, visit(testing::_)).Times(1);
    EXPECT_CALL(uVisitor, visit(testing::_)).Times(1);

    aex::BaseSimpleSimulation<primitive_simulation_traits_type> pSimulation;
    aex::BaseSimpleSimulation<duration_simulation_traits_type> dSimulation;
    aex::BaseSimpleSimulation<user_simulation_traits_type> uSimulation;
    pSimulation.accept(pVisitor);
    dSimulation.accept(dVisitor);
    uSimulation.accept(uVisitor);
}

TEST(SimulationClockVisitor, UpdateSimulationWithTime)
{
    typedef aex_time_unit_simulation_traits_type traits;
    aex::AexTimer& clock = aex::AexTimer::getInstance();
    aex::SimulationClockVisitor<traits> primitiveClockSimulation(&clock);

    int samples = 0;
    clock.setFrameTimeNow();
    auto sampleTime = clock.getElapsedTime();
    aex::BaseSimpleSimulation<traits> simulation;
    simulation.accept(primitiveClockSimulation);
    EXPECT_EQ(sampleTime, simulation.getData());

    samples = 0;
    while(samples < 10)
    {
        clock.setFrameTimeNow();
        if(!traits::eq(sampleTime, clock.getElapsedTime()))
        {
            sampleTime = clock.getElapsedTime();
            simulation.accept(primitiveClockSimulation);
            EXPECT_EQ(sampleTime, simulation.getData());
            samples++;
        }
    }
}

TEST(SimulationClock, ClockVisitor)
{
    typedef aex_time_unit_simulation_traits_type traits;
    typedef std::chrono::milliseconds ms;

    aex::SimulationClock clock;
    aex::BaseSimpleSimulation<traits> simulation;
    aex::ALoop loop;
    auto visitor = clock.getVisitor<traits>();
    loop.add_node(&clock);

    loop.once();
    simulation.accept(visitor);
    auto currentData = simulation.getData();
    std::this_thread::sleep_for(ms(15));

    for(int i = 0; i < 10; i++)
    {
        loop.once();
        simulation.accept(visitor);
        std::this_thread::sleep_for(ms(15));
        EXPECT_FALSE(traits::eq(currentData, simulation.getData()));
        currentData = simulation.getData();
    }
}
