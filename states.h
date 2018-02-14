/*
 * WifiStates.h
 *
 *  Created on: Jan 25, 2018
 *      Author: JohnTaylor
 */

#ifndef STATES_H_
#define STATES_H_


#define WIFI_HANDSHAKE          0xC0
#define WIFI_STATUS             0xE2
#define WIFI_CONNECTED          0x07

#define WIFI_DISCONNECTED       0x09
#define WIFI_PASSFAILED         0x0B
#define WIFI_CONNECTING         0x0D
#define WIFI_SCAN               0xC3
#define WIFI_CONNECT            0xCA
#define WIFI_DISCONNECT         0xC6
#define WIFI_C2000VERSION       0x61
#define WIFI_CHECK_UPDATE       0x63
#define WIFI_VERSIONREQUEST     0x69
#define WIFI_UPDATE_DEVICE      0x97
#define WIFI_DEVICEIP           0xE5
#define WIFI_DEVICEMAC          0xEA
#define WIFI_CURRENT_SSID       0xEE
#define WIFI_WORKOUT_STREAM     0x51
#define WIFI_LEVEL              0x52
#define WIFI_SUMMARY            0x55
#define WIFI_USER_NUMBER_CHECK  0x56
#define USER_PROFILE_EXITS      0x57
#define USER_PROFILE_DELETE     0x58
#define USER_PROFILE_DELETED    0x59
#define USER_PROFILE_ACTIVE_CHECK      0x5A
#define USER_PROFILE_NOT_ACTIVE        0x5B
#define USER_PROFILE_ACTIVE            0x5C
#define WIFI_USER_PROFILE_PARAMETER    0x5D
#define WIFI_USER_PARAMETER_REQUEST    0x5E
#define WIFI_SCAN_USER_PROFILES        0x5F
#define WIFI_SCAN_NEXT                 0x60
#define WIFI_SCAN_PREVIOUS             0x62
#define WIFI_NEW_PROFILE_ADDED         0x64
#define WIFI_CONNECT_HIDDEN            0x65
#define WIFI_STIMULATION_LEVEL_UPDATE  0x66
#define WIFI_UPDATE_SURVEY_COUNTER     0x67
#define WIFI_CONTACT_INFO              0x68
#define WIFI_CLEAR_FRAM                0x70
#define WIFI_IS_PRO_VERSION            0x71
#define WIFI_SET_VERSION               0x72
#define WIFI_LOGIN                     0x73
#define WIFI_LOGIN_CHECK               0x74
#define WIFI_LOGIN_SET                 0x75
#define WIFI_LOGIN_LAST                0x76
#define WIFI_BEACON_SEND               0x77
#define WIFI_CONNECTED_NO_INTERNET     0x78



#endif /* STATES_H_ */
