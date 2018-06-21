#ifndef __AEX_GLUNIFORMS_HPP
#define __AEX_GLUNIFORMS_HPP
#include <aex/Common.hpp>
#include <cstdint>
#include <string>
#include <functional>

namespace aex
{
    /*
     * @brief State of uniform in program
     */
    class UniformInfo
    {
        public:
            /*
             * @brief Supported uniform types.
             */
            enum Type
            {
                BOOL,
                FLOAT,
                FLOAT2,
                FLOAT3,
                FLOAT4,
                INT,
                INT2,
                INT3,
                INT4,
                VEC1F,
                VEC2F,
                VEC3F,
                VEC4F,
                VEC1I,
                VEC2I,
                VEC3I,
                VEC4I,
                MAT2F,
                MAT3F,
                MAT4F,
                UNSET
            };

            /*
             * @brief Empty uniform object.
             */
            UniformInfo();

            /*
             * Create UniformInfo for program.
             */
            UniformInfo(uint32_t progId, const aex::string& name, Type type = UNSET);
            UniformInfo(uint32_t progId, uint32_t location, Type type = UNSET);
            virtual ~UniformInfo();

            /*
             * @brief Refreash uniform value on GPU.
             */
            void set();

            /*
             * @brief Get type of this uniform.
             */
            Type getType() const;

        protected:
            int32_t location;
            uint32_t program;
            Type type;
            bool dirty;
        protected:
            virtual void setOnDirty() = 0;

    };
    class UnsetUniformInfo : public UniformInfo
    {
        public:
            UnsetUniformInfo(uint32_t progId, const aex::string& name);
        protected:
            void setOnDirty();
    };
}

#endif
