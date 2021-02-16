#ifndef PodomaticStateService_h
#define PodomaticStateService_h


#include <HttpEndpoint.h>
#include <WebSocketTxRx.h>

#define PODOMATIC_STATE_SETTINGS_SOCKET_PATH "/ws/PodomaticState"


#define PODOMATIC_STATE_SETTINGS_ENDPOINT_PATH "/rest/PodomaticState"


class PodomaticState {
 public:
	String etat;
	float mesure_niveau;
  bool presence;
  float duree_etat;

  static void read(PodomaticState& settings, JsonObject& root) {
    root["etat"] = settings.etat;
    root["mesure_niveau"] = settings.mesure_niveau;
    root["presence"] = settings.presence;
    root["duree_etat"]=settings.duree_etat;
  }

  static StateUpdateResult update(JsonObject& root, PodomaticState& savedState) {

    String newTSpray;

    if(root.containsKey("etat"))
    {
      newTSpray= root.containsKey("etat") ;
    }
    else{
      newTSpray="mich";
    }

    bool newPres = root.containsKey("presence") ? root["presence"] : 0;
    float newTPassage = root.containsKey("mesure_niveau") ? root["mesure_niveau"]:1;
    float newDuree = root.containsKey("duree_etat") ? root["duree_etat"]:1;

      savedState.etat = newTSpray;
      savedState.mesure_niveau = newTPassage;
      savedState.presence=newPres;
      savedState.duree_etat=newDuree;
      return StateUpdateResult::CHANGED;
  }

};

class PodomaticStateService : public StatefulService<PodomaticState> {
 public:
  PodomaticStateService(AsyncWebServer* server,
                    SecurityManager* securityManager);
  void begin();

 private:
  HttpEndpoint<PodomaticState> _httpEndpoint;
  WebSocketTxRx<PodomaticState> _webSocket;
 
  void onConfigUpdated();
};

#endif
