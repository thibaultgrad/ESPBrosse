#include <ESP8266React.h>
#include <SavedDataStateService.h>
#include <SettingsDataStateService.h>
#include <PodomaticStateService.h>
#include <Wire.h>

#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);
ESP8266React esp8266React(&server);

SavedDataStateService savedDataStateService = SavedDataStateService(&server, esp8266React.getSecurityManager());

SettingsDataStateService settingsDataStateService =
    SettingsDataStateService(&server, esp8266React.getSecurityManager());
PodomaticStateService podomaticStateService = PodomaticStateService(&server, esp8266React.getSecurityManager());
double temps_total_spray;
unsigned long nb_total_passage;
unsigned int D_Min_mm;
unsigned int D_Max_mm;
unsigned long MS_SPRAY;
unsigned int MS_RETARD_DEMARRAGE;
float MS_Arret;
String Date_RAZ;
bool Reset_counters;

unsigned long refresh_date;

//Declaring some global variables
int gyro_x, gyro_y, gyro_z;

boolean set_gyro_angles;

long acc_x, acc_y, acc_z;

//long loop_timer;

int temp;

enum Etats
{
  Attente,
  Spraying,
  Attente_demarrage,
  Erreur,
  Niveau_produit_bas,
};

boolean etat_spray;

unsigned long loop_timer = 0UL;

long t_debut_etat;
unsigned int duree_etat;
bool presence = false;
Etats etat = Attente;

#define pin_moteur_relais1 2
#define pin_moteur_relais2 4

#define pin_detection 5

int nb_spray_non_enregistre;
#define nb_spray_avt_refresh 10

const char *stateStr[] = {"Attente", "Spraying", "Attente_demarrage", "Erreur", "Niveau_produit_bas"};

double gravity;
double angle;
double angle_brut = 0;

int i = 0;
// periode d'échantillonnage en ms
#define MS_PERIOD_ECH  2

// temps de moyennage du courant
#define MS_MOYENNE    140
#define TAILLE_TABLEAU_ECHANTILLONS   (MS_MOYENNE / MS_PERIOD_ECH)

int indexechantilon = 0;
uint8_t echantillon_angle[TAILLE_TABLEAU_ECHANTILLONS] = { 0 };

void ReadSavedDatas()
{
  savedDataStateService.read([](SavedDataState _state) {
    temps_total_spray = _state.temps_total_spray;
    nb_total_passage = _state.nb_total_passage;
  });
}
void ReadSettings()
{
  settingsDataStateService.read([](SettingsDataState _state) {
    MS_SPRAY = _state.MS_Brossage;
    MS_RETARD_DEMARRAGE = _state.MS_Surcourant;
    MS_Arret = _state.Courant_max;
    Date_RAZ = _state.Date_RAZ;
    Reset_counters = _state.Reset_counters;
  });
}

void UpdateSavedDatas()
{
  savedDataStateService.update(
      [](SavedDataState &state) {
        state.temps_total_spray = temps_total_spray;
        state.nb_total_passage = nb_total_passage;
        return StateUpdateResult::CHANGED;
      },
      "Jean");
}
void UpdateSettings()
{
  settingsDataStateService.update(
      [](SettingsDataState &state) {
        state.MS_Brossage = MS_SPRAY;
        state.MS_Surcourant = MS_RETARD_DEMARRAGE;
        state.Courant_max = MS_Arret;
        state.Date_RAZ = Date_RAZ;
        state.Reset_counters = Reset_counters;
        return StateUpdateResult::CHANGED;
      },
      "Jean");
}
void UpdatePodoState()
{
  podomaticStateService.update(
      [](PodomaticState &state) {
        state.etat = (String)stateStr[(int)etat];
        state.mesure_niveau = angle;
        state.presence = presence;
        state.duree_etat = duree_etat;
        return StateUpdateResult::CHANGED;
      },
      "Jean");
}
void SprayOff()
{
  t_debut_etat = millis();
  digitalWrite(pin_moteur_relais1, LOW);
  // delay(2);
  // digitalWrite(pin_moteur_relais2, HIGH);
  etat_spray = 0;
}
void SprayOn()
{
  t_debut_etat = millis();
  etat_spray = 1;
  digitalWrite(pin_moteur_relais1, HIGH);
  // delay(2);
  // digitalWrite(pin_moteur_relais2, LOW);
}
void ajout_temps_spraying()
{
  temps_total_spray += (double)(duree_etat) / (1000.0 * 3600.0);
  nb_total_passage++;
  UpdateSavedDatas();
}
void setup_mpu_6050_registers()
{
  //Activate the MPU-6050
  Wire.beginTransmission(0x68); //Start communicating with the MPU-6050
  Wire.write(0x6B);             //Send the requested starting register
  Wire.write(0x00);             //Set the requested starting register
  Wire.endTransmission();
  //Configure the accelerometer (+/-8g)
  Wire.beginTransmission(0x68); //Start communicating with the MPU-6050
  Wire.write(0x1C);             //Send the requested starting register
  Wire.write(0x10);             //Set the requested starting register
  Wire.endTransmission();
  //Configure the gyro (500dps full scale)
  Wire.beginTransmission(0x68); //Start communicating with the MPU-6050
  Wire.write(0x1B);             //Send the requested starting register
  Wire.write(0x08);             //Set the requested starting register
  Wire.endTransmission();
}

void read_mpu_6050_data()
{                               //Subroutine for reading the raw gyro and accelerometer data
  Wire.beginTransmission(0x68); //Start communicating with the MPU-6050
  Wire.write(0x3B);             //Send the requested starting register
  Wire.endTransmission();       //End the transmission
  Wire.requestFrom(0x68, 14);   //Request 14 bytes from the MPU-6050
  while (Wire.available() < 14)
    ; //Wait until all the bytes are received
  acc_x = Wire.read() << 8 | Wire.read();
  acc_y = Wire.read() << 8 | Wire.read();
  acc_z = Wire.read() << 8 | Wire.read();
  temp = Wire.read() << 8 | Wire.read();
  gyro_x = Wire.read() << 8 | Wire.read();
  gyro_y = Wire.read() << 8 | Wire.read();
  gyro_z = Wire.read() << 8 | Wire.read();
}
void Echantillonnageangle()
{
	echantillon_angle[indexechantilon] = angle_brut;
	for (i = 0; i < TAILLE_TABLEAU_ECHANTILLONS; i++)
	{
		angle += echantillon_angle[i];
	}
	angle = angle / TAILLE_TABLEAU_ECHANTILLONS;
	indexechantilon++;
	if (indexechantilon == TAILLE_TABLEAU_ECHANTILLONS)
	{
		indexechantilon = 0;
	}
}

void setup()
{
  // start serial and filesystem
  pinMode(pin_moteur_relais1, OUTPUT);

  digitalWrite(pin_moteur_relais1, LOW);

  Serial.begin(SERIAL_BAUD_RATE);

  // start the framework and demo project
  esp8266React.begin();

  savedDataStateService.begin();

  settingsDataStateService.begin();

  podomaticStateService.begin();

  refresh_date = millis();

  // start the server
  server.begin();
  Wire.begin(4,5); //Start I2C as master
  setup_mpu_6050_registers();

  //ReadSavedDatas();
  delay(40);
  read_mpu_6050_data();
  gravity = (double)acc_z;

  pinMode(pin_detection, INPUT);

  t_debut_etat = millis();
  etat_spray = 0;
}

void loop()
{
  // run the framework's loop function
  // loop_timer = millis();
  esp8266React.loop();

  read_mpu_6050_data();
  float ratio = (double)acc_z / gravity;
  ratio = constrain(ratio,-1,1);
  angle_brut = acos(ratio) * 57.296;
  Echantillonnageangle(); 
  digitalWrite(pin_moteur_relais1,angle >=25); 
 if (abs(refresh_date - millis()) > 1000)
  {
    ReadSettings();
    if (Reset_counters == true)
    {
      nb_total_passage = 0;
      temps_total_spray = 0;
      Reset_counters = false;
      UpdateSavedDatas();
    }
    refresh_date = millis();
  }

  ///presence = digitalRead(pin_detection);
  duree_etat = (unsigned int)abs(millis() - loop_timer);
  UpdatePodoState();
  // int val_etat = (int)etat;
  // switch (val_etat)
  // {
  // case (int)Attente:
  // {
  //   if ((presence) && (duree_etat > MS_Arret))
  //   {
  //     etat = Attente_demarrage;
  //     t_debut_etat = millis();
  //   }
  //   break;
  // }
  // case (int)Attente_demarrage:
  // {
  //   if (duree_etat > MS_RETARD_DEMARRAGE)
  //   {
  //     etat = Spraying;
  //     SprayOn();
  //   }
  //   break;
  // }
  // case (int)Spraying:
  // {
  //   if (duree_etat > MS_SPRAY)
  //   {
  //     ajout_temps_spraying();
  //     if (MS_RETARD_DEMARRAGE <= 0 && MS_Arret <= 0 && presence)
  //     {
  //       t_debut_etat = millis();
  //     }
  //     else
  //     {
  //       etat = Attente;
  //       SprayOff();
  //     }
  //   }

  //   break;
  // }
  // case (int)Erreur:
  // {
  //   break;
  // }
  // case (int)Niveau_produit_bas:
  // {
  //   break;
  // }
  // default:
  // {
  //   break;
  // }
  // }
}
