export interface LightState {
  led_color: number;
  led_on: boolean;
}
export interface SavedDataState {
  temps_total_brossage : number;
  nb_total_demarrage : number;
  nb_surcourant : number;
}

export interface SettingsDataState{
  MS_Brossage : number;
  Courant_max : number;
  MS_Surcourant : number;
  Date_RAZ : String;
  Reset_counters : boolean;
  Angle_declenchement : number;
  MS_DEMARRAGE_MOTEUR : number;
  MS_ARRET: number;
}

export interface LightMqttSettings { 
  unique_id : string;
  name: string;
  mqtt_path : string;
}

export interface BrosseState{
  etat:string;
  angle:number;
  ResetJournal:boolean;
  duree_etat:number;
  ResetGravity:boolean;
  courant:number;
}
