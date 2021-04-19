import React, { Component } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { Typography, Box,TextField, FormControlLabel, Switch, Paper} from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';
import RefreshIcon from '@material-ui/icons/Refresh';

import { ENDPOINT_ROOT } from '../api';
import { restController, RestControllerProps, RestFormLoader, RestFormProps, FormActions, FormButton, SectionContent} from '../components';

import { SavedDataState } from './types';
import NumberFormat from 'react-number-format';

//import { TextFieldsOutlined } from '@material-ui/icons';

export const LIGHT_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "SavedDataState";

type SavedDataStateRestControllerProps = RestControllerProps<SavedDataState>;

class SavedDataStateRestController extends Component<SavedDataStateRestControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title='Historique podomatic' titleGutter >
        <RestFormLoader
          {...this.props}
          render={props => (
            <SavedDataStateRestControllerForm {...props} />
          )}
        />
        <FormButton startIcon={<RefreshIcon />} variant="contained" color="primary" onClick={this.props.loadData}>
          Rafraichir
        </FormButton>
      </SectionContent>
    )
  }

}

export default restController(LIGHT_SETTINGS_ENDPOINT, SavedDataStateRestController);

type SavedDataStateRestControllerFormProps = RestFormProps<SavedDataState>;

function refreshPage() {

}

function SavedDataStateRestControllerForm(props: SavedDataStateRestControllerFormProps) {
  const { data, saveData, handleValueChange } = props;
  return (
    
      <><Typography variant="subtitle2" gutterBottom>
      Temps total de brossage : <NumberFormat value={data.temps_total_brossage} decimalSeparator=',' displayType={'text'} decimalScale={2} /> heures ou <NumberFormat value={data.temps_total_brossage*60} decimalSeparator=',' displayType={'text'} decimalScale={2} /> minutes
    </Typography>
      <Typography variant="subtitle2" gutterBottom>
        Nombre total de demarrages: {data.nb_total_demarrage}
      </Typography>
      <Typography variant="subtitle2" gutterBottom>
        Nombre total d'arrêt en surcourant : {data.nb_surcourant}
      </Typography></>

  );
}
