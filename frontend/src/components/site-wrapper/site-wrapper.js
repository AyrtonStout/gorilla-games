import React from 'react';
import {Slide, toast, ToastContainer} from "react-toastify";
import {Api} from "../../services/api";
import {TetrisContainer} from "../tetris-container/tetris-container";
import {GameQueue} from "../game-queue/game-queue";

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
			console.log(serverVersion);
			this.setState({ version: serverVersion.version });
		})
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
						<TetrisContainer/> :
						<GameQueue setGameId={this.setGameId.bind(this)}/>
				}
				<h5>{ this.state.gameId ? 'Your game ID is ' + this.state.gameId : '' }</h5>
			</div>
		)
	}
}
