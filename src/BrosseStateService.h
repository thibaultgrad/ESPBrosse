#ifndef BrosseStateService_h
#define BrosseStateService_h


#include <HttpEndpoint.h>
#include <WebSocketTxRx.h>

#define PODOMATIC_STATE_SETTINGS_SOCKET_PATH "/ws/BrosseState"


#define PODOMATIC_STATE_SETTINGS_ENDPOINT_PATH "/rest/BrosseState"


class BrosseState {
 public:
	String etat;
	float mesure_angle;
  float mesureCourant;
  float duree_etat;
  bool shouldRAZ;

  static void read(BrosseState& settings, JsonObject& root) {
    root["shouldRAZ"]=settings.shouldRAZ;
    root["etat"] = settings.etat;
    root["mesure_angle"] = settings.mesure_angle;
    root["mesureCourant"] = settings.mesureCourant;
    root["duree_etat"]=settings.duree_etat;
  }

  static StateUpdateResult update(JsonObject& root, BrosseState& savedState) {

    String newEtat;

    if(root.containsKey("etat"))
    {
      newEtat= root.containsKey("etat") ;
    }
    else{
      newEtat="mich";
    }

    bool newCourant = root.containsKey("mesureCourant") ? root["mesureCourant"] : 0;
    float newAngle = root.containsKey("mesure_angle") ? root["mesure_angle"]:1;
    float newDuree = root.containsKey("duree_etat") ? root["duree_etat"]:1;

      savedState.etat = newEtat;
      savedState.mesure_angle = newAngle;
      savedState.mesureCourant=newCourant;
      savedState.duree_etat=newDuree;
      savedState.shouldRAZ=root.containsKey("shouldRAZ") ? root["shouldRAZ"] : 0;
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
