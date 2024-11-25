const express = require('express');
const nocache = require('nocache');
const app = express()
const port = 3000


app.use(nocache());
app.use(express.static('./'));
app.use(express.static('public'));


app.listen(port, () => {
  console.log(`Example app listening on port ${port}`)
})