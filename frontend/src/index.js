import React from 'react';
import ReactDOM from 'react-dom';
import {SiteWrapper} from "./components/site-wrapper/site-wrapper";

window.globalTestJs = () => {
    console.log("Hey it worked");
};

ReactDOM.render(
  <SiteWrapper/>,
  document.getElementById('root')
);
