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
	unsigned int Angle_declenchement;
	unsigned int MS_Surcourant;
  unsigned int MS_DEMARRAGE_MOTEUR;
  unsigned int MS_ARRET;
	int Courant_max;
    bool Reset_counters; 
    String Date_RAZ;

  static void read(SettingsDataState& settings, JsonObject& root) {
    root["MS_Brossage"] = settings.MS_Brossage;
    root["Angle_declenchement"] = settings.Angle_declenchement;
    root["MS_Surcourant"] = settings.MS_Surcourant;
    root["Courant_max"] = settings.Courant_max;
    root["Reset_counters"] = settings.Reset_counters;
    root["Date_RAZ"]=settings.Date_RAZ;
    root["MS_DEMARRAGE_MOTEUR"]=settings.MS_DEMARRAGE_MOTEUR;
    root["MS_ARRET"]=settings.MS_ARRET;
  }

  static StateUpdateResult update(JsonObject& root, SettingsDataState& savedState) {
    unsigned long newMBrossage = root.containsKey("MS_Brossage") ? root["MS_Brossage"]:1;
    unsigned int newMSRetard = root.containsKey("Angle_declenchement") ? root["Angle_declenchement"]:1;   
    unsigned int newMSArret = root.containsKey("MS_Surcourant") ? root["MS_Surcourant"]:1;
    unsigned int newMSDem = root.containsKey("MS_DEMARRAGE_MOTEUR") ? root["MS_DEMARRAGE_MOTEUR"]:1;
    unsigned int newMSArr = root.containsKey("MS_ARRET") ? root["MS_ARRET"]:1;
    int newCourantMax = root.containsKey("Courant_max") ? root["Courant_max"]:0;
    bool newReset = root.containsKey("Reset_counters") ? root["Reset_counters"]:false;
    String newDateRaz=root.containsKey("Date_RAZ") ? root["Date_RAZ"]: String("no date");
    //if ( ((savedState.MS_Brossage) != newMBrossage) |((savedState.Angle_declenchement) != newMSRetard) |((savedState.MS_Surcourant) != newMSArret) |((savedState.Courant_max) != newCourantMax)|((savedState.Reset_counters) != newReset) ) {
      savedState.MS_Brossage = newMBrossage;
      savedState.Angle_declenchement = newMSRetard;
      savedState.MS_Surcourant = newMSArret;
      savedState.Courant_max = newCourantMax;
      savedState.Reset_counters=newReset;
      savedState.Date_RAZ=root["Date_RAZ"].as<String>();
      savedState.MS_DEMARRAGE_MOTEUR=newMSDem;
      savedState.MS_ARRET=newMSArr;
      return StateUpdateResult::CHANGED;
    //}
    //return StateUpdateResult::UNCHANGED;
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
