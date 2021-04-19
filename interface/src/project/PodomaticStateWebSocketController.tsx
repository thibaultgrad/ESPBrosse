import React, { Component } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { Typography, Box, Switch, Checkbox } from '@material-ui/core';
import { WEB_SOCKET_ROOT } from '../api';
import { WebSocketControllerProps, WebSocketFormLoader, WebSocketFormProps, webSocketController } from '../components';
import { SectionContent, BlockFormControlLabel } from '../components';

import { BrosseState } from './types';
import NumberFormat from 'react-number-format';
import { format } from 'path';

export const LIGHT_SETTINGS_WEBSOCKET_URL = WEB_SOCKET_ROOT + "PodomaticState";

type PodomaticStateWebSocketControllerProps = WebSocketControllerProps<BrosseState>;

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

type PodomaticStateWebSocketControllerFormProps = WebSocketFormProps<BrosseState>;

function PodomaticStateWebSocketControllerForm(props: PodomaticStateWebSocketControllerFormProps) {
  const { data, saveData, setData } = props;

  return (
    <ValidatorForm onSubmit={saveData}>
      <Typography variant="body1">
    Etat en cours : {data.etat} (<NumberFormat value={data.duree_etat} displayType={'text'} decimalSeparator=',' decimalScale={1} /> s)
    </Typography>
    <Typography variant="body1">
    Angle mesuré : <NumberFormat value={data.mesure_angle} displayType={'text'} decimalSeparator=',' decimalScale={1} /> °
    </Typography>
    <Typography variant="body1"> Cellule {data.presence ? "barrée":"ouverte"} <Checkbox checked={data.presence} readOnly/></Typography>
    <Typography variant="body1">
    Temps passé dans cet état : <NumberFormat value={data.duree_etat} displayType={'text'} decimalSeparator=',' decimalScale={1} /> s
    </Typography>
    </ValidatorForm>
  );
}