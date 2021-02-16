#include <PodomaticStateService.h>

PodomaticStateService::PodomaticStateService(AsyncWebServer* server, SecurityManager* securityManager) :
    _httpEndpoint(PodomaticState::read,
                  PodomaticState::update,
                  this,
                  server,
                  PODOMATIC_STATE_SETTINGS_ENDPOINT_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _webSocket(PodomaticState::read,
               PodomaticState::update,
               this,
               server,
               PODOMATIC_STATE_SETTINGS_SOCKET_PATH,
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED){
  // configure settings service update handler to update LED state
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void PodomaticStateService::begin() {
  _state.etat = "michel";
  _state.mesure_niveau = 1;
  _state.presence=0;
  _state.duree_etat=0;

  onConfigUpdated();
}

void PodomaticStateService::onConfigUpdated() {
}
