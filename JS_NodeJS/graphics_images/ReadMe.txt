VArious libraries appears to exist to handle the image processing


Here is a video that explain a way using GraphicsMagick & Express-processimage

   https://www.youtube.com/watch?v=pbIbVKknLio
 
 Installation
 -------------
 	 GraphicsMagick       (http://www.graphicsmagick.org/download.html)
  	 Express-processimage (npm install -S express-processimage)

   
   
   GM:
   https://aheckmann.github.io/gm/docs.html#manipulation
   
   Status (March 21, 2020):
      Tried to use gm, but it always end up with an error
      
      const gm  = require('gm');
          
      gm('./Node.png')
         .resize(200,100)
         .autoOrient()
         .write('./output.png',   
         		 function(err) {
         		 	if (err) console.log('An error happen!');
         		 	else console.log('Fonction succeed!');
               }
      
   	);
   