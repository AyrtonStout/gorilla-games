import React from 'react';
import {Slide, toast, ToastContainer} from "react-toastify";
import {Api} from "../../services/api";
import {TetrisContainer} from "../tetris-container/tetris-container";
import {GameQueue} from "../game-queue/game-queue";
import * as LocalStorage from "../../services/local-storage";
import {uuid4} from "../../services/util";

export class SiteWrapper extends React.Component {
	constructor(props) {
		super(props);

		this.state = {
			version: null,
			gameId: null
		}
	}

	componentDidMount() {
		Api.get('version').then((serverVersion) => {
			this.setState({ version: serverVersion.version });
		});

		if (LocalStorage.getString('playerGuid') === undefined) {
			LocalStorage.setString('playerGuid', uuid4());
		}
	}

	setGameId(gameId) {
		this.setState({ gameId: gameId });
	}

	render() {
		return (
			<div id="site-wrapper">
				<ToastContainer autoClose={5000} hideProgressBar={true} transition={Slide}/>
				{
					this.state.gameId !== null ?
						<TetrisContainer gameId={this.state.gameId}/> :
						<GameQueue setGameId={this.setGameId.bind(this)}/>
				}
				<h5>{ this.state.gameId ? 'Your game ID is ' + this.state.gameId : '' }</h5>
			</div>
		)
	}
}
