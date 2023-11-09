
// this example reads a text file, in on pass, and displays it to the console

const fs = require('fs');

// Note the utf8 encoding here, to have the file content is ascii, ready to output to a text console
fs.readFile('textfile.txt', { encoding: 'utf8' }, (err, content) => {
// If an error occurred, output it and return
if(err) return console.error(err);
// No error occurred, content is a string
console.log(content);
});