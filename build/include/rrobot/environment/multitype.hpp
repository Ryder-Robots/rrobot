#ifndef MULTITYPE_T_HPP
#define MULTITYPE_T_HPP

#include <unordered_map>
#include <string>

using namespace std;

namespace rrobot {
    enum MULTITYPE_T { 
       TRI,
       QUADP,
       QUADX,
       BI,
       GIMBAL,
       Y6,
       HEX6,
       FLYING_WING,
       Y4,
       HEX6X,
       OCTOX8,
       OCTOFLATP,
       OCTOFLATX,
       AIRPLANE,
       HELI_120,
       HELI_90,
       VTAIL4,
       HEX6H,
       SINGLECOPTER,
       DUALCOPTER,
       LANDDRONE_2W,
       LANDDRONE_4W,

    };

    #define VALID_MULTITYPE_KEYS_INIT  unordered_map<string, MULTITYPE_T> VALID_MULTITYPE_KEYS = {{"TRI",TRI},{"QUADP",QUADP},{"QUADX",QUADX},{"BI",BI},{"GIMBAL",GIMBAL},{"Y6",Y6},{"HEX6",HEX6},{"FLYING_WING",FLYING_WING},{"Y4",Y4},{"HEX6X",HEX6X},{"OCTOX8",OCTOX8},{"OCTOFLATP",OCTOFLATP},{"OCTOFLATX",OCTOFLATX},{"AIRPLANE",AIRPLANE},{"HELI_120",HELI_120},{"HELI_90",HELI_90},{"VTAIL4",VTAIL4},{"HEX6H",HEX6H},{"SINGLECOPTER",SINGLECOPTER},{"DUALCOPTER",DUALCOPTER},{"LANDDRONE_2W",LANDDRONE_2W},{"LANDDRONE_4W",LANDDRONE_4W},}
}  // namespace rrobot

#endif  // MULTITYPE_T_HPP
