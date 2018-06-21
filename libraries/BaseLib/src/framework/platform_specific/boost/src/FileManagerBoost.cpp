/**************************************************************
 *                                                            * 
 *      Filename:   FileManagerBoost.cpp                      * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2016 lut 24 13:43:21                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/

#include <aex/FileManagerBase.hpp>
#include "../../include/FileManagerIMPL.hpp"
#include <cstdlib>
#include <iterator>

namespace aex
{
#if __TARGET_PLATFORM__ == __AEX_WIN32__
    static constexpr const char* HOME_ENV = "USERPROFILE";
#else // __TARGET_PLATFORM__ == __AEX_WIN32__ 
    static constexpr const char* HOME_ENV = "HOME";
#endif
    class FileManagerBoost : public IFileManagerIMPL
    {
        public:
            using path =  path;
            using path_list = std::vector<path>;
            bool init(void)
            {
                return true;
            }

            path resourceDir() const
            {
                return boost::filesystem::current_path();
            }

            path writeDir() const
            {
                return userDir();
            }
            
            path alternativeDir() const
            {
				throw FileManagerError("Alternative dir unsupported.");
            }

            path temporaryDir() const
            {
                return boost::filesystem::temp_directory_path();
            }

            path userDir() const
            {
                const char* home = std::getenv(HOME_ENV);
                return path(home);
            }

            path logDir() const
            {
				throw FileManagerError("Log dir unsupported.");
            }

            virtual path_list getDirFileNames(const path& dir) const
            {
                path_list contents;
				try{
					if(!boost::filesystem::exists(dir))
						FileManagerError(aex::string("No such file or directory: ") + CPP_STRING(dir.native()));
					if(!boost::filesystem::is_directory(dir))
						FileManagerError(CPP_STRING(dir.native())+" is not a directory.");
					std::copy(boost::filesystem::directory_iterator(dir), boost::filesystem::directory_iterator(), std::back_inserter(contents));
                } catch(const boost::filesystem::filesystem_error&)
                {
					throw FileManagerError("Could not list directory contetns.");
                }
                return contents;
            }

            size_t file_size(const path& fName) const
            {
                return boost::filesystem::file_size(fName);
            }
    };

	IFileManager
		FileManagerIMPLFactory::getNewFileManagerImplementation() const
	{
		return IFileManager(new FileManagerBoost);
	}
}
