#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <map>
#include "fileparser.h"

namespace fs = std::filesystem;

// Analyzes a single file to count total, blank, comment, and code lines.
void analyze_file(const fs::path& file_path, FileParser::FileStats& stats, const FileParser::Language& lang) {
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
}

// Main function to drive the code analysis.
int main(int argc, char* argv[]) {
    // Check for the correct number of command-line arguments.
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <directory> [--per-file]" << std::endl;
        return 1;
    }

    // Get the starting directory from the command-line arguments.
    fs::path start_dir = argv[1];
    if (!fs::exists(start_dir) || !fs::is_directory(start_dir)) {
        std::cerr << "Error: Invalid directory path." << std::endl;
        return 1;
    }

    // Check for the optional --per-file flag.
    bool per_file_report = false;
    for (int i = 2; i < argc; ++i) {
        if (std::string(argv[i]) == "--per-file") {
            per_file_report = true;
        }
    }

    // Define language specifications.
    // Define C++ language
    FileParser::Language cpp_lang;
    cpp_lang.name = "C++";
    cpp_lang.extensions = {".cpp", ".h"};
    cpp_lang.line_comment = "//";
    cpp_lang.multi_line_comment_start = "/*";
    cpp_lang.multi_line_comment_end = "*/";

    // Define Markdown language
    FileParser::Language markdown_lang;
    markdown_lang.name = "Markdown";
    markdown_lang.extensions = {".md", ".markdown"};
    markdown_lang.line_comment = "";
    markdown_lang.multi_line_comment_start = "";
    markdown_lang.multi_line_comment_end = "";
    markdown_lang.code_block_start = "```";

    // Define Text language
    FileParser::Language text_lang;
    text_lang.name = "Text";
    text_lang.extensions = {".txt"};
    text_lang.line_comment = "";
    text_lang.multi_line_comment_start = "";
    text_lang.multi_line_comment_end = "";

    // Create a map of file extensions to languages.
    std::map<std::string, FileParser::Language> lang_map;
    for (const auto& ext : cpp_lang.extensions) lang_map[ext] = cpp_lang;
    for (const auto& ext : markdown_lang.extensions) lang_map[ext] = markdown_lang;
    for (const auto& ext : text_lang.extensions) lang_map[ext] = text_lang;

    // Create a FileParser and parse the directory for all supported file types.
    FileParser parser;
    parser.parseDirectory(start_dir, cpp_lang.extensions);
    parser.parseDirectory(start_dir, markdown_lang.extensions);
    parser.parseDirectory(start_dir, text_lang.extensions);

    // Initialize variables for storing statistics.
    std::vector<FileParser::FileStats> all_stats;
    int total_lines = 0;
    int total_blank_lines = 0;
    int total_comment_lines = 0;
    int total_code_lines = 0;

    // Analyze each file and aggregate the statistics.
    for (const auto& file : parser.getDirectories()) {
        FileParser::FileStats stats;
        std::string ext = file.extension().string();
        if (lang_map.count(ext)) {
            analyze_file(file, stats, lang_map.at(ext));
        }
        all_stats.push_back(stats);
        total_lines += stats.total_lines;
        total_blank_lines += stats.blank_lines;
        total_comment_lines += stats.comment_lines;
        total_code_lines += stats.code_lines;
    }

// Print the per-file report if requested.
if (per_file_report) {
      // Find the length of the longest file name
      size_t max_file_len = 4; // "File" length
      for (const auto& stats : all_stats) {
            size_t len = stats.file_path.string().length();
            if (len > max_file_len) max_file_len = len;
      }

      // Set column widths
      const int lines_width = 15;
      const int blanks_width = 15;
      const int comments_width = 15;
      const int code_width = 15;

      // Print header with dynamic spacing and lines
      std::cout << std::left << std::setw(static_cast<int>(max_file_len) + 4) << "File"
                    << std::setw(lines_width) << "Total_Lines"
                    << std::setw(blanks_width) << "Blank_Lines"
                    << std::setw(comments_width) << "Comment_Lines"
                    << std::setw(code_width) << "Code_Lines" << std::endl;

      std::cout << std::string(max_file_len + 4, '-') 
                    << std::string(lines_width, '-')
                    << std::string(blanks_width, '-')
                    << std::string(comments_width, '-')
                    << std::string(code_width, '-') << std::endl;

      // Print each file's stats with aligned columns
      for (const auto& stats : all_stats) {
            std::cout << std::left << std::setw(static_cast<int>(max_file_len) + 4)
                          << stats.file_path.string()
                          << std::setw(lines_width) << stats.total_lines
                          << std::setw(blanks_width) << stats.blank_lines
                          << std::setw(comments_width) << stats.comment_lines
                          << std::setw(code_width) << stats.code_lines << std::endl;
      }
}

    // Print the total statistics.
    std::cout << "Total Lines: " << total_lines << std::endl;
    std::cout << "Blank Lines: " << total_blank_lines << std::endl;
    std::cout << "Comment Lines: " << total_comment_lines << std::endl;
    std::cout << "Code Lines: " << total_code_lines << std::endl;


    return 0;
}
