import React, { Component } from 'react';

class EventBind extends Component {

  constructor(props){
     super(props);

     this.state = {
        message : 'Hello'
     }

     this.constructorClickHandler = this.constructorClickHandler.bind(this);
  }


  // PREFERRED WAY
  fullArrowFunctionClickHandler = () => {
    // This should be a best way, and it should be used  
    // as the 'usingBindClickHandler' method...
    // Note: There is no () are used at the end in the onClick below...
    this.setState({ message: 'In class array method handler'});
  }

  notWorkingClickHandler(){
    // This handler the way it is called, will not have access to the this pointer,
    // as it is 'undefined'...
     this.setState({ message: 'This will not work at all!'});
  }

  usingBindClickHandler(){
    // Using the bind in the onClick, it works...
    // But it may cause some performance issues, as a event handler
    // a every randering of the page...
    this.setState({ message: 'Using a bind works'});
  }

  arrowFunctionClickHandler(){
    // This works too, but may also have some perfomance issue, 
    // as the 'usingBindClickHandler' method...
    // Note: You notice that () are used at the end in the onClick below...
    this.setState({ message: 'Using array method handler'});
  }

  constructorClickHandler(){
    // This should be the preferred way, as no new event handler  is
    // recreated a every render() invocation...
    // Note: No () in the onClick invocation
    this.setState({ message: 'handler bind in Constructor'});
  }

 

    render() {
        return (
            <div>
                <div>{ this.state.message }</div>
                { /* Below SHOULD BE THE PREFERRED WAY! */ }
                <button onClick={ this.fullArrowFunctionClickHandler }>In class Array function(Best!)</button>

                <button onClick={this.notWorkingClickHandler}>Not Working</button>
                <button onClick={this.usingBindClickHandler.bind(this)}>Use Bind(Works)</button>
                <button onClick={() => this.arrowFunctionClickHandler()}>Array method (Works)</button>
                <button onClick={ this.constructorClickHandler }>Constructor Bind(Works)</button>
                
                
                
                
                
                
            </div>
        );
    }
}

export default EventBind;