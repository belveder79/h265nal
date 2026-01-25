#include "h265_wutils.h"

// should go into H265Utils
int WriteFile(const char* filename, std::vector<uint8_t>& buffer) {
    
    // open the file
    FILE* ofp = fopen(filename, "wb");
    if (ofp == nullptr) {
        // did not work
        fprintf(stderr, "Could not open output file: \"%s\"\n", filename);
        return -1;
    }
    fwrite(reinterpret_cast<char*>(buffer.data()), 1, static_cast<size_t>(buffer.size()),
           ofp);
    // clean up
    fclose(ofp);
    return 0;
}

// should go into H265Utils
int WriteFile(const char* filename, uint8_t* buffer, size_t sz) {
    
    // open the file
    FILE* ofp = fopen(filename, "wb");
    if (ofp == nullptr) {
        // did not work
        fprintf(stderr, "Could not open output file: \"%s\"\n", filename);
        return -1;
    }
    fwrite(buffer, 1, sz, ofp);
    // clean up
    fclose(ofp);
    return 0;
}

bool wbsp_trailing_bits(h265nal::BitBufferWriter *bit_buffer) {
  uint32_t bits_tmp = 1;

  if (!bit_buffer->WriteBits(bits_tmp, 1)) {
    return false;
  }
  bits_tmp = 0;
  while (!byte_aligned(bit_buffer)) {
    // rbsp_alignment_zero_bit  f(1) // equal to 0
    if (!bit_buffer->WriteBits(bits_tmp, 1)) {
      return false;
    }
  }
  return true;
}

/*
 NAL byte emulation prevention in H.264/H.265 ensures that a \(0x000001\)
 start code prefix—used to identify the beginning of a NAL unit—does not
 accidentally appear within the raw byte stream payload (RBSP). It inserts
 a \(0x03\) "emulation prevention byte" between sequences that resemble a
 start code (e.g., \(0x000000\) becomes \(0x00000300\)), allowing parsers
 to safely locate valid NAL boundaries.
 CHECK THIS AGAIN!!!!
 */
std::vector<uint8_t> EscapeRbsp(uint8_t* data, h265nal::BitBufferWriter* writer) {
  std::vector<uint8_t> out;
    size_t out_byte_offset, out_bit_offset;
    writer->GetCurrentOffset(&out_byte_offset, &out_bit_offset);
    out.reserve(out_byte_offset);
    size_t i = 0;
    for (;i < out_byte_offset - 3;)
    {
        if(data[i] == 0x00 && data[i + 1] == 0x00 && data[i + 2] == 0x00)
        {
            out.push_back(data[i++]);
            out.push_back(data[i++]);
            out.push_back(0x03);
        }
        else
        {
            out.push_back(data[i++]);
        }
    }
    out.push_back(data[i++]);
    out.push_back(data[i++]);
    out.push_back(data[i++]);

  return out;
}
