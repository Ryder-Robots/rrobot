
# Configure MultiType
set(LOOP_CONTENT "")
set(MULTITYPE_LIST TRI QUADP QUADX BI GIMBAL Y6 HEX6 FLYING_WING Y4 HEX6X OCTOX8 OCTOFLATP OCTOFLATX AIRPLANE HELI_120 HELI_90 VTAIL4 HEX6H SINGLECOPTER DUALCOPTER LANDDRONE_2W LANDDRONE_4W)
foreach(item ${MULTITYPE_LIST})
    string(APPEND LOOP_CONTENT "       ${item},\n")
endforeach()

foreach(item ${MULTITYPE_LIST})
  string(APPEND VALID_MT "\{\"${item}\"\,${item}\},")
endforeach()

set(GENERATED_TYPE ${LOOP_CONTENT})
set(VALID_MT ${VALID_MT})
# end configure multitype


set(rrobot_INCLUDE 
    rrobot/environment/version.hpp
    rrobot/environment/mspsensors.hpp
    rrobot/environment/multitype.hpp
    rrobot/environment/mspversion.hpp
)

foreach(item ${rrobot_INCLUDE})
  configure_file("${item}.in" ${item} @ONLY)
endforeach()