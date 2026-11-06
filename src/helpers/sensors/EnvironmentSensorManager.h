#pragma once

#include <Mesh.h>
#include <helpers/SensorManager.h>
#include <helpers/sensors/LocationProvider.h>

class EnvironmentSensorManager : public SensorManager {
protected:
  static const int MAX_ACTIVE_SENSORS = 16;

  // Query function pointer + sub-channel index (for multi-channel sensors like INA3221).
  // Sub-channel is 0 for all single-output sensors.
  struct ActiveSensor {
    void    (*query)(uint8_t channel, uint8_t sub_channel, CayenneLPP& telemetry);
    uint8_t   sub_channel;
  };

  ActiveSensor _active_sensors[MAX_ACTIVE_SENSORS];
  int          _active_sensor_count = 0;
  uint8_t      next_available_channel = TELEM_CHANNEL_SELF + 1;

  bool     gps_detected = false;
  bool     gps_active = false;
  uint32_t gps_update_interval_sec = 1;

  #if ENV_INCLUDE_GPS
  LocationProvider* _location;
  void start_gps();
  void stop_gps();
  void initBasicGPS();
  #ifdef RAK_BOARD
  void rakGPSInit();
  bool gpsIsAwake(uint8_t ioPin);
  #endif
  #ifdef GPS_AUTO_SYNC
  uint32_t gps_auto_interval_ms = 0;  // 0: auto sync is off
  uint32_t gps_auto_wait_ms = 0;      // time from gps_auto_last until the next sync
  unsigned long gps_auto_last = 0;    // when the current or last sync started or ended
  bool gps_auto_syncing = false;
  bool gps_auto_advert_held = false;
  bool gps_auto_advert_due = false;
  const char* gps_auto_last_result = "none";
  void updateGpsAutoSync();
  #endif
  #endif

public:
  #if ENV_INCLUDE_GPS
  EnvironmentSensorManager(LocationProvider &location): _location(&location){};
  LocationProvider* getLocationProvider() { return _location; }
  bool isGPSDetected() const override { return gps_detected; }
  #ifdef GPS_AUTO_SYNC
  void setGpsAutoSync(uint32_t interval_mins) override;
  long gpsAutoNextMins() const override;
  const char* gpsAutoLastResult() const override { return gps_auto_last_result; }
  bool deferAdvertToGpsAutoSync() override;
  bool takeGpsAutoAdvert() override;
  #endif
  #else
  EnvironmentSensorManager(){};
  #endif
  bool begin() override;
  bool querySensors(uint8_t requester_permissions, CayenneLPP& telemetry) override;
  #if ENV_INCLUDE_GPS || defined(ENV_INCLUDE_BME680_BSEC)
  void loop() override;
  #endif
  int getNumSettings() const override;
  const char* getSettingName(int i) const override;
  const char* getSettingValue(int i) const override;
  bool setSettingValue(const char* name, const char* value) override;
};
