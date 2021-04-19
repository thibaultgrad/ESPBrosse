#include <SavedDataStateService.h>

SavedDataStateService::SavedDataStateService(AsyncWebServer* server,
                                     SecurityManager* securityManager) :
    _httpEndpoint(SavedDataState::read,
                  SavedDataState::update,
                  this,
                  server,
                  SAVED_DATAS_SETTINGS_ENDPOINT_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
     _fsPersistence(SavedDataState::read, SavedDataState::update, this, &ESPFS, SAVED_DATAS_SETTINGS_FILE)
 {

  // configure settings service update handler to update LED state
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void SavedDataStateService::begin() {
      _state.temps_total_brossage = 0;
      _state.nb_total_demarrage = 0;
      _state.nb_surcourant=0;
      _state.Reset_counters=0;
  _fsPersistence.readFromFS();
  onConfigUpdated();
}

void SavedDataStateService::onConfigUpdated() {

}

