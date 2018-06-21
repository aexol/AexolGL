/* 
 * File:   MeshExporter.hpp
 * Author: mritke
 *
 * Created on April 22, 2015, 10:37 AM
 */

#ifndef MESHEXPORTER_HPP
#define	MESHEXPORTER_HPP

#include <aex/Common.hpp>
#include <map>
#include <memory>
#include <functional>
#include <aex/fstream.hpp>
#include <aex/FileManager.hpp>
#include <aex/MeshImporter.hpp>

#ifdef USING_ASSIMP
//#include <assimp/Exporter.hpp>

namespace aex {

    class AEX_PUBLIC_API SharedExport
    {
    public:
        SharedExport();
         void* GetExporter() const;
    private:
        bool prepare();
        std::unique_ptr<void, std::function<void(void*) >> m_impl;
        friend class ExporterBase;
    };

    class AEX_PUBLIC_API ExporterBase
    {
    public:
        const SharedExport& getFile(const aex::string& fileName);
        const SharedExport& getFile();
    protected:
        bool prepare(const aex::string& fileName);
        bool prepare();
    private:
        aex::shared_ptr<SharedExport> m_base;
        aex::string m_fileName;
    };

    class AEX_PUBLIC_API MeshExporter : public ExporterBase
    {
    private:

    public:
        MeshExporter();
        MeshExporter(const aex::string& fileName);
        void exportMesh(const aex::string& filename, Mesh* mesh);
    };
}

#endif/* USING_ASSIMP */

#endif	/* MESHEXPORTER_HPP */

