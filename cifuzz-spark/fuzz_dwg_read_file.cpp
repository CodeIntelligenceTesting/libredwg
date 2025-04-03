#include <assert.h>
#include <cifuzz/cifuzz.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "dwg.h" // Adjusting to the correct header for the DWG functionality


FUZZ_TEST_SETUP() {
  // Setup not necessary for this instance.
}

FUZZ_TEST(const uint8_t *data, size_t size) {
  if (size == 0) return;
  
  Dwg_Data dwg;
  dwg.opts = 0;

  // Use std::filesystem to manage temporary files for fuzz testing.
  std::filesystem::path tmp_filename = std::filesystem::temp_directory_path() / "temp.dwg";
  //std::cerr << tmp_filename << std::endl;
  std::ofstream outfile(tmp_filename, std::ios::binary);

  if (!outfile.is_open()) return;

  outfile.write(reinterpret_cast<const char *>(data), static_cast<std::streamsize>(size));
  outfile.close();

  int error = dwg_read_file(tmp_filename.string().c_str(), &dwg);

  // Free resources associated with the Dwg_Data structure
  dwg_free(&dwg);

  // Cleanup: Remove the temporary file
  std::filesystem::remove(tmp_filename);
}
