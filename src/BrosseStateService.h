#ifndef BrosseStateService_h
#define BrosseStateService_h


#include <HttpEndpoint.h>
#include <WebSocketTxRx.h>

#define PODOMATIC_STATE_SETTINGS_SOCKET_PATH "/ws/BrosseState"


#define PODOMATIC_STATE_SETTINGS_ENDPOINT_PATH "/rest/BrosseState"


class BrosseState {
 public:
	String etat;
	float angle;
  float courant;
  bool ResetJournal;
  bool ResetGravity;
  bool startMotor;
  float duree_etat;

  static void read(BrosseState& settings, JsonObject& root) {
    root["etat"] = settings.etat;
    root["angle"] = settings.angle;
    root["ResetJournal"] = settings.ResetJournal;
    root["ResetGravity"] = settings.ResetGravity;
    root["duree_etat"]=settings.duree_etat;
    root["courant"]=settings.courant;
    root["startMotor"]=settings.startMotor;

  }

  static StateUpdateResult update(JsonObject& root, BrosseState& savedState) {

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
    bool newStartMotor = root.containsKey("startMotor") ? root["startMotor"] : 0;
    float newTPassage = root.containsKey("angle") ? root["angle"]:1;
    float newCour = root.containsKey("courant") ? root["courant"]:1;
    float newDuree = root.containsKey("duree_etat") ? root["duree_etat"]:1;
      savedState.ResetGravity=newGrav;
      savedState.etat = newTSpray;
      savedState.angle = newTPassage;
      savedState.ResetJournal=newPres;
      savedState.duree_etat=newDuree;
      savedState.courant=newCour;
      savedState.startMotor=newStartMotor;
      return StateUpdateResult::CHANGED;
  }

};

class BrosseStateService : public StatefulService<BrosseState> {
 public:
  BrosseStateService(AsyncWebServer* server,
                    SecurityManager* securityManager);
  void begin();

 private:
  HttpEndpoint<BrosseState> _httpEndpoint;
  WebSocketTxRx<BrosseState> _webSocket;
 
  void onConfigUpdated();
};

#endif
