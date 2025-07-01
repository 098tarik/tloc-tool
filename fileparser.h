#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <vector>
#include <string>
#include <filesystem>

// Namespace alias for std::filesystem.
namespace fs = std::filesystem;

class FileParser
{
public:
    FileParser() = default;

    // Parses a directory for files with specific extensions.
    void parseDirectory(const std::filesystem::path& dir, const std::vector<std::string>& extensions);

    // Returns the list of files found.
    const std::vector<std::filesystem::path>& getDirectories() const { return files; }

    // Clears the list of files.
    void clearDirectories() { files.clear(); }

    // Struct to hold language-specific information.
    struct Language {
        std::string name;
        std::vector<std::string> extensions;
        std::string line_comment;
        std::string multi_line_comment_start;
        std::string multi_line_comment_end;
        std::string code_block_start;
        std::string code_block_end;
    };

    // Struct to hold statistics for a single file.
    struct FileStats {
        fs::path file_path;
        int total_lines = 0;
        int blank_lines = 0;
        int comment_lines = 0;
        int code_lines = 0;
    };

private:
    // Vector to store the paths of the files found.
    std::vector<std::filesystem::path> files;

};

#endif // FILEPARSER_H