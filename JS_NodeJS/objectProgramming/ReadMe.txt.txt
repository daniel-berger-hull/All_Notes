Class Inheritence
=================

Javascript allow the creation of class, and also permits the inheritence.

class Passage {

   //Allows Constructor method
   constructor(id, worklocation, officerId) {
   	this.id = id;
   	this.worklocation = worklocation;
   	this.officerId = officerId;
   }
   
   // Permit methods
   save(){
   	console.log('Passage #' + this.id + ' at Work location #' + this.worklocation);
   }
}

// And then instanciate and use the class defined above...
const pearsonMarch20 = new Passage(1,4971,'dxb160');
pearsonMarch20.save();



Data Encapsulation (Private Field) & private method
===================================================

  As C++ and Java, a Javaascript class can have private fields and method.
  
  Example:
	  class ClassWithPrivateField {
	    #privateField;

	    constructor() {
	      this.#privateField = 42;
	      delete this.#privateField; // Syntax error
	      this.#undeclaredField = 444; // Syntax error
	    }
	  }

	  const instance = new ClassWithPrivateField();
	  instance.#privateField === 42; // Syntax error


  See following web site:  https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Classes/Private_class_fields


Static Fields & Methods
=======================

  A class can have also static fields and Methods
  
  
  class Car {
  
  
      static modelID = 1234;
  
      constructor() {
      }
  
      toString() {
          return    "The ID of all cars is " + Car.modelID;    //Not use the class name here, and not the 'this' keyword
       
      }
    
      static restartModelIDSequence() {
          console.log("restartModelIDSequence");
          Car.modelID = 0;
      }
  
  };
  
  ...
    let myCar =  new Car();
    console.log(myCar.toString());		// Will display the 'The ID of all cars is 1234'
    Car.restartModelIDSequence();
    console.log(myCar.toString());		// Will display the 'The ID of all cars is 0'
  
