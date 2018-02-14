
#include "Arduino.h"
#include "ESP8266WiFi.h"
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_FRAM_I2C.h"
#include "states.h"
#include "constants.h"
#include "ESP8266Ping.h"
String DeviceID = "000010";
unsigned long last;
unsigned long current;
unsigned long diff;
Adafruit_FRAM_I2C fram = Adafruit_FRAM_I2C();
wl_status_t currentStatus;
bool internetConnectionStatus = false;
bool lastInternetConnectionStatus = false;
char password[PASS_LENGTH] = {
  0x00
};
char ssid[SSID_LENGTH] = {
  0x00
};
char current_ssid[SSID_LENGTH] = {
  0x00
};

//Contact info

char contactName[NAME_LENGTH] = {
  0x00
};
char contactEmail[EMAIL_LENGTH] = {
  0x00
};
char contactPhone[PHONE_LENGTH] = {
  0x00
};
char UserID[7] = {
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00
};
char User_Pin[5] = {
  0x00,
  0x00,
  0x00,
  0x00
};
char UserID_default[7] = {
  0x31,
  0x32,
  0x33,
  0x34,
  0x35,
  0x36,
  0x37
};
char User_Pin_default[5] = {
  0x39,
  0x38,
  0x37,
  0x36
};

char host[50] = "updates.myolyn.com";

char kernel[5000];
char AppCodeVersion[20];
char AppCodeChecksum[20];
char AppCodeBytes[20];
char AppCodeHex[210];
char DeviceVersionArray[9];
char DeviceNewVersionArray[9];
char DeviceIDArray[7];
char DeviceIP[16];
char DeviceMACarray[20];

uint8_t stored = 0;
uint8_t sendbyte = 0;
byte baudratelock = 0;
bool UpdateGo = false;
bool ResetAppCode = false;

bool Update_Flag = false;

String line;
String line1;
String line2;
String line3;
String line4;
String line5;
String ServerID = "000000";



String Version = "00000000";
String NewVersion = "00000000";

String Workout = "";
String WiFiStatus = "1";
uint16_t workout_Data_count = 0;

uint32_t k = 0;
int state = 0;
uint8_t flag = 1;
uint32_t loops = 0;

uint16_t AppSendChecksum = 0;
uint16_t AppRecChecksum = 0;

uint32_t AppTotalChecksum = 0;

uint32_t AppBytesIndex = 0;
uint32_t AppTotalBytes = 0;
uint32_t txCount = 0;

uint32_t downloadedChecksum = 0;
uint32_t FramAddress = 0;

uint32_t AppBlockSize = 0;

uint32_t ChecksumCounts = 0;

String AppCodeVersionVar;
uint32_t AppCodeChecksumVar = 0;
uint32_t AppCodeBytesVar = 0;

uint8_t TestChecksumArray[1030];
uint8_t workout_summary_array[10];

bool showInitialStatus = true;

uint8_t Networks = 0;

uint8_t receiveBuffer[RECEIVE_BUFFER_SIZE];
boolean receiveComplete = false; // whether the string is complete
uint8_t receiveBufferIndex = 0;
uint8_t leftIndex = 0;

unsigned char Wifi_end_bytes_A[4] = {
  0xAA,
  0x44,
  0xA4,
  0x4A
};
unsigned char Wifi_handshake_A[2] = {
  0xBB,
  WIFI_HANDSHAKE
};
unsigned char Wifi_status_A[3] = {
  0xBB,
  WIFI_STATUS,
  WIFI_DISCONNECTED
};
unsigned char Wifi_scan_A[24] = {
  0xBB,
  WIFI_SCAN
};
unsigned char Wifi_checkUpdate_A[3] = {
  0xBB,
  WIFI_CHECK_UPDATE,
  0x00
};
unsigned char Wifi_versionInfo_A[25] = {
  0xBB,
  WIFI_VERSIONREQUEST
};
unsigned char Wifi_deviceIPInfo_A[7] = {
  0xBB,
  WIFI_DEVICEIP
};
unsigned char Wifi_deviceMACInfo_A[20] = {
  0xBB,
  WIFI_DEVICEMAC
};
unsigned char Wifi_deviceSSID_A[32] = {
  0xBB,
  WIFI_CURRENT_SSID
};
unsigned char Wifi_userProfileNumber_A[3] = {
  0xBB,
  WIFI_USER_NUMBER_CHECK
};

unsigned char Wifi_userProfileParameters_A[9] = {
  0xBB,
  WIFI_USER_PARAMETER_REQUEST,
  0x01,
  0x00,
  0x00,
  0x6A,
  0xAF,
  0x8C,
  0x06
};
unsigned char Wifi_requestProfile_A[3] = {
  0xBB,
  WIFI_SCAN_NEXT,
  0x00
};
unsigned char user_profile_status[50] = {
  0x00
};
unsigned char Wifi_workoutSummary_A[2] = {
  0xBB,
  WIFI_SUMMARY
};
unsigned char Wifi_updateSurveryCounter[4] = {
  0xBB,
  WIFI_UPDATE_SURVEY_COUNTER,
  0x01,
  0x06
};
unsigned char Wifi_clearFRAM[2] = {
  0xBB,
  WIFI_CLEAR_FRAM
};
unsigned char Wifi_contactInfoReceived[2] = {
  0xBB,
  WIFI_CONTACT_INFO
};
unsigned char Wifi_proVersionStatus[3] = {
  0xBB,
  WIFI_IS_PRO_VERSION
};
unsigned char Wifi_serverLoginStatus[3] = {
  0xBB,
  WIFI_LOGIN,
  0x00
};
unsigned char Wifi_serverLoginCheck[3] = {
  0xBB,
  WIFI_LOGIN_CHECK,
  0x00
};
char SSIDarray[SSID_BUFFER_SIZE];

uint8_t SSIDsize = 0;
uint8_t PSWDsize = 0;
uint8_t SSIDsizeInit = 0;
uint8_t index8 = 0;
uint8_t MACsize = 0;
//For contact info
uint8_t contactNameSize = 0;
uint8_t contactEmailSize = 0;

uint8_t workout_user_profile = 0;
uint8_t workout_user_status = 0;
uint8_t user_profile_levels_left_spasm = 0;
uint8_t user_profile_levels_right = 0;
uint8_t user_profile_thigh = 0;
uint8_t user_profile_lower_leg = 0;
uint8_t user_profile_seat_height = 0;
uint8_t status_val = 0;
int i = 0;
uint8_t user_profile_survery_counter = 0;
int current_user_profile_index = 0;
boolean next_available = false;
uint8_t proVersionStatus = 0;
uint8_t userLoginSet = 0;
bool isConnectedToInternet() {

  return internetConnectionStatus;
}
void updateInternetConnectionStatus()
{
	  if(Ping.ping(host, 1))
	  {
		  internetConnectionStatus = true;
	  }
	  else
	  {
		  internetConnectionStatus = false;
	  }
}
void Wifi_Send_Command(volatile unsigned char * p, int size) {
  int itr = 0;
  for (itr = 0; itr < size; itr++) {
    Serial.write( * p);
    p++;
  }

  for (itr = 0; itr < 4; itr++) {
    Serial.write(Wifi_end_bytes_A[itr]);
  }
}

void disconnect() {
  WiFiStatus = "1";
  sendBeacon();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Wifi_status_A[2] = WIFI_DISCONNECTED;
  Wifi_Send_Command(Wifi_status_A, 3);
  fram.write8(FRAM1, 0x30, WIFI_DISCONNECTED);
}
void scan() {
  Networks = WiFi.scanNetworks();
}
bool SetDeviceModel() {
  WiFiClient client;
  int httpPort = 80;
  if (isConnectedToInternet()) {
    client.connect(host, httpPort);

    String url = "/Updates/SetModelVersion.php?DeviceID=" + DeviceID + "&Model=" + proVersionStatus;

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" +
      "Connection: close\r\n\r\n");
    delay(100);
    return true;
  } else {
    return false;
  }
}
bool connect() {
  WiFi.begin(ssid, password);
  Wifi_status_A[2] = WIFI_CONNECTING;
  if (showInitialStatus) {
    Wifi_Send_Command(Wifi_status_A, 3);
  }
  int j = 0;
  while (WiFi.status() != WL_CONNECTED && j <= 40) {
    delay(500);
    ++j;
    Wifi_status_A[2] = WIFI_CONNECTING;
  }
  if (j >= 40) {
    Wifi_status_A[2] = WIFI_PASSFAILED;
    if (showInitialStatus) {
      Wifi_Send_Command(Wifi_status_A, 3);
    }
    return false;
  } else {

    fram.write8(FRAM1, 0x30, WIFI_CONNECTED);
    fram.write8(FRAM1, 0x31, SSIDsizeInit);
    for (index8 = 0; index8 < SSID_LENGTH; index8++) {
      fram.write8(FRAM1, 0x32 + index8, ssid[index8]);
    }
    fram.write8(FRAM1, 0x52, PSWDsize);
    for (index8 = 0; index8 < PASS_LENGTH; index8++) {
      fram.write8(FRAM1, 0x53 + index8, password[index8]);
    }

    Wifi_status_A[2] = WIFI_CONNECTED;

    if (showInitialStatus) {
      Wifi_Send_Command(Wifi_status_A, 3);
    }
    Serial.println("C");
    WiFiStatus = "2";
    sendBeacon();
  }


}

bool scanForUpdate() {
  state = 0;
  Serial.read();
  WiFiClient client;
  int httpPort = 80;
  if (isConnectedToInternet()) {
    client.connect(host, httpPort);
    String url = "/Updates/MyoCycleIDList.txt";

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" +
      "Connection: close\r\n\r\n");
    delay(100);

    while (client.available() && flag == 1) {
      if (state == 0) {
        line = client.readStringUntil('=');
      } else if (state == 1) {
        ServerID = client.readStringUntil('?');

        if (DeviceID != ServerID) {
          state = -1;
          Wifi_checkUpdate_A[2] = 0x21;
        } else {
          Wifi_checkUpdate_A[2] = 0x22;
        }
      } else if (state == 2) {
        NewVersion = client.readStringUntil('#');
        if (NewVersion != Version) {
          Update_Flag = true;
          Wifi_checkUpdate_A[2] = 0x23;
        } else {
          Wifi_checkUpdate_A[2] = 0x24;
        }
      }

      state++;

      if (state > 2) {
        state = 0;
        flag = 0;
      }
    }

    state = 0;
    flag = 1;
    return true;
  } else {
    return false;
  }

}
bool scanForUserID() {
  state = 0;
  Serial.read();
  WiFiClient client;
  int httpPort = 80;
  if (isConnectedToInternet()) {
    client.connect(host, httpPort);
    String url = "/Workouts/Download.php?ClientID=" + DeviceID + "&UserID=" + UserID + "&Pin=" + User_Pin;

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" +
      "Connection: close\r\n\r\n");
    delay(100);
    Wifi_serverLoginStatus[2] = SERVER_ERROR;
    while (client.available() && flag == 1) {
      if (state == 0) {
        line = client.readStringUntil('!');
      } else if (state == 1) // User_ID
      {
        line2 = client.readStringUntil('@');
        // Serial.println("User_ID: " + line2);
      } else if (state == 2) // Status
      {
        line3 = client.readStringUntil('#');
        Wifi_serverLoginStatus[2] = line3.toInt();
        if (Wifi_serverLoginStatus[2] == 2) {
          fram.write8(FRAM1, 0x80, 1);
        } else {
          fram.write8(FRAM1, 0x80, 0);
        }
        //      Serial.println("Loging: " + fram.read8(FRAM1,0x80));
        //      Serial.println("Status: " + Wifi_serverLoginStatus[2]);
      } else if (state == 3) // Thigh
      {
        line3 = client.readStringUntil('$');
        user_profile_thigh = ((line3.toInt()) - 50);
        fram.write8(FRAM1, 0x1003, user_profile_thigh);
        //      Serial.println("Thigh: " + fram.read8(FRAM1,0x1003));
      } else if (state == 4) // LowerLeg
      {
        line3 = client.readStringUntil('%');
        user_profile_lower_leg = ((line3.toInt()) - 50);
        fram.write8(FRAM1, 0x1004, user_profile_lower_leg);
        //      Serial.println("LowerLeg: " + fram.read8(FRAM1,0x1004));
      } else if (state == 5) // Seat
      {
        line3 = client.readStringUntil('^');
        user_profile_seat_height = ((line3.toInt()) - 50);
        fram.write8(FRAM1, 0x1005, user_profile_seat_height);
        //      Serial.println("Seat: " + fram.read8(FRAM1,0x1005));
      } else if (state == 6) // Left Gain  Spasm
      {
        line3 = client.readStringUntil('&');
        user_profile_levels_left_spasm = line3.toInt();
        fram.write8(FRAM1, 0x1001, user_profile_levels_left_spasm);
        //      Serial.println("Left Gains: " + fram.read8(FRAM1,0x1001));
      } else if (state == 7) //  Right Gain
      {
        line3 = client.readStringUntil('*');
        user_profile_levels_right = line3.toInt();
        fram.write8(FRAM1, 0x1002, user_profile_levels_right);
        //      Serial.println("Right Gains: " + fram.read8(FRAM1,0x1002));
      }

      state++;

      if (state > 7) {
        state = 0;
        flag = 0;
      }
    }
    //Serial.println("DONE!");
    state = 0;
    flag = 1;
    return true;
  } else {
    return false;
  }

}
bool uploadSummary(void) {
  WiFiClient client;
  int httpPort = 80;
  if (isConnectedToInternet()) {
    client.connect(host, httpPort);
    String url = "/Workouts/SummaryWorkout.php?DeviceID=" + DeviceID +
      "&UserID=" + UserID +
      "&Duration=" + workout_summary_array[0] +
      "&Speed=" + workout_summary_array[1] +
      "&Power=" + workout_summary_array[2] +
      "&Distance=" + workout_summary_array[3] +
      "&Calories=" + workout_summary_array[4] +
      "&Spasm=" + workout_summary_array[8] +
      "&Status=" + workout_summary_array[9] +
      "&LeftQuadsGain=" + ((user_profile_levels_left_spasm >> 6) & 0x03) +
      "&LeftHamsGain=" + ((user_profile_levels_left_spasm >> 4) & 0x03) +
      "&LeftGlutesGain=" + ((user_profile_levels_left_spasm >> 2) & 0x03) +
      "&RightQuadsGain=" + ((user_profile_levels_right >> 6) & 0x03) +
      "&RightHamsGain=" + ((user_profile_levels_right >> 4) & 0x03) +
      "&RightGlutesGain=" + ((user_profile_levels_right >> 2) & 0x03);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" +
      "Connection: close\r\n\r\n");
    delay(100);

    //  Serial.println (url);
    return true;
  } else {
    return false;
  }

}
bool uploadSummaryOnCancel(void) {
  WiFiClient client;
  int httpPort = 80;
  if (isConnectedToInternet()) {
    client.connect(host, httpPort);
    Wifi_Send_Command(Wifi_workoutSummary_A, 2);
    String url = "/Workouts/SummaryWorkout.php?DeviceID=" + DeviceID +
      "&UserID=" + UserID +
      "&Duration=" + workout_summary_array[0] +
      "&Speed=" + workout_summary_array[1] +
      "&Power=" + workout_summary_array[2] +
      "&Distance=" + workout_summary_array[3] +
      "&Calories=" + workout_summary_array[4] +
      "&Spasm=" + workout_summary_array[8] +
      "&Status=" + workout_summary_array[9] +
      "&LeftQuadsGain=" + ((user_profile_levels_left_spasm >> 6) & 0x03) +
      "&LeftHamsGain=" + ((user_profile_levels_left_spasm >> 4) & 0x03) +
      "&LeftGlutesGain=" + ((user_profile_levels_left_spasm >> 2) & 0x03) +
      "&RightQuadsGain=" + ((user_profile_levels_right >> 6) & 0x03) +
      "&RightHamsGain=" + ((user_profile_levels_right >> 4) & 0x03) +
      "&RightGlutesGain=" + ((user_profile_levels_right >> 2) & 0x03);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" +
      "Connection: close\r\n\r\n");
    delay(100);
    Wifi_Send_Command(Wifi_workoutSummary_A, 2);
    return true;
  } else {
    return false;
  }
}
bool getModel() {
  state = 0;
  Serial.read();
  WiFiClient client;
  int httpPort = 80;
  if (isConnectedToInternet()) {
    client.connect(host, httpPort);
    String url = "/Updates/ModelVersion.php?DeviceID=" + DeviceID;

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" +
      "Connection: close\r\n\r\n");
    delay(100);

    while (client.available() && flag == 1) {
      if (state == 0) {
        line = client.readStringUntil('!');
      } else if (state == 1) // Device_ID
      {
        line2 = client.readStringUntil('@');
        // Serial.println("Device_ID: " + line2);
      } else if (state == 2) // Model
      {
        line3 = client.readStringUntil('#');
        proVersionStatus = line3.toInt();
        if (proVersionStatus == 0) {
          fram.write8(FRAM1, 0x73, 0);
        } else if (proVersionStatus == 1) {
          fram.write8(FRAM1, 0x73, 1);
        } else {
          //
        }

      } else {
        //
      }

      state++;

      if (state > 2) {
        state = 0;
        flag = 0;
      }
    }
    //Serial.println("DONE!");
    state = 0;
    flag = 1;
    return true;

  } else {
    return false;
  }

}
bool uploadContactInfo(void) {
  WiFiClient client;
  int httpPort = 80;
  if (isConnectedToInternet()) {
    client.connect(host, httpPort);
    String url = "/Workouts/ContactInfo.php?DeviceID=" + DeviceID +
      "&UserID=" + UserID +
      "&Name=" + contactName +
      "&Email=" + contactEmail +
      "&Phone=" + contactPhone;

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" +
      "Connection: close\r\n\r\n");
    delay(100);
    return true;
  } else {
    return false;
  }

}
bool startWorkout(void) {
  WiFiClient client;
  int httpPort = 80;
  if (isConnectedToInternet()) {
    client.connect(host, httpPort);

    String url = "/Workouts/StartWorkout.php?DeviceID=" + DeviceID +
      "&UserID=" + UserID +
      "&Firmware=" + Version +
      "&Thigh=" + workout_summary_array[5] +
      "&LowerLeg=" + workout_summary_array[6] +
      "&SeatHeight=" + workout_summary_array[7] +
      "&SpasmLevel=" + (user_profile_levels_left_spasm & 0x03) +
      "&Status=" + workout_summary_array[9];

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" +
      "Connection: close\r\n\r\n");

    delay(100);
  } else {
    return false;
  }

  return true;
}
bool uploadWorkout(void) {
  WiFiClient client;
  int httpPort = 80;
  if (isConnectedToInternet()) {
    client.connect(host, httpPort);
    String url = "/Workouts/StreamWorkout.php?DeviceID=" + DeviceID +
      "&UserID=" + UserID +
      "&Data=" + Workout;

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" +
      "Connection: close\r\n\r\n");

    Workout = "";
    delay(100);
    //Serial.println(url);
  } else {
    return false;
  }

  return true;
}
bool sendBeacon(void) {
  WiFiClient client;
  int httpPort = 80;
  updateInternetConnectionStatus();
  if (isConnectedToInternet()) {
    client.connect(host, httpPort);
    String url = "/Workouts/DeviceStatus.php?DeviceID=" + DeviceID +
      "&WiFiStatus=" + WiFiStatus;

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" +
      "Connection: close\r\n\r\n");
    delay(100);

    //  Serial.println (url);
    return true;
  } else {
    return false;
  }
}
void SetLoginDefault(void) {
  userLoginSet = fram.read8(FRAM1, 0x80);
  if (userLoginSet != 1) {
    for (index8 = 0; index8 < 7; index8++) {
      UserID[index8] = UserID_default[index8];
    }
    for (index8 = 0; index8 < 4; index8++) {
      User_Pin[index8] = User_Pin_default[index8];
    }
  } else {
    for (index8 = 0; index8 < 7; index8++) {
      UserID[index8] = (uint8_t) fram.read8(FRAM1, (0x85 + index8));
    }
    for (index8 = 0; index8 < 4; index8++) {
      User_Pin[index8] = (uint8_t) fram.read8(FRAM1, (0x8C + index8));
    }
  }
}
byte merge(char MSB, char LSB) {
  byte resultM = 0;
  byte resultL = 0;
  byte result = 0;
  switch (MSB) {
  case 'A':
    resultM = 0xA0;
    break;
  case 'B':
    resultM = 0xB0;
    break;
  case 'C':
    resultM = 0xC0;
    break;
  case 'D':
    resultM = 0xD0;
    break;
  case 'E':
    resultM = 0xE0;
    break;
  case 'F':
    resultM = 0xF0;
    break;
  case 'a':
    resultM = 0xA0;
    break;
  case 'b':
    resultM = 0xB0;
    break;
  case 'c':
    resultM = 0xC0;
    break;
  case 'd':
    resultM = 0xD0;
    break;
  case 'e':
    resultM = 0xE0;
    break;
  case 'f':
    resultM = 0xF0;
    break;
  default:
    resultM = ((byte) MSB) << 4;
    resultM = resultM & 0xF0;
    break;
  }
  switch (LSB) {
  case 'A':
    resultL = 0x0A;
    break;
  case 'B':
    resultL = 0x0B;
    break;
  case 'C':
    resultL = 0x0C;
    break;
  case 'D':
    resultL = 0x0D;
    break;
  case 'E':
    resultL = 0x0E;
    break;
  case 'F':
    resultL = 0x0F;
    break;
  case 'a':
    resultL = 0x0A;
    break;
  case 'b':
    resultL = 0x0B;
    break;
  case 'c':
    resultL = 0x0C;
    break;
  case 'd':
    resultL = 0x0D;
    break;
  case 'e':
    resultL = 0x0E;
    break;
  case 'f':
    resultL = 0x0F;
    break;
  default:
    resultL = ((byte) LSB) & 0x0F;
    break;
  }
  result = resultM | resultL;
  return result;
}
void FindChecksum() {
  AppRecChecksum = 0;
  ChecksumCounts++;
  bool timeout_flag = false;
  uint16_t timeout_count = 0;

  while (Serial.available() == 0);
  AppRecChecksum = (Serial.read() & 0x00FF);

  while (Serial.available() == 0);
  AppRecChecksum |= ((Serial.read() << 8) & 0xFF00);

  delay(1);

  if (AppSendChecksum != AppRecChecksum) {
    ResetAppCode = true;

    delay(1000);
    Serial.write(0xAA);
    Serial.write(0xAA);
    delay(1000);
    Serial.write(0xAA);
    Serial.write(0xAA);
    delay(1000);
    Serial.write((byte)(AppRecChecksum >> 8));
    Serial.write((byte) AppRecChecksum);

    Serial.write((byte)(AppSendChecksum >> 8));
    Serial.write((byte) AppSendChecksum);

    Serial.write(0xAA);
    Serial.write(0xAA);
    delay(1000);
    Serial.write(0xAA);
    Serial.write(0xAA);
    delay(1000);

    Serial.write(ChecksumCounts);

    Serial.write(0xAA);
    Serial.write(0xAA);
    delay(2000);
    Serial.write(0xAA);
    Serial.write(0xAA);
    delay(2000);
  } else {
    AppTotalChecksum += AppRecChecksum;
    AppSendChecksum = 0;
    AppRecChecksum = 0;
  }
}
bool KernelCode() {
  state = 0;
  Serial.read();
  WiFiClient client;
  int httpPort = 80;
  if (isConnectedToInternet()) {
    client.connect(host, httpPort);
    String url = "/Updates/FlashKernel.txt";

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" +
      "Connection: close\r\n\r\n");
    delay(100);

    while (client.available() && flag == 1) {
      Serial.println();
      if (state == 0) {
        line = client.readStringUntil('=');
      } else if (state == 1) {
        line1 = client.readStringUntil('?');
        line1.toCharArray(kernel, 5000);
        uint32_t i = 0;
        for (k = 0; k < 5000; k = k + 2) {
          stored = merge(kernel[k], kernel[k + 1]);
          fram.write8(FRAM1, 0x100 + i, stored);
          i++;
        }

      }
      state++;
      if (state > 1) {
        state = 0;
        flag = 0;
      }
    }

    state = 0;
    flag = 1;
    // Serial.println();
    // Serial.println("Kernel Stored");
    return true;
  } else {
    return false;
  }

}
bool AppCode() {
  state = 0;
  WiFiClient client;
  int httpPort = 80;
  if (isConnectedToInternet()) {
    client.connect(host, httpPort);
    String url = "/";

    if (Update_Flag) {
      url = ("/Updates/" + NewVersion + ".txt");
      //  Serial.println("Downloading New Version");
    } else {
      url = ("/Updates/" + Version + ".txt");
      //  Serial.println("Downloading Old Version");
    }

    //  Serial.print("Requesting URL: ");
    //  Serial.println(url);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" +
      "Connection: close\r\n\r\n");
    delay(100);

    while (client.available() && flag == 1) {
      Serial.println();
      if (state == 0) {
        line = client.readStringUntil('=');
      } else if (state == 1) {
        line2 = client.readStringUntil('#');
        AppCodeVersionVar = line2;
        Serial.println(line2);
        line2.toCharArray(AppCodeVersion, 20);
      } else if (state == 2) {
        line3 = client.readStringUntil('?');
        Serial.println(line3);
        line3.toCharArray(AppCodeChecksum, 20);
      } else if (state == 3) {
        line4 = client.readStringUntil('<');
        Serial.println(line4);
        line4.toCharArray(AppCodeBytes, 20);
        i = 0;
        for (k = 0; k < 12; k = k + 2) {
          stored = merge(AppCodeBytes[k], AppCodeBytes[k + 1]);
          fram.write8(FRAM1, 0x20A0 + i, stored);
          i++;
        }
        AppCodeBytesVar = ((fram.read8(FRAM1, 0x20A0) << 24) & 0xFF000000) | ((fram.read8(FRAM1, 0x20A1) << 16) & 0x00FF0000) | ((fram.read8(FRAM1, 0x20A2) << 8) & 0x0000FF00) | ((fram.read8(FRAM1, 0x20A3) & 0xFF));
      } else if (state == 4) {
        line5 = client.readStringUntilSize2(201);
        delay(1);
        Serial.println(line5);
        line5.toCharArray(AppCodeHex, 210);
        uint32_t i = 0;
        for (k = 0; k < 202; k = k + 2) {
          stored = merge(AppCodeHex[k], AppCodeHex[k + 1]);
          //calculate checksum after online download
          if (((loops * 101) + i) < AppCodeBytesVar) {
            downloadedChecksum = downloadedChecksum + (stored & 0x00FF);
          }

          fram.write8(FRAM1, 0x2100 + (101 * loops) + i, stored);
          i++;
        }
        loops++;
        state = 3;
      }
      state++;

      if (state > 5) {
        state == 0;
        flag = 0;
      }
    }

    uint32_t i = 0;
    for (k = 0; k < 12; k = k + 2) {
      stored = merge(AppCodeVersion[k], AppCodeVersion[k + 1]);
      fram.write8(FRAM1, 0x2000 + i, stored);
      i++;
    }
    // Serial.println();
    //  Serial.println("AppCodeVersion Stored");

    i = 0;
    for (k = 0; k < 12; k = k + 2) {
      stored = merge(AppCodeChecksum[k], AppCodeChecksum[k + 1]);
      fram.write8(FRAM1, 0x2050 + i, stored);
      i++;
    }
    AppCodeChecksumVar = ((fram.read8(FRAM1, 0x2050) << 24) & 0xFF000000) | ((fram.read8(FRAM1, 0x2051) << 16) & 0x00FF0000) | ((fram.read8(FRAM1, 0x2052) << 8) & 0x0000FF00) | ((fram.read8(FRAM1, 0x2053) & 0xFF));

    // Serial.println(AppCodeChecksumVar);
    // Serial.println("AppCodeChecksum Stored");

    if (downloadedChecksum != AppCodeChecksumVar) {

      // Serial.println("Downloaded Checksum Error! Retrying");
      ResetAppCode = true;
    }

    AppTotalBytes = AppCodeBytesVar;
    // Serial.println(AppCodeBytesVar);
    // Serial.println("AppCodeBytes Stored");
  } else {
    return false;
  }

}
bool FlashKernel() {
  int counter = 0;
  while (baudratelock != 0x41) {
    Serial.print('A'); ///  changed it to Serial.print
    delay(5);
    if (Serial.available() > 0) {
      baudratelock = Serial.read();
    } else {
      delay(1000);
      counter++;
      if (counter >= 60) {
        return false;
      }
    }
  }

  for (uint32_t z = 0; z < 2168; z++) {
    sendbyte = (fram.read8(FRAM1, 0x100 + z) & 0x000000ff);
    Serial.write(sendbyte);
    delay(1);
  }
  baudratelock = 0;
}
bool FlashAppCode() {
  int counter = 0;
  ChecksumCounts = 0;
  AppSendChecksum = 0;
  Serial.read();
  while (baudratelock != 0x41) {
    Serial.print('A');
    delay(5);
    while (Serial.available() > 0) {
      baudratelock = Serial.read();
    }
    counter++;
    if (counter >= 500) {
      return false;
    }
  }
  delay(5500);

  for (uint32_t z = 0; z < 22; z++) {
    sendbyte = (fram.read8(FRAM1, 0x2100 + z));
    Serial.write(sendbyte);
    AppSendChecksum += (sendbyte & 0x00ff);
  }

  FindChecksum();

  FramAddress = 0x2116;

  while (AppBytesIndex < AppTotalBytes) {
    sendbyte = fram.read8(FRAM1, FramAddress);
    Serial.write(sendbyte);
    AppSendChecksum += (sendbyte & 0x00ff);

    if (txCount == 0x00) {
      AppBlockSize = fram.read8(FRAM1, FramAddress);
    } else if (txCount == 0x01) {
      AppBlockSize |= (((fram.read8(FRAM1, FramAddress)) << 8) & 0xff00);
    }

    txCount++;
    FramAddress++;
    AppBytesIndex++;

    if (AppBlockSize == 0x00 && txCount > 1) //If the next block size is 0, exit the while loop.
    {
      AppBlockSize = 0;
      break;
    } else if ((txCount - 6) % 0x800 == 0 && txCount > 6) //If the block size is bigger than 0x400 words, every 0x400 words later it takes time for flash program. Them waiting for feedback.
    {
      FindChecksum();

      delay(3000); //
    } else if (txCount == 2 * (AppBlockSize + 3)) //If CountInt meets the block size, countint and dest addr will be initialized.
    {
      FindChecksum();
      AppBlockSize = 0;
      txCount = 0x00;
      delay(2000);
    }
  }

}
void ClearFRAM(void) {
  uint8_t i = 0;
  // Serial.println("Started Cleaning");
  for (uint32_t a = 0; a < 0x1FFFF; a++) //   CLEARS THE FRAM
  {
    fram.write8(FRAM1, 0x01 + a, 0x00);
    delay(1);
    switch (a) {
    case 0x06000:
      //    Serial.println("20%");
      break;
    case 0x0D000:
      //    Serial.println("40%");
      break;
    case 0x13000:
      //    Serial.println("60%");
      break;
    case 0x19000:
      //    Serial.println("80%");
      break;
    default:
      break;
    }
  }
  // Serial.println("Done Cleaning");

  //Reset user profile status array
  for (i = 0; i < 50; i++) {
    user_profile_status[i] = 0x00;
  }

  //Reset Password array
  for (i = 0; i < PASS_LENGTH; i++) {
    password[i] = 0x00;
  }
  //Reset SSID array
  for (i = 0; i < SSID_LENGTH; i++) {
    ssid[i] = 0x00;
  }
}
void ReadFRAM(void) {
  uint8_t value; //   READS THE FRAM and PRINTS VALUE
  for (uint32_t a = 0; a < 0x20000; a++) {
    value = fram.read8(FRAM1, a);
    if ((a % 32) == 0) {
      Serial.print("\n 0x");
      Serial.print(a, HEX);
      Serial.print(": ");
    }
    Serial.print("0x");
    if (value < 0x1)
      Serial.print('0');
    Serial.print(value, HEX);
    Serial.print(" ");
  }
}
void TestFRAM(void) {
  //  Serial.println("Started Testing");
  for (uint32_t a = 0xF000; a < 0x1FFFF; a++) //   CLEARS THE FRAM
  {
    fram.write8(FRAM1, a, a);
    delay(1);
  }
  //  Serial.println("Done Testing");
}
void setup() {

  Serial.begin(74880);
  Serial.println("");
  if (fram.begin(FRAM1)) { // you can stick the new i2c addr in here, e.g. begin(0x50);  U8
    Serial.println("Found U8 I2C FRAM");
  } else {
    Serial.println("No U8 I2C FRAM1 found ... check your connections\r\n");
  }

  pinMode(C2000_XRS, OUTPUT); // C2000 Reset XRS
  pinMode(C2000_TDO, OUTPUT); // C2000 TDO
  digitalWrite(C2000_TDO, LOW);
  digitalWrite(C2000_XRS, LOW);

  pinMode(LCD_POWER, OUTPUT); // LCD OFF
  digitalWrite(LCD_POWER, LOW);

  String Hostname = ("MyoCycle_" + DeviceID);
  WiFi.hostname(Hostname);

  Serial.print("Current Device ID: ");
  Serial.println(DeviceID);

  uint8_t V10 = fram.read8(FRAM1, 0x10);
  uint8_t V11 = fram.read8(FRAM1, 0x11);
  uint8_t V12 = fram.read8(FRAM1, 0x12);
  uint8_t V13 = fram.read8(FRAM1, 0x13);

  if (V10 < 10) {
    Version = "0" + String(V10, HEX);
  } else {
    Version = String(V10, HEX);
  }
  if (V11 < 10) {
    Version = Version + "0" + String(V11, HEX);
  } else {
    Version = Version + String(V11, HEX);
  }
  if (V12 < 10) {
    Version = Version + "0" + String(V12, HEX);
  } else {
    Version = Version + String(V12, HEX);
  }
  if (V13 < 10) {
    Version = Version + "0" + String(V13, HEX);
  } else {
    Version = Version + String(V13, HEX);
  }

  Serial.print("Current Device Version: ");
  Serial.println(Version);

  //  Serial.println("Ready for uploading firmware");

  // Serial.println("Reconnecting...");
  for (index8 = 0; index8 < SSID_LENGTH; index8++) {
    ssid[index8] = (char) fram.read8(FRAM1, 0x32 + index8);
  }
  for (index8 = 0; index8 < PASS_LENGTH; index8++) {
    password[index8] = (char) fram.read8(FRAM1, 0x53 + index8);
  }
  showInitialStatus = false;
  SSIDsizeInit = fram.read8(FRAM1, 0x31);

  connect();



  //  scanForUserID();
  //  startWorkout();
  //  Upload_Summary();
  //uploadWorkout();

  //  ClearFRAM();
  //  TestFRAM();
  //  ReadFRAM();
}

void loop() {

  //check if wifi connection has changed
  if (WiFi.status() != currentStatus || internetConnectionStatus != lastInternetConnectionStatus) {
	lastInternetConnectionStatus = internetConnectionStatus;
    currentStatus = WiFi.status();
    if (currentStatus == WL_DISCONNECTED) {
      Wifi_status_A[2] = WIFI_DISCONNECTED;
    } else if (currentStatus == WL_CONNECTED) {

      if(internetConnectionStatus == true)
      {
    	  Wifi_status_A[2] = WIFI_CONNECTED;
      }
      else
      {
    	  Wifi_status_A[2] = WIFI_CONNECTED_NO_INTERNET;
      }

    }

    Wifi_Send_Command(Wifi_status_A, 3);
  }
  while (Serial.available() > 0) {
    uint8_t temp = 0;
    receiveBuffer[receiveBufferIndex] = (byte) Serial.read();
    if (receiveBufferIndex >= 4 && (receiveBuffer[receiveBufferIndex - 3]) == 0xAA && (receiveBuffer[receiveBufferIndex - 2]) == 0x44 && (receiveBuffer[receiveBufferIndex - 1]) == 0xA4 && (receiveBuffer[receiveBufferIndex]) == 0x4A) {
      for (leftIndex = receiveBufferIndex + 1; leftIndex < RECEIVE_BUFFER_SIZE; leftIndex++) {
        receiveBuffer[leftIndex] = 0;
      }

      receiveBufferIndex = 0;
      switch (receiveBuffer[1])

      {
      case WIFI_HANDSHAKE:
        Wifi_Send_Command(Wifi_handshake_A, 2);
        break;
      case WIFI_STATUS:
        currentStatus = WiFi.status();

        if (currentStatus == WL_DISCONNECTED) {
          Wifi_status_A[2] = WIFI_DISCONNECTED;
        } else if (currentStatus == WL_CONNECTED) {
          if(isConnectedToInternet())
          {
        	  Wifi_status_A[2] = WIFI_CONNECTED;
          }
          else
          {
        	  Wifi_status_A[2] = WIFI_CONNECTED_NO_INTERNET;
          }

        }
        Wifi_Send_Command(Wifi_status_A, 3);
        break;
      case WIFI_SCAN:
        if (receiveBuffer[2] > 0) {
          if (Networks == 0) {
            Wifi_scan_A[2] = Networks;
            Wifi_Send_Command(Wifi_scan_A, 3);
          } else {
            Wifi_scan_A[2] = Networks;
            Wifi_scan_A[3] = receiveBuffer[2];
            SSIDsize = strlen(WiFi.SSID(receiveBuffer[2] - 1));
            if (SSIDsize >= SSID_BUFFER_SIZE)
              SSIDsize = SSID_BUFFER_SIZE;
            strncpy(SSIDarray, WiFi.SSID(receiveBuffer[2] - 1), SSIDsize);
            Wifi_scan_A[4] = SSIDsize;
            for (index8 = 0; index8 < SSIDsize; index8++) {
              Wifi_scan_A[index8 + 5] = SSIDarray[index8];
            }
            Wifi_Send_Command(Wifi_scan_A, 5 + SSIDsize);
          }
        } else {
          scan();
        }
        break;
      case WIFI_CONNECT:
        SSIDsize = strlen(WiFi.SSID(receiveBuffer[2] - 1));
        SSIDsizeInit = SSIDsize;
        PSWDsize = PASS_LENGTH;
        strncpy(ssid, WiFi.SSID(receiveBuffer[2] - 1), SSIDsize);
        for (index8 = 0; index8 < (SSID_LENGTH - SSIDsize); index8++) {
          ssid[SSIDsize + index8] = 0x00;
        }
        for (index8 = 0; index8 < PASS_LENGTH; index8++) {
          password[index8] = receiveBuffer[3 + index8];
        }
        showInitialStatus = true;
        connect();

        break;
      case WIFI_CONNECT_HIDDEN:
        SSIDsize = (uint8_t) receiveBuffer[2];
        //  Serial.println(SSIDsize);
        SSIDsizeInit = SSIDsize;
        PSWDsize = (uint8_t) receiveBuffer[3];
        //  Serial.println(PSWDsize);

        for (index8 = 0; index8 < SSIDsize; index8++) {
          ssid[index8] = (uint8_t) receiveBuffer[4 + index8];
        }

        for (index8 = 0; index8 < (SSID_LENGTH - SSIDsize); index8++) {
          ssid[SSIDsize + index8] = 0x00;
        }

        for (index8 = 0; index8 < PSWDsize; index8++) {
          password[index8] = (uint8_t) receiveBuffer[36 + index8];
        }

        for (index8 = 0; index8 < (PASS_LENGTH - PSWDsize); index8++) {
          password[PSWDsize + index8] = 0x00;
        }
        showInitialStatus = true;
        connect();
        break;
      case WIFI_DISCONNECT:
        disconnect();
        break;
      case WIFI_LOGIN:
        for (index8 = 0; index8 < 7; index8++) {
          UserID[index8] = (uint8_t) receiveBuffer[2 + index8];
          fram.write8(FRAM1, (0x85 + index8), UserID[index8]);
        }
        for (index8 = 0; index8 < 4; index8++) {
          User_Pin[index8] = (uint8_t) receiveBuffer[9 + index8];
          fram.write8(FRAM1, (0x8C + index8), User_Pin[index8]);
        }
        if (!scanForUserID()) {
          Wifi_serverLoginStatus[2] = SERVER_ERROR;
        }

        Wifi_Send_Command(Wifi_serverLoginStatus, 3);
        break;
      case WIFI_LOGIN_CHECK:
        userLoginSet = fram.read8(FRAM1, 0x80);
        Wifi_serverLoginCheck[2] = userLoginSet;
        Wifi_Send_Command(Wifi_serverLoginCheck, 3);
        break;
      case WIFI_LOGIN_SET:
        userLoginSet = (uint8_t) receiveBuffer[2];
        fram.write8(FRAM1, 0x80, userLoginSet);
        break;
      case WIFI_BEACON_SEND:
        WiFiStatus = "2";
        sendBeacon();
        break;
      case WIFI_LOGIN_LAST:
        for (index8 = 0; index8 < 7; index8++) {
          UserID[index8] = (uint8_t) fram.read8(FRAM1, (0x85 + index8));
        }
        for (index8 = 0; index8 < 4; index8++) {
          User_Pin[index8] = (uint8_t) fram.read8(FRAM1, (0x8C + index8));
        }
        if (!scanForUserID()) {
          Wifi_serverLoginStatus[2] = SERVER_ERROR;
        }

        Wifi_Send_Command(Wifi_serverLoginStatus, 3);
        break;
      case WIFI_CHECK_UPDATE:
        index8 = 0;
        while (!scanForUpdate() && index8 != 50) {
          delay(200);
          index8++;
        }
        Wifi_Send_Command(Wifi_checkUpdate_A, 3);
        break;
      case WIFI_VERSIONREQUEST:
        index8 = 0;
        DeviceID.toCharArray(DeviceIDArray, 7);
        for (index8 = 0; index8 < 7; index8++) {
          Wifi_versionInfo_A[2 + index8] = DeviceIDArray[index8];
        }
        Version.toCharArray(DeviceVersionArray, 9);
        for (index8 = 0; index8 < 9; index8++) {
          Wifi_versionInfo_A[8 + index8] = DeviceVersionArray[index8];
        }
        NewVersion.toCharArray(DeviceNewVersionArray, 9);
        for (index8 = 0; index8 < 9; index8++) {
          Wifi_versionInfo_A[16 + index8] = DeviceNewVersionArray[index8];
        }
        Wifi_Send_Command(Wifi_versionInfo_A, 24);
        index8 = 0;
        break;
      case WIFI_UPDATE_DEVICE:
        UpdateGo = true;
        break;
      case WIFI_DEVICEIP:
        for (index8 = 0; index8 < 4; index8++) {
          Wifi_deviceIPInfo_A[2 + index8] = WiFi.localIP()[index8];
        }
        Wifi_Send_Command(Wifi_deviceIPInfo_A, 6);
        index8 = 0;
        break;
      case WIFI_DEVICEMAC:
        WiFi.macAddress().toCharArray(DeviceMACarray, 19);
        for (index8 = 0; index8 < 17; index8++) {
          Wifi_deviceMACInfo_A[2 + index8] = DeviceMACarray[index8];
        }
        Wifi_Send_Command(Wifi_deviceMACInfo_A, 19);
        index8 = 0;
        break;
      case WIFI_CURRENT_SSID:
        index8 = 0;
        SSIDsizeInit = fram.read8(FRAM1, 0x31);
        Wifi_deviceSSID_A[2] = SSIDsizeInit;
        for (index8 = 0; index8 < SSIDsizeInit; index8++) {
          Wifi_deviceSSID_A[3 + index8] = (char) fram.read8(FRAM1, (0x32 + index8));
        }
        Wifi_Send_Command(Wifi_deviceSSID_A, 3 + SSIDsizeInit);
        break;
      case WIFI_WORKOUT_STREAM:
        SetLoginDefault();
        for (index8 = 0; index8 < 6; index8++) {
          uint8_t data = (receiveBuffer[2 + index8]);
          if (data < 0x10) {
            Workout = Workout + "0" + String(data, HEX);
          } else {
            Workout = Workout + String(data, HEX);
          }
          workout_Data_count++;
        }
        if (workout_Data_count >= 500) {
          uploadWorkout();
          workout_Data_count = 0;
        }

        break;
      case WIFI_SUMMARY:
        SetLoginDefault();
        for (index8 = 0; index8 < 10; index8++) {
          workout_summary_array[index8] = (receiveBuffer[2 + index8]);
        }
        workout_user_profile = (uint8_t) receiveBuffer[12];
        if (receiveBuffer[2 + 9] == 11) {
          startWorkout();
        } else if (receiveBuffer[11] == 22 || receiveBuffer[11] == 33) {
          uploadSummaryOnCancel();
        } else {
          uploadSummary();
        }
        break;
      case WIFI_USER_NUMBER_CHECK:
        workout_user_profile = (uint8_t) receiveBuffer[2];
        workout_user_status = fram.read8(FRAM1, (0x1000 + (7 * (workout_user_profile - 1))));
        if (workout_user_status == 1) {
          if (workout_user_profile != 0) {
            Wifi_userProfileNumber_A[1] = WIFI_USER_NUMBER_CHECK;
            Wifi_userProfileNumber_A[2] = USER_PROFILE_EXITS;
            Wifi_Send_Command(Wifi_userProfileNumber_A, 3);
          }
        } else {
          //Whenever a new profile is created, set the default values
          fram.write8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)), 0x01);
          fram.write8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)) + 1, 0x00);
          fram.write8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)) + 2, 0x00);
          fram.write8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)) + 3, 0x6A);
          fram.write8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)) + 4, 0xAF);
          fram.write8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)) + 5, 0x8C);
          fram.write8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)) + 6, 0x04);

          user_profile_status[workout_user_profile - 1] = 1;

          if (workout_user_profile != 0) {
            Wifi_userProfileNumber_A[1] = WIFI_USER_NUMBER_CHECK;
            Wifi_userProfileNumber_A[2] = WIFI_NEW_PROFILE_ADDED;
            Wifi_Send_Command(Wifi_userProfileNumber_A, 3);
          }
        }
        break;
      case USER_PROFILE_DELETE:
        workout_user_profile = (uint8_t) receiveBuffer[2];
        workout_user_status = fram.read8(FRAM1, (0x1000 + (7 * (workout_user_profile - 1))));
        fram.write8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)), 0x00);
        user_profile_status[workout_user_profile - 1] = 0;
        Wifi_userProfileNumber_A[1] = USER_PROFILE_DELETE;
        Wifi_userProfileNumber_A[2] = USER_PROFILE_DELETED;
        Wifi_Send_Command(Wifi_userProfileNumber_A, 3);
        break;
      case USER_PROFILE_ACTIVE_CHECK:
        workout_user_profile = (uint8_t) receiveBuffer[2];
        workout_user_status = fram.read8(FRAM1, (0x1000 + (7 * (workout_user_profile - 1))));
        if (workout_user_status != 1) {
          Wifi_userProfileNumber_A[1] = USER_PROFILE_ACTIVE_CHECK;
          Wifi_userProfileNumber_A[2] = USER_PROFILE_NOT_ACTIVE;
          Wifi_Send_Command(Wifi_userProfileNumber_A, 3);
        } else {
          Wifi_userProfileNumber_A[1] = USER_PROFILE_ACTIVE_CHECK;
          Wifi_userProfileNumber_A[2] = USER_PROFILE_ACTIVE;
          Wifi_Send_Command(Wifi_userProfileNumber_A, 3);
        }
        break;
      case WIFI_USER_PROFILE_PARAMETER:
        //When START is pressed all parameters are sent to Wifi chip to update user profile
        workout_user_profile = (uint8_t) receiveBuffer[2];
        user_profile_levels_left_spasm = (uint8_t) receiveBuffer[3];
        user_profile_levels_right = (uint8_t) receiveBuffer[4];
        user_profile_thigh = (uint8_t) receiveBuffer[5];
        user_profile_lower_leg = (uint8_t) receiveBuffer[6];
        user_profile_seat_height = (uint8_t) receiveBuffer[7];

        fram.write8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)) + 1, user_profile_levels_left_spasm);
        fram.write8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)) + 2, user_profile_levels_right);
        fram.write8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)) + 3, user_profile_thigh);
        fram.write8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)) + 4, user_profile_lower_leg);
        fram.write8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)) + 5, user_profile_seat_height);

        break;
      case WIFI_STIMULATION_LEVEL_UPDATE:
        workout_user_profile = (uint8_t) receiveBuffer[2];
        user_profile_levels_left_spasm = (uint8_t) receiveBuffer[3];
        user_profile_levels_right = (uint8_t) receiveBuffer[4];
        fram.write8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)) + 1, user_profile_levels_left_spasm);
        fram.write8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)) + 2, user_profile_levels_right);
        break;

      case WIFI_UPDATE_SURVEY_COUNTER:
        workout_user_profile = (uint8_t) receiveBuffer[2];
        user_profile_survery_counter = (uint8_t) receiveBuffer[3];
        fram.write8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)) + 6, user_profile_survery_counter);
        break;

      case WIFI_USER_PARAMETER_REQUEST:
        workout_user_profile = (uint8_t) receiveBuffer[2];
        user_profile_levels_left_spasm = fram.read8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)) + 1);
        user_profile_levels_right = fram.read8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)) + 2);
        user_profile_thigh = fram.read8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)) + 3);
        user_profile_lower_leg = fram.read8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)) + 4);
        user_profile_seat_height = fram.read8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)) + 5);
        user_profile_survery_counter = fram.read8(FRAM1, 0x1000 + (7 * (workout_user_profile - 1)) + 6);

        Wifi_userProfileParameters_A[2] = workout_user_profile;
        Wifi_userProfileParameters_A[3] = user_profile_levels_left_spasm;
        Wifi_userProfileParameters_A[4] = user_profile_levels_right;
        Wifi_userProfileParameters_A[5] = user_profile_thigh;
        Wifi_userProfileParameters_A[6] = user_profile_lower_leg;
        Wifi_userProfileParameters_A[7] = user_profile_seat_height;
        Wifi_userProfileParameters_A[8] = user_profile_survery_counter;
        Wifi_Send_Command(Wifi_userProfileParameters_A, 9);

        break;

      case WIFI_SCAN_USER_PROFILES:
        for (i = 1; i < 51; i++) {
          status_val = fram.read8(FRAM1, 0x1000 + (7 * (i - 1)));

          if (status_val == 1) {
            user_profile_status[i - 1] = 1;
          }
        }

        break;

      case WIFI_SCAN_NEXT:
        Wifi_requestProfile_A[1] = WIFI_SCAN_NEXT;
        current_user_profile_index = (uint8_t) receiveBuffer[2] - 1;
        next_available = false;

        for (i = current_user_profile_index + 1; i < 50; i++) {
          if (user_profile_status[i] == 1) {
            next_available = true;
            break;
          }
        }

        if (next_available == true) {
          Wifi_requestProfile_A[2] = (uint8_t)(i + 1);
          Wifi_Send_Command(Wifi_requestProfile_A, 3);
        } else {
          Wifi_requestProfile_A[2] = (uint8_t)(current_user_profile_index + 1);
          Wifi_Send_Command(Wifi_requestProfile_A, 3);
        }
        break;

      case WIFI_SCAN_PREVIOUS:
        Wifi_requestProfile_A[1] = WIFI_SCAN_PREVIOUS;
        current_user_profile_index = (uint8_t) receiveBuffer[2] - 1;
        next_available = false;

        if (current_user_profile_index > 0) {
          for (i = current_user_profile_index - 1; i >= 0; i--) {
            if (user_profile_status[i] == 1) {
              next_available = true;
              break;
            }
          }
        }

        if (next_available == true) {
          Wifi_requestProfile_A[2] = (uint8_t) i + 1;
          Wifi_Send_Command(Wifi_requestProfile_A, 3);
        } else {
          Wifi_requestProfile_A[2] = (uint8_t) current_user_profile_index + 1;
          Wifi_Send_Command(Wifi_requestProfile_A, 3);
        }
        break;
      case WIFI_CONTACT_INFO:
        contactNameSize = (uint8_t) receiveBuffer[2];
        contactEmailSize = (uint8_t) receiveBuffer[3];

        //Name
        for (index8 = 0; index8 < contactNameSize; index8++) {
          contactName[index8] = (uint8_t) receiveBuffer[5 + index8];
        }

        for (index8 = 0; index8 < (NAME_LENGTH - contactNameSize); index8++) {
          contactName[contactNameSize + index8] = 0x00;
        }

        //Email
        for (index8 = 0; index8 < contactEmailSize; index8++) {
          contactEmail[index8] = (uint8_t) receiveBuffer[25 + index8];
        }

        for (index8 = 0; index8 < (EMAIL_LENGTH - contactEmailSize); index8++) {
          contactEmail[contactEmailSize + index8] = 0x00;
        }

        //Phone (it will always have 10 chars)
        for (index8 = 0; index8 < PHONE_LENGTH; index8++) {
          contactPhone[index8] = (uint8_t) receiveBuffer[57 + index8];
        }

        uploadContactInfo();
        Wifi_Send_Command(Wifi_contactInfoReceived, 2);
        break;
      case WIFI_CLEAR_FRAM:
        ClearFRAM();
        /*
         * Disconnect device from Wifi whenever FRAM is cleared
         */
        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        fram.write8(FRAM1, 0x30, WIFI_DISCONNECTED);
        Wifi_Send_Command(Wifi_clearFRAM, 2);
        break;
      case WIFI_IS_PRO_VERSION:
        getModel();
        proVersionStatus = fram.read8(FRAM1, 0x73);
        Wifi_proVersionStatus[2] = proVersionStatus;
        Wifi_Send_Command(Wifi_proVersionStatus, 3);
        break;
      case WIFI_SET_VERSION:
        proVersionStatus = (uint8_t) receiveBuffer[2];
        SetDeviceModel();
        fram.write8(FRAM1, 0x73, proVersionStatus);
        break;
      default:
        break;
      }
      receiveComplete = true;
    } else {
      receiveBufferIndex++;
    }
  }

  if (UpdateGo == true) {
    digitalWrite(C2000_XRS, HIGH); // C2000 Reset  true
    while (!KernelCode());
    delay(50);
    while (!AppCode());

    if (!ResetAppCode) {
      delay(5000);
      //////////////////////////////////
      digitalWrite(C2000_TDO, HIGH);
      delay(300);
      digitalWrite(C2000_XRS, LOW); // C2000 Reset  false
      delay(300);
      //////////////////////////////////
      FlashKernel();
      delay(5000);
      FlashAppCode();
      //////////////////////////////////
      digitalWrite(C2000_TDO, LOW);
      delay(500);
      digitalWrite(C2000_XRS, HIGH);
      delay(500);
      digitalWrite(C2000_XRS, LOW);
      delay(500);
    }

    /////////////////////////////////

    if (ResetAppCode == true) {
      //Serial.println("YOU NEED TO RESTART");
      UpdateGo = true;
    } else {
      if ((AppTotalChecksum & 0xFFFF) != (AppCodeChecksumVar & 0xFFFF)) {
        //Serial.println(AppTotalChecksum);
        //Serial.println(AppCodeChecksumVar);
        //Serial.println("YOU NEED TO RESTART...TOTAL CHECKSUM ERROR");
        UpdateGo = true;
      } else {
        NewVersion.toCharArray(DeviceNewVersionArray, 10);
        uint8_t k = 0;
        uint32_t i = 0;
        stored = 0;
        for (k = 0; k < 10; k = k + 2) {
          delay(1);
          stored = merge(DeviceNewVersionArray[k], DeviceNewVersionArray[k + 1]);
          fram.write8(FRAM1, 0x10 + i, stored);
          i++;
        }
        Version = NewVersion;
        //Serial.println("Updating Done!");
        UpdateGo = false;
      }
    }
  }

}
