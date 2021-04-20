import React, { Component } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { Typography, Box,TextField, FormControlLabel, Switch} from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';

import { restController, RestControllerProps, RestFormLoader, RestFormProps, FormActions, FormButton, SectionContent} from '../components';

import { SettingsDataState } from './types';
import { ENDPOINT_ROOT } from '../api';
//import { TextFieldsOutlined } from '@material-ui/icons';

export const LIGHT_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "SettingsDataState";

type SettingsDataStateRestControllerProps = RestControllerProps<SettingsDataState>;

class SettingsDataStateRestController extends Component<SettingsDataStateRestControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title='Reglages podomatic' titleGutter>
        <RestFormLoader
          {...this.props}
          render={props => (
            <SettingsDataStateRestControllerForm {...props} />
          )}
        />
         </SectionContent>
    )
  }

}

export default restController(LIGHT_SETTINGS_ENDPOINT, SettingsDataStateRestController);

type SettingsDataStateRestControllerFormProps = RestFormProps<SettingsDataState>;

function SettingsDataStateRestControllerForm(props: SettingsDataStateRestControllerFormProps) {
  const { data, saveData, handleValueChange } = props;
  return (
    <ValidatorForm onSubmit={saveData}>
      <Box bgcolor="primary.main" color="primary.contrastText" p={2} mt={2} mb={2}>
        <Typography variant="body1">
        Reglage des parametres :
        </Typography>
      </Box>
      <TextField id="standard-basic" label="Duréee d'un cycle de brossage (ms)" onChange={handleValueChange('MS_Brossage')} value={data.MS_Brossage} color="secondary" />
      <TextField id="standard-basic" label="Durée d'arret apres surcourant (ms)" onChange={handleValueChange('MS_Surcourant')} value={data.MS_Surcourant} color="secondary" />
      <TextField id="standard-basic" label="Date de remise à zero" type="datetime-local" onChange={handleValueChange('Date_RAZ')} value={data.Date_RAZ} color="secondary" />
      <TextField id="standard-basic" label="Courant Max (A)" onChange={handleValueChange('Courant_max')} value={data.Courant_max} color="secondary" />
      <TextField id="standard-basic" label="Temps minimum d'arrêt (ms)" onChange={handleValueChange('MS_ARRET')} value={data.MS_ARRET} color="secondary" />
      <TextField id="standard-basic" label="Tps d'arrêt apres surcourant (ms)" onChange={handleValueChange('MS_DEMARRAGE_MOTEUR')} value={data.MS_DEMARRAGE_MOTEUR} color="secondary" />
      <TextField id="standard-basic" label="Angle déclenchement (°)" onChange={handleValueChange('Angl_declenchement')} value={data.Angl_declenchement} color="secondary" />
      <FormActions>
        <FormButton startIcon={<SaveIcon />} variant="contained" color="primary" type="submit">
          Save
        </FormButton>
        </FormActions>

    </ValidatorForm>
  );
}
