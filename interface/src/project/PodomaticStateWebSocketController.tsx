import React, { Component } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { Typography, Box, Switch, Checkbox, FormControlLabel } from '@material-ui/core';
import { WEB_SOCKET_ROOT } from '../api';
import { WebSocketControllerProps, WebSocketFormLoader, WebSocketFormProps, webSocketController } from '../components';
import { SectionContent, BlockFormControlLabel } from '../components';

import { PodomaticState } from './types';
import NumberFormat from 'react-number-format';
import { format } from 'path';

export const LIGHT_SETTINGS_WEBSOCKET_URL = WEB_SOCKET_ROOT + "PodomaticState";

type PodomaticStateWebSocketControllerProps = WebSocketControllerProps<PodomaticState>;

class PodomaticStateWebSocketController extends Component<PodomaticStateWebSocketControllerProps> {

  render() {
    return (
      <SectionContent title='Etat courant' titleGutter>
        <WebSocketFormLoader
          {...this.props}
          render={props => (
            <PodomaticStateWebSocketControllerForm {...props} />
          )}
        />
      </SectionContent>
    )
  }

}

export default webSocketController(LIGHT_SETTINGS_WEBSOCKET_URL, 100, PodomaticStateWebSocketController);

type PodomaticStateWebSocketControllerFormProps = WebSocketFormProps<PodomaticState>;

function PodomaticStateWebSocketControllerForm(props: PodomaticStateWebSocketControllerFormProps) {
  const { data, saveData, setData } = props;

  const changepresence = (event: React.ChangeEvent<HTMLInputElement>) => {
    setData({ ResetJournal: event.target.checked,etat:data.etat,angle:data.angle,duree_etat:data.duree_etat,ResetGravity:false,courant:data.courant }, saveData);
  }
  const changegrav = (event: React.ChangeEvent<HTMLInputElement>) => {
    setData({ ResetGravity: event.target.checked,etat:data.etat,angle:data.angle,duree_etat:data.duree_etat,ResetJournal:false ,courant:data.courant}, saveData);
  }

  return (
    <ValidatorForm onSubmit={saveData}>
      <Typography variant="body1">
    Etat en cours : {data.etat} (<NumberFormat value={data.duree_etat} displayType={'text'} decimalSeparator=',' decimalScale={1} /> s)
    </Typography>
    <Typography variant="body1">
    Angle mesuré : <NumberFormat value={data.angle} displayType={'text'} decimalSeparator=',' decimalScale={1} /> °
    </Typography>
    <Typography variant="body1">
    Courant mesuré : <NumberFormat value={data.courant} displayType={'text'} decimalSeparator=',' decimalScale={2} /> A
    </Typography>
    <FormControlLabel
        control={<Switch checked={data.ResetJournal} onChange={changepresence} name="checkedA" />}
        label="Remise à zero compteurs"
      />
          <FormControlLabel
        control={<Switch checked={data.ResetGravity} onChange={changegrav} name="checkedB" />}
        label="Remise à zero de mesure angle"
      />
    <Typography variant="body1">
    Temps passé dans cet état : <NumberFormat value={data.duree_etat} displayType={'text'} decimalSeparator=',' decimalScale={2} /> s
    </Typography>
    </ValidatorForm>
  );
}