#ifndef PodomaticStateService_h
#define PodomaticStateService_h


#include <HttpEndpoint.h>
#include <WebSocketTxRx.h>

#define PODOMATIC_STATE_SETTINGS_SOCKET_PATH "/ws/PodomaticState"


#define PODOMATIC_STATE_SETTINGS_ENDPOINT_PATH "/rest/PodomaticState"


class PodomaticState {
 public:
	String etat;
	float angle;
  float courant;
  bool ResetJournal;
  bool ResetGravity;
  float duree_etat;

  static void read(PodomaticState& settings, JsonObject& root) {
    root["etat"] = settings.etat;
    root["angle"] = settings.angle;
    root["ResetJournal"] = settings.ResetJournal;
    root["ResetGravity"] = settings.ResetGravity;
    root["duree_etat"]=settings.duree_etat;
    root["courant"]=settings.courant;
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

    bool newPres = root.containsKey("ResetJournal") ? root["ResetJournal"] : 0;
    bool newGrav = root.containsKey("ResetGravity") ? root["ResetGravity"] : 0;
    float newTPassage = root.containsKey("angle") ? root["angle"]:1;
    float newCour = root.containsKey("courant") ? root["courant"]:1;
    float newDuree = root.containsKey("duree_etat") ? root["duree_etat"]:1;
      savedState.ResetGravity=newGrav;
      savedState.etat = newTSpray;
      savedState.angle = newTPassage;
      savedState.ResetJournal=newPres;
      savedState.duree_etat=newDuree;
      savedState.courant=newCour;
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
