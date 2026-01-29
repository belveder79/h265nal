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

bool wbsp_trailing_bits(BitBufferWriter *bit_buffer) {
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

bool wbsp_trailing_bits2(BitBufferWriter *bit_buffer) {
  if(!byte_aligned(bit_buffer))
  {
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
  }
  return true;
}

bool PrefixNalUnit(std::vector<uint8_t>& buffer, bool startcode3)
{
    if(startcode3) buffer.push_back(0x00);
    buffer.push_back(0x00);
    buffer.push_back(0x00);
    buffer.push_back(0x01);
    return true;
}

// NALU packing uses a mechanism to identify the start of a new NALU
// based on a 3-byte start code sequence. The idea is that every NALU
// starts with the binary string "\x00\x00\x01" ("start code prefix").
// In order to avoid the start code prefix to appear by chance in the
// middle of a NALU, the NALU is checked, and every appearance of a
// start code prefix is replaced by a 4-byte escaped version. The
// escaped version consists of adding a "\x03" byte as the third
// byte of the string. This means that the "\x00\x00\x01" string,
// when it is not a start code prefix, is replaced with the
// "\x00\x00\x03\x01" string. Note that we also need to escape the
// "\x00\x00\x03" string (using "\x00\x00\x03\x03" instead). For
// completeness, "\x00\x00\x00" and "\x00\x00\x02" are also escaped.
//
// UnescapeRbsp() takes a escaped string (where any 3-byte string
// where the first 2x bytes are "\x00" and the third byte is "\x00",
// "\x01", "\x02", or "\x03" have been escaped (and extra "\x03"
// has been inserted as third byte), and returns the unescaped one.
/*
std::vector<uint8_t> UnescapeRbsp(const uint8_t *data, size_t length) {
  std::vector<uint8_t> out;
  out.reserve(length);

  for (size_t i = 0; i < length;) {
    // Be careful about over/underflow here. byte_length_ - 3 can underflow, and
    // i + 3 can overflow, but byte_length_ - i can't, because i < byte_length_
    // above, and that expression will produce the number of bytes left in
    // the stream including the byte at i.
    if (length - i >= 3 && data[i] == 0x00 && data[i + 1] == 0x00 &&
        data[i + 2] == 0x03) {
      // Two RBSP bytes.
      out.push_back(data[i++]);
      out.push_back(data[i++]);
      // Skip the emulation byte.
      i++;
    } else {
      // Single rbsp byte.
      out.push_back(data[i++]);
    }
  }
  return out;
}
*/

std::vector<uint8_t> EscapeRbsp(uint8_t* data, BitBufferWriter* writer) {
  std::vector<uint8_t> out;
    size_t out_byte_offset, out_bit_offset;
    writer->GetCurrentOffset(&out_byte_offset, &out_bit_offset);
    out.reserve(out_byte_offset);
    size_t i = 0;

    for (;i < out_byte_offset - 3;)
    {
        if((data[i] == 0x00) && (data[i + 1] == 0x00) &&
            ((data[i + 2] == 0x00) || (data[i + 2] == 0x01) || (data[i + 2] == 0x02) || (data[i + 2] == 0x03)))
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
