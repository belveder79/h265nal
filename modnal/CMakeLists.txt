
set (modnal_SOURCES
  h265_aud_writer.cc
  h265_vps_writer.cc
  h265_sps_writer.cc
  h265_pps_writer.cc
  h265_sei_writer.cc
  h265_slice_writer.cc
  h265_nal_unit_writer.cc
  h265_wutils.cc
)

add_executable(modnal main.cc ${modnal_SOURCES})

target_sources(modnal
  PUBLIC
  FILE_SET HEADERS
  BASE_DIRS "."
  FILES "h265_aud_writer.h"
        "h265_vps_writer.h"
        "h265_sps_writer.h"
        "h265_pps_writer.h"
        "h265_sei_writer.h"
        "h265_slice_writer.h"
        "h265_nal_unit_writer.h"
        "h265_wutils.h"
)

get_target_property(HeaderList modnal HEADER_SET)


target_include_directories(modnal PUBLIC ../src)
target_link_libraries(modnal PUBLIC h265nal)
# rename executable using target properties
set_target_properties(modnal PROPERTIES OUTPUT_NAME modnal)
