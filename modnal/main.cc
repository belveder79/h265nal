#include <string>
#include <iostream>
#include <assert.h>

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
    auto nalu_indices = h265nal::H265BitstreamParser::FindNaluIndices(buffer.data(), buffer.size());
    
    for (const auto &nalu_index : nalu_indices) {
        // 4.1. parse 1 NAL unit
        // note: If the NALU comes from an unescaped bitstreams, i.e.,
        // one with an explicit NALU length mechanism (like mp4 mdat
        // boxes), the right function is `ParseNalUnitUnescaped()`.
        auto nal_unit = h265nal::H265NalUnitParser::ParseNalUnit(
                                                                 &(buffer.data())[nalu_index.payload_start_offset], nalu_index.payload_size,
                                                                 &bitstream_parser_state, parsing_options);
        uint32_t type = nal_unit.get()->nal_unit_header->nal_unit_type;
        //if(nalu_index.payload_size != nal_unit->parsed_length)
        //    std::cout << "Parsed length (" << nal_unit->parsed_length << ") and payload size (" << nalu_index.payload_size << ") does not match up! for NAL index " << type << std::endl;
            
        uint8_t* buf = new uint8_t[nal_unit->parsed_length+(nal_unit->trailing0x80Byte?1:0)]; memset(buf, 0x00, nal_unit->parsed_length+(nal_unit->trailing0x80Byte?1:0));
        h265nal::BitBufferWriter wbit_buffer(buf, nal_unit->parsed_length+(nal_unit->trailing0x80Byte?1:0)); // add extra byte for whatever
        if(WriteNalUnit(nal_unit.get(), std::prev(bitstream_parser_state.sps.end())->second.get(), &wbit_buffer))
        {
            size_t bytesoff, bitoff;
            wbit_buffer.GetCurrentOffset(&bytesoff, &bitoff); // bytesoff is size
            //if(bitoff > 0)
            //    std::cout << "bit offset!" << std::endl;
            WriteFile("/Users/arth/Development/TILE/kvazaar/buildosx/unescapednal.nal", buf, bytesoff); // check real fill rate of wbit_buffer
            
            // this is the original NAL
            std::vector<uint8_t> original;
            if(!h265nal::H265Utils::ReadFile("/Users/arth/Development/TILE/kvazaar/buildosx/original_unescaped.nal", original))
            {
                // check size first
                if(((int32_t)(bytesoff) - (int32_t)original.size()) != 0)
                {
                    std::cout << "Written NAL size " << bytesoff << " and Original NAL size " <<
                    original.size() << " don't match! NAL Type is " << type << " (" <<
                    h265nal::NalUnitTypeToString(type) << ")" << std::endl;
                    
                    // check content
                    size_t i = 0;
                    auto it = original.begin();
                    for(; (i < bytesoff) && (it < original.end()); it++, i++)
                    {
                        if(*it != buf[i]) {
                            std::cout << "Content for written NAL and original NAL do not match for NAL Type " << type << " (" <<
                            h265nal::NalUnitTypeToString(type) << ") at position " << i << std::endl;
                            break;
                        }
                    }
                }
                else // check content
                {
                    size_t i = 0;
                    auto it = original.begin();
                    for(; i < bytesoff; it++, i++)
                    {
                        if(*it != buf[i]) {
                            std::cout << "Content for written NAL and original NAL do not match for NAL Type " << type << " (" <<
                            h265nal::NalUnitTypeToString(type) << ")" << std::endl;
                            break;
                        }
                    }
                }
            }
            else
            {
                std::cout << "Error reading original unescaped NAL..." << std::endl;
            }
            // CHECK ESCAPED
            std::vector<uint8_t> packed_buffer = EscapeRbsp(buf, &wbit_buffer);
            WriteFile("/Users/arth/Development/TILE/kvazaar/buildosx/escapednal.nal", packed_buffer.data(), packed_buffer.size());
            
            size_t i = nalu_index.payload_start_offset;
            auto it = packed_buffer.begin();
            for(; (i < nalu_index.payload_start_offset+nalu_index.payload_size) && (it < packed_buffer.end()); it++, i++)
            {
                if(*it != buffer[i]) {
                    std::cout << "Content for written NAL and original NAL do not match for NAL Type " << type << " (" <<
                    h265nal::NalUnitTypeToString(type) << ") at position " << i << std::endl;
                    std::cout << std::hex << (int)buffer[i] << " " << (int)buffer[i+1] << " " << (int)buffer[i+2] << " " << (int)buffer[i+3] << " " << (int)buffer[i+4] << " " << (int)buffer[i+5] << " " << (int)buffer[i+6] << " " << (int)buffer[i+7] << std::endl;
                    std::cout << (int)*it << " " << (int)*(it+1) << " " << (int)*(it+2) << " " << (int)*(it+3) << " " << (int)*(it+4) << " " << (int)*(it+5) << " " << (int)*(it+6) << " " << (int)*(it+7) << std::endl;
                    
                    break;
                }
            }
            
            
        }
        else
            std::cout << "ERROR!" << std::endl;
        delete[] buf;
    }
    
    std::vector<uint8_t> finalbuffer;
    for (const auto &nalu_index : nalu_indices) {
        // 4.1. parse 1 NAL unit
        // note: If the NALU comes from an unescaped bitstreams, i.e.,
        // one with an explicit NALU length mechanism (like mp4 mdat
        // boxes), the right function is `ParseNalUnitUnescaped()`.
        auto nal_unit = h265nal::H265NalUnitParser::ParseNalUnit(&(buffer.data())[nalu_index.payload_start_offset], nalu_index.payload_size,
                                                                 &bitstream_parser_state, parsing_options);
    
        // TEST FLIP SLICES
        // 0, 10, 120, 130
        if(nal_unit->nal_unit_payload->slice_segment_layer != nullptr)
        {
            if(nal_unit->nal_unit_payload->slice_segment_layer->slice_segment_header->slice_segment_address == 120) {
                nal_unit->nal_unit_payload->slice_segment_layer->slice_segment_header->slice_segment_address = 130;
            }
            else if(nal_unit->nal_unit_payload->slice_segment_layer->slice_segment_header->slice_segment_address == 130) {
                nal_unit->nal_unit_payload->slice_segment_layer->slice_segment_header->slice_segment_address = 120;
            }/*
            else if(nal_unit->nal_unit_payload->slice_segment_layer->slice_segment_header->slice_segment_address == 10) {
                nal_unit->nal_unit_payload->slice_segment_layer->slice_segment_header->slice_segment_address = 130;
            }
            else if(nal_unit->nal_unit_payload->slice_segment_layer->slice_segment_header->slice_segment_address == 130) {
                nal_unit->nal_unit_payload->slice_segment_layer->slice_segment_header->slice_segment_address = 10;
            }*/
        }
        
        // uint32_t type = nal_unit.get()->nal_unit_header->nal_unit_type;
        uint8_t* buf = new uint8_t[nal_unit->parsed_length+(nal_unit->trailing0x80Byte?1:0)]; memset(buf, 0x00, nal_unit->parsed_length+(nal_unit->trailing0x80Byte?1:0));
        h265nal::BitBufferWriter wbit_buffer(buf, nal_unit->parsed_length+(nal_unit->trailing0x80Byte?1:0));
        assert(WriteNalUnit(nal_unit.get(), std::prev(bitstream_parser_state.sps.end())->second.get(), &wbit_buffer) == true);
        std::vector<uint8_t> unpacked_buffer = EscapeRbsp(buf, &wbit_buffer);
        PrefixNalUnit(finalbuffer,true); // nal_unit->sizePrefix == 3);
        finalbuffer.insert(finalbuffer.end(), unpacked_buffer.begin(), unpacked_buffer.end());
        delete[] buf;
    }
    WriteFile("/Users/arth/Development/TILE/kvazaar/buildosx/out2x2_mod.hevc", finalbuffer);

    delete options;
    
    //================================================================================================================
    // now read both files and show the parts with differences
    std::vector<uint8_t> bufferA, bufferB;
    if (h265nal::H265Utils::ReadFile("/Users/arth/Development/TILE/kvazaar/buildosx/out2x2.hevc", bufferA) < 0) {
        std::cout << "Original file load failed" << std::endl;
        return -1;
    }
    if (h265nal::H265Utils::ReadFile("/Users/arth/Development/TILE/kvazaar/buildosx/out2x2_mod.hevc", bufferB) < 0) {
        std::cout << "Modified file load failed" << std::endl;
        return -1;
    }
    
    auto itA = bufferA.begin();
    auto itB = bufferB.begin();
    size_t i = 0;
    for(; (itA < bufferA.end()) && (itB < bufferB.end()); itA++, itB++, i++)
    {
        if(*itA != *itB) {
            std::cout << "WARNING! Written file and original file do not match!" << std::endl;
            std::cout << std::hex << (int)*(itA-4) << " " << (int)*(itA-3) << " " << (int)*(itA-2) << " " << (int)*(itA-1) << " " << (int)*(itA) << " " <<
                (int)*(itA+1) << " " << (int)*(itA+2) << " " << (int)*(itA+3) << " " << (int)*(itA+4) << std::endl;
            std::cout << std::hex << (int)*(itB-4) << " " << (int)*(itB-3) << " " << (int)*(itB-2) << " " << (int)*(itB-1) << " " << (int)*(itB) << " " <<
                (int)*(itB+1) << " " << (int)*(itB+2) << " " << (int)*(itB+3) << " " << (int)*(itB+4) << std::endl;
            
            break;
        }
    }
    
    
    
    

    
    std::cout << "Byebye World" << std::endl;
    return 0;
}
