#ifndef SavedDataStateService_h
#define SavedDataStateService_h


#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <ESPFS.h>

#define LED_PIN 2
#define PRINT_DELAY 5000


#define SAVED_DATAS_SETTINGS_ENDPOINT_PATH "/rest/SavedDataState"

#define SAVED_DATAS_SETTINGS_FILE "/config/SavedDataState.json"

class SavedDataState {
 public:
	double temps_total_spray;
	unsigned long nb_total_passage;

  static void read(SavedDataState& settings, JsonObject& root) {
    root["temps_total_spray"] = settings.temps_total_spray;
    root["nb_total_passage"] = settings.nb_total_passage;
  }

  static StateUpdateResult update(JsonObject& root, SavedDataState& savedState) {
    double newTSpray = root.containsKey("temps_total_spray") ? (double)root["temps_total_spray"]:1;
    unsigned long newTPassage = root.containsKey("nb_total_passage") ? root["nb_total_passage"]:1;

    if (((savedState.temps_total_spray) != newTSpray) | ((savedState.nb_total_passage) != newTPassage)  ) {
      savedState.temps_total_spray = newTSpray;
      savedState.nb_total_passage = newTPassage;
      return StateUpdateResult::CHANGED;
    }
    return StateUpdateResult::UNCHANGED;
  }

};

class SavedDataStateService : public StatefulService<SavedDataState> {
 public:
  SavedDataStateService(AsyncWebServer* server,
                    SecurityManager* securityManager);
  void begin();

 private:
  HttpEndpoint<SavedDataState> _httpEndpoint;
  FSPersistence<SavedDataState> _fsPersistence;
 
  void onConfigUpdated();
};

#endif
