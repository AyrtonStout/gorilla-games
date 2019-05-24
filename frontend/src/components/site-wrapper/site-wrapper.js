import React from 'react';
import {Slide, toast, ToastContainer} from "react-toastify";
import {Api} from "../../services/api";
import {TetrisContainer} from "../tetris-container/tetris-container";

export class SiteWrapper extends React.Component {
	constructor(props) {
		super(props);

		this.state = {
			version: null
		}
	}

	componentDidMount() {
		Api.get('version').then((serverVersion) => {
			console.log(serverVersion);
			this.setState({ version: serverVersion.version });
			toast.info("Hello")
		})
	}

	render() {
		return (
			<div>
				<ToastContainer autoClose={5000} hideProgressBar={true} transition={Slide}/>
				<div>
					{this.state.version}
				</div>
				<TetrisContainer/>
			</div>
		)
	}
}
