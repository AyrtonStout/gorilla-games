import React from 'react';

export class TetrisContainer extends React.Component {
	constructor(props) {
		super(props);
	}

	getModuleProperties() {
		return {
			preRun: [],
			postRun: [],
            locateFile: (path) => {
				return '/dist/' + path
			},
			print: (output) => console.log(output),
			printErr: (text) => console.error(text),
			canvas: document.getElementById("canvas"),
			setStatus: (text) => {
				document.getElementById("blocks-status").innerHTML = text;
			}
		};
	}

	componentDidMount() {
		let script = document.createElement('script');
		script.type = 'text/javascript';
		script.src = "../../../dist/blocks.js";

		document.head.appendChild(script);
		script.onload = () => {
			Module(this.getModuleProperties());
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
