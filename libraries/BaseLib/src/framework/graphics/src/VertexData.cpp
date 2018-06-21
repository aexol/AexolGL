#include "../include/VertexDataInternal.hpp"
#include <aex/VertexData.hpp>
#include <aex/GPUApi.hpp>
#include <aex/Exceptions.hpp>

namespace aex
{
    VertexData::VertexData() :
        m_impl{nullptr, [](void *p){UNUSED(p);}},
        m_data{nullptr, [](void *p){UNUSED(p);}},
        m_dataCopy{[](void *p) {UNUSED(p); return nullptr;}},
        m_dataAccess{[](void *p) {UNUSED(p); return nullptr;}},
        m_bitSize{0}
        {
        }
    VertexData::VertexData(void* data, const AttribList& attribList, const DeleterType& dataCleanup, const DataCopy& dataCopy, const Data& dataAccess, std::size_t bitSize) :
        m_impl {nullptr, [](void* p)
    {
        freeInternalVertexData(p);
    }
           },
           m_data {data, dataCleanup},
           m_attribList {attribList},
           m_dataCopy {dataCopy},
           m_dataAccess(dataAccess),
           m_bitSize(bitSize),
           m_ready(false)
    {
        try{
            m_impl.reset(makeInternalVertexData());
        } catch(UninitializedContextError& e)
        {
            m_impl.reset(nullptr);
        }
    }
    VertexData::VertexData(const VertexData& vd):
        m_impl {copyInternalVertexData(vd.m_impl.get()), vd.m_impl.get_deleter()},
    m_data {nullptr, vd.m_data.get_deleter()},
    m_attribList {vd.m_attribList},
    m_dataCopy {vd.m_dataCopy},
    m_dataAccess(vd.m_dataAccess),
    m_bitSize(vd.m_bitSize),
    m_ready(false)
    {
        std::lock_guard<std::mutex> lock {vd.m_lock};
        m_data.reset(vd.m_dataCopy(vd.m_data.get()));
    }
    VertexData&
    VertexData::operator=(const VertexData& vd)
    {
        {
            Impl implCopy{copyInternalVertexData(vd.m_impl.get()), vd.m_impl.get_deleter()};
            DataCache cacheCopy{nullptr, vd.m_data.get_deleter()};
            {
                std::lock_guard<std::mutex> lock {vd.m_lock};
                cacheCopy.reset(vd.m_dataCopy(vd.m_data.get()));
            }
            m_impl.swap(implCopy);
            m_data.swap(cacheCopy);
        }
        m_attribList = vd.m_attribList;
        m_dataCopy = vd.m_dataCopy;
        m_dataAccess = vd.m_dataAccess;
        m_bitSize  = vd.m_bitSize;
        m_ready.store(false);
        return *this;
    }
    VertexData::VertexData(VertexData&& vd) :
        m_impl(std::move(vd.m_impl)),
        m_data(std::move(vd.m_data)),
        m_attribList(std::move(vd.m_attribList)),
        m_dataCopy(std::move(vd.m_dataCopy)),
        m_dataAccess(std::move(vd.m_dataAccess)),
        m_bitSize(vd.m_bitSize),
        m_ready(vd.m_ready.load())
    {}
    VertexData&
    VertexData::operator=(VertexData&& vd)
    {
        if(this != &vd)
        {
            Impl t_impl(std::move(vd.m_impl));
            m_impl.swap(t_impl);
            {
                std::lock_guard<std::mutex> lock{vd.m_lock};
                DataCache t_cache(std::move(vd.m_data));
                m_data.swap(t_cache);
            }
            m_attribList  = std::move(vd.m_attribList);
            m_dataCopy = std::move(vd.m_dataCopy);
            m_dataAccess = std::move(vd.m_dataAccess);
            m_bitSize = vd.m_bitSize;
            m_ready.store(vd.m_ready.load());
        }
        return *this;
    }

    VertexData*
    VertexData::createVertexData(const std::vector<short>& data, const aex::string& name, size_t components)
    {
        return new VertexData(makeVertexData(data, name, components));
    }
    VertexData
    VertexData::makeVertexData(const std::vector<short>& data, const aex::string& name, size_t components)
    {
        using namespace api_detail;
        AttribList attribList;

        switch(components)
        {
        case 1:
            attribList.push_back(std::make_pair(name, DataFormatCommon {DataFormat<short>{}, 0, 0}));
            break;

        case 2:
            attribList.push_back(std::make_pair(name, DataFormatCommon {DataFormat<short[2]>{}, 0, 0}));
            break;

        case 3:
            attribList.push_back(std::make_pair(name, DataFormatCommon {DataFormat<short[3]>{}, 0, 0}));
            break;

        case 4:
            attribList.push_back(std::make_pair(name, DataFormatCommon {DataFormat<short[4]>{}, 0, 0}));
            break;

        default:
            throw std::runtime_error {"Unsupported data format."};
        }

        return internalCreate(data, attribList);
    }

    VertexData*
    VertexData::createVertexData(const std::vector<aex::math::Vector3>& data, const aex::string& name)
    {
        return new VertexData(makeVertexData(data, name));
    }

    VertexData
    VertexData::makeVertexData(const std::vector<aex::math::Vector3>& data, const aex::string& name)
    {
        using namespace api_detail;
        AttribList attribList;
        attribList.push_back(std::make_pair(name, DataFormatCommon {DataFormat<aex::math::Vector3>{}, 0, 0}));
        return internalCreate(data, attribList);
    }

    VertexData
    VertexData::makeVertexData(const std::vector<aex::math::Vector2>& data, const aex::string& name)
    {
        using namespace api_detail;
        AttribList attribList;
        attribList.push_back(std::make_pair(name, DataFormatCommon {DataFormat<aex::math::Vector2>{}, 0, 0}));
        return internalCreate(data, attribList);
    }

    VertexData*
    VertexData::createVertexData(const std::vector<float>& data, const aex::string& name, size_t components)
    {
        return new VertexData(makeVertexData(data, name, components));
    }

    VertexData
    VertexData::makeVertexData(const std::vector<float>& data, const aex::string& name, size_t components)
    {
        using namespace api_detail;
        AttribList attribList;

        switch(components)
        {
        case 1:
            attribList.push_back(std::make_pair(name, DataFormatCommon {DataFormat<float>{}, 0, 0}));
            break;

        case 2:
            attribList.push_back(std::make_pair(name, DataFormatCommon {DataFormat<float[2]>{}, 0, 0}));
            break;

        case 3:
            attribList.push_back(std::make_pair(name, DataFormatCommon {DataFormat<float[3]>{}, 0, 0}));
            break;

        case 4:
            attribList.push_back(std::make_pair(name, DataFormatCommon {DataFormat<float[4]>{}, 0, 0}));
            break;

        default:
            throw std::runtime_error {"Unsupported data format."};
        }

        return internalCreate(data, attribList);
    }
    void
    VertexData::init() const
    {
        if(!m_ready.load(std::memory_order_relaxed))
        {
            std::lock_guard<std::mutex> lock(m_lock);
            if(!m_ready.load(std::memory_order_relaxed))
            {
                if(!m_impl)
                    throw InvalidBufferError("Can not enable buffer. Buffer was not properly initialized.");
                initInternalVertexData(m_impl.get(), m_dataAccess(m_data.get()), m_bitSize);
                m_ready.store(true, std::memory_order_release);
            }
        }
    }
    void
    VertexData::onContextDirty()
    {
        std::lock_guard<std::mutex> lock(m_lock);
        m_ready.store(false, std::memory_order_relaxed);
    }
    void
    VertexData::enable() const
    {
        init();
        enableInternalVertexData(m_impl.get());
    }
    void
    VertexData::disable() const
    {
        init();
        disableInternalVertexData(m_impl.get());
    }

    VertexData::operator bool() const
    {
        return (bool)m_impl;
    }

    const VertexData::AttribList&
    VertexData::getAttribs() const
    {
        return m_attribList;
    }

    /*************************************************************
     * IndexData
     * **********************************************************/

    IIndexBuffer::~IIndexBuffer(){}

    IndexData::~IndexData(){}

    IndexData::IndexData() :
        m_impl{nullptr,[](void *p){freeInternalIndexData(p);}},
        m_data {nullptr},
        m_mode {0},
        m_ready(false)
    {}

    IndexData::IndexData(IIndexBuffer* data, const std::size_t mode) :
        m_impl{nullptr, [](void* p){freeInternalIndexData(p);}},
        m_data {data},
        m_mode {mode},
        m_ready(false)
    {
        try{
            m_impl.reset(makeInternalIndexData(mode, data->elementCount()));
        } catch(UninitializedContextError& e)
        {
            LOG(e.what());
        }
    }
    IndexData::IndexData(const IndexData& id):
        m_impl{copyInternalIndexData(id.m_impl.get()), [](void* p){freeInternalIndexData(p);}},
        m_data {nullptr},
        m_mode {id.m_mode},
        m_ready(false)
    {
        std::lock_guard<std::mutex> lock{id.m_lock};
        if(id.m_data)
            m_data.reset(id.m_data->copy());
    }

    IndexData&
    IndexData::operator=(const IndexData& id)
    {
        m_impl.reset(copyInternalIndexData(id.m_impl.get()));
        m_mode = id.m_mode;
        m_ready = false;
        std::lock_guard<std::mutex> lock{id.m_lock};
        if(id.m_data)
            m_data.reset(id.m_data->copy());
        return *this;
    }

    IndexData::IndexData(IndexData&& id):
        IndexData()
    {
        *this = std::move(id);
    }

    IndexData&
    IndexData::operator=(IndexData&& id)
    {
        if(this != &id)
        {
            Impl t_impl(std::move(id.m_impl));
            m_impl.swap(t_impl);
            std::swap(m_mode, id.m_mode);
            std::lock_guard<std::mutex> lock{id.m_lock};
            m_data.swap(id.m_data);
            m_ready = static_cast<bool>(id.m_ready);
        }
        return *this;
    }

    void
    IndexData::enable() const
    {
        init();
        enableInternalIndexData(m_impl.get());
    }

    IndexData::operator bool() const
    {
        return (bool)m_impl;
    }

    void
    IndexData::init() const
    {
        if(!m_ready.load(std::memory_order_relaxed))
        {
            std::lock_guard<std::mutex> lock(m_lock);
            if(!m_ready.load(std::memory_order_relaxed))
            {
                if(!m_impl)
                    throw InvalidBufferError("Can not enable buffer. Buffer was not properly initialized.");
                initInternalIndexData(m_impl.get(), m_data->data(), m_data->bitSize() * m_data->elementCount());
                m_ready.store(true, std::memory_order_release);
            }
        }
    }
    void
    IndexData::onContextDirty()
    {
        std::lock_guard<std::mutex> lock(m_lock);
        m_ready.store(false, std::memory_order_relaxed);
    }

    IndexData
    IndexData::createIndexData(const std::vector<uint16_t>& tris, size_t type)
    {
        switch(type)
        {
            case TRIANGLES:
                return createTriangles(tris);
            case TRIANGLE_STIRP:
                return createTraingleStrip(tris);
            case LINES:
                return createLines(tris);
            case LINE_STRIP :
                return createLineStrip(tris);

            default:
                LOG_ERR("Wrong index type!");
                return createTriangles(tris);
        }

    }

    IndexData
    IndexData::createIndexData(const std::vector<uint32_t>& tris, size_t type)
    {
        switch(type)
        {
            case TRIANGLES:
                return createTriangles(tris);
            case TRIANGLE_STIRP:
                return createTraingleStrip(tris);
            case LINES:
                return createLines(tris);
            case LINE_STRIP :
                return createLineStrip(tris);

            default:
                LOG_ERR("Wrong index type!");
                return createTriangles(tris);
        }
    }

    IndexData
    IndexData::createIndexData (const std::vector<SPoint3D>& tris, size_t type)
    {
                switch(type)
        {
            case TRIANGLES:
                return createTriangles(tris);
            case TRIANGLE_STIRP:
                return createTriangleStrip(tris);
            case LINES:
                return createLines(tris);
            case LINE_STRIP :
                return createLineStrip(tris);

            default:
                LOG_ERR("Wrong index type!");
                return createTriangles(tris);
        }
    }

    IndexData
    IndexData::createIndexData(const std::vector<IPoint3D>& tris, size_t type)
    {
        switch(type)
        {
            case TRIANGLES:
                return createTriangles(tris);
            case TRIANGLE_STIRP:
                return createTriangleStrip(tris);
            case LINES:
                return createLines(tris);
            case LINE_STRIP :
                return createLineStrip(tris);

            default:
                LOG_ERR("Wrong index type!");
                return createTriangles(tris);
        }
    }

    IndexData
    IndexData::createTriangles(const std::vector<uint16_t>& tris)
    {
        return internalCreate(tris, TRIANGLES);
    }

    IndexData
    IndexData::createTriangles(const std::vector<uint32_t>& tris)
    {
        return internalCreate(tris, TRIANGLES);
    }

    IndexData
    IndexData::createTriangles(const std::vector<SPoint3D>& tris)
    {
        return internalCreate(tris, TRIANGLES);
    }

    IndexData
    IndexData::createTriangles(const std::vector<IPoint3D>& tris)
    {
        return internalCreate(tris, TRIANGLES);
    }

    IndexData
    IndexData::createTraingleStrip(const std::vector<uint16_t>& tris)
    {
        return internalCreate(tris, TRIANGLE_STIRP);
    }

    IndexData
    IndexData::createTraingleStrip(const std::vector<uint32_t>& tris)
    {
        return internalCreate(tris, TRIANGLE_STIRP);
    }

    IndexData
    IndexData::createTriangleStrip(const std::vector<SPoint3D>& tris)
    {
        return internalCreate(tris, TRIANGLE_STIRP);
    }

    IndexData
    IndexData::createTriangleStrip(const std::vector<IPoint3D>& tris)
    {
        return internalCreate(tris, TRIANGLE_STIRP);
    }

    IndexData
    IndexData::createLines(const std::vector<uint16_t>& lines)
    {
        return internalCreate(lines, LINES);
    }

    IndexData
    IndexData::createLines(const std::vector<uint32_t>& lines)
    {
        return internalCreate(lines, LINES);
    }

    IndexData
    IndexData::createLines(const std::vector<SPoint3D>& lines)
    {
        return internalCreate(lines, LINES);
    }

    IndexData
    IndexData::createLines(const std::vector<IPoint3D>& lines)
    {
        return internalCreate(lines, LINES);
    }

    IndexData
    IndexData::createLineStrip(const std::vector<uint16_t>& lines)
    {
        return internalCreate(lines, LINE_STRIP);
    }

    IndexData
    IndexData::createLineStrip(const std::vector<uint32_t>& lines)
    {
        return internalCreate(lines, LINE_STRIP);
    }

        IndexData
    IndexData::createLineStrip(const std::vector<SPoint3D>& lines)
    {
        return internalCreate(lines, LINE_STRIP);
    }

    IndexData
    IndexData::createLineStrip(const std::vector<IPoint3D>& lines)
    {
        return internalCreate(lines, LINE_STRIP);
    }

    VertexData
    PositionNormalUV::createVertexDataFromPositionNormalUV(const std::vector<PositionNormalUV>& data,
            const aex::string& positionAttribName,
            const aex::string& normalAttribName,
            const aex::string& uvAttribName
            )
    {
        std::array<const aex::string, 3> names{{
            aex::string(positionAttribName),
            aex::string(normalAttribName),
            aex::string(uvAttribName)
        }};
        return VertexData::makeVertexData(data, LayoutType{std::move(names)});;
    }

    VertexData
    PositionNormal::createVertexDataFromPositionNormal(const std::vector<PositionNormal>& data,
            const aex::string& positionAttribName,
            const aex::string& normalAttribName
            )
    {
        std::array<const aex::string, 2> names{{
            aex::string(positionAttribName),
            aex::string(normalAttribName)
        }};
        return VertexData::makeVertexData(data, LayoutType{std::move(names)});;
    }
} // end of naemspace aex
