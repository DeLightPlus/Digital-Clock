#ifndef WEBSERVER_HTML_H
#define WEBSERVER_HTML_H

// HTML Dashboard for iGO-Buddy Web Interface
// Auto-generated from webserver/index.html
const char HTML_DASHBOARD[] = R"HTMLEOF(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>iGO-Buddy Control Panel</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 20px;
        }
        
        .container {
            background: white;
            border-radius: 15px;
            box-shadow: 0 20px 60px rgba(0, 0, 0, 0.3);
            max-width: 500px;
            width: 100%;
            padding: 30px;
        }
        
        .header {
            text-align: center;
            margin-bottom: 30px;
        }
        
        .header h1 {
            color: #667eea;
            font-size: 28px;
            margin-bottom: 10px;
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 10px;
        }
        
        .header p {
            color: #999;
            font-size: 14px;
        }
        
        .status-card {
            background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%);
            color: white;
            padding: 20px;
            border-radius: 10px;
            margin-bottom: 20px;
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 15px;
        }
        
        .status-item {
            text-align: center;
        }
        
        .status-value {
            font-size: 24px;
            font-weight: bold;
            margin-bottom: 5px;
        }
        
        .status-label {
            font-size: 12px;
            opacity: 0.9;
        }
        
        .event-card {
            background: #f8f9fa;
            border-left: 4px solid #667eea;
            padding: 20px;
            border-radius: 8px;
            margin-bottom: 20px;
        }
        
        .event-card h3 {
            color: #333;
            font-size: 14px;
            margin-bottom: 8px;
            text-transform: uppercase;
            opacity: 0.7;
        }
        
        .event-title {
            color: #667eea;
            font-size: 18px;
            font-weight: bold;
            margin-bottom: 5px;
        }
        
        .event-time {
            color: #999;
            font-size: 14px;
        }
        
        .controls {
            display: flex;
            flex-direction: column;
            gap: 10px;
        }
        
        button {
            padding: 12px 20px;
            font-size: 14px;
            font-weight: 600;
            border: none;
            border-radius: 8px;
            cursor: pointer;
            transition: all 0.3s ease;
            text-transform: uppercase;
            letter-spacing: 0.5px;
        }
        
        .btn-notify {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
        }
        
        .btn-notify:hover {
            transform: translateY(-2px);
            box-shadow: 0 10px 20px rgba(102, 126, 234, 0.3);
        }
        
        .btn-alarm {
            background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%);
            color: white;
        }
        
        .btn-alarm:hover {
            transform: translateY(-2px);
            box-shadow: 0 10px 20px rgba(245, 87, 108, 0.3);
        }
        
        .btn-refresh {
            background: linear-gradient(135deg, #4facfe 0%, #00f2fe 100%);
            color: white;
        }
        
        .btn-refresh:hover {
            transform: translateY(-2px);
            box-shadow: 0 10px 20px rgba(79, 172, 254, 0.3);
        }
        
        button:active {
            transform: translateY(0);
        }
        
        .footer {
            text-align: center;
            margin-top: 20px;
            font-size: 12px;
            color: #999;
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🎯 iGO-Buddy</h1>
            <p>Remote Control Panel</p>
        </div>
        
        <div class="status-card">
            <div class="status-item">
                <div class="status-value">85%</div>
                <div class="status-label">Battery</div>
            </div>
            <div class="status-item">
                <div class="status-value">Active</div>
                <div class="status-label">Status</div>
            </div>
        </div>
        
        <div class="event-card">
            <h3>📅 Next Event</h3>
            <div class="event-title">Team Meeting</div>
            <div class="event-time">15 minutes from now</div>
        </div>
        
        <div class="controls">
            <button class="btn-notify" onclick="sendNotify()">Send Notification</button>
            <button class="btn-alarm" onclick="sendAlarm()">Test Alarm</button>
            <button class="btn-refresh" onclick="location.reload()">Refresh Status</button>
        </div>
        
        <div class="footer">
            <p>Connected to: 192.168.4.1</p>
        </div>
    </div>
    
    <script>
        function sendNotify() {
            fetch('/api/notify')
                .then(response => response.text())
                .then(() => alert('Notification sent to TFT!'))
                .catch(error => alert('Error: ' + error));
        }
        
        function sendAlarm() {
            fetch('/api/alarm')
                .then(response => response.text())
                .then(() => alert('Alarm triggered on TFT!'))
                .catch(error => alert('Error: ' + error));
        }
        
        // Auto-refresh every 5 seconds
        setInterval(() => {
            console.log('Auto-refreshing status...');
        }, 5000);
    </script>
</body>
</html>
)HTMLEOF";

#endif // WEBSERVER_HTML_H
