//  This example load a simple text file, and leverage the line functions
//  int he readline package to scan, line by line and display the content...

const fs = require('fs');
const readline = require('readline');

async function processLineByLine() {

     var myInterface = readline.createInterface({
           input: fs.createReadStream( 'textfile.txt' )
         });


         myInterface.on('line', function (line) {
           console.log(line);
      });

}

processLineByLine();