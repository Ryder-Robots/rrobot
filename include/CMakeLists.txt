
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

# Serial type configuration.
set(BAUD_RATES BAUD_9600 BAUD_19200 BAUD_38400 BAUD_57600 BAUD_115200 BAUD_DEFAULT)
set(LOOP_CONTENT "")
foreach(item ${BAUD_RATES})
  string(APPEND LOOP_CONTENT "\{\"${item}\"\, LibSerial::BaudRate::${item}\},")
endforeach()
set(VALID_BAUDRATES ${LOOP_CONTENT})

set(VALID_CHAR_SZ CHAR_SIZE_5 CHAR_SIZE_6 CHAR_SIZE_7 CHAR_SIZE_8 CHAR_SIZE_DEFAULT)
set(LOOP_CONTENT "")
foreach(item ${VALID_CHAR_SZ})
  string(APPEND LOOP_CONTENT "\{\"${item}\"\, LibSerial::CharacterSize::${item}\},")
endforeach()
set(VALID_CHAR_SZ ${LOOP_CONTENT})

set(VALID_FLOW_CNT FLOW_CONTROL_HARDWARE FLOW_CONTROL_SOFTWARE FLOW_CONTROL_NONE)
set(LOOP_CONTENT "")
foreach(item ${VALID_FLOW_CNT})
  string(APPEND LOOP_CONTENT "\{\"${item}\"\, LibSerial::FlowControl::${item}\},")
endforeach()
set(VALID_FLOW_CNT ${LOOP_CONTENT})

set(VALID_PARITY PARITY_EVEN PARITY_ODD PARITY_NONE)
set(LOOP_CONTENT "")
foreach(item ${VALID_PARITY})
  string(APPEND LOOP_CONTENT "\{\"${item}\"\, LibSerial::Parity::${item}\},")
endforeach()
set(VALID_PARITY ${LOOP_CONTENT})

set(VALID_STOPBIT STOP_BITS_1 STOP_BITS_2 STOP_BITS_DEFAULT)
set(LOOP_CONTENT "")
foreach(item ${VALID_STOPBIT})
  string(APPEND LOOP_CONTENT "\{\"${item}\"\, LibSerial::StopBits::${item}\},")
endforeach()
set(VALID_STOPBIT ${LOOP_CONTENT})
#end serialtype configuration


set(rrobot_INCLUDE 
    rrobot/environment/version.hpp
    rrobot/environment/mspsensors.hpp
    rrobot/environment/multitype.hpp
    rrobot/environment/mspversion.hpp
    rrobot/environment/chwmodel.hpp
    rrobot/environment/crrserial.hpp
)

foreach(item ${rrobot_INCLUDE})
  configure_file("${item}.in" ${item} @ONLY)
endforeach()