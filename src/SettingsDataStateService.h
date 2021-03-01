#ifndef SettingsDataStateService_h
#define SettingsDataStateService_h


#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <ESPFS.h>
//#include <Time.h>


#define SETTINGS_DATAS_SETTINGS_ENDPOINT_PATH "/rest/SettingsDataState"

#define SETTINGS_DATAS_SETTINGS_FILE "/config/SettingsDataState.json"

class SettingsDataState {
 public:
	unsigned long MS_Brossage;
	unsigned int Angl_declenchement;
	unsigned int MS_Surcourant;
	float Courant_max;
    bool Reset_counters; 
    String Date_RAZ;

  static void read(SettingsDataState& settings, JsonObject& root) {
    root["MS_Brossage"] = settings.MS_Brossage;
    root["Angl_declenchement"] = settings.Angl_declenchement;
    root["MS_Surcourant"] = settings.MS_Surcourant;
    root["Courant_max"] = settings.Courant_max;
    root["Reset_counters"] = settings.Reset_counters;
    root["Date_RAZ"]=settings.Date_RAZ;
  }

  static StateUpdateResult update(JsonObject& root, SettingsDataState& savedState) {
    unsigned long newMBrossage = root.containsKey("MS_Brossage") ? root["MS_Brossage"]:1;
    unsigned int newMSRetard = root.containsKey("Angl_declenchement") ? root["Angl_declenchement"]:1;   
    unsigned int newMSArret = root.containsKey("MS_Surcourant") ? root["MS_Surcourant"]:1;
    float newCourantMax = root.containsKey("Courant_max") ? root["Courant_max"]:1;
    bool newReset = root.containsKey("Reset_counters") ? root["Reset_counters"]:false;
    String newDateRaz=root.containsKey("Date_RAZ") ? root["Date_RAZ"]: String("no date");
    if ( ((savedState.MS_Brossage) != newMBrossage) |((savedState.Angl_declenchement) != newMSRetard) |((savedState.MS_Surcourant) != newMSArret) |((savedState.Courant_max) != newCourantMax)|((savedState.Reset_counters) != newReset) ) {
      savedState.MS_Brossage = newMBrossage;
      savedState.Angl_declenchement = newMSRetard;
      savedState.MS_Surcourant = newMSArret;
      savedState.Courant_max = newCourantMax;
      savedState.Reset_counters=newReset;
      savedState.Date_RAZ=root["Date_RAZ"].as<String>();
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
