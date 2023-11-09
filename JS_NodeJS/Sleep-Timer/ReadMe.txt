Sleep
=====

See:  https://medium.com/dev-genius/how-to-make-javascript-sleep-or-wait-d95d33c99909

There appears to have NO Sleep method in Javascript. The only way around is to write a method 
returning a Promise, and invoke it:


	const sleep = (delay) => new Promise((resolve) => setTimeout(resolve, delay))
	
	// Note: The method containing a piece of async code i.e an await keyword
	//       must be labeled as async...
	const exampleOfMethod = async () => {
	
		//the await is important here, because a Promise is returned by sleep method
		await sleep(1000);   // Will wait for a second
	}