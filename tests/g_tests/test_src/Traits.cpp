#include "UserMockType.hpp"
#include <aex/Traits.hpp>
#include "gtest/gtest.h"
#include <iostream>

class SimulationTraitsTesting : public ::testing::Test
{
    protected:
        typedef std::chrono::high_resolution_clock Clock;
        typedef Clock::duration StdDuration;
        typedef aex::SimulationDataTraits<StdDuration> DurationTraits;
        typedef aex::SimulationDataTraits<double> PrimitiveTimeTraits;
        typedef aex::SimulationDataTraits<UserSimulationTypeMock> UserMockTraits;
        typedef aex::SimulationDataTraits<UserSimulationTypeMockWithFB> UserMockTraitsWithFB;
        typedef typename DurationTraits::simulation_data_type DurationType;
        typedef typename PrimitiveTimeTraits::simulation_data_type PrimitiveType;
        typedef typename UserMockTraits::simulation_data_type UserType;
        typedef typename UserMockTraitsWithFB::simulation_data_type UserTypeWithFB;
        virtual void SetUp()
        {
            mNow = Clock::now();
            mDuration = mNow.time_since_epoch();
            std::chrono::duration<double> fn = mDuration;
            mPrimitiveTime = fn.count();
            mUserMock = {mDuration.count(), fn.count()};
            mUserMockWithFB = UserTypeWithFB{0ll, 0.0};
        }
        
        Clock::time_point mNow;
        DurationType mDuration;
        PrimitiveType mPrimitiveTime;
        UserType mUserMock;
        UserTypeWithFB mUserMockWithFB;
};

TEST_F(SimulationTraitsTesting, ComparisonOps)
{
    typedef std::chrono::duration<double> fsec;
    auto now = Clock::now().time_since_epoch();
    fsec fs = now;
    DurationType d_time = std::chrono::duration_cast<DurationType>(now);
    DurationType d_time2 = d_time;
    PrimitiveType f_time = fs.count();
    PrimitiveType f_time2 = f_time;
    UserType u_time = {d_time.count(), f_time};
    UserType u_time2 = u_time;

    /* Equality */
    EXPECT_TRUE(DurationTraits::eq(d_time, d_time2));
    EXPECT_TRUE(PrimitiveTimeTraits::eq(f_time, f_time2));
    EXPECT_TRUE(UserMockTraits::eq(u_time, u_time2));

    d_time2 = d_time+DurationType(1);
    f_time2 = f_time+1.0;
    u_time2 = u_time+UserType{1ll, 1.0};

    /* Not equal */
    EXPECT_FALSE(DurationTraits::eq(d_time, d_time2));
    EXPECT_FALSE(PrimitiveTimeTraits::eq(f_time, f_time2));
    EXPECT_FALSE(UserMockTraits::eq(u_time, u_time2));

    /* Greater than */
    EXPECT_FALSE(DurationTraits::gt(d_time, d_time2));
    EXPECT_FALSE(PrimitiveTimeTraits::gt(f_time, f_time2));
    EXPECT_FALSE(UserMockTraits::gt(u_time, u_time2));
    EXPECT_TRUE(DurationTraits::gt(d_time2, d_time));
    EXPECT_TRUE(PrimitiveTimeTraits::gt(f_time2, f_time));
    EXPECT_TRUE(UserMockTraits::gt(u_time2, u_time));

    /* Lesser than */

    EXPECT_TRUE(DurationTraits::lt(d_time, d_time2));
    EXPECT_TRUE(PrimitiveTimeTraits::lt(f_time, f_time2));
    EXPECT_TRUE(UserMockTraits::lt(u_time, u_time2));
    EXPECT_FALSE(DurationTraits::lt(d_time2, d_time));
    EXPECT_FALSE(PrimitiveTimeTraits::lt(f_time2, f_time));
    EXPECT_FALSE(UserMockTraits::lt(u_time2, u_time));
}

TEST_F(SimulationTraitsTesting, Assignment)
{
    typedef std::chrono::duration<double> fsec;
    auto now = Clock::now().time_since_epoch();
    fsec fs = now;
    DurationType d_time = std::chrono::duration_cast<DurationType>(now);
    PrimitiveType f_time = fs.count();
    UserType u_time = {d_time.count(), f_time};
    DurationTraits::assign(mDuration, d_time); 
    PrimitiveTimeTraits::assign(mPrimitiveTime, f_time);
    UserMockTraits::assign(mUserMock, u_time);
    EXPECT_EQ(mDuration, d_time);
    EXPECT_EQ(mPrimitiveTime, f_time);
    EXPECT_EQ(mUserMock, u_time);
    UserMockTraits::assign(mUserMock, 5ll);
    u_time = {5ll, u_time.b};
    EXPECT_EQ(mUserMock, u_time);
    UserMockTraits::assign(mUserMock, 2.0f);
    u_time = {u_time.a, 2.0f};
    EXPECT_EQ(mUserMock, u_time);
}

TEST_F(SimulationTraitsTesting, Addition)
{
    typedef std::chrono::duration<double> fsec;
    auto now = Clock::now();
    fsec fs = now - mNow;
    StdDuration d_time = std::chrono::duration_cast<StdDuration>(fs);
    PrimitiveType f_time = fs.count();
    UserType u_time = {d_time.count(), f_time};
    auto expectedDur = mDuration + d_time;
    auto expectedPrim = mPrimitiveTime + f_time;
    auto expectedUser = UserType{mUserMock.a + u_time.a, mUserMock.b + u_time.b};
    auto resultDur = DurationTraits::add(mDuration, d_time); 
    auto resultPrim = PrimitiveTimeTraits::add(mPrimitiveTime, f_time);
    auto resultUserMock = UserMockTraits::add(mUserMock, u_time);
    EXPECT_EQ(resultDur, expectedDur);
    EXPECT_EQ(resultPrim, expectedPrim);
    EXPECT_EQ(resultUserMock, expectedUser);
    if(now != mNow)
    {
        EXPECT_NE(mDuration, expectedDur);
        EXPECT_NE(mPrimitiveTime, expectedPrim);
        EXPECT_NE(mUserMock, expectedUser);
    }
}

TEST_F(SimulationTraitsTesting, CompoundAddition)
{
    typedef std::chrono::duration<double> fsec;
    auto now = Clock::now();
    fsec fs = now - mNow;
    StdDuration d_time = std::chrono::duration_cast<StdDuration>(fs);
    PrimitiveType f_time = fs.count();
    UserType u_time = {d_time.count(), f_time};
    auto expectedDur = mDuration + d_time;
    auto expectedPrim = mPrimitiveTime + f_time;
    auto expectedUser = UserType{mUserMock.a + u_time.a, mUserMock.b + u_time.b};
    auto& resultDur = DurationTraits::iadd(mDuration, d_time); 
    auto& resultPrim = PrimitiveTimeTraits::iadd(mPrimitiveTime, f_time);
    auto& resultUserMock = UserMockTraits::iadd(mUserMock, u_time);
    EXPECT_EQ(mDuration, expectedDur);
    EXPECT_EQ(mPrimitiveTime, expectedPrim);
    EXPECT_EQ(mUserMock, expectedUser);
    EXPECT_EQ(&mDuration, &resultDur);
    EXPECT_EQ(&mPrimitiveTime, &resultPrim);
    EXPECT_EQ(&mUserMock, &resultUserMock);
}

TEST_F(SimulationTraitsTesting, Subtraction)
{
    typedef std::chrono::duration<double> fsec;
    auto now = Clock::now();
    fsec fs = now - mNow;
    StdDuration d_time = std::chrono::duration_cast<StdDuration>(fs);
    PrimitiveType f_time = fs.count();
    UserType u_time = {d_time.count(), f_time};
    auto expectedDur = mDuration - d_time;
    auto expectedPrim = mPrimitiveTime - f_time;
    auto expectedUser = UserType{mUserMock.a - u_time.a, mUserMock.b - u_time.b};
    auto resultDur = DurationTraits::sub(mDuration, d_time); 
    auto resultPrim = PrimitiveTimeTraits::sub(mPrimitiveTime, f_time);
    auto resultUserMock = UserMockTraits::sub(mUserMock, u_time);
    EXPECT_EQ(resultDur, expectedDur);
    EXPECT_EQ(resultPrim, expectedPrim);
    EXPECT_EQ(resultUserMock, expectedUser);
    if(now != mNow)
    {
        EXPECT_NE(mDuration, expectedDur);
        EXPECT_NE(mPrimitiveTime, expectedPrim);
        EXPECT_NE(mUserMock, expectedUser);
    }
}

TEST_F(SimulationTraitsTesting, CompoundSubtraction)
{
    typedef std::chrono::duration<double> fsec;
    auto now = Clock::now();
    fsec fs = now - mNow;
    StdDuration d_time = std::chrono::duration_cast<StdDuration>(fs);
    PrimitiveType f_time = fs.count();
    UserType u_time = {d_time.count(), f_time};
    auto expectedDur = mDuration - d_time;
    auto expectedPrim = mPrimitiveTime - f_time;
    auto expectedUser = UserType{mUserMock.a - u_time.a, mUserMock.b - u_time.b};
    auto& resultDur = DurationTraits::isub(mDuration, d_time); 
    auto& resultPrim = PrimitiveTimeTraits::isub(mPrimitiveTime, f_time);
    auto& resultUserMock = UserMockTraits::isub(mUserMock, u_time);
    EXPECT_EQ(mDuration, expectedDur);
    EXPECT_EQ(mPrimitiveTime, expectedPrim);
    EXPECT_EQ(mUserMock, expectedUser);
    EXPECT_EQ(&mDuration, &resultDur);
    EXPECT_EQ(&mPrimitiveTime, &resultPrim);
    EXPECT_EQ(&mUserMock, &resultUserMock);
}


TEST_F(SimulationTraitsTesting, Modulo)
{
    typedef std::chrono::duration<double> fsec;
    auto now = Clock::now();
    fsec fs = now - mNow;
    StdDuration d_time = std::chrono::duration_cast<StdDuration>(fs);
    PrimitiveType f_time = fs.count();
    UserType u_time = {d_time.count(), f_time};
    auto expectedDur = mDuration % d_time;
    auto expectedPrim = std::fmod(mPrimitiveTime, f_time);
    auto expectedUser = UserType{mUserMock.a % u_time.a, std::fmod(mUserMock.b, u_time.b)};
    auto resultDur = DurationTraits::mod(mDuration, d_time); 
    auto resultPrim = PrimitiveTimeTraits::mod(mPrimitiveTime, f_time);
    auto resultUserMock = UserMockTraits::mod(mUserMock, u_time);
    EXPECT_EQ(resultDur, expectedDur);
    EXPECT_DOUBLE_EQ(resultPrim, expectedPrim);
    EXPECT_EQ(resultUserMock, expectedUser);
    if(now != mNow)
    {
        EXPECT_NE(mDuration, expectedDur);
        EXPECT_NE(mPrimitiveTime, expectedPrim);
        EXPECT_NE(mUserMock, expectedUser);
    }
}

TEST_F(SimulationTraitsTesting, CompoundModulo)
{
    typedef std::chrono::duration<double> fsec;
    auto now = Clock::now();
    fsec fs = now - mNow;
    StdDuration d_time = std::chrono::duration_cast<StdDuration>(fs);
    PrimitiveType f_time = fs.count();
    UserType u_time = {d_time.count(), f_time};
    auto expectedDur = mDuration % d_time;
    auto expectedPrim = std::fmod(mPrimitiveTime, f_time);
    auto expectedUser = UserType{mUserMock.a % u_time.a, std::fmod(mUserMock.b, u_time.b)};
    auto& resultDur = DurationTraits::imod(mDuration, d_time); 
    auto& resultPrim = PrimitiveTimeTraits::imod(mPrimitiveTime, f_time);
    auto& resultUserMock = UserMockTraits::imod(mUserMock, u_time);
    EXPECT_EQ(mDuration, expectedDur);
    EXPECT_FLOAT_EQ(mPrimitiveTime, expectedPrim);
    EXPECT_EQ(mUserMock, expectedUser);
    EXPECT_EQ(&mDuration, &resultDur);
    EXPECT_EQ(&mPrimitiveTime, &resultPrim);
    EXPECT_EQ(&mUserMock, &resultUserMock);
}

TEST_F(SimulationTraitsTesting, ForwardRewind)
{
    /* Test for overriden forward/rewind */
    UserTypeWithFB expectedUserFB = UserTypeWithFB{5ll, 5.0};
    EXPECT_EQ(UserMockTraitsWithFB::forward(mUserMockWithFB, mUserMockWithFB, mUserMockWithFB), expectedUserFB);
    EXPECT_EQ(UserMockTraitsWithFB::rewind(mUserMockWithFB, mUserMockWithFB, mUserMockWithFB), expectedUserFB);

    /* Test default behaviour on user type */
    const UserType userMockValue = UserType{6ll, 6.0};
    const UserType userDiff = UserType{1ll, 1.0};
    const UserType userRange = userMockValue;

    UserType expectedUserForward = UserType{1ll, 1.0};
    UserType expectedUserRewind = UserType{5ll, 5.0};

    EXPECT_EQ(UserMockTraits::forward(userMockValue, userDiff, userRange), expectedUserForward);
    EXPECT_EQ(UserMockTraits::rewind(userMockValue, userDiff, userRange), expectedUserRewind);

    UserType userMockForward = userMockValue;
    UserType userMockRewind = userMockValue;
    auto& userMockForwardRef = UserMockTraits::forward(userMockForward, userDiff, userRange);
    auto& userMockRewindRef = UserMockTraits::rewind(userMockRewind, userDiff, userRange);

    EXPECT_EQ(userMockForward, expectedUserForward);
    EXPECT_EQ(userMockRewind, expectedUserRewind);
    EXPECT_EQ(&userMockForward, &userMockForwardRef);
    EXPECT_EQ(&userMockRewind, &userMockRewindRef);

    /* Test on primitive */

    const PrimitiveType primitiveMockValue = 6.0;
    const PrimitiveType primitiveDiff = 1.0;
    const PrimitiveType primitiveRange = primitiveMockValue;

    EXPECT_EQ(PrimitiveTimeTraits::forward(primitiveMockValue, primitiveDiff, primitiveRange),
            std::fmod(primitiveMockValue + primitiveDiff, primitiveRange));
    EXPECT_EQ(PrimitiveTimeTraits::rewind(primitiveMockValue, primitiveDiff, primitiveRange),
            std::fmod(primitiveMockValue - primitiveDiff, primitiveRange));

    PrimitiveType primitiveMockValueForward = primitiveMockValue;
    PrimitiveType primitiveMockValueRewind = primitiveMockValue;
    PrimitiveType& primitiveMockValueForwardRef =
        PrimitiveTimeTraits::forward(primitiveMockValueForward, primitiveDiff, primitiveRange);
    PrimitiveType& primitiveMockValueRewindRef =
        PrimitiveTimeTraits::rewind(primitiveMockValueRewind, primitiveDiff, primitiveRange);

    EXPECT_EQ(primitiveMockValueForward,
            std::fmod(primitiveMockValue + primitiveDiff, primitiveRange));
    EXPECT_EQ(primitiveMockValueRewind,
            std::fmod(primitiveMockValue - primitiveDiff, primitiveRange));
    EXPECT_EQ(&primitiveMockValueForward, &primitiveMockValueForwardRef);
    EXPECT_EQ(&primitiveMockValueRewind, &primitiveMockValueRewindRef);
}
