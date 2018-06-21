/**************************************************************
 *                                                            * 
 *      Filename:   FileManagerBase.hpp                       * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2016 lut 12 11:19:32                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#ifndef FILEMANAGERBASE_HPP
#define FILEMANAGERBASE_HPP 
#include <aex/Common.hpp>
#include <aex/Exceptions.hpp>
#include <aex/fstream.hpp>
#include <boost/filesystem.hpp>
namespace aex
{
    DECLARE_AEX_EXCEPTION(FileManagerError);
    enum class WriteStorage
    {
        Default,
        Alternative,
        Temporary
    };

    class AEX_PUBLIC_API IFileManagerIMPL
    {
        public:
            using path = boost::filesystem::path;
            using path_list = std::vector<path>;
            /**
             * @brief Initialize IFileManager implementation.
             *
             * Prepare IFileManager for use.
             *
             * @return 
             */
            virtual bool init(void) = 0;

            /**
             * @brief Get default application resource dir path.
             *
             * @return path
             */
            virtual path resourceDir() const = 0;

            /**
             * @brief Get default application write path.
             *
             * Returns default path in which application has write permissions.
             * Path value is platform dependant.
             *
             * @return path
             */
            virtual path writeDir() const = 0;
            
            /**
             * @brief Get alternative write path.
             *
             * Return alternative wirte path for application. Value is platform dependant.
             *
             * @return path
             */
            virtual path alternativeDir() const = 0;

            /**
             * @brief Temporary path.
             *
             * Temporary path for application. Value is platform dependant.
             *
             * @return path
             */
            virtual path temporaryDir() const = 0;

            /**
             * @brief User dir for platform.
             * 
             * User home path, if exists for platform.
             *
             * @return path
             */
            virtual path userDir() const = 0;

            /**
             * @brief Log directory for application.
             *
             * @return path
             */
            virtual path logDir() const = 0;

            /**
             * @brief 
             *
             * @param dir
             *
             * @return 
             */
            virtual path_list getDirFileNames(const path& dir) const = 0;

            /**
             * @brief 
             *
             * @param fName
             *
             * @return 
             */
            virtual size_t file_size(const path& fName) const = 0;

            virtual ~IFileManagerIMPL() = 0;
    };
    inline IFileManagerIMPL::~IFileManagerIMPL() {}
    using IFileManager = std::unique_ptr<IFileManagerIMPL>;

    class AEX_PUBLIC_API IFileManagerFactory
    {
        public:
            /**
             * @brief Create implementation for file manager.
             *
             * @return New file manager implementation.
             */
            virtual IFileManager getNewFileManagerImplementation() const = 0;
            virtual ~IFileManagerFactory() = 0;
    };
    inline IFileManagerFactory::~IFileManagerFactory(){}

    class AEX_PUBLIC_API FileManagerBase
    {
        using path = IFileManagerIMPL::path;
        using path_list = IFileManagerIMPL::path_list;
        typedef std::ios_base ios_base;
        typedef aex::ifstream ifstream;
        typedef aex::ofstream ofstream;
        typedef aex::fstream fstream;
    public:
        FileManagerBase();
        FileManagerBase(const IFileManagerFactory& factory);
        ~FileManagerBase();

        path_list  getDirFileNames(const path& fName);
        std::vector<aex::string> getDirFileNames(const aex::string& fName);
        /**
         *	@brief	Get size of file.
         *
         *	@param 	fName 	Path to file.
         *
         *	@return	Size.
         */
        size_t      file_size(const aex::string& fName);
        size_t      file_size(const path& fName);

        path work_dir_base() const;
        path write_dir_base() const;
        path alternative_dir_base() const;
        path temp_dir_base() const;
        path log_dir_base() const;
        path user_dir_base() const;
        path write_to_base(WriteStorage storage) const;
    protected:
        ifstream open_asset_base(const path& __p);
        ofstream open_write_file_base(const path& __p, ios_base::openmode __mode = ios_base::out);
        ifstream open_read_file_base(const path& __p, ios_base::openmode __mode = ios_base::in);
        fstream  open_rw_file_base(const path& __p, ios_base::openmode __mode = ios_base::in | ios_base::out);

    private:
        IFileManager _impl;
    };
}

/**
 * DEPRACATED. For backwards compatibility only.
 */
namespace aexFramework {
    using WriteStorage = aex::WriteStorage;
}

#endif /* FILEMANAGERBASE_HPP */
