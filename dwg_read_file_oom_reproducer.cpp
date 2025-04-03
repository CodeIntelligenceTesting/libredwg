#include <filesystem>
#include <fstream>
#include <sys/resource.h>

#include "dwg.h"

int main(int argc, char** argv) {

    // Limit the available memory
    struct rlimit limit;
    limit.rlim_cur = 2048L * 1024 * 1024; // 2 GB
    limit.rlim_max = 2048L * 1024 * 1024; // 2 GB
    setrlimit(RLIMIT_AS, &limit);

    Dwg_Data dwg;
    dwg.opts = 0;

    // Use std::filesystem to manage temporary files for fuzz testing.
    std::filesystem::path tmp_filename = "../dwg_read_file_oom_file";
    int error = dwg_read_file(tmp_filename.string().c_str(), &dwg);
    
    // Free resources associated with the Dwg_Data structure
    dwg_free(&dwg);

    // Cleanup: Remove the temporary file
    std::filesystem::remove(tmp_filename);
    return 0;
}
