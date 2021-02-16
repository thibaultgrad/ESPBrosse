import React, { Component } from 'react';
import { Link, withRouter, RouteComponentProps } from 'react-router-dom';

import {List, ListItem, ListItemIcon, ListItemText} from '@material-ui/core';
import BathtubIcon from '@material-ui/icons/Bathtub';

import { PROJECT_PATH } from '../api';

class ProjectMenu extends Component<RouteComponentProps> {

  render() {
    const path = this.props.match.url;
    return (
      <List>
        <ListItem to={`/${PROJECT_PATH}/demo/`} selected={path.startsWith(`/${PROJECT_PATH}/demo/`)} button component={Link}>
          <ListItemIcon>
            <BathtubIcon />
          </ListItemIcon>
          <ListItemText primary="Podomatic" />
        </ListItem>
      </List>
    )
  }

}

export default withRouter(ProjectMenu);
