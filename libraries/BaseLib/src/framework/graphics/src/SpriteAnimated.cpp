#include <aex/SpriteAnimated.hpp>
#include <aex/TextureManager.hpp>
#include <json/json.h>
namespace aex
{

    SpriteFrame::SpriteFrame(int fNumber, int pNumber, float sizeX, float sizeY, float offsetX, float offsetY) :
        frameNumber(fNumber),
        pageNumber(pNumber),
        UvOffset(offsetX, offsetY),
        UvSize(sizeX, sizeY)
    {

    }

    AnimationTypeInfo::AnimationTypeInfo(aex::string sName, int sFrame, int eFrame) :
        animationName(sName),
        startFrame(sFrame),
        endFrame(eFrame),
        animationSize(eFrame - sFrame)
    {

    }

    SpriteAnimated::SpriteAnimated() :
        m_UvOffsetUniform(0.0f, 0.0f, "UvOffset"),
        m_UvSizeUniform(0.0f, 0.0f, "UvSize")
    {

    }

    SpriteAnimated::~SpriteAnimated()
    {
        if(m_mulitiTextureAtlas)
        {
            m_Sprite = nullptr;

            for(uint32_t i = 0; i < m_textures.size(); ++i)
            {
                TextureManager::GetInstance().releaseTexture(m_textures[i]);
            }
        }
    }

    void SpriteAnimated::draw(AexRenderer* gl)
    {
        Sprite::draw(gl);

        const AexShader& program = gl->getShader()->GetProgram();

        m_UvOffsetUniform.setValue(m_framesInfo[m_currenFrame].UvOffset);
        m_UvOffsetUniform.setUniform(program);

        if(!m_frameUniformSize || !m_isUniformSizeSet)
        {
            m_UvSizeUniform.setValue(m_framesInfo[m_currenFrame].UvSize);
            m_UvSizeUniform.setUniform(program);
            m_isUniformSizeSet = true;
        }
    }

    void SpriteAnimated::update(aexTimeUnit elapsedtime)
    {
        if(!m_paused)
        {
            if(!m_rewind)
            {
                m_time += elapsedtime * m_speed;
            }
            else
            {
                m_time -= elapsedtime * m_speed;
            }

            calculateFrame();
        }
    };

    void SpriteAnimated::calculateFrame()
    {

        m_time = aex::math::Math::clamp_value_float(0.0f, m_duration, m_time);
        float temp = m_time / m_frameduration;

        if(m_multiAnimationTypes)
        {
            m_currenFrame = m_currentAnimationType.startFrame + ((int) temp) % m_currentAnimationType.animationSize;

        }
        else
        {
            m_currenFrame = ((int) temp) % m_frames;
        }

        if(m_mulitiTextureAtlas && m_framesInfo[m_currenFrame].pageNumber != m_currentPage)
        {
            m_Sprite = m_textures[m_framesInfo[m_currenFrame].pageNumber];
        }

        //LOG_DEBUG("m_currenFrame: ", m_currenFrame);
    }

    void SpriteAnimated::finishdraw(AexRenderer* gl)
    {
        Sprite::finishdraw(gl);
    }

    bool SpriteAnimated::setAnimationType(aex::string animationTypeName)
    {
        m_animationType = animationTypeName;
        auto animationtype = m_animationsInfo.find(animationTypeName);

        if(animationtype != m_animationsInfo.end())
        {
            return true;
        }

        return false;
    }

    void SpriteAnimated::generateJson(const aex::string& filename, int pixelsX, int pixelsY, int framesX, int framesY)
    {

        int frameSizeX = pixelsX / framesX;
        int frameSizeY = pixelsY / framesY;

        Json::Value root;

        int i = 0;
        Json::Value frames;

        for(int x = 0; x < framesX; ++x)
        {
            for(int y = 0; y < framesX; ++y)
            {
                Json::Value frame;

                frame["w"] = frameSizeX;
                frame["h"] = frameSizeY;
                frame["x"] = frameSizeX * x;
                frame["y"] = frameSizeY * y;
                ++i;

                if(i < 10)
                {
                    frames["0" + aex::to_string(i)]["frame"] = frame;
                }
                else
                {
                    frames[aex::to_string(i)]["frame"] = frame;
                }
            }
        }

        root["frames"] = frames;

        FileManager fm;
        boost::filesystem::path dir = fm.write_dir();
        dir /= filename;
        auto file = fm.open_write_file(dir);
        LOG_DEBUG(root.toStyledString());

        if(file.is_open())
        {
            file << root.toStyledString();
        }


    }

    void LoadSeqenceFromFile(const aex::string& filename)
    {
        FileManager fm;
        //LOG_DEBUG("JSON");
        boost::filesystem::path fallback = fm.work_dir();
        fallback /= filename;

        Json::Value root;
        Json::Reader reader;
        //LOG_DEBUG(fallback.string());
        auto file = fm.open_read_file(fallback);

        if(file.is_open())
        {
            bool parsingSuccessful = reader.parse(file, root);

            if(!parsingSuccessful)
            {
                // report to the user the failure and their locations in the document.
                LOG_DEBUG("Failed to parse configuration");
                LOG_DEBUG(reader.getFormattedErrorMessages());
                return;
            }

            Json::Value sequence = root["sequence"];

            if(!sequence.empty())
            {

            }
        }
    }

    void SpriteAnimated::LoadAnimationsFromFile(const aex::string& filename)
    {

        FileManager fm;
        //LOG_DEBUG("JSON");
        boost::filesystem::path fallback = fm.work_dir();
        fallback /= filename;

        Json::Value root;
        Json::Reader reader;
        //LOG_DEBUG(fallback.string());
        auto file = fm.open_read_file(fallback);

        if(file.is_open())
        {
            bool parsingSuccessful = reader.parse(file, root);

            if(!parsingSuccessful)
            {
                // report to the user the failure and their locations in the document.
                LOG_DEBUG("Failed to parse configuration");
                LOG_DEBUG(reader.getFormattedErrorMessages());
                return;
            }

            float fileWidth = 0;
            float fileHeight = 0;
            Json::Value file = root["file"];

            if(!file.empty())
            {
                if(file["path"].empty())
                {
                    setSprite(file.asString());
                }
                else
                {
                    setSprite(file["path"].asString());

                    if(!file["width"].empty())
                    {
                        fileWidth = file["width"].asFloat();
                    }

                    if(!file["height"].empty())
                    {
                        fileHeight = file["height"].asFloat();
                    }

                }

                //LOG_DEBUG("file in file: ", file.asString());
            }

            Json::Value files = root["files"];

            if(!files.empty())
            {
                std::vector<aex::string> filesNames = files.getMemberNames();

                if(filesNames.size() > 1)
                {
                    for(uint32_t i = 0; i < filesNames.size(); ++i)
                    {
                        m_textures.push_back(TextureManager::GetInstance().getTexture(files[filesNames[i]].asString()));
                    }

                    setSprite(m_textures[0]);
                    m_mulitiTextureAtlas = true;
                }
                else
                {
                    setSprite(files[filesNames[0]].asString());
                }

                //LOG_DEBUG("file in file: ", file.asString());
            }

            Json::Value animations = root["animations"];

            if(!animations.empty())
            {
                std::vector<aex::string> animationNames = animations.getMemberNames();

                if(animationNames.size() > 1)
                {
                    for(uint32_t i = 0; i < animationNames.size(); ++i)
                    {
                        Json::Value animationType = animations[animationNames[i]];
                        AnimationTypeInfo newAnimtype(animationNames[i], animationType["startFrame"].asInt(), animationType["endFrame"].asInt());
                        m_animationsInfo.emplace(animationNames[i], newAnimtype);
                    }

                    setSprite(m_textures[0]);
                    m_multiAnimationTypes = true;
                }
                else
                {
                    setAnimationType(animationNames[0]);
                }

                //LOG_DEBUG("file in file: ", file.asString());
            }

            Json::Value frames = root["frames"];
            std::vector<aex::string> frameNames = frames.getMemberNames();
            //LOG_DEBUG(root["frames"].asString(), "::::::::::");

            for(Json::ArrayIndex index = 0; index < frameNames.size(); ++index)
            {
                // LOG_DEBUG(frameNames[index]);
                Json::Value frame = frames[frameNames[index]]["frame"];
                Json::Value page = frames[frameNames[index]]["page"];

                if(m_Sprite)
                {

                    int pageNmb = 0;

                    if(!page.empty())
                    {
                        pageNmb = page.asInt();
                    }

                    float width = 0.0f;

                    if(!frame["e"].empty())
                    {
                        width = frame["e"].asFloat();
                    }
                    else if(!frame["w"].empty())
                    {
                        width = frame["w"].asFloat();
                    }

                    if(fileWidth == 0 && fileHeight == 0)
                    {
                        m_Sprite->bind();
                        m_Sprite->unbind();
                        fileWidth = (float) m_Sprite->getSizeW();
                        fileHeight = (float) m_Sprite->getSizeW();
                    }

                    SpriteFrame newFrameInfo(index, pageNmb, width / fileWidth, frame["h"].asFloat() / fileHeight, frame["x"].asFloat() / fileWidth, frame["y"].asFloat() / fileHeight);
                    m_framesInfo.push_back(newFrameInfo);
                }
            }

            Json::Value fps = root["fps"];

            if(!fps.empty())
            {
                m_fps = fps.asInt();
                // LOG_DEBUG("Duration in file: ", m_duration);
            }

            Json::Value duration = root["duration"];

            if(!duration.empty())
            {
                m_duration = duration.asFloat();
                // LOG_DEBUG("Duration in file: ", m_duration);
            }


            Json::Value texsize = root["texture_size"];

            if(!texsize.empty())
            {
                texsize.asFloat();
                //LOG_DEBUG("file in file: ", file.asString());
            }


            m_frames = m_framesInfo.size();

            if(m_duration == 0.0f)
            {
                m_frameduration = 1.0f / (float) m_fps;
            }
            else
            {
                m_frameduration = m_duration / (float) m_frames;
            }

        }
        else
        {
            LOG_DEBUG("Failed to open configuration file.");
        }
    }

    void SpriteAnimated::setFramesInfo(std::vector<SpriteFrame> framesInfo)
    {
        m_framesInfo = framesInfo;
    }

    std::vector<SpriteFrame> SpriteAnimated::getFramesInfo() const
    {
        return m_framesInfo;
    }

    void SpriteAnimated::setFrameUniformSize(bool frameUniformSize)
    {
        m_frameUniformSize = frameUniformSize;
    }

    bool SpriteAnimated::isFrameUniformSize() const
    {
        return m_frameUniformSize;
    }

    void SpriteAnimated::setTime(float time)
    {
        m_time = time;
        calculateFrame();
    }

    void SpriteAnimated::reset()
    {
        m_time = 0.0f;
        calculateFrame();
    }

    float SpriteAnimated::getTime() const
    {
        return m_time;
    }

    void SpriteAnimated::setFrameduration(float frameduration)
    {
        m_frameduration = frameduration;
    }

    float SpriteAnimated::getFrameduration() const
    {
        return m_frameduration;
    }

    void SpriteAnimated::setDuration(float duration)
    {
        m_duration = duration;
    }

    float SpriteAnimated::getDuration() const
    {
        return m_duration;
    }

    void SpriteAnimated::setCurrenFrame(int currenFrame)
    {
        m_currenFrame = currenFrame;
    }

    int SpriteAnimated::getCurrenFrame() const
    {
        return m_currenFrame;
    }

    void SpriteAnimated::setFrames(int frames)
    {
        m_frames = frames;
    }

    int SpriteAnimated::getFrames() const
    {
        return m_frames;
    }

    aex::string SpriteAnimated::getAnimationType() const
    {
        return m_animationType;
    }

    void SpriteAnimated::setCurrentAnimationType(AnimationTypeInfo currentAnimationType)
    {
        m_currentAnimationType = currentAnimationType;
    }

    AnimationTypeInfo SpriteAnimated::getCurrentAnimationType() const
    {
        return m_currentAnimationType;
    }

    void SpriteAnimated::setSpeed(float speed)
    {
        m_speed = speed;
    }

    float SpriteAnimated::getSpeed() const
    {
        return m_speed;
    }



}
