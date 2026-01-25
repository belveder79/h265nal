#ifndef H265_WUTILS_INCLUDED
#define H265_WUTILS_INCLUDED

#include <iostream>
#include <h265_bitstream_parser.h>
#include <h265_common.h>

int WriteFile(const char* filename, std::vector<uint8_t>& buffer);
int WriteFile(const char* filename, uint8_t* buffer, size_t sz);
bool wbsp_trailing_bits(h265nal::BitBufferWriter *bit_buffer);
std::vector<uint8_t> EscapeRbsp(uint8_t* data, h265nal::BitBufferWriter* writer);
#endif // H265_WUTILS_INCLUDED
