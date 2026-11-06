#pragma once

#include <CayenneLPP.h>
#include "sensors/LocationProvider.h"

#define TELEM_PERM_BASE         0x01   // 'base' permission includes battery
#define TELEM_PERM_LOCATION     0x02
#define TELEM_PERM_ENVIRONMENT  0x04   // permission to access environment sensors

#define TELEM_CHANNEL_SELF   1   // LPP data channel for 'self' device

#ifdef GPS_AUTO_SYNC
#define GPS_AUTO_DEFAULT_MINS  4320UL    // 3 days
#define GPS_AUTO_MIN_MINS      10UL
#define GPS_AUTO_MAX_MINS      43200UL   // 30 days
#endif

class SensorManager {
public:
  double node_lat, node_lon;  // modify these, if you want to affect Advert location
  double node_altitude;       // altitude in meters

  SensorManager() { node_lat = 0; node_lon = 0; node_altitude = 0; }
  virtual bool begin() { return false; }
  virtual bool querySensors(uint8_t requester_permissions, CayenneLPP& telemetry) { return false; }
  virtual void loop() { }
  virtual int getNumSettings() const { return 0; }
  virtual const char* getSettingName(int i) const { return NULL; }
  virtual const char* getSettingValue(int i) const { return NULL; }
  virtual bool setSettingValue(const char* name, const char* value) { return false; }
  virtual LocationProvider* getLocationProvider() { return NULL; }
  virtual bool isGPSDetected() const { return false; }
#ifdef GPS_AUTO_SYNC
  virtual void setGpsAutoSync(uint32_t interval_mins) { }
  virtual long gpsAutoNextMins() const { return -1; }  // -1: not active, 0: syncing now or due
  virtual const char* gpsAutoLastResult() const { return "none"; }
  virtual bool deferAdvertToGpsAutoSync() { return false; }  // true: hold the boot advert until the sync ends
  virtual bool takeGpsAutoAdvert() { return false; }         // true once, when a held advert is due
#endif

  // Helper functions to manage setting by keys (useful in many places ...)
  const char* getSettingByKey(const char* key) {
    int num = getNumSettings();
    for (int i = 0; i < num; i++) {
      if (strcmp(getSettingName(i), key) == 0) {
        return getSettingValue(i);
      }
    }
    return NULL;
  }
};
