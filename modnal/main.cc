#include <string>
#include <iostream>

#include <config.h>
#include <h265_bitstream_parser.h>
#include <h265_common.h>
#include <h265_configuration_box_parser.h>
#include <h265_utils.h>
#include <rtc_common.h>

#include "h265_nal_unit_writer.h"

enum Dumpmode { dump_all, dump_length };

typedef struct arg_options {
    int debug;
    Dumpmode dumpmode;
    bool as_one_line;
    bool add_offset;
    bool add_length;
    bool add_parsed_length;
    bool add_checksum;
    bool add_resolution;
    bool add_contents;
    int nalu_length_bytes;
    int frames_per_second;
    char *hvcc_file;
    char *infile;
    char *outfile;
} arg_options;

// default option values
arg_options DEFAULT_OPTIONS{
    .debug = 0,
    .dumpmode = dump_all,
    .as_one_line = true,
    .add_offset = false,
    .add_length = false,
    .add_parsed_length = false,
    .add_checksum = false,
    .add_resolution = false,
    .add_contents = false,
    .nalu_length_bytes = -1,
    .frames_per_second = 30,
    .hvcc_file = nullptr,
    .infile = nullptr,
    .outfile = nullptr,
};






/*
 bool WriteSps(h265nal::H265SpsParser::SpsState* sps, h265nal::BitBufferWriter* bit_buffer) noexcept
 {
     
     return true;
 }

 bool WritePps(h265nal::H265PpsParser::PpsState* pps,h265nal::BitBufferWriter* bit_buffer) noexcept
 {
 return true;
 }
 */

int main(int argc, char* argv[])
{
    if(argc > 1) {
        const char* t = argv[1];
        std::cout << t << std::endl;
    }
    
    std::cout << "Hello World" << std::endl;
    
    arg_options* options = new arg_options(DEFAULT_OPTIONS);
    
    // 1. prepare bitstream parsing
    h265nal::ParsingOptions parsing_options;
    parsing_options.add_offset = options->add_offset;
    parsing_options.add_length = options->add_length;
    parsing_options.add_parsed_length = options->add_parsed_length;
    parsing_options.add_checksum = options->add_checksum;
    parsing_options.add_resolution = options->add_resolution;
    
    h265nal::H265BitstreamParserState bitstream_parser_state;
    std::vector<uint8_t> buffer;
    std::unique_ptr<h265nal::H265BitstreamParser::BitstreamState> bitstream;
    
    // 3.1. read infile into buffer
    if (h265nal::H265Utils::ReadFile("/Users/arth/Development/TILE/kvazaar/buildosx/out2x2.hevc", buffer) < 0) {
        return -1;
    }
    /*
     // 3.2. parse buffer
     if (options->nalu_length_bytes < 0) {
     bitstream = h265nal::H265BitstreamParser::ParseBitstream(
     buffer.data(), buffer.size(), &bitstream_parser_state,
     parsing_options);
     } else {
     bitstream = h265nal::H265BitstreamParser::ParseBitstreamNALULength(
     buffer.data(), buffer.size(),
     static_cast<size_t>(options->nalu_length_bytes),
     &bitstream_parser_state, parsing_options);
     }
     */
    auto nalu_indices = h265nal::H265BitstreamParser::FindNaluIndices(buffer.data(), buffer.size());
    for (const auto &nalu_index : nalu_indices) {
        // 4.1. parse 1 NAL unit
        // note: If the NALU comes from an unescaped bitstreams, i.e.,
        // one with an explicit NALU length mechanism (like mp4 mdat
        // boxes), the right function is `ParseNalUnitUnescaped()`.
        auto nal_unit = h265nal::H265NalUnitParser::ParseNalUnit(
                                                                 &(buffer.data())[nalu_index.payload_start_offset], nalu_index.payload_size,
                                                                 &bitstream_parser_state, parsing_options);
        
        
        // TODO: need to implement a writer here...
        uint8_t* buf = new uint8_t[nal_unit->parsed_length]; memset(buf, 0x00, nal_unit->parsed_length);
        h265nal::BitBufferWriter wbit_buffer(buf, nal_unit->parsed_length);
        if(WriteNalUnit(nal_unit.get(), &wbit_buffer))
        {
            WriteFile("/Users/arth/Development/TILE/kvazaar/buildosx/vps.nal", buf, nal_unit->parsed_length);
            std::vector<uint8_t> unpacked_buffer = EscapeRbsp(buf, &wbit_buffer);
            WriteFile("/Users/arth/Development/TILE/kvazaar/buildosx/escaped_vps.nal", unpacked_buffer.data(), unpacked_buffer.size());
        }
        else
            std::cout << "ERROR!" << std::endl;
        delete[] buf;
        return -1;
        /*
         // to be extended looking for other types also
         uint32_t naltype = nal_unit->nal_unit_header->nal_unit_type;
         if(naltype == 19 || naltype == 1 || naltype == 21)
         {
         if(nal_unit->nal_unit_payload->slice_segment_layer != nullptr)
         {
         uint32_t address = nal_unit->nal_unit_payload->slice_segment_layer->slice_segment_header->slice_segment_address;
         if(address == 0)
         {
         std::vector<uint8_t> unpacked_buffer = h265nal::UnescapeRbsp(&(buffer.data())[nalu_index.payload_start_offset], nalu_index.payload_size);
         h265nal::BitBuffer bit_buffer(unpacked_buffer.data(), unpacked_buffer.size());
         //ParseSliceSegmentHeader(bit_buffer)
         // bingo
         //nal_unit->nal_unit_payload->slice_segment_layer->slice_segment_header->slice_segment_address = 10;
         
         }
         else if(address == 10)
         {
         // bingo
         //nal_unit->nal_unit_payload->slice_segment_layer->slice_segment_header->slice_segment_address = 0;
         
         }
         }
         }
         */
    }
    
    delete options;
    
    WriteFile("/Users/arth/Development/TILE/kvazaar/buildosx/out2x2_mod.hevc", buffer);
    
    std::cout << "Byebye World" << std::endl;
    return 0;
}
