#include <SettingsDataStateService.h>

SettingsDataStateService::SettingsDataStateService(AsyncWebServer* server, SecurityManager* securityManager) :
    _httpEndpoint(SettingsDataState::read,
                  SettingsDataState::update,
                  this,
                  server,
                  SETTINGS_DATAS_SETTINGS_ENDPOINT_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _fsPersistence(SettingsDataState::read, SettingsDataState::update, this, &ESPFS, SETTINGS_DATAS_SETTINGS_FILE) {
  // configure settings service update handler to update LED state
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void SettingsDataStateService::begin() {
  _state.MS_Brossage = 120000;
  _state.Courant_max = 2.3;
  _state.Date_RAZ = (String)__DATE__;
  _state.MS_Surcourant = 30000;
  _state.Reset_counters = false;
  _state.Angle_declenchement=25;
  _state.MS_DEMARRAGE_MOTEUR=1500;
  _state.MS_ARRET=1500;
  _fsPersistence.readFromFS();
  onConfigUpdated();
}

void SettingsDataStateService::onConfigUpdated() {
}
