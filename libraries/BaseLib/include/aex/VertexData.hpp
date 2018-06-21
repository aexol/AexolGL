#ifndef __AEX_VERTEXDATA_HPP
#define __AEX_VERTEXDATA_HPP
#include <aex/Vector3.hpp>
#include <aex/Point.hpp>
#include <aex/GPUApi.hpp>
#include <boost/signals2.hpp>
#include <atomic>
#include <mutex>
#include <list>
namespace aex
{
template<class... Args> class Layout;
namespace api_detail
{
enum class PrimitiveType
{
    FLOAT,
    SHORT,
    UNSIGNED_SHORT
};
        /* 
         * @brief Define how to treat data.
         *
         * Specialize DataFormat for user defined type T to be able to use it as a buffer structure for VertexData.
         * Specialization requires 3 static fields: size(size of T), count(number of elements in T) and type (what kind of primitive should data be treated as). 
         */
template<class T> struct DataFormat;

        /*
         * @brief Information about data in common format.
         *
         * Created from specialization of DataFormat to be kept in AttributeList with information needed to pass it to underlaying api 8
         */
struct DataFormatCommon
{
            /*
             * @brief Constructor
             *
             * @param df Specialized DataFormat with details about binary representation of data.
             * @param sstride Byte offset between consecutive attributes.
             * @param offset Offset from the begining of data.
             */
    template<class T>
            CONSTEXPR DataFormatCommon(const DataFormat<T>& df, std::size_t sstride, std::size_t ooffset);
            std::size_t size;
            std::size_t count;
            std::size_t stride;
            std::uintptr_t offset;
            PrimitiveType type;
};

        typedef std::pair<aex::string, DataFormatCommon> AttribType; //<! String data format pair.
        typedef std::list<AttribType> AttribList; //<! List of attributes type.

template<std::size_t I = 0, class... Args>
        inline typename std::enable_if<I == sizeof...(Args), AttribList>::type buildOffsets(const Layout<Args...>& l, size_t stride = 0, size_t currentOffset = 0); //<! Helper method to calculate offsets
template<std::size_t I = 0, class... Args>
        inline typename std::enable_if < I <sizeof...(Args), AttribList>::type buildOffsets(const Layout<Args...>& l, size_t stride = 0, size_t currentOffset = 0); //<! Helper method to calculate offsets
}
    /*
     * @brief Specialize for new primtive support.
     *
     */
template<class T> struct is_supported_primitive_type : std::false_type {};
    /*
     * @brief Class contatining information about layout of data buffer.
     *
     * Template keeping info about data layout in buffer.
     */
template<class... Args>
class Layout
{
    typedef std::tuple<Args...> TypeList;
public:
        /*
         * @brief Constructor
         *
         * @param tags Array of names for each arg in Args template argument.
         */
    explicit Layout(const std::array<const aex::string, sizeof...(Args)>& tags) : m_tags(tags) {};
    explicit Layout(std::array<const aex::string, sizeof...(Args)>&& tags) : m_tags(std::move(tags)) {};

        /*
         *  @brief Return pair of name and DataFormat information at idx.
         *
         *  Extract information at idx. Returns a pair DataFormat info and name identified by string.
         */
        template<size_t idx, class T = typename std::tuple_element<idx, TypeList>::type>
        std::pair<T, aex::string> getInfo() const;

        /*
         * @brief Return size of layout.
         *
         * @returns Layout size.
         */
        CONSTEXPR size_t size() const;
private:
    TypeList m_typeList;
    const std::array<const aex::string, sizeof...(Args)> m_tags;
    friend class VertexData;
};

class AEX_PUBLIC_API VertexData : public IOnContextDirty
{
        typedef std::function<void(void*)> DeleterType; //!< Implementation deleter type
        typedef std::unique_ptr<void, DeleterType> Impl; //!< VertexData underlaying implementation pointer
        typedef std::unique_ptr<void, DeleterType> DataCache; //!< Data cache pointer
        typedef std::function<void* (void*)> DataCopy; //!< Function type returning deep copy of arg pointer
        typedef std::function<const void* (void*)> Data; //!< Function returning pointer to raw data block.
    public:
        typedef api_detail::AttribType AttribType;
        typedef api_detail::AttribList AttribList;
        /*
         *  @brief Default constructor.
         *
         *  Creates invalid VertexData object.
         */
        VertexData();

        /*
         * @brief Copy constructor
         */
        VertexData(const VertexData& vd);

        /*
         * @brief Copy assignment
         *
         * @returns Reference to this.
         */
        VertexData& operator=(const VertexData& vd);

        /*
         * @brief Move constructor.
         *
         */
        virtual ~VertexData() {}
        VertexData(VertexData&& vd);
        VertexData& operator=(VertexData&& vd);
        /*
         * @brief Use buffer.
         *
         */
        void enable() const;
        /*
         * @brief Mark buffer as unused.
         */
        void disable() const;

        /*
         * @brief Create new VertexData
         *
         * @param data Vector of T elements to fill buffer with.
         * @param layout Layout of T.
         */
        template<class T, class... Args>
        static VertexData * createVertexData(const std::vector<T>& data, Layout<Args...> layout);

        /*
         * @brief Overloard for simple float data.
         *
         * Overload for simple float data where components is the dimension of the size of one attribute value.
         *
         * @param data Vector of data
         * @param name Name of attribute in shader
         * @param components Number of components per attribtue
         */

        static VertexData* createVertexData(const std::vector<float>& data, const aex::string& name, size_t componenets);

        /*
         * @brief Overloard for Point template.
         *
         * Overload for point template.
         *
         * @param data Vector of data
         * @param name Name of attribute in shader
         */
        template<class T, size_t SIZE>
        static VertexData* createVertexData(const std::vector<Point<T, SIZE>>& data, const aex::string& name);

        /*
         * @brief Overloard for array of vectors.
         *
         * Overload for array of vectors.
         *
         * @param data Vector of data
         * @param name Name of attribute in shader
         */
        static VertexData* createVertexData(const std::vector<aex::math::Vector3>& data, const aex::string& name);

        /*
         * @brief Overloard for simple short data.
         *
         * Overload for simple short data where components is the dimension of the size of one attribute value.
         *
         * @param data Vector of data
         * @param name Name of attribute in shader
         * @param components Number of components per attribtue
         */
        static VertexData* createVertexData(const std::vector<short>& data, const aex::string& name, size_t componenets);

        /*
         * @brief Create new VertexData
         *
         * @param data Vector of T elements to fill buffer with.
         * @param layout Layout of T.
         */
        template<class T, class... Args>
        static VertexData makeVertexData(const std::vector<T>& data, Layout<Args...> layout);

        /*
         * @brief Overloard for simple float data.
         *
         * Overload for simple float data where components is the dimension of the size of one attribute value.
         *
         * @param data Vector of data
         * @param name Name of attribute in shader
         * @param components Number of components per attribtue
         */

        static VertexData makeVertexData(const std::vector<float>& data, const aex::string& name, size_t componenets);

        /*
         * @brief Overloard for Point template.
         *
         * Overload for point template.
         *
         * @param data Vector of data
         * @param name Name of attribute in shader
         */
        template<class T, size_t SIZE>
        static VertexData makeVertexData(const std::vector<Point<T, SIZE>>& data, const aex::string& name);

        /*
         * @brief Overloard for array of vectors.
         *
         * Overload for array of vectors.
         *
         * @param data Vector of data
         * @param name Name of attribute in shader
         */
        static VertexData makeVertexData(const std::vector<aex::math::Vector3>& data, const aex::string& name);

        /*
         * @brief Overloard for array of vectors.
         *
         * Overload for array of vectors.
         *
         * @param data Vector of data
         * @param name Name of attribute in shader
         */
        static VertexData makeVertexData(const std::vector<aex::math::Vector2>& data, const aex::string& name);

        /*
         * @brief Overloard for simple short data.
         *
         * Overload for simple short data where components is the dimension of the size of one attribute value.
         *
         * @param data Vector of data
         * @param name Name of attribute in shader
         * @param components Number of components per attribtue
         */
        static VertexData makeVertexData(const std::vector<short>& data, const aex::string& name, size_t componenets);

        /*
         * @brief Is buffer valid.
         *
         * @returns True if buffer is ready to be used.
         */
        operator bool() const;
        /*
         * @brief Get information about attribute layout.
         */
        const AttribList& getAttribs() const;
protected:
    /* Private methods */
    template<class T>
    static VertexData internalCreate(const std::vector<T>& data, const AttribList& attributeList);

    template<class T, size_t SIZE>
    static VertexData internalCreate(const std::array<T, SIZE>& data, const AttribList& attributeList);

private:

    VertexData(void* data, const AttribList& attribList, const DeleterType& dataCleanup, const DataCopy& dataCopy, const Data& dataAccess, std::size_t bitSize);
    void init()const;
    void onContextDirty();

    /* Member fields */

   	Impl m_impl;
    mutable DataCache m_data;
    AttribList m_attribList;
    DataCopy m_dataCopy;
    Data m_dataAccess;
    std::size_t m_bitSize;
    mutable std::atomic<bool> m_ready;
    mutable std::mutex m_lock;
    friend class ShaderProgram;
};

class AEX_PUBLIC_API IIndexBuffer
{
    public:
        virtual IIndexBuffer* copy() const = 0;
        virtual std::size_t elementCount() const = 0;
        virtual std::size_t bitSize() const = 0;
        virtual const void* data() const = 0;
        virtual ~IIndexBuffer();
};

class AEX_PUBLIC_API IndexData : public IOnContextDirty
{
    using DeleterType = std::function<void(void*)>;
    using Impl = std::unique_ptr<void, DeleterType>;
    using DataType = std::unique_ptr<IIndexBuffer>;
public:
    static CONSTEXPR std::size_t TRIANGLES = 0x0001;
    static CONSTEXPR std::size_t TRIANGLE_STIRP = TRIANGLES << 1;
    static CONSTEXPR std::size_t LINES = TRIANGLE_STIRP << 1;
    static CONSTEXPR std::size_t LINE_STRIP = LINES << 1;
    IndexData();
    IndexData(const IndexData& id);
    IndexData& operator=(const IndexData& id);
    IndexData(IndexData&& id);
    IndexData& operator=(IndexData&& id);
    virtual ~IndexData();
    
    static IndexData createIndexData(const std::vector<uint16_t>& tris, size_t type= TRIANGLES);
    static IndexData createIndexData(const std::vector<uint32_t>& tris, size_t type= TRIANGLES);
    static IndexData createIndexData(const std::vector<SPoint3D>& tris, size_t type= TRIANGLES);
    static IndexData createIndexData(const std::vector<IPoint3D>& tris, size_t type= TRIANGLES);
    
    static IndexData createTriangles(const std::vector<uint16_t>& tris);
    static IndexData createTriangles(const std::vector<uint32_t>& tris);
    static IndexData createTriangles(const std::vector<SPoint3D>& tris);
    static IndexData createTriangles(const std::vector<IPoint3D>& tris);
    static IndexData createTraingleStrip(const std::vector<uint16_t>& tris);
    static IndexData createTraingleStrip(const std::vector<uint32_t>& tris);
    static IndexData createTriangleStrip(const std::vector<SPoint3D>& tris);
    static IndexData createTriangleStrip(const std::vector<IPoint3D>& tris);
    static IndexData createLines(const std::vector<uint16_t>& lines);
    static IndexData createLines(const std::vector<uint32_t>& lines);
    static IndexData createLines(const std::vector<SPoint3D>& tris);
    static IndexData createLines(const std::vector<IPoint3D>& tris);
    static IndexData createLineStrip(const std::vector<uint16_t>& lines);
    static IndexData createLineStrip(const std::vector<uint32_t>& lines);
    static IndexData createLineStrip(const std::vector<SPoint3D>& tris);
    static IndexData createLineStrip(const std::vector<IPoint3D>& tris);
    void enable() const;
    /*
     * @brief Is buffer valid.
     *
     * @returns True if buffer is ready to be used.
     */
    operator bool() const;
private:
    template<class T>
    static IndexData internalCreate(const std::vector<T>& tris, const std::size_t mode);
    IndexData(IIndexBuffer* data, const std::size_t mode);
    void init() const;
    void onContextDirty();
    Impl m_impl;
    DataType m_data;
    mutable std::mutex m_lock;
    std::size_t m_mode;
    mutable std::atomic<bool> m_ready;
    boost::signals2::scoped_connection m_onContextDirtyCon;
};
    /*
     * @brief Simple structure for typical vertex made from position normal and UV.
     */
    struct AEX_PUBLIC_API PositionNormalUV
{
        /*
         * @brief 3D position.
         */
        aex::math::Vector3 position;
        /*
         * @brief Normal
         */
        aex::math::Vector3 normal;
        /*
         * @brief UV
         */
        aex::math::Vector2 texCoord;
        /*
         * @brief Create VertexData buffer from vector of PositionNormalUV values.
         *
         * From vector of PositionNormalUV values create an interleaved VertexData buffer. 
         *
         * @param positionAttribName Name of attribute in shader. By default gl_Vertex.
         * @param normalAttribName Name of attribute in shader. By default gl_Normal.
         * @param uvAttribName Name of attribute in shader. By default gl_TexCoord.
         */
        static VertexData createVertexDataFromPositionNormalUV(const std::vector<PositionNormalUV>& data,
                const aex::string& positionAttribName = "gl_Vertex",
                const aex::string& normalAttribName = "gl_Normal",
                const aex::string& uvAttribName = "gl_TexCoord"
                );
        typedef Layout<aex::math::Vector3, aex::math::Vector3, aex::math::Vector2> LayoutType;
    };

    struct AEX_PUBLIC_API PositionNormal
{
        /*
         * @brief 3D position.
         */
        aex::math::Vector3 position;
        /*
         * @brief Normal
         */
        aex::math::Vector3 normal;
        /*
         * @brief Create VertexData buffer from vector of PositionNormalUV values.
         *
         * From vector of PositionNormalUV values create an interleaved VertexData buffer. 
         *
         * @param positionAttribName Name of attribute in shader. By default gl_Vertex.
         * @param normalAttribName Name of attribute in shader. By default gl_Normal.
         */
        static VertexData createVertexDataFromPositionNormal(const std::vector<PositionNormal>& data,
                const aex::string& positionAttribName = "gl_Vertex",
                const aex::string& normalAttribName = "gl_Normal"
                );
        typedef Layout<aex::math::Vector3, aex::math::Vector3> LayoutType;
    };
}
#include <aex/VertexData.ipp>
#endif
