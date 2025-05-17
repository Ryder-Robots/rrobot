
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
    rrobot/environment/cqueues.hpp
    rrobot/environment/crr_server.hpp
    rrobot/environment/clogging.hpp
    rrobot/environment/environment.hpp
    rrobot/environment/environmentProcessor.hpp
    rrobot/state/rrpqueue_names.hpp
    rrobot/state/rrqueue_manager.hpp
    rrobot/state/state.hpp
    rrobot/state/state_manager.hpp
    rrobot/state/rrpstatus.hpp
    rrobot/events/cmodes.hpp
    rrobot/events/mspdirection.hpp
    rrobot/events/eventhandler.hpp
    rrobot/events/eventrunner.hpp
    rrobot/events/serializers/serializer.hpp
    rrobot/events/serializers/jserializer.hpp
    rrobot/events/serializers/mserializer.hpp
    rrobot/protocols/curators/rrp/rrpcuratorabstract.hpp
    rrobot/protocols/curators/multiwii/crc.hpp
    rrobot/protocols/curators/multiwii/rmultiwii_curator.hpp
    rrobot/protocols/curators/multiwii/rmmsp_sonic_curator.hpp
    rrobot/protocols/curators/multiwii/abstract_sensor_curator.hpp
    rrobot/protocols/curators/multiwii/rmmsp_sensor_mag_curator.hpp
    rrobot/protocols/curators/multiwii/rmmsp_sensor_acc_curator.hpp
    rrobot/protocols/curators/multiwii/rmmsp_sensor_gyro_curator.hpp
    rrobot/protocols/curators/encoder.hpp
    rrobot/com/external.hpp
    rrobot/com/server.hpp
    rrobot/com/rrserialmc.hpp
    rrobot/handlers/uihandler.hpp
    rrobot/handlers/statushandler.hpp
    rrobot/handlers/rrmapper.hpp
    rrobot/handlers/threadmanager.hpp
    rrobot/handlers/identhandler.hpp
    rrobot/handlers/aihandler.hpp
    rrobot/handlers/currentstatehandler.hpp
    rrobot/ai/aifeatures.hpp
    rrobot/ai/ai.hpp
    rrobot/ai/greedyai.hpp
    rrobot/ai/aiiface.hpp
    rrobot/ai/aibasefunctions.hpp
)   

foreach(item ${rrobot_INCLUDE})
  configure_file("${item}.in" ${item} @ONLY)
endforeach()

#Exceptions
set(EXCEPTIONS 
  AttemptToDequeueToEmptyQueue
  InvalidManifestException 
  MissingRequiredAttributeException 
  QueueDoesNotExit 
  ExceedMaxMessage 
  InvalidMessageSent 
  InvalidFormat 
  InvalidCommand 
  UnsupportedAttribute 
  BadConnectionException 
  FailedToInitilizeHandler 
  NetworkIOException
  TimeOutException
  InvalidStatus
  RrIOException)
set(INCLUDE_CONTENT "")
foreach(item ${EXCEPTIONS})
  set(EXCEPTION_NAME ${item})
  set(OUTPUT_NAME "rrobot/exceptions/${EXCEPTION_NAME}.hpp")
  string(TOUPPER "${EXCEPTION_NAME}_HPP" EXCEPTION_HEADING)
  configure_file(rrobot/exceptions/exception.hpp.in ${OUTPUT_NAME} @ONLY)
  string(APPEND INCLUDE_CONTENT "\#include <rrobot/exceptions/${EXCEPTION_NAME}.hpp>\n")
endforeach()
set(GENERATED_INCLUDES ${INCLUDE_CONTENT})
configure_file(rrobot/exceptions/exceptions.hpp.in rrobot/exceptions/exceptions.hpp @ONLY)


# MSP104 and MSP215 should be obsleted.

# MSP COMMANDS
# structure of hash is
# 0 command
# 1 command value
# 2 direction (which queue to message should be directed to)
# 3 if true then generate serializer
# 4 if true then generate deseralizer
# 5 if true supported by multiwii
# 6..n members in pairs, type [attribute]

set(MSPCOMMANDS 
  "MSP_NONE 0 INVALID 1 1 1"
  "MSP_MODE 1 EXTERNAL_IN 0 0 0 RR_CMODES mode"
  "MSP_ERROR 2 EXTERNAL_OUT 1 1 1 string message"
  "MSP_AUTHKEY 7 EXTERNAL_IN 1 1 0 string key"
  "MSP_IDENT 100 EXTERNAL_OUT 0 0 0 std::string version MULTITYPE_T multitype MSP_VERSION msp_version uint32_t capability"
  "MSP_STATUS 101 EXTERNAL_OUT 1 1 1 uint16_t cycletime uint16_t i2c_errors_count uint16_t sensor uint32_t flag uint8_t current_set"
  "MSP_SONAR_ALTITUDE 58 EXTERNAL_OUT 1 1 1 uint32_t distance uint32_t temperature" 
  "MSP_SENSOR_ACC 216 EXTERNAL_OUT 0 0 1 uint32_t available dlib::vector<float,3> data"
  "MSP_SENSOR_GYRO 217 EXTERNAL_OUT 0 0 1 uint32_t available dlib::vector<float,3> data"
  "MSP_SENSOR_MAG 218 EXTERNAL_OUT 0 0 1 uint32_t available dlib::vector<float,3> data"
  "MSP_STOP 220 EXTERNAL_OUT 1 1 1"
  "MSP_MOVE 221 EXTERNAL_OUT 1 1 1"
  "MSP_VECTOR 222 EXTERNAL_IN 1 1 1 float x float y float z")

set(LOOP_CONTENT "")
set(VALID_MSPCOMMANDS "")
set(INCLUDE_CONTENT "")
set(INCLUDE_CURATOR_CONTENT "")
set(CURATOR_PAIR "")
set(MULTIWII_COMMANDS "")
set(MULTWII_I 0)
set(DECONSTRUCT_EVENT "")
set(MULTWII_I 0)

foreach(item ${MSPCOMMANDS})
  separate_arguments(_elements UNIX_COMMAND ${item})
  list(LENGTH _elements _length)
  list(GET _elements 0 _key)
  list(GET _elements 1 _value)
  list(GET _elements 2 _direction)
  list(GET _elements 3 _serializer)
  list(GET _elements 4 _deserializer)
  list(GET _elements 5 _multiwii_supported)

  set(CURATATOR_SERIALIZER ${_serializer})
  set(CURATATOR_DESERIALIZER ${_deserializer})
  string(TOUPPER "${_key}_MCURATATOR_SERIALIZER" MCURATATOR_SERIALIZER)
  string(TOUPPER  "${_key}_MCURATATOR_DESERIALIZER" MCURATATOR_DESERIALIZER)

  string(APPEND LOOP_CONTENT "${_key} = ${_value},\n")
  string(APPEND VALID_MSPCOMMANDS "\{\"${_key}\"\, ${_key}\},\\\n")

  string(TOUPPER "${_key}_HPP" EVENT_HEADING)
  string(TOUPPER "${_key}_CURATOR_HPP" CURATOR_HEADING)
  string(TOLOWER "${_key}" CLASS_NAME)
  string(TOLOWER "${_key}_curator" CURATOR_CLASS_NAME)
  string(TOLOWER "rrobot/events/${_key}.hpp" OUTPUT_NAME)
  string(TOLOWER "rrobot/protocols/curators/rrp/${_key}_curator.hpp" OUTPUT_CURATOR_NAME)
  string(APPEND INCLUDE_CONTENT "\#include <${OUTPUT_NAME}>\n")
  string(APPEND INCLUDE_CURATOR_CONTENT "\#include <${OUTPUT_CURATOR_NAME}>\n")
  string(APPEND CURATOR_PAIR "\{${_key}, new ${CURATOR_CLASS_NAME}()\},")

  set(MSP_COMMAND ${_key})
  string(APPEND DECONSTRUCT_EVENT "                case MSPCOMMANDS::${MSP_COMMAND}:\n                   delete(reinterpret_cast<${CLASS_NAME}*>(_payload));\n")
  string(APPEND DECONSTRUCT_EVENT "                   break;\n")
  set(GETTERS "")
  set(MEMBERS "")
  set(CURATOR_SETTERS "")
  set(DIRECTION ${_direction})
  set(CURATOR_SERIALIZER_SETTERS "")
  
  if(${_multiwii_supported})
    string(APPEND MULTIWII_COMMANDS "        _mspCommands[${MULTWII_I}] = static_cast<uint8_t>(MSPCOMMANDS::${_key});\n")
    math(EXPR MULTWII_I "${MULTWII_I} + 1")
  endif()
  
  math(EXPR last_index "${_length} - 1")
  if (${_length} GREATER 6)
    set(start_index 6)
    foreach(i RANGE ${start_index} ${last_index} 2)
      list(SUBLIST _elements ${i} 2 pair)
      list(GET pair 0 type)
      list(GET pair 1 member)
      string(APPEND GETTERS "    void set_${member}(${type} ${member})\{_${member} = ${member};\}\n    ${type} get_${member}()\{return _${member};\}\n")
      string(APPEND MEMBERS "    ${type} _${member};\n")
      string(APPEND CURATOR_SETTERS "                validate(\"${member}\", in);\n                payload->set_${member}(in[\"${member}\"]);\n")
      string(APPEND CURATOR_SERIALIZER_SETTERS "                    \{\"${member}\", payload.get_${member}()\},\n")
    endforeach()
  endif()
  configure_file(rrobot/events/event.tmpl.hpp.in ${OUTPUT_NAME} @ONLY)
  configure_file(rrobot/protocols/curators/rrp/curator.tmpl.hpp.in ${OUTPUT_CURATOR_NAME} @ONLY)
endforeach()
configure_file(rrobot/protocols/curators/multiwii/rmultiwii.hpp.in rrobot/protocols/curators/multiwii/rmultiwii.hpp @ONLY)
configure_file(rrobot/events/mspcommands.hpp.in rrobot/events/mspcommands.hpp @ONLY)
configure_file(rrobot/protocols/curators/rrp/mspevent.hpp.in rrobot/protocols/curators/rrp/mspevent.hpp)
configure_file(rrobot/protocols/curators/rrp/rrpcurator.hpp.in rrobot/protocols/curators/rrp/rrpcurator.hpp)
configure_file(rrobot/events/event.hpp.in   rrobot/events/event.hpp @ONLY)
