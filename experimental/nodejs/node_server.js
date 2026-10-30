const http = require('http');
const fs = require('fs');
const path = require('path');

http.createServer((req, res) => {
    // Manually routing "/" to a specific file location
    if (req.url === '/') {
        //const filePath = path.join(__dirname, 'public', 'index.html');
        const filePath = path.join(__dirname, 'public', '');
        fs.readFile(filePath, (err, content) => {
            res.writeHead(200, { 'Content-Type': 'text/html' });
            res.end(content);
        });
    }
}).listen(3000);
