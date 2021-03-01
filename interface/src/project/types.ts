export interface LightState {
  led_color: number;
  led_on: boolean;
}
export interface SavedDataState {
  temps_total_spray : number;
  nb_total_passage : number;

}

export interface SettingsDataState{
  MS_Brossage : number;
  Courant_max : number;
  MS_Surcourant : number;
  Date_RAZ : String;
  Reset_counters : boolean;
}

export interface LightMqttSettings { 
  unique_id : string;
  name: string;
  mqtt_path : string;
}

export interface PodomaticState{
  etat:string;
  mesure_niveau:number;
  presence:boolean;
  duree_etat:number;
}
