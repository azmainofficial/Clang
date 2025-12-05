#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// Joystick values
int LX = 0, LY = 0;
int JRX = 0, JRY = 0;

// ------------- HTML PAGE ----------------
const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>AzmainOfficial Dual Joystick</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <style>
        body {
            margin: 0;
            padding: 0;
            overflow: hidden;
            background: #0a0a0a;
            color: #0f0;
            font-family: Arial, sans-serif;
            height: 100vh;
            width: 100vw;
            position: fixed;
            touch-action: none;
        }

        .container {
            width: 100%;
            height: 100%;
            position: relative;
        }

        .joystick-container {
            position: absolute;
            width: 200px;
            height: 200px;
            border-radius: 50%;
            background: rgba(0, 255, 0, 0.1);
            border: 2px solid #0f0;
            touch-action: none;
            user-select: none;
        }

        .joystick-handle {
            position: absolute;
            width: 80px;
            height: 80px;
            background: #0f0;
            border-radius: 50%;
            top: 60px;
            left: 60px;
            transition: transform 0.1s;
            touch-action: none;
        }

        #left-joystick {
            bottom: 30px;
            left: 30px;
        }

        #right-joystick {
            bottom: 30px;
            right: 30px;
        }

        .info-panel {
            position: absolute;
            top: 20px;
            left: 50%;
            transform: translateX(-50%);
            text-align: center;
            background: rgba(0, 0, 0, 0.7);
            padding: 15px;
            border-radius: 10px;
            border: 1px solid #0f0;
            z-index: 100;
        }

        .info-title {
            font-size: 24px;
            margin-bottom: 10px;
            color: #0f0;
        }

        .info-item {
            margin: 5px 0;
            font-size: 16px;
        }

        .label {
            color: #8f8;
        }

        .value {
            color: #0f0;
            font-weight: bold;
        }

        .bars-container {
            position: absolute;
            top: 120px;
            left: 50%;
            transform: translateX(-50%);
            width: 300px;
            background: rgba(0, 0, 0, 0.7);
            padding: 15px;
            border-radius: 10px;
            border: 1px solid #0f0;
            z-index: 100;
        }

        .bar-item {
            margin: 10px 0;
        }

        .bar-label {
            display: block;
            margin-bottom: 5px;
            color: #8f8;
        }

        .bar-outer {
            width: 100%;
            height: 20px;
            background: #222;
            border-radius: 10px;
            border: 1px solid #0f0;
            overflow: hidden;
        }

        .bar-inner {
            height: 100%;
            width: 0%;
            border-radius: 10px;
            background: linear-gradient(to right, #004400, #00ff00);
            transition: width 0.2s;
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="info-panel">
            <div class="info-title">AzmainOfficial Controller</div>
            <div class="info-item">
                <span class="label">Status:</span>
                <span id="status" class="value">Disconnected</span>
            </div>
            <div class="info-item">
                <span class="label">IP:</span>
                <span id="ip" class="value">192.168.4.1</span>
            </div>
        </div>

        <div class="bars-container">
            <div class="bar-item">
                <span class="bar-label">Left Joystick Power:</span>
                <div class="bar-outer">
                    <div id="left-bar" class="bar-inner"></div>
                </div>
                <div style="font-size: 14px; color: #0f0; margin-top: 5px;">
                    LX: <span id="lx-value">0</span> | LY: <span id="ly-value">0</span>
                </div>
            </div>
            <div class="bar-item">
                <span class="bar-label">Right Joystick Power:</span>
                <div class="bar-outer">
                    <div id="right-bar" class="bar-inner"></div>
                </div>
                <div style="font-size: 14px; color: #0f0; margin-top: 5px;">
                    JRX: <span id="rx-value">0</span> | JRY: <span id="ry-value">0</span>
                </div>
            </div>
        </div>

        <div id="left-joystick" class="joystick-container">
            <div id="left-handle" class="joystick-handle"></div>
        </div>
        <div id="right-joystick" class="joystick-container">
            <div id="right-handle" class="joystick-handle"></div>
        </div>
    </div>

    <script>
        // WebSocket connection
        var connection = new WebSocket('ws://' + window.location.hostname + ':81/');
        
        connection.onopen = function () {
            console.log('WebSocket Connected');
            document.getElementById('status').textContent = 'Connected';
            document.getElementById('status').style.color = '#0f0';
        };
        
        connection.onerror = function (error) {
            console.log('WebSocket Error:', error);
            document.getElementById('status').textContent = 'Error';
            document.getElementById('status').style.color = '#f00';
        };
        
        connection.onclose = function () {
            console.log('WebSocket Disconnected');
            document.getElementById('status').textContent = 'Disconnected';
            document.getElementById('status').style.color = '#f00';
        };
        
        function sendJoystickData(side, x, y) {
            if (connection.readyState === WebSocket.OPEN) {
                var data = side + "," + x + "," + y;
                connection.send(data);
                console.log("Sent " + side + ":", x, y);
            }
        }

        // Initialize when page loads
        window.addEventListener('load', function() {
            // Update IP display with actual IP
            var hostname = window.location.hostname || "192.168.4.1";
            document.getElementById('ip').textContent = hostname;
            
            // Setup joysticks with isolated event handling
            setupJoystick('left');
            setupJoystick('right');
        });

        function setupJoystick(side) {
            var container = document.getElementById(side + '-joystick');
            var handle = document.getElementById(side + '-handle');
            var bar = document.getElementById(side + '-bar');
            var xValue = document.getElementById(side.substring(0,1) + 'x-value');
            var yValue = document.getElementById(side.substring(0,1) + 'y-value');
            
            var isActive = false;
            var maxDistance = 60;
            var activeTouchId = null; // For multi-touch support
            
            function updatePosition(clientX, clientY) {
                if (!isActive) return;
                
                var rect = container.getBoundingClientRect();
                var x = clientX - rect.left - rect.width / 2;
                var y = clientY - rect.top - rect.height / 2;
                
                var distance = Math.sqrt(x * x + y * y);
                
                if (distance > maxDistance) {
                    x = (x / distance) * maxDistance;
                    y = (y / distance) * maxDistance;
                    distance = maxDistance;
                }
                
                handle.style.transform = 'translate(' + x + 'px, ' + y + 'px)';
                
                var normX = Math.round((x / maxDistance) * 100);
                var normY = Math.round((-y / maxDistance) * 100);
                
                var power = Math.round((distance / maxDistance) * 100);
                bar.style.width = power + '%';
                
                xValue.textContent = normX;
                yValue.textContent = normY;
                
                sendJoystickData(side.substring(0,1).toUpperCase(), normX, normY);
            }
            
            function resetPosition() {
                handle.style.transform = 'translate(0px, 0px)';
                bar.style.width = '0%';
                xValue.textContent = '0';
                yValue.textContent = '0';
                
                if (connection.readyState === WebSocket.OPEN) {
                    sendJoystickData(side.substring(0,1).toUpperCase(), 0, 0);
                }
            }
            
            // === MOUSE EVENTS (for desktop) ===
            container.addEventListener('mousedown', function(e) {
                e.preventDefault();
                e.stopPropagation(); // Prevent event from bubbling up
                isActive = true;
                updatePosition(e.clientX, e.clientY);
            });
            
            // Attach mousemove to the container instead of document
            container.addEventListener('mousemove', function(e) {
                if (!isActive) return;
                e.preventDefault();
                e.stopPropagation();
                updatePosition(e.clientX, e.clientY);
            });
            
            // Use container-specific mouseup
            container.addEventListener('mouseup', function(e) {
                if (isActive) {
                    e.preventDefault();
                    e.stopPropagation();
                    isActive = false;
                    resetPosition();
                }
            });
            
            // Handle mouse leaving the container
            container.addEventListener('mouseleave', function(e) {
                if (isActive) {
                    isActive = false;
                    resetPosition();
                }
            });
            
            // === TOUCH EVENTS (for mobile - with multi-touch support) ===
            container.addEventListener('touchstart', function(e) {
                if (isActive) return; // Already active
                
                e.preventDefault();
                e.stopPropagation();
                
                var touch = e.changedTouches[0];
                activeTouchId = touch.identifier;
                isActive = true;
                updatePosition(touch.clientX, touch.clientY);
            }, { passive: false });
            
            container.addEventListener('touchmove', function(e) {
                if (!isActive) return;
                
                e.preventDefault();
                e.stopPropagation();
                
                // Find the correct touch by ID
                for (var i = 0; i < e.changedTouches.length; i++) {
                    if (e.changedTouches[i].identifier === activeTouchId) {
                        var touch = e.changedTouches[i];
                        updatePosition(touch.clientX, touch.clientY);
                        break;
                    }
                }
            }, { passive: false });
            
            container.addEventListener('touchend', function(e) {
                if (!isActive) return;
                
                // Check if our active touch ended
                for (var i = 0; i < e.changedTouches.length; i++) {
                    if (e.changedTouches[i].identifier === activeTouchId) {
                        e.preventDefault();
                        e.stopPropagation();
                        isActive = false;
                        activeTouchId = null;
                        resetPosition();
                        break;
                    }
                }
            }, { passive: false });
            
            container.addEventListener('touchcancel', function(e) {
                if (!isActive) return;
                
                for (var i = 0; i < e.changedTouches.length; i++) {
                    if (e.changedTouches[i].identifier === activeTouchId) {
                        isActive = false;
                        activeTouchId = null;
                        resetPosition();
                        break;
                    }
                }
            }, { passive: false });
            
            // Prevent context menu
            container.addEventListener('contextmenu', function(e) {
                e.preventDefault();
                return false;
            });
        }

        window.addEventListener('resize', function() {
            document.getElementById('left-handle').style.transform = 'translate(0px, 0px)';
            document.getElementById('right-handle').style.transform = 'translate(0px, 0px)';
        });
    </script>
</body>
</html>
)rawliteral";

// ------------------ SERVER HANDLERS ------------------------
void handleRoot() {
  server.send_P(200, "text/html", webpage);
}

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  if (type == WStype_TEXT) {
    String data = String((char*)payload);
    
    // Parse data in format: "L,100,-50" or "R,75,25"
    int firstComma = data.indexOf(',');
    int secondComma = data.indexOf(',', firstComma + 1);
    
    if (firstComma > 0 && secondComma > firstComma) {
      char id = data.charAt(0);
      int x = data.substring(firstComma + 1, secondComma).toInt();
      int y = data.substring(secondComma + 1).toInt();
      
      if (id == 'L') {
        LX = x;
        LY = y;
        Serial.print("LEFT: ");
      } else if (id == 'R') {
        JRX = x;
        JRY = y;
        Serial.print("RIGHT: ");
      }
      
      Serial.print("X=");
      Serial.print(x);
      Serial.print(" Y=");
      Serial.println(y);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Configure WiFi as Access Point
  WiFi.mode(WIFI_AP);
  
  // Set up AP with specific configuration
  IPAddress local_ip(192, 168, 4, 1);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);
  
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP("Ecchi-Bot", "loveForAzmian99+");
  
  Serial.println("AP Started: SoccerBot");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
  
  // Set up web server
  server.on("/", handleRoot);
  server.begin();
  
  // Set up WebSocket server
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
  
  Serial.println("Setup complete. Ready for connections.");
}

void loop() {
  server.handleClient();
  webSocket.loop();
}
