//
//  FileManager.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 17.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#include "../include/FileManagerIMPL.hpp"
#include <aex/FileManager.hpp>
#include <aex/Logger.hpp>
#include <array>

namespace aex {
    DEFINE_AEX_EXCEPTION(FileManagerError);
    using boost::filesystem::path;
    using path_list = std::vector<boost::filesystem::path>;

    std::vector<aex::string> FileManagerBase::getDirFileNames(const aex::string& fName)
    {
        const auto paths = getDirFileNames(path(fName));
        std::vector<aex::string> ret(paths.size());
        for (const auto& p : paths)
        {
            ret.push_back(CPP_STRING(p.native()));
        }
        return ret;
    }

    path_list FileManagerBase::getDirFileNames(const path& fName)
    {
        return _impl->getDirFileNames(fName);
    }

    size_t FileManagerBase::file_size(const aex::string& fName)
    {
        return file_size(path(fName));
    }

    size_t FileManagerBase::file_size(const path& fName)
    {
        return _impl->file_size(fName);
    }

    path FileManagerBase::work_dir_base() const
    {
        return _impl->resourceDir();
    }

    path FileManagerBase::write_dir_base() const
    {
        return _impl->writeDir();
    }

    path FileManagerBase::alternative_dir_base() const
    {
        return _impl->alternativeDir();
    }

    path FileManagerBase::temp_dir_base() const
    {
        return _impl->temporaryDir();
    }

    path FileManagerBase::log_dir_base() const
    {
        return _impl->logDir();
    }

    path FileManagerBase::user_dir_base() const
    {
        return _impl->userDir();
    }

    path FileManagerBase::write_to_base(WriteStorage storage) const
    {
        switch (storage) {
            case WriteStorage::Default:
            {
                return write_dir_base();
            }

            case WriteStorage::Alternative:
            {
                return alternative_dir_base();
            }

            case WriteStorage::Temporary:
            {
                return temp_dir_base();
            }
            default:
            {
                return write_dir_base();
            }
        }
    }

    enum class PATH_TYPE
    {
        WORK_DIR,
        WRITE_DIR,
        ALTERNATIVE_DIR,
        TEMP_DIR,
        LOG_DIR,
        USER_DIR,
        NO_DIR
    };

    template<class STREAM_TYPE>
    static bool open_st_fm(STREAM_TYPE& stream, FileManagerBase& fm, const PATH_TYPE& __pt, const path& __path, std::ios_base::openmode __mode)
    {
        path _p;
        bool succes = false;
        try
        {
            switch (__pt) {
                case PATH_TYPE::WORK_DIR:
                {
                    _p = fm.work_dir_base();
                    break;
                }
                case PATH_TYPE::WRITE_DIR:
                {
                    _p = fm.write_dir_base();
                    break;
                }
                case PATH_TYPE::ALTERNATIVE_DIR:
                {
                    _p = fm.alternative_dir_base();
                    break;
                }
                case PATH_TYPE::TEMP_DIR:
                {
                    _p = fm.temp_dir_base();
                    break;
                }
                case PATH_TYPE::LOG_DIR:
                {
                    _p = fm.log_dir_base();
                    break;
                }
                case PATH_TYPE::USER_DIR:
                {
                    _p = fm.user_dir_base();
                    break;
                }
                default:
                {
                    break;
                }
            }
            if (_p.empty())
                _p = __path;
            else
                _p /= __path;

            if (stream.rdbuf()->open(CPP_STRING(_p.native()), __mode))
            {
                stream.clear();
                succes = true;
            }
        } catch (const FileManagerError&)
        {

        }
        return succes;
    }

    aex::ofstream FileManagerBase::open_write_file_base(const path& __p, std::ios_base::openmode __mode)
    {
        aex::ofstream file;
        std::array<PATH_TYPE, 3> cdirs = {
            {
                PATH_TYPE::WRITE_DIR,
                PATH_TYPE::ALTERNATIVE_DIR,
                PATH_TYPE::NO_DIR
            }};
        for (const auto pt : cdirs)
        {
            if (open_st_fm(file, *this, pt, __p, __mode))
                return file;
        }
        file.setstate(std::ios_base::failbit);
        return file;
    }

    aex::ifstream FileManagerBase::open_read_file_base(const path& __p, std::ios_base::openmode __mode)
    {
        aex::ifstream file;
        std::array<PATH_TYPE, 4> cdirs = {
            {
                PATH_TYPE::WRITE_DIR,
                PATH_TYPE::ALTERNATIVE_DIR,
                PATH_TYPE::WORK_DIR,
                PATH_TYPE::NO_DIR
            }};
        for (const auto pt : cdirs)
        {
            if (open_st_fm(file, *this, pt, __p, __mode))
                return file;
        }

        file.setstate(std::ios_base::failbit);
        return file;
    }

    aex::fstream FileManagerBase::open_rw_file_base(const path& __p, std::ios_base::openmode __mode)
    {
        aex::fstream file;
        std::array<PATH_TYPE, 3> cdirs = {
            {
                PATH_TYPE::WRITE_DIR,
                PATH_TYPE::ALTERNATIVE_DIR,
                PATH_TYPE::NO_DIR
            }};
        for (const auto pt : cdirs)
        {
            if (open_st_fm(file, *this, pt, __p, __mode))
                return file;
        }
        file.setstate(std::ios_base::failbit);
        return file;
    }

    aex::ifstream FileManagerBase::open_asset_base(const path& __p)
    {
        return open_read_file_base(__p);
    }
    FileManagerBase::FileManagerBase() :
    _impl{FileManagerIMPLFactory().getNewFileManagerImplementation()}
    {
        if (!_impl->init())
            throw FileManagerError("Could not initialize file manager.");
    }

    FileManagerBase::~FileManagerBase()
    {
    }
}
