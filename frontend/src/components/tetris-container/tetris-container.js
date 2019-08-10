import React from 'react';
import {Api} from "../../services/api";
import * as LocalStorage from "../../services/local-storage";

let gameId = 0; // Keep a reference to gameId outside of the class so our global game-move-send function can send it

export class TetrisContainer extends React.Component {
	constructor(props) {
		super(props);

		this.state = {
			gameModule: undefined
		}
	}

	getModuleProperties() {
		return {
		  arguments: ['1234'], // Seed
			preRun: [],
			postRun: [],
			locateFile: path => {
				return '/dist/' + path
			},
			print: output => console.log(output),
			printErr: text => console.error(text),
			canvas: document.getElementById("canvas"),
			setStatus: text => {
				document.getElementById("blocks-status").innerHTML = text;
			}
		};
	}

	componentDidUpdate(prevProps) {
		gameId = prevProps.gameId;
	}

	componentDidMount() {
		let script = document.createElement('script');
		script.type = 'text/javascript';
		script.src = "../../../dist/blocks.js";

		document.head.appendChild(script);
		script.onload = () => {
			Module(this.getModuleProperties()).then(gameModule => {
				this.setState({ gameModule: gameModule });

				// Example of setting the seed for the game, though naturally won't work exactly like this forever
				gameModule.set_seed(50);
			});
		};
		script.onerror = () => {
			document.getElementById("blocks-status").innerHTML = "Failed to load Blocks. Did it get compiled?";
		}
	}

	render() {
		return (
			<div>
				<h4>Tetris</h4>
				<div id="blocks-status"/>
				<canvas id="canvas"/>
			</div>
		)
	}
}

window.sendGameCode = actionPerformed => {
	Api.post('game-move/game/' + gameId + '/move', {
		actionPerformed: actionPerformed,
		playerGuid: LocalStorage.getString('playerGuid')
	});
};

