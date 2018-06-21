#ifndef __ANIMATIONFACTORY_HPP
#define __ANIMATIONFACTORY_HPP

#include <aex/Common.hpp>
#include <aex/AnimationDrw.hpp>
#include <aex/AbstractFactory.hpp>

namespace aex
{

    template<typename TAG, typename... Args> using AnimationDrwFactory = ConcreteFactory<AnimationDrw, AnimationDrw, TAG, Args...>;

    AEX_PUBLIC_API void LoadFramesFromAlembicSingleMesh(const shared_ptr<AnimationDrw>& anim, const aex::string& file);

}

#endif
