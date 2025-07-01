#include "fileparser.h" // Include the header
#include <iostream>

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