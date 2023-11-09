
* Javascript Export/Import
==========================

  The Export and Import are used in Jascript Module (the regular script included in HTML are not working on export/impact)

1 - Declare a module in HTML

    <script type="module">
  	import {sayHi} from './say.js';

	document.body.innerHTML = sayHi('John');
    </script>

2 - Export variable and function from the Javascript Module file

   Use the 'export' keyword in front of variable and functions to use in HTML (or other javascript files)
  export function sayHi(user) {
    alert(`Hello, ${user}!`);
  }



*NodeJS Export/Import
=====================

  The export-import are a bit different for NodeJS, as the 'require' keyword is used in the caller file
  (or theone who is importing as seen in the previous section Javascript Export/Import)

Export variable/string and function from a Javascript module

  a very great way to export in the file questionSets.js
  --> A string is exported  ( selectAllQuestionSets )
  --> 2 functions
  
  
  Usage:
  1 -  Import in the caller's js file
  const GiveAName = require( 'questionSets.js' );
  
  2 - Invoke methods or variable/string from the import key/name
  
  i.e:  GiveAName.createNewQuestionSet(newQuestionSet)
  i.e:  console.log(  GiveAName.selectAllQuestionSets );   --> Will display the string exported in questionSets.js
  
  
  Good site here that explain simply: https://www.tutorialsteacher.com/nodejs/nodejs-module-exports


