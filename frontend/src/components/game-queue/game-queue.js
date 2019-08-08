import React from 'react';
import {Api} from "../../services/api";

export class GameQueue extends React.Component {
	constructor(props) {
		super(props);

		this.state = {
			inQueue: false,
			queueId: 0,
			gameId: 0,
			queueCalls: 0
		}
	}

	enterQueue() {
		this.setState({
			inQueue: true,
			queueCalls: 0
		});

		Api.post('queue').then(res => {
			this.setState({ queueId: res.id }, this.pollForMatch);
		});
	}

	pollForMatch() {
		this.setState({ queueCalls: this.state.queueCalls + 1 });

		Api.get('queue/' + this.state.queueId).then(res => {
			if (res.gameId === null) {
				this.pollForMatch();
				return;
			}

			this.props.setGameId(res.gameId);
			this.setState({ inQueue: false });
		});
	}

	getEllipses() {
		return '.'.repeat(((this.state.queueCalls - 1) % 3) + 1);
	}

	render() {
		return (
			<div id="game-queue">
				<h4>{ this.state.inQueue ? 'You are in queue ' : 'Hello, Human' }</h4>
				<h5>{ this.getEllipses() }</h5>

				<div>
					{ !this.state.inQueue ? (
						<button onClick={() => this.enterQueue()}>
							Enter Queue
						</button>
					) : null }
				</div>
			</div>
		)
	}
}
