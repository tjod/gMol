const express = require('express');
const app = express();

// Serve all static files from the 'public' folder
app.use(express.static('public')); 

app.listen(3000, () => console.log('Server running on port 3000'));
