import React, { Component } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { Typography, Box, Switch, Checkbox, FormControlLabel } from '@material-ui/core';
import { WEB_SOCKET_ROOT } from '../api';
import { WebSocketControllerProps, WebSocketFormLoader, WebSocketFormProps, webSocketController } from '../components';
import { SectionContent, BlockFormControlLabel } from '../components';

import { BrosseState } from './types';
import NumberFormat from 'react-number-format';
import { format } from 'path';

export const LIGHT_SETTINGS_WEBSOCKET_URL = WEB_SOCKET_ROOT + "BrosseState";

type BrosseStateWebSocketControllerProps = WebSocketControllerProps<BrosseState>;

class BrosseStateWebSocketController extends Component<BrosseStateWebSocketControllerProps> {

  render() {
    return (
      <SectionContent title='Etat courant' titleGutter>
        <WebSocketFormLoader
          {...this.props}
          render={props => (
            <BrosseStateWebSocketControllerForm {...props} />
          )}
        />
      </SectionContent>
    )
  }

}

export default webSocketController(LIGHT_SETTINGS_WEBSOCKET_URL, 100, BrosseStateWebSocketController);

type BrosseStateWebSocketControllerFormProps = WebSocketFormProps<BrosseState>;

function BrosseStateWebSocketControllerForm(props: BrosseStateWebSocketControllerFormProps) {
  const { data, saveData, handleValueChange } = props;

  return (
    <ValidatorForm onSubmit={saveData}>
      <FormControlLabel
        control={<Switch checked={data.shouldRAZ} onChange={handleValueChange('shouldRAZ')} name="checkedA" />}
        label="Remise à zero compteurs (à remettre à zero)"
      />
      <Typography variant="body1">
    Etat en cours : {data.etat} (<NumberFormat value={data.duree_etat} displayType={'text'} decimalSeparator=',' decimalScale={1} /> s)
    </Typography>
    <Typography variant="body1">
    Angle mesuré : <NumberFormat value={data.mesure_angle} displayType={'text'} decimalSeparator=',' decimalScale={1} /> °
    </Typography>
    <Typography variant="body1">
    Courant mesuré : <NumberFormat value={data.mesureCourant} displayType={'text'} decimalSeparator=',' decimalScale={2} /> A
    </Typography>
    <Typography variant="body1">
    Temps passé dans cet état : <NumberFormat value={data.duree_etat} displayType={'text'} decimalSeparator=',' decimalScale={1} /> s
    </Typography>
    </ValidatorForm>
  );
}