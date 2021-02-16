#ifndef SettingsDataStateService_h
#define SettingsDataStateService_h


#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <ESPFS.h>


#define SETTINGS_DATAS_SETTINGS_ENDPOINT_PATH "/rest/SettingsDataState"

#define SETTINGS_DATAS_SETTINGS_FILE "/config/SettingsDataState.json"

class SettingsDataState {
 public:
	unsigned long MS_SPRAY;
	unsigned int MS_RETARD_DEMARRAGE;
	unsigned int MS_Arret;
	unsigned int D_Min_level_cuve;
    bool Reset_counters;

  static void read(SettingsDataState& settings, JsonObject& root) {
    root["MS_SPRAY"] = settings.MS_SPRAY;
    root["MS_RETARD_DEMARRAGE"] = settings.MS_RETARD_DEMARRAGE;
    root["MS_Arret"] = settings.MS_Arret;
    root["D_Min_level_cuve"] = settings.D_Min_level_cuve;
    root["Reset_counters"] = settings.Reset_counters;
  }

  static StateUpdateResult update(JsonObject& root, SettingsDataState& savedState) {
    unsigned long newMSpray = root.containsKey("MS_SPRAY") ? root["MS_SPRAY"]:1;
    unsigned int newMSRetard = root.containsKey("MS_RETARD_DEMARRAGE") ? root["MS_RETARD_DEMARRAGE"]:1;   
    unsigned int newMSArret = root.containsKey("MS_Arret") ? root["MS_Arret"]:1;
    unsigned int newMinLevel = root.containsKey("D_Min_level_cuve") ? root["D_Min_level_cuve"]:1;
    bool newReset = root.containsKey("Reset_counters") ? root["Reset_counters"]:false;
    if ( ((savedState.MS_SPRAY) != newMSpray) |((savedState.MS_RETARD_DEMARRAGE) != newMSRetard) |((savedState.MS_Arret) != newMSArret) |((savedState.D_Min_level_cuve) != newMinLevel)|((savedState.Reset_counters) != newReset) ) {
      savedState.MS_SPRAY = newMSpray;
      savedState.MS_RETARD_DEMARRAGE = newMSRetard;
      savedState.MS_Arret = newMSArret;
      savedState.D_Min_level_cuve = newMinLevel;
      savedState.Reset_counters=newReset;
      return StateUpdateResult::CHANGED;
    }
    return StateUpdateResult::UNCHANGED;
  }

};

class SettingsDataStateService : public StatefulService<SettingsDataState> {
 public:
  SettingsDataStateService(AsyncWebServer* server,
                    SecurityManager* securityManager);
  void begin();

 private:
  HttpEndpoint<SettingsDataState> _httpEndpoint;
  FSPersistence<SettingsDataState> _fsPersistence;
 
  void onConfigUpdated();
};

#endif
