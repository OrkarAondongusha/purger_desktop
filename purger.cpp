#include <iostream>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <vector>

namespace fs = std::filesystem;

// Function to convert file time to time_t for proper printing
std::time_t to_time_t(const fs::file_time_type& ftime) {
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
    return std::chrono::system_clock::to_time_t(sctp);
}

// C-compatible function to list files and their modification times
extern "C" {
    void list_files_with_dates(const char* directory) {
        std::string dir_str(directory);  // Convert C-style string to std::string
        std::vector<fs::path> file_batch;

        try {
            for (const auto& entry : fs::recursive_directory_iterator(dir_str)) {
                if (fs::is_regular_file(entry)) {
                    auto ftime = fs::last_write_time(entry);
                    std::time_t cftime = to_time_t(ftime);
                    std::cout << "File Path: " << entry.path().string() << "\n";
                    std::cout << "  Modified: " << std::ctime(&cftime);  // Print time in human-readable format
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error iterating directory: " << e.what() << "\n";
        }
    }
}
