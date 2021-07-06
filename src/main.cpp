#include <ESP8266React.h>
#include <SavedDataStateService.h>
#include <SettingsDataStateService.h>
#include <BrosseStateService.h>
#include <Wire.h>

#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);
ESP8266React esp8266React(&server);

SavedDataStateService savedDataStateService = SavedDataStateService(&server, esp8266React.getSecurityManager());

SettingsDataStateService settingsDataStateService =
    SettingsDataStateService(&server, esp8266React.getSecurityManager());
BrosseStateService brosseStateService = BrosseStateService(&server, esp8266React.getSecurityManager());

//#pragma region Parametres de brossage
float AngleDeclenchement;
unsigned long nb_total_demarrage;
unsigned long MS_BROSSAGE;
unsigned int MS_SurCourant = 15000000;
unsigned int MS_ARRET = 3000000;
unsigned int MS_DEMARRAGE_MOTEUR = 1500000;
float IMax;
//#define MS_SURCOURANT 15000000 //en microseconde
//#define MS_DEMARRAGE_MOTEUR 1500000 //en microseconde
//#define MS_ARRET 3000000 //en microseconde
//#pragma endregion Parametres de brossage

//#pragma region Donnees sauvegardees
double temps_total_brossage;
unsigned int nb_surcourant;
String Date_RAZ;
bool Reset_counters;
bool ResetGravity;
unsigned long refresh_date;
//#pragma endregion

//Declaring some global variables
int gyro_x, gyro_y, gyro_z;

boolean set_gyro_angles;

long acc_x, acc_y, acc_z;

//long loop_timer;

int temp;

enum Etats
{
  Attente,
  Demarrage,
  Brossage,
  SurCourant,
  DelayAfterStop,
};

bool etat_moteur = false;
bool SensRotation = true;

unsigned long loop_timer = 0UL;

unsigned long t_debut_etat;
unsigned int duree_etat;
bool presence = false;
Etats etat = Attente;

#define pin_moteur_On 14
#define pin_moteur_Sens_rotation 12
#define pinCourant A0

//int nb_spray_non_enregistre;
//#define nb_spray_avt_refresh 10

const char *stateStr[] = {"Attente",  "Demarrage", "Brossage", "SurCourant", "Attente apres arrêt"};

double gravity;
double angle;
double angle_brut = 0;

int i = 0;
// periode d'échantillonnage en ms
#define MS_PERIOD_ECH 2

// temps de moyennage du courant
#define MS_MOYENNE 140
#define TAILLE_TABLEAU_ECHANTILLONS (MS_MOYENNE / MS_PERIOD_ECH)

int indexechantilon = 0;
uint8_t echantillon_angle[TAILLE_TABLEAU_ECHANTILLONS] = {0};

float mVperAmpValue = 100;   // If using ACS712 current module : for 5A module key in 185, for 20A module key in 100, for 30A module key in 66
                             // If using "Hall-Effect" Current Transformer, key in value using this formula: mVperAmp = maximum voltage range (in milli volt) / current rating of CT
                             // For example, a 20A Hall-Effect Current Transformer rated at 20A, 2.5V +/- 0.625V, mVperAmp will be 625 mV / 20A = 31.25mV/A
float offsetSampleRead = 0;  /* to read the value of a sample for offset purpose later */
float currentSampleRead = 0; /* to read the value of a sample including currentOffset1 value*/
float currentLastSample = 0; /* to count time for each sample. Technically 1 milli second 1 sample is taken */
float currentSampleSum = 0;  /* accumulation of sample readings */
int currentSampleCount = 0;  /* to count number of sample. */
float currentMean;           /* to calculate the average value from all samples, in analog values*/
float RMSCurrentMean;        /* square roof of currentMean, in analog values */
float adjustRMSCurrentMean;  /* RMScurrentMean including currenOffset2, in analog values */
float FinalRMSCurrent;       /* the final RMS current reading*/

/*1.1 Offset AC Current */

float currentOffset1 = 0; // to Offset deviation and accuracy. Offset any fake current when no current operates.
                          // Offset will automatically callibrate when SELECT Button on the LCD Display Shield is pressed.
                          // If you do not have LCD Display Shield, look into serial monitor to add or minus the value manually and key in here.
                          // 26 means add 26 to all analog value measured
float currentOffset2 = 0; // to offset value due to calculation error from squared and square root.

float Courant = 0;

void ReadSavedDatas()
{
  savedDataStateService.read([](SavedDataState _state) {
    temps_total_brossage = _state.temps_total_brossage;
    nb_total_demarrage = _state.nb_total_demarrage;
    nb_surcourant = _state.nb_surcourant;
  });
}
void ReadSettings()
{
  settingsDataStateService.read([](SettingsDataState _state) {
    MS_BROSSAGE = _state.MS_Brossage;
    MS_SurCourant = _state.MS_Surcourant;
    MS_DEMARRAGE_MOTEUR = _state.MS_DEMARRAGE_MOTEUR;
    IMax = _state.Courant_max;
    Date_RAZ = _state.Date_RAZ;
    Reset_counters = _state.Reset_counters;
    AngleDeclenchement=_state.Angle_declenchement;
    MS_ARRET=_state.MS_ARRET;
  });
}

void UpdateSavedDatas()
{
  savedDataStateService.update(
      [](SavedDataState &state) {
        state.temps_total_brossage = temps_total_brossage;
        state.nb_total_demarrage = nb_total_demarrage;
        return StateUpdateResult::CHANGED;
      },
      "Jean");
}
void UpdateSettings()
{
  settingsDataStateService.update(
      [](SettingsDataState &state) {
        state.MS_Brossage = MS_BROSSAGE;
        state.MS_Surcourant = MS_SurCourant;
        state.Courant_max = IMax;
        state.Date_RAZ = Date_RAZ;
        state.Reset_counters = false;
        return StateUpdateResult::CHANGED;
      },
      "Jean");
}
void UpdatePodoState()
{
  brosseStateService.update(
      [](BrosseState &state) {
        state.etat = (String)stateStr[(int)etat];
        state.angle = angle;
        state.ResetJournal = Reset_counters;
        state.duree_etat = duree_etat/1000.0f;
        state.ResetGravity=false;
        state.courant=Courant;
        return StateUpdateResult::CHANGED;
      },
      "Jean");
}
void ReadPodoState()
{
  brosseStateService.read([](BrosseState _state) {
    Reset_counters = _state.ResetJournal;
    ResetGravity=_state.ResetGravity;
  });
}
void MoteursOff()
{
  t_debut_etat = millis();
  digitalWrite(pin_moteur_On, LOW);
  etat_moteur = 0;
}
void InversionSensRotation()
{
  SensRotation = !SensRotation;
  digitalWrite(pin_moteur_Sens_rotation, SensRotation);
}
void MoteursOn()
{
  InversionSensRotation();
  nb_total_demarrage++;
  t_debut_etat = millis();
  etat_moteur = 1;
  digitalWrite(pin_moteur_On, HIGH);
}
void ajout_temps_brossage()
{
  temps_total_brossage += (double)(duree_etat) / (1000.0 * 3600.0);
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
void SetGravity()
{
  gravity = (double)acc_z;
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
  float ratio = (double)acc_z / gravity;
  ratio = constrain(ratio, -1, 1);
  angle_brut = acos(ratio) * 57.296;
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
void echantillonnagecourant()
{

  offsetSampleRead = analogRead(pinCourant) - 512;                   /* Read analog value. This is for offset purpose */
  currentSampleRead = analogRead(pinCourant) - 512 + currentOffset1; /* read the sample value including offset value*/
  currentSampleSum = currentSampleSum + sq(currentSampleRead);       /* accumulate total analog values for each sample readings*/

  currentSampleCount += 1;
  //tension += 0.01;/* to count and move on to the next following count */
  /* to reset the time again so that next cycle can start again*/

  if (currentSampleCount >= 50) /* after 1000 count or 1000 milli seconds (1 second), do this following codes*/
  {
    currentMean = currentSampleSum / currentSampleCount;                        /* average accumulated analog values*/
    RMSCurrentMean = sqrt(currentMean);                                         /* square root of the average value*/
    adjustRMSCurrentMean = RMSCurrentMean + currentOffset2;                     /* square root of the average value including offset value */
    FinalRMSCurrent = (((adjustRMSCurrentMean / 1024) * 5000) / mVperAmpValue); /* calculate the final RMS current*/
    Courant = FinalRMSCurrent;
    currentSampleSum = 0;   /* to reset accumulate sample values for the next cycle */
    currentSampleCount = 0; /* to reset number of sample for the next cycle */
  }
}
void setup()
{
  // start serial and filesystem
  pinMode(pin_moteur_On, OUTPUT);

  digitalWrite(pin_moteur_On, LOW);

  pinMode(pin_moteur_Sens_rotation, OUTPUT);

  digitalWrite(pin_moteur_Sens_rotation, HIGH);

  Serial.begin(SERIAL_BAUD_RATE);

  // start the framework and demo project
  esp8266React.begin();

  savedDataStateService.begin();

  settingsDataStateService.begin();

  brosseStateService.begin();

  refresh_date = millis();

  // start the server
  server.begin();
  Wire.begin(4, 5); //Start I2C as master
  setup_mpu_6050_registers();

  //ReadSavedDatas();
  delay(40);
  read_mpu_6050_data();

  t_debut_etat = millis();
  etat_moteur = 0;
  SetGravity();
}

void loop()
{
  // run the framework's loop function
  // loop_timer = millis();
  esp8266React.loop();

  read_mpu_6050_data();
  Echantillonnageangle();

  if (abs((long)(refresh_date - millis())) > 1000)
  {
    ReadSettings();
    refresh_date = millis();
  }

  ///presence = digitalRead(pin_detection);
  duree_etat = (unsigned int)abs((long)(millis() - t_debut_etat));
  ReadPodoState();
    if (Reset_counters)
    {
      nb_total_demarrage = 0;
      temps_total_brossage = 0;
      Reset_counters = false;
      UpdateSavedDatas();
    }
    if (ResetGravity) SetGravity();
  UpdatePodoState();
  int val_etat = (int)etat;
  switch (val_etat)
  {
  case (int)Attente:
    echantillonnagecourant();
    if (angle >= AngleDeclenchement)
    {
      etat = Demarrage;
      t_debut_etat = millis();
    }
    break;
  case (int)Demarrage:
    if (etat_moteur == 0)
    {
      MoteursOn();
    }
    if ((duree_etat) > MS_DEMARRAGE_MOTEUR)
    {
      etat = Brossage;
    }
    break;
  case (int)Brossage:
    echantillonnagecourant();
    if (Courant > IMax || ((duree_etat > MS_BROSSAGE) && angle < AngleDeclenchement))
    {
      etat = (Courant > IMax) ? SurCourant : DelayAfterStop;
      MoteursOff();
      t_debut_etat = millis();
      ajout_temps_brossage();
    }
    else if ((duree_etat > MS_BROSSAGE) && angle >= AngleDeclenchement)
    {
      ajout_temps_brossage();
      t_debut_etat = millis();
    }
    break;
  case (int)DelayAfterStop:
    if (duree_etat > MS_ARRET)
    {
      etat = Attente;
    }
    break;
  case (int)SurCourant:
    if (duree_etat > MS_SurCourant)
    {
      etat = Attente;
    }
    break;

  default:

    break;
  }
}
