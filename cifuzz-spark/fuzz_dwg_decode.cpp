#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <cifuzz/cifuzz.h>

// Ensure restrict macro consistency
#ifdef restrict
#undef restrict
#endif
#define restrict __restrict

// Include the headers where dwg_decode is declared
#include "dwg.h"
#include "bits.h"
#include "decode.h"

/*
Structure:
1. FUZZ_TEST_SETUP: Simple initialization since no extra setup is required.
2. Use FuzzedDataProvider.
3. Define Bit_Chain and simulate dwg_read_file-like data flow for dwg_decode testing.
*/

FUZZ_TEST_SETUP() {
  // No specific initialization necessary.
}

FUZZ_TEST(const uint8_t *data, size_t size) {
  FuzzedDataProvider fdp(data, size);

  Dwg_Data dwg_data;
  memset(&dwg_data, 0, sizeof(Dwg_Data));

  Bit_Chain bit_chain;
  memset(&bit_chain, 0, sizeof(Bit_Chain));

  // Populate Bit_Chain using fuzzed data.
  std::vector<uint8_t> chain_data = fdp.ConsumeBytes<uint8_t>(fdp.remaining_bytes());
  if (chain_data.empty()) return; // Ensure valid data for further processing
  bit_chain.chain = chain_data.data();
  bit_chain.size = chain_data.size();

  // Invoke dwg_decode with prepared data structures.
  dwg_decode(&bit_chain, &dwg_data);
}

