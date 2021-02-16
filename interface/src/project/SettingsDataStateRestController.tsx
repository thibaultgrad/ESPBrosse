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
      <FormControlLabel
        control={<Switch checked={data.Reset_counters} onChange={handleValueChange('Reset_counters')} name="checkedA" />}
        label="Remise à zero compteurs (à remettre à zero)"
      />
      <Box bgcolor="primary.main" color="primary.contrastText" p={2} mt={2} mb={2}>
        <Typography variant="body1">
        Reglage des parametres :
        </Typography>
      </Box>
      <TextField id="standard-basic" label="Temps de spray(ms)" onChange={handleValueChange('MS_SPRAY')} value={data.MS_SPRAY} color="secondary" />
      <TextField id="standard-basic" label="Retard demarrage(ms)" onChange={handleValueChange('MS_RETARD_DEMARRAGE')} value={data.MS_RETARD_DEMARRAGE} color="secondary" />
      <TextField id="standard-basic" label="Temps arret (ms)" onChange={handleValueChange('MS_Arret')} value={data.MS_Arret} color="secondary" />
      <TextField id="standard-basic" label="Level min cuve(mm)" onChange={handleValueChange('D_Min_level_cuve')} value={data.D_Min_level_cuve} color="secondary" />
      <FormActions>
        <FormButton startIcon={<SaveIcon />} variant="contained" color="primary" type="submit">
          Save
        </FormButton>
        </FormActions>

    </ValidatorForm>
  );
}
