#include <BrosseStateService.h>

BrosseStateService::BrosseStateService(AsyncWebServer* server, SecurityManager* securityManager) :
    _httpEndpoint(BrosseState::read,
                  BrosseState::update,
                  this,
                  server,
                  PODOMATIC_STATE_SETTINGS_ENDPOINT_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _webSocket(BrosseState::read,
               BrosseState::update,
               this,
               server,
               PODOMATIC_STATE_SETTINGS_SOCKET_PATH,
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED){
  // configure settings service update handler to update LED state
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void BrosseStateService::begin() {
  _state.etat = "michel";
  _state.mesure_angle = 1;
  _state.shouldRAZ=0;
  _state.mesureCourant=0;
  _state.duree_etat=0;

  onConfigUpdated();
}

void BrosseStateService::onConfigUpdated() {
  
}
