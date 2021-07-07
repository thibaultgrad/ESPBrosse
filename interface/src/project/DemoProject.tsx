import React, { Component } from 'react';
import { Redirect, Switch, RouteComponentProps } from 'react-router-dom'

import { Tabs, Tab } from '@material-ui/core';

import { PROJECT_PATH } from '../api';
import { MenuAppBar } from '../components';
import { AuthenticatedRoute } from '../authentication';

import DemoInformation from './DemoInformation';
import SavedDataStateRestController from './SavedDataStateRestController';
import SettingsDataStateRestController from './SettingsDataStateRestController';
import BrosseStateWebSocketController from './BrosseStateWebSocketController';

class RestComponent extends Component {

  render() {

    return (
      <React.Fragment>
        <SavedDataStateRestController {...this.props}/>
        <SettingsDataStateRestController {...this.props}/>
        <BrosseStateWebSocketController {...this.props}/>
        </React.Fragment>
    )
  }

}


class DemoProject extends Component<RouteComponentProps> {

  handleTabChange = (event: React.ChangeEvent<{}>, path: string) => {
    this.props.history.push(path);
  };

  render() {

    return (
      <MenuAppBar sectionTitle="Gestion Brosse">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} variant="fullWidth">
          <Tab value={`/${PROJECT_PATH}/demo/rest`} label="Brosse" />
          <Tab value={`/${PROJECT_PATH}/demo/information`} label="Information" />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/demo/rest`} component={RestComponent} /> 
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/demo/information`} component={DemoInformation} />
          <Redirect to={`/${PROJECT_PATH}/demo/rest`} />
        </Switch>
      </MenuAppBar>
    )
  }

}

export default DemoProject;
