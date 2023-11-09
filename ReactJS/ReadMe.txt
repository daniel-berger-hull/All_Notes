1 - UDEMY COURSE #1 (Mexican guy)
=====================================

1.1 CREATE A NEW PROJECT (create-react-app)
-------------------------------------------

This is a project from Facebook, that helps to create ReactJS project from scratch.

  	npx create-react-app <future-Project-name>
   	i.e: npx create-react-app portfolio
   	
   	
1.2 Start and test a project
------------------------------
	 
	   Use the following command, from the root of a ReactJS project (where the package.json and other files are)
	   
       npm run start 
   	
  
#2 - UDEMY COURSE #2 (Maximillian ReactJS )
=====================================


CREATE A NEW PROJECT (create-react-app)
---------------------------------------

   the uDemy course of Maximillian suggest an approach that not only creates a ReactJS
   project, but also create all other things to support the project (like a Web Server,
   and other things)
   
   Note: if not already installed on your machine, run: npm install create-react-app -g
          (-g is to make sure that it is installed to be accessible on your system later, not just for 
          a single time)

     create-react-app <Your_Project_Name> --scripts-version 1.1.5

 	

 
 
 
  Start and test a project
 --------------------------
  npm start
     Starts the development server.
 
   npm run build
     Bundles the app into static files for production.
 
   npm test
     Starts the test runner.
 
   npm run eject
     Removes this tool and copies build dependencies, configuration files
     and scripts into the app directory. If you do this, you can’t go back!
 
 We suggest that you begin by typing:
 
   cd reactjs-test
  npm start
 
 
 
 ReactJS States
 -----------------
 
    Ways to have Global variables/methods, and have the application react to it.
    
     Steps:
     1 - Declare the variables in the class constructor as a JSON pair (state is part of the class oblect)
     2 - (Important) Declare also getter/setter methods who will interface with global variable (in the constructor)
     
       constructor(){
          super();
	  // Declaring a state here named 'detailsVisible'
          this.state = { detailsVisible: false };     
          // Declare a getter/setter (a toggler here) that will inteface with 'detailsVisible'
          this.toggleDetails = this.toggleDetails.bind(this);
     }
     
     3 - User the setState method on the state
   
      Should not reassign  the value of a state directly (like a variable i.e:  detailsVisible = false)
      Use a getter/setting (declared in step #2)
      
      // Exemple of a setter method
      showDetails(){
           
           this.setState( { detailsVisible: true } );
      }
     
     Check uDemy ReactJS , chap 12 & 13