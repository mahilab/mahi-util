#include <fcntl.h>
#include <Mahi/Util/Logging/File.hpp>
#include <sys/stat.h>
#include <Mahi/Util/System.hpp>
#include <Mahi/Util/Logging/Log.hpp>

#ifdef _WIN32
#include <io.h>
#include <share.h>
#include <windows.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

namespace mahi {
namespace util {

File::File() : file_handle_(-1) {}

File::File(const std::string& filepath, WriteMode w_mode, OpenMode o_mode) : 
    file_handle_(-1)
{
    open(filepath, w_mode, o_mode);
}

File::~File() {
    close();
}

bool File::open(const std::string& filepath, WriteMode w_mode, OpenMode o_mode) {

    // parse filepath
    std::string directory, filename, ext, full;
    if (!parse_filepath(filepath, directory, filename, ext, full)) {
        LOG(Error) << "Failed to parse filepath: " << filepath;
        return false;
    }

    // make directory if it doesn't exist
    if (o_mode == OpenMode::OpenOrCreate && !directory_exits(directory)) {
        if (!create_directory(directory)) {
            return false;
        }
        LOG(Verbose) << "Created directory " << directory << " for file: " << full;
    }

    // open file
#if defined(_WIN32) && (defined(__BORLANDC__) || defined(__MINGW32__))
    auto open_flags = o_mode == OpenMode::OpenOrCreate ? _O_CREAT | _O_RDWR : _O_RDWR;
    if (w_mode == WriteMode::Truncate)
        open_flags |= _O_TRUNC;
    file_handle_ = ::_sopen(full.c_str(), open_flags | _O_BINARY, SH_DENYWR, _S_IREAD | _S_IWRITE);
#elif defined(_WIN32)
    auto open_flags = o_mode == OpenMode::OpenOrCreate ? _O_CREAT | _O_RDWR : _O_RDWR;
    if (w_mode == WriteMode::Truncate)
        open_flags |= _O_TRUNC;
    auto result = ::_sopen_s(&file_handle_, full.c_str(), open_flags | _O_BINARY, _SH_DENYWR, _S_IREAD | _S_IWRITE);
#else
    auto open_flags = o_mode == OpenMode::OpenOrCreate ? O_CREAT | O_RDWR : O_RDWR;
    if (w_mode == WriteMode::Truncate)
        open_flags |= O_TRUNC;
    file_handle_ = ::open(full.c_str(), open_flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
#endif

    seek_end(0);
    if (is_open())
        return true;
    else {
        LOG(Error) << "Failed to open file: " << full;
        return false;
    } 
}

int File::write(const void* data, std::size_t size) {
#ifdef _WIN32
    return file_handle_ != -1 ? ::_write(file_handle_, data, static_cast<unsigned int>(size)) : -1;
#else
    return file_handle_ != -1 ? static_cast<int>(::write(file_handle_, data, size)) : -1;
#endif
}

off_t File::seek(off_t offset, int whence) {
#ifdef _WIN32
    return file_handle_ != -1 ? ::_lseek(file_handle_, offset, whence) : -1;
#else
    return file_handle_ != -1 ? ::lseek(file_handle_, offset, whence) : -1;
#endif
}

off_t File::seek_set(off_t offset) {
    return seek(offset, SEEK_SET);
}

off_t File::seek_cur(off_t offset) {
    return seek(offset, SEEK_CUR);
}

off_t File::seek_end(off_t offset) {
    return seek(offset, SEEK_END);
}

bool File::is_open() {
    return file_handle_ != -1;
}

void File::close() {
    if (file_handle_ != -1) {
#ifdef _WIN32
        ::_close(file_handle_);
#else
        ::close(file_handle_);
#endif
        file_handle_ = -1;
    }
}

int File::unlink(const std::string& filepath) {
    auto tidy = tidy_path(filepath, true);
#ifdef _WIN32
    return ::_unlink(filepath.c_str());
#else
    return ::unlink(filepath.c_str());
#endif
}

int File::rename(const std::string& old_filepath, const std::string& new_filepath) {
    auto old_tidy = tidy_path(old_filepath, true);
    auto new_tidy = tidy_path(new_filepath, true);
#ifdef _WIN32
    return MoveFileA(old_tidy.c_str(), new_tidy.c_str());
#else
    return ::rename(old_tidy.c_str(), new_tidy.c_str());
#endif
}

}  // namespace util
}  // namespace mahi
