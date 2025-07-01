#include "fileparser.h" // Include the header
#include <iostream>
#include <fstream> // Include fstream for file handling

// Parses a directory for files with specific extensions and stores them.
void FileParser::parseDirectory(const std::filesystem::path& dir, const std::vector<std::string>& extensions)
{
      // Iterate over each entry in the directory.
      for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            // Check if the entry is a regular file.
            if (entry.is_regular_file()) {
                  // Check if the file has one of the specified extensions.
                  for (const auto& ext : extensions) {
                  if (entry.path().extension() == ext) {
                        // If it does, add it to the list of files.
                        files.push_back(entry.path());
                  }
                  }
            }
      }
}

// Analyzes a single file to count total, blank, comment, and code lines.
FileParser::FileStats FileParser::analyzeFile(const fs::path& file_path, const FileParser::Language& lang) {
    FileParser::FileStats stats;
    stats.file_path = file_path;
    std::ifstream file(file_path);
    std::string line;
    bool in_multiline_comment = false;
    bool in_code_block = false;

    // Read the file line by line.
    while (std::getline(file, line)) {
        stats.total_lines++;
        // Check for blank lines.
        if (line.find_first_not_of(" \t\n\v\f\r") == std::string::npos) {
            stats.blank_lines++;
            continue;
        }

        // Handle Markdown files.
        if (lang.name == "Markdown") {
            if (line.rfind(lang.code_block_start, 0) == 0) {
                in_code_block = !in_code_block;
                continue;
            }
            if (in_code_block) {
                stats.code_lines++;
            }
            continue;
        }

        // Handle Text files.
        if (lang.name == "Text") {
            stats.code_lines++;
            continue;
        }

        size_t first_char = line.find_first_not_of(" \t");
        bool code_on_line = false;

        // Handle multi-line comments.
        if (in_multiline_comment) {
            stats.comment_lines++;
            if (!lang.multi_line_comment_end.empty() && line.find(lang.multi_line_comment_end) != std::string::npos) {
                in_multiline_comment = false;
                size_t comment_end = line.find(lang.multi_line_comment_end) + lang.multi_line_comment_end.length();
                if (comment_end < line.length() && line.find_first_not_of(" \t", comment_end) != std::string::npos) {
                    code_on_line = true;
                }
            }
        } else {
            // Handle single-line and multi-line comments.
            if (first_char != std::string::npos) {
                if (!lang.line_comment.empty() && line.substr(first_char, lang.line_comment.length()) == lang.line_comment) {
                    stats.comment_lines++;
                } else if (!lang.multi_line_comment_start.empty() && line.find(lang.multi_line_comment_start) != std::string::npos) {
                    stats.comment_lines++;
                    if (line.find(lang.multi_line_comment_end, line.find(lang.multi_line_comment_start) + lang.multi_line_comment_start.length()) == std::string::npos) {
                        in_multiline_comment = true;
                    }
                    if (line.find_first_not_of(" \t") < line.find(lang.multi_line_comment_start)) {
                        code_on_line = true;
                    }
                } else {
                    code_on_line = true;
                }
            } else {
                code_on_line = true;
            }
        }

        // If there is code on the line, increment the code line count.
        if (code_on_line) {
            stats.code_lines++;
        }
    }
    return stats;
}