#ifndef SavedDataStateService_h
#define SavedDataStateService_h

#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <ESPFS.h>

#define LED_PIN 2
#define PRINT_DELAY 5000

#define SAVED_DATAS_SETTINGS_ENDPOINT_PATH "/rest/SavedDataState"

#define SAVED_DATAS_SETTINGS_FILE "/config/SavedDataState.json"

class SavedDataState
{
public:
double temps_total_brossage;
unsigned int nb_surcourant;
unsigned long nb_total_demarrage;
bool Reset_counters;

  static void read(SavedDataState &settings, JsonObject &root)
  {
    root["temps_total_brossage"] = settings.temps_total_brossage;
    root["nb_total_demarrage"] = settings.nb_total_demarrage;
    root["nb_surcourant"] = settings.nb_surcourant;
    root["Reset_counters"]=settings.Reset_counters;
  }

  static StateUpdateResult update(JsonObject &root, SavedDataState &savedState)
  {
    savedState.temps_total_brossage = (double)root["temps_total_brossage"];
    savedState.nb_total_demarrage = root["nb_total_demarrage"];
    savedState.nb_surcourant = root["nb_surcourant"];
    savedState.Reset_counters=root["Reset_counters"];
    return StateUpdateResult::CHANGED;
  }
};

class SavedDataStateService : public StatefulService<SavedDataState>
{
public:
  SavedDataStateService(AsyncWebServer *server,
                        SecurityManager *securityManager);
  void begin();

private:
  HttpEndpoint<SavedDataState> _httpEndpoint;
  FSPersistence<SavedDataState> _fsPersistence;

  void onConfigUpdated();
};

#endif
