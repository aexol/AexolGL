
#include <aex/AnimationFactory.hpp>
#include <aex/AnimationDrw.hpp>
#include <aex/GetRawData.hpp>
#include <aex/Logger.hpp>
#ifdef ALEMBIC_SUPPORT
#include <Alembic/Abc/All.h>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreOgawa/All.h>
#include <Alembic/AbcCoreHDF5/All.h>
#include <Alembic/AbcCoreFactory/All.h>
#include <Alembic/AbcCoreAbstract/All.h>
#include <Alembic/Abc/ErrorHandler.h>
#include <aex/AlembicFile.hpp>
#endif

#ifdef ALEMBIC_SUPPORT
using namespace Alembic::Abc;
using namespace Alembic::AbcGeom; // Contains Abc, AbcCoreAbstract
using namespace Alembic::AbcCoreAbstract;
using namespace Alembic::AbcCoreFactory;
using namespace Alembic::Util;
#endif

namespace aex
{
//    shared_ptr<AnimationDrw>
//    FactorialUnit<AnimationDrw, AnimationDrw>::createObject( std::tuple<const aex::string&> fName, Alembic_TAG*)
//    {
//        AnimationDrwPtr ret = AnimationDrw::makeAnimationDrw();
//#ifdef ALEMBIC_SUPPORT
//        AlembicFile sc( std::get<0>(fName) );
//        sc.parseScene( nullptr );
//
////        LoadFramesFromAlembicSingleMesh( ret, std::get<0>(fName) );
//#endif
//        return ret;
//    }
//
//    FactorialUnit<AnimationDrw, AnimationDrw>&
//    FactorialUnit<AnimationDrw, AnimationDrw>::getInstance()    {
//        static FactorialUnit<AnimationDrw, AnimationDrw> instance;
//        return instance;
//    }

    void
    LoadFramesFromAlembicSingleMesh(const AnimationDrwPtr& anim, const aex::string& file)
    {

#ifdef ALEMBIC_SUPPORT

        Alembic::AbcCoreFactory::IFactory factory;
        IArchive archive = factory.getArchive(file);

        IPolyMesh animatedMesh;


        IObject obj = archive.getTop();
        unsigned int numChildren = obj.getNumChildren();
        LOG_DEBUG("found", numChildren, " children in file");

        for(int i = 0; i < numChildren; ++i)
        {
            LOG_DEBUG(obj.getChildHeader(i).getFullName());
            IObject child(obj, obj.getChildHeader(i).getName());


            LOG_DEBUG("Children ", child.getNumChildren());

            for(int x = 0; x < child.getNumChildren(); x++)
            {
                IObject child2(child, child.getChildHeader(x).getName());
                const MetaData& md2 = child2.getMetaData();

                if(IPolyMeshSchema::matches(md2) || ISubDSchema::matches(md2))
                {
                    LOG_DEBUG("Found a mesh ", child2.getName());
                    IPolyMesh mesha(child, child2.getName());
                    animatedMesh = mesha;

                }
            }
        }


        size_t numSamples = animatedMesh.getSchema().getNumSamples();

        for(size_t i = 0; i < numSamples; i++)
        {
            Alembic::Abc::ISampleSelector ss((AbcA::index_t)i);
            Alembic::AbcGeom::IPolyMeshSchema::Sample psamp;


            if(animatedMesh.getSchema().getNumSamples() > 0)
            {
                animatedMesh.getSchema().get(psamp, ss);
            }

            //Positions
            Alembic::Abc::P3fArraySamplePtr P = psamp.getPositions();
            auto vertices  = GetRawData<Alembic::Abc::P3fArraySample::traits_type>(P->get(), P->size());
            anim->AddFrame(
                KeyframeVertex(i * anim->GetUniformFrameDuration(), (i + 1)*anim->GetUniformFrameDuration(), *vertices)
            );
            // AbcA::M44d final_transform = IXformDrw::getFinalMatrix( m_polyMesh, ss );

        }

        if(anim->GetAnimatedMesh())
        {
            anim->GetAnimatedMesh()->setFloatAttribute(anim->GetFrames()[1].GetFrame(), "Frames", "gl_nextFrame", 3);
            anim->GetAnimatedMesh()->compile();
        }

#else
        LOG_ERR(" ANIMATION : Library build without alembic support.");
#endif

    }



}
