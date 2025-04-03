#include <assert.h>
#include <cifuzz/cifuzz.h>
#include <dwg.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// One-time initialization tasks are not required for this combined fuzz test.
FUZZ_TEST_SETUP() {}

// Combined Fuzz Test Function
FUZZ_TEST(const uint8_t *data, size_t size) {
  FuzzedDataProvider fdp(data, size);

  // Determine which original fuzz test to execute
  int choice = fdp.ConsumeIntegralInRange<int>(0, 1);

  switch (choice) {
    case 0: {
      // First fuzz test logic
      Dwg_Data dwg;
      int opts = fdp.ConsumeIntegralInRange<int>(0, 9);
      int error;
      
      dwg.opts = opts;

      // Use the fuzzer data to simulate file content
      std::string filename_in = fdp.ConsumeRandomLengthString(30);
      std::string filename_out = fdp.ConsumeRandomLengthString(30);

      // Initialize a Dwg_Version_Type with fuzzed string
      Dwg_Version_Type dwg_version = dwg_version_as(fdp.ConsumeRandomLengthString(10).c_str());

      if (dwg_version == R_INVALID) {
        return; // Skip invalid version
      }

      // Mimics the dwg read, write, and free sequence
      error = dwg_read_file(filename_in.c_str(), &dwg);
      if (error >= DWG_ERR_CRITICAL)
        return;

      dwg.header.version = dwg_version;

      error = dwg_write_file(filename_out.c_str(), &dwg);

      dwg_free(&dwg);
      break;
    }
    case 1: {
      // Second fuzz test logic
      Dwg_Data dwg;
      
      // Safely handle version setting, using a valid integral conversion.
      dwg.header.version = static_cast<Dwg_Version_Type>(fdp.ConsumeIntegral<uint32_t>());

      // Simulate file content for DWG source by consuming remaining bytes.
      std::string filename_in_content = fdp.ConsumeRemainingBytesAsString();

      if (filename_in_content.size() < 100) {
        return; // Ensure minimum useful input length to invoke functions meaningfully.
      }

      // Initialize Dwg_Chain properly based on its structure definition.
      Dwg_Chain dat;
      memset(&dat, 0, sizeof(dat));  // Initialize all fields to zero

      dat.size = filename_in_content.size();
      dat.chain = reinterpret_cast<uint8_t*>(malloc(dat.size));
      if (dat.chain == nullptr) {
        return; // Exit if memory allocation fails to prevent further issues.
      }
      memcpy(dat.chain, filename_in_content.data(), dat.size);
      
      // Read DXF file (assuming part of DWG read logic)
      int error = dxf_read_file(reinterpret_cast<const char *>(dat.chain), &dwg);
      if (error >= DWG_ERR_CRITICAL) {
        free(dat.chain);
        return;
      }

      // Perform DWG writing logic, using a temporary filename for output.
      error = dwg_write_file("fuzz_output.tmp.dwg", &dwg);
      if (error) {
        free(dat.chain);
        return;
      }

      // Clean up resources
      dwg_free(&dwg);
      free(dat.chain);
      break;
    }
  }
}
