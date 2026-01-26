#ifndef H265_WUTILS_INCLUDED
#define H265_WUTILS_INCLUDED

#include <iostream>
#include <h265_bitstream_parser.h>
#include <h265_common.h>

using namespace h265nal;

int WriteFile(const char* filename, std::vector<uint8_t>& buffer);
int WriteFile(const char* filename, uint8_t* buffer, size_t sz);
bool wbsp_trailing_bits(BitBufferWriter *bit_buffer);
std::vector<uint8_t> EscapeRbsp(uint8_t* data, BitBufferWriter* writer);
#endif // H265_WUTILS_INCLUDED
