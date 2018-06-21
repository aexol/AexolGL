/** @file FileManager.hpp
 *  @brief Definition of FileManager.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __aexolGL__FileManager__
#define __aexolGL__FileManager__

#include <aex/FileManagerBase.hpp>
#include <string>
#include <vector>
#include <exception>

namespace aex
{
    class AEX_PUBLIC_API FileManager : public FileManagerBase
    {
        typedef boost::filesystem::path path;
        typedef std::ios_base ios_base;
        typedef aex::ifstream ifstream;
        typedef aex::ofstream ofstream;
        typedef aex::fstream fstream;
        public:
        /*void            set_working_dir( aex::string working_dir ) { m_working_dir = working_dir; }
          const   std::ifstream   get_file_handle( aex::string, bool relative = true ) const;
          static  FileManager*    getInstance();
          protected:
          aex::string     m_working_dir;
          static  FileManager*    instance;*/
        FileManager() = default;
        ofstream open_write_file(const path& __p, ios_base::openmode __mode = ios_base::out)
        {
            return open_write_file_base(__p, __mode);
        }
        ifstream open_read_file(const path& __p, ios_base::openmode __mode = ios_base::in)
        {
            return open_read_file_base(__p, __mode);
        }
        fstream  open_rw_file(const path& __p, ios_base::openmode __mode = ios_base::in | ios_base::out)
        {
            return open_rw_file_base(__p, __mode);
        }
        ifstream open_asset(const path& __p)
        {
            return open_asset_base(__p);
        }
        /**
         *	@brief	Get current work dir.
         *
         *	@return	Path.
         */
        path work_dir() const
        {
            return work_dir_base();
        }

        /**
         *	@brief	Default write dir.
         *
         *	@return	Path.
         */

        path write_dir() const
        {
            return write_dir_base();
        }

        path write_to(WriteStorage storage) const
        {
            return write_to_base(storage);
        }
        path log_dir() const
        {
            return log_dir_base();
        }
        path user_dir() const
        {
            return user_dir_base();
        }
    };
}

/**
 * DEPRACATED. For backwards compatibility only.
 */
namespace aexFramework {
    using FileManager = aex::FileManager;
}

#endif /* defined(__aexolGL__FileManager__) */
