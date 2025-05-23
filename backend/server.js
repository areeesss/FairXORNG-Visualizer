const express = require('express');
const cors = require('cors');
const http = require('http');
const WebSocket = require('ws');
const path = require('path');

// Load the native addon
let fairxorng;
try {
    fairxorng = require('../build/Release/fairxorng');
} catch (error) {
    console.error('Failed to load fairxorng native addon:', error);
    process.exit(1);
}

const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

// Middleware
app.use(cors());
app.use(express.json());

// Serve frontend static files only in production
if (process.env.NODE_ENV === 'production') {
    app.use(express.static(path.join(__dirname, '../frontend/build')));
}

// WebSocket connection handling
wss.on('connection', (ws) => {
    console.log('Client connected');
    
    ws.on('message', (message) => {
        try {
            const data = JSON.parse(message);
            
            if (data.type === 'runTrial') {
                // Run the trial using the native addon
                const result = fairxorng.evaluate(data.classes);
                
                // Send the result back to the client
                ws.send(JSON.stringify({
                    type: 'trialResult',
                    result: result
                }));
            }
        } catch (error) {
            console.error('Error processing message:', error);
            ws.send(JSON.stringify({
                type: 'error',
                error: error.message
            }));
        }
    });

    ws.on('close', () => {
        console.log('Client disconnected');
    });
});

// API endpoints
app.get('/api/health', (req, res) => {
    res.json({ status: 'ok' });
});

// Serve the React app for any other routes only in production
if (process.env.NODE_ENV === 'production') {
    app.get('*', (req, res) => {
        res.sendFile(path.join(__dirname, '../frontend/build/index.html'));
    });
}

const PORT = process.env.PORT || 3001;
server.listen(PORT, () => {
    console.log(`Server running on port ${PORT}`);
}); 