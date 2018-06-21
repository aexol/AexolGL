#ifndef __AEX_VERTEXDATA_IPP
#define __AEX_VERTEXDATA_IPP
#include <aex/VertexData.hpp>

namespace aex
{
    namespace api_detail
    {
        /* DataFormat specialization for specific data types */
        template<> struct DataFormat<std::array<float, 4>>
        {
            static constexpr std::size_t size = sizeof(std::array<float, 4>);
            static constexpr std::size_t count = 4;
            static constexpr PrimitiveType type = PrimitiveType::FLOAT;
        };
        template<> struct DataFormat<float[4]>
        {
            static constexpr std::size_t size = sizeof(float[4]);
            static constexpr std::size_t count = 4;
            static constexpr PrimitiveType type = PrimitiveType::FLOAT;
        };
        template<> struct DataFormat<std::array<float, 3>>
        {
            static constexpr std::size_t size = sizeof(std::array<float, 3>);
            static constexpr std::size_t count = 3;
            static constexpr PrimitiveType type = PrimitiveType::FLOAT;
        };
        template<> struct DataFormat<float[3]>
        {
            static constexpr std::size_t size = sizeof(float[3]);
            static constexpr std::size_t count = 3;
            static constexpr PrimitiveType type = PrimitiveType::FLOAT;
        };
        template<> struct DataFormat<std::array<float, 2>>
        {
            static constexpr std::size_t size = sizeof(std::array<float, 2>);
            static constexpr std::size_t count = 2;
            static constexpr PrimitiveType type = PrimitiveType::FLOAT;
        };
        template<> struct DataFormat<float[2]>
        {
            static constexpr std::size_t size = sizeof(float[2]);
            static constexpr std::size_t count = 2;
            static constexpr PrimitiveType type = PrimitiveType::FLOAT;
        };
        template<> struct DataFormat<std::array<short, 4>>
        {
            static constexpr std::size_t size = sizeof(std::array<short, 4>);
            static constexpr std::size_t count = 4;
            static constexpr PrimitiveType type = PrimitiveType::SHORT;
        };
        template<> struct DataFormat<short[4]>
        {
            static constexpr std::size_t size = sizeof(short[4]);
            static constexpr std::size_t count = 4;
            static constexpr PrimitiveType type = PrimitiveType::SHORT;
        };
        template<> struct DataFormat<std::array<short, 3>>
        {
            static constexpr std::size_t size = sizeof(std::array<short, 3>);
            static constexpr std::size_t count = 3;
            static constexpr PrimitiveType type = PrimitiveType::SHORT;
        };
        template<> struct DataFormat<short[3]>
        {
            static constexpr std::size_t size = sizeof(short[3]);
            static constexpr std::size_t count = 3;
            static constexpr PrimitiveType type = PrimitiveType::SHORT;
        };
        template<> struct DataFormat<std::array<short, 2>>
        {
            static constexpr std::size_t size = sizeof(short[2]);
            static constexpr std::size_t count = 2;
            static constexpr PrimitiveType type = PrimitiveType::SHORT;
        };
        template<> struct DataFormat<short[2]>
        {
            static constexpr std::size_t size = sizeof(short[2]);
            static constexpr std::size_t count = 2;
            static constexpr PrimitiveType type = PrimitiveType::SHORT;
        };
        template<> struct DataFormat<std::array<unsigned short, 4>>
        {
            static constexpr std::size_t size = sizeof(std::array<unsigned short, 4>);
            static constexpr std::size_t count = 4;
            static constexpr PrimitiveType type = PrimitiveType::UNSIGNED_SHORT;
        };
        template<> struct DataFormat<unsigned short[4]>
        {
            static constexpr std::size_t size = sizeof(unsigned short[4]);
            static constexpr std::size_t count = 4;
            static constexpr PrimitiveType type = PrimitiveType::UNSIGNED_SHORT;
        };
        template<> struct DataFormat<std::array<unsigned short, 3>>
        {
            static constexpr std::size_t size = sizeof(std::array<unsigned short, 3>);
            static constexpr std::size_t count = 3;
            static constexpr PrimitiveType type = PrimitiveType::UNSIGNED_SHORT;
        };
        template<> struct DataFormat<unsigned short[3]>
        {
            static constexpr std::size_t size = sizeof(unsigned short[3]);
            static constexpr std::size_t count = 3;
            static constexpr PrimitiveType type = PrimitiveType::UNSIGNED_SHORT;
        };
        template<> struct DataFormat<std::array<unsigned short, 2>>
        {
            static constexpr std::size_t size = sizeof(unsigned short[2]);
            static constexpr std::size_t count = 2;
            static constexpr PrimitiveType type = PrimitiveType::UNSIGNED_SHORT;
        };
        template<> struct DataFormat<unsigned short[2]>
        {
            static constexpr std::size_t size = sizeof(unsigned short[2]);
            static constexpr std::size_t count = 2;
            static constexpr PrimitiveType type = PrimitiveType::UNSIGNED_SHORT;
        };
        template<> struct DataFormat<aex::math::Vector3>
        {
            static constexpr std::size_t size = sizeof(aex::math::Vector3);
            static constexpr std::size_t count = 3;
            static constexpr PrimitiveType type = PrimitiveType::FLOAT;
        };
        template<> struct DataFormat<aex::math::Vector2>
        {
            static constexpr std::size_t size = sizeof(aex::math::Vector2);
            static constexpr std::size_t count = 2;
            static constexpr PrimitiveType type = PrimitiveType::FLOAT;
        };
        template<> struct DataFormat<float>
        {
            static constexpr std::size_t size = sizeof(float);
            static constexpr std::size_t count = 1;
            static constexpr PrimitiveType type = PrimitiveType::FLOAT;
        };
        template<> struct DataFormat<short>
        {
            static constexpr std::size_t size = sizeof(short);
            static constexpr std::size_t count = 1;
            static constexpr PrimitiveType type = PrimitiveType::SHORT;
        };
        template<> struct DataFormat<unsigned short>
        {
            static constexpr std::size_t size = sizeof(unsigned short);
            static constexpr std::size_t count = 1;
            static constexpr PrimitiveType type = PrimitiveType::UNSIGNED_SHORT;
        };
        template<class T>
        constexpr DataFormatCommon::DataFormatCommon(const DataFormat<T>& df, std::size_t sstride, std::size_t ooffset):
                size(std::remove_reference<typename std::remove_cv<decltype(df)>::type>::type::size),
                count(std::remove_reference<typename std::remove_cv<decltype(df)>::type>::type::count),
                stride(sstride),
                offset(ooffset),
                type(std::remove_reference<typename std::remove_cv<decltype(df)>::type>::type::type)
            {}

    }
    template<> struct is_supported_primitive_type<std::array<float, 4>> : std::true_type {};
    template<> struct is_supported_primitive_type<float[4]> : std::true_type {};
    template<> struct is_supported_primitive_type<std::array<float, 3>> : std::true_type {};
    template<> struct is_supported_primitive_type<float[3]> : std::true_type {};
    template<> struct is_supported_primitive_type<std::array<float, 2>> : std::true_type {};
    template<> struct is_supported_primitive_type<float[2]> : std::true_type {};
    template<> struct is_supported_primitive_type<std::array<short, 4>> : std::true_type {};
    template<> struct is_supported_primitive_type<short[4]> : std::true_type {};
    template<> struct is_supported_primitive_type<std::array<short, 3>> : std::true_type {};
    template<> struct is_supported_primitive_type<short[3]> : std::true_type {};
    template<> struct is_supported_primitive_type<std::array<short, 2>> : std::true_type {};
    template<> struct is_supported_primitive_type<short[2]> : std::true_type {};
    template<> struct is_supported_primitive_type<std::array<unsigned short, 4>> : std::true_type {};
    template<> struct is_supported_primitive_type<unsigned short[4]> : std::true_type {};
    template<> struct is_supported_primitive_type<std::array<unsigned short, 3>> : std::true_type {};
    template<> struct is_supported_primitive_type<unsigned short[3]> : std::true_type {};
    template<> struct is_supported_primitive_type<std::array<unsigned short, 2>> : std::true_type {};
    template<> struct is_supported_primitive_type<unsigned short[2]> : std::true_type {};
    template<> struct is_supported_primitive_type<aex::math::Vector3> : std::true_type {};
    template<> struct is_supported_primitive_type<aex::math::Vector2> : std::true_type {};
    template<> struct is_supported_primitive_type<float> : std::true_type {};
    template<> struct is_supported_primitive_type<short> : std::true_type {};
    template<class... Args>
    template<size_t idx, class T>
    std::pair<T, std::string> Layout<Args...>::getInfo() const
    {
        return std::make_pair(T {}, m_tags[idx]);
    }
    template<class... Args>
    constexpr size_t Layout<Args...>::size() const
    {
        return sizeof...(Args);
    }

    template <class T, class... Args>
    VertexData* VertexData::createVertexData(const std::vector<T>& data, Layout<Args...> layout)
    {
        return new VertexData(makeVertexData(data, layout));
    }

    template <class T, class... Args>
    VertexData VertexData::makeVertexData(const std::vector<T>& data, Layout<Args...> layout)
    {
        static_assert(std::is_standard_layout<T>::value, "Unsupported data format.");
        AttribList attributeList = api_detail::buildOffsets(layout, sizeof(T));
        return internalCreate(data, attributeList);
    }

    template<class T>
    VertexData VertexData::internalCreate(const std::vector<T>& data, const AttribList& attributeList)
    {
        typedef std::vector<T> Container;
        Container* dataCpy = new Container(data);
        return VertexData(dataCpy,
                              attributeList,
                              [](void* p)
        {
            delete static_cast<Container*>(p);
        },
        [](const void* p)
        {
            Container* copy = nullptr;

            if(p)
            {
                copy = new Container(*static_cast<const Container*>(p));
            }

            return copy;
        },
        [](const void* p)
        {
            const void* pr = nullptr;

            if(p)
            {
                pr = static_cast<const Container*>(p)->data();
            }

            return pr;
        },
        sizeof(typename std::remove_reference<Container>::type::value_type)*data.size());
    }

    template<class T, size_t SIZE>
    VertexData VertexData::internalCreate(const std::array<T, SIZE>& data, const AttribList& attributeList)
    {
        typedef std::array<T, SIZE> Container;
        Container* dataCpy = new Container(data);
        return VertexData(dataCpy,
                              attributeList,
                              [](void* p)
        {
            delete static_cast<Container*>(p);
        },
        [](const void* p)
        {
            Container* copy = nullptr;

            if(p)
            {
                copy = new Container(*static_cast<Container*>(p));
            }

            return copy;
        },
        [](const void* p)
        {
            const void* pr = nullptr;

            if(p)
            {
                pr = static_cast<const Container*>(p)->data();
            }

            return pr;
        },
        sizeof(typename std::remove_reference<Container>::type::value_type)*data.size());
    }

    template<class T, size_t SIZE>
    VertexData* VertexData::createVertexData(const std::vector<Point<T, SIZE>>& data, const std::string& name)
    {
        return new VertexData(makeVertexData(data, name));
    }

    template<class T, size_t SIZE>
    VertexData VertexData::makeVertexData(const std::vector<Point<T, SIZE>>& data, const std::string& name)
    {
        using namespace api_detail;
        AttribList attribList;

        if(SIZE == 1)
        {
            attribList.push_back(std::make_pair(name, DataFormatCommon {DataFormat<T>{}, 0, 0}));
        }
        else
        {
            attribList.push_back(std::make_pair(name, DataFormatCommon {DataFormat<T[SIZE]>{}, 0, 0}));
        }

        return internalCreate(data, attribList);
    }

    namespace __vd_detail
    {
        template<class T>
        struct ElementCountMult
        {
            static constexpr std::size_t mul = 1;
        };
        template<>
        struct ElementCountMult<SPoint3D>
        {
            static constexpr std::size_t mul = 3;
        };
        template<>
        struct ElementCountMult<IPoint3D>
        {
            static constexpr std::size_t mul = 3;
        };
        template<class T> class IndexBuffer;

        template<class T, class _Alloc>
        class IndexBuffer<std::vector<T, _Alloc>> : public IIndexBuffer
        {
            public:
                typedef T value_type;
                typedef std::vector<value_type> value_array;
                static constexpr std::size_t BIT_SIZE = sizeof(value_type);

                IndexBuffer(const value_array& arr) : m_buf(arr){}

                IIndexBuffer* copy() const
                {
                    return new IndexBuffer<value_array>{m_buf};
                }

                std::size_t elementCount() const 
                {
                    return m_buf.size() * ElementCountMult<T>::mul;
                }

                std::size_t bitSize() const
                {
                    return BIT_SIZE;
                }

                const void* data() const
                {
                    return m_buf.data();
                }
            private:
                    value_array m_buf;
        };
    }

    template<class T>
    IndexData IndexData::internalCreate(const std::vector<T>& tris, const std::size_t mode)
    {
        __vd_detail::IndexBuffer<std::vector<T>>* data = new __vd_detail::IndexBuffer<std::vector<T>>{tris};
        return IndexData
        {
            data,
            mode
        };
    }
    namespace api_detail
    {
        template <typename R, typename C, typename A1> R get_return_type(R(C::*)(A1));

        template<std::size_t I, class... Args>
        inline typename std::enable_if<I == sizeof...(Args), AttribList>::type buildOffsets(const Layout<Args...>& l, std::size_t stride, size_t currentAlignment)
        {
            AttribList ret;
            return ret;
        }
        template<std::size_t I, class... Args>
        inline typename std::enable_if < I <sizeof...(Args), AttribList>::type buildOffsets(const Layout<Args...>& l, std::size_t stride, size_t currentAlignment)
        {
            auto pair = l.template getInfo<I>();
            typedef decltype(pair) PairType;
            typedef typename PairType::first_type VertexDataType;
            static_assert(is_supported_primitive_type<VertexDataType>::value, "Invalid data format.");
            size_t sizeOfMember = sizeof(VertexDataType);
            size_t alignmentOfMember = std::alignment_of<VertexDataType>::value;
            currentAlignment +=
                alignmentOfMember ^ (alignmentOfMember - currentAlignment % alignmentOfMember);
            AttribList ret = buildOffsets < I + 1 > (l, stride, currentAlignment + sizeOfMember);

            if(!ret.empty())
            {
                stride = ret.back().second.stride;
            } else if(stride == 0)
            {
                stride = currentAlignment + sizeOfMember;
            }

            DataFormatCommon df(DataFormat<VertexDataType> {}, stride, currentAlignment);
            ret.push_front(std::make_pair(pair.second, df));
            return ret;
        }
    }
}

#endif
