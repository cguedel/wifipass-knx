#define MANUFACTURER_ID 1234
#define DEVICE_ID 1
#define REVISION 0

#define COMOBJ_ssid1 0
#define COMOBJ_ssid2 1
#define COMOBJ_ssid1_status 2
#define COMOBJ_ssid2_status 3
#define COMOBJ_security 4
#define COMOBJ_security_status 5
#define COMOBJ_password1 6
#define COMOBJ_password2 7
#define COMOBJ_password1_status 8
#define COMOBJ_password2_status 9
#define PARAM_comobject_ssid 0
#define PARAM_ssid1 1
#define PARAM_ssid2 2
#define PARAM_comobject_security 3
#define PARAM_security 4
#define PARAM_comobject_password 5
#define PARAM_password1 6
#define PARAM_password2 7
        
KnxComObject KnxDevice::_comObjectsList[] = {
    /* Index 0 - ssid1 */ KnxComObject(KNX_DPT_16_000, 0x2a),
    /* Index 1 - ssid2 */ KnxComObject(KNX_DPT_16_000, 0x2a),
    /* Index 2 - ssid1_status */ KnxComObject(KNX_DPT_16_000, 0x34),
    /* Index 3 - ssid2_status */ KnxComObject(KNX_DPT_16_000, 0x34),
    /* Index 4 - security */ KnxComObject(KNX_DPT_5_010, 0x2a),
    /* Index 5 - security_status */ KnxComObject(KNX_DPT_5_010, 0x34),
    /* Index 6 - password1 */ KnxComObject(KNX_DPT_16_000, 0x2a),
    /* Index 7 - password2 */ KnxComObject(KNX_DPT_16_000, 0x2a),
    /* Index 8 - password1_status */ KnxComObject(KNX_DPT_16_000, 0x34),
    /* Index 9 - password2_status */ KnxComObject(KNX_DPT_16_000, 0x34)
};
const byte KnxDevice::_numberOfComObjects = sizeof (_comObjectsList) / sizeof (KnxComObject); // do not change this code
       
byte KonnektingDevice::_paramSizeList[] = {
    /* Index 0 - comobject_ssid */ PARAM_UINT8,
    /* Index 1 - ssid1 */ PARAM_STRING11,
    /* Index 2 - ssid2 */ PARAM_STRING11,
    /* Index 3 - comobject_security */ PARAM_UINT8,
    /* Index 4 - security */ PARAM_UINT8,
    /* Index 5 - comobject_password */ PARAM_UINT8,
    /* Index 6 - password1 */ PARAM_STRING11,
    /* Index 7 - password2 */ PARAM_STRING11
};
const int KonnektingDevice::_numberOfParams = sizeof (_paramSizeList); // do not change this code