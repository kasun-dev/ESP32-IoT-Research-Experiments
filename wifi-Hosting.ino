#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Xperia 1";        // Replace with your Wi-Fi name
const char* password = "0760853136"; // Replace with your Wi-Fi password

WebServer server(80); // Create a web server on port 80

// HTML content with an image, description, and buttons
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 Web Server</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            background-color: #f4f4f4;
            padding: 20px;
        }
        .container {
            max-width: 700px;
            margin: auto;
            background: white;
            padding: 20px;
            box-shadow: 0px 0px 15px rgba(0, 0, 0, 0.1);
            border-radius: 12px;
        }
        h1 {
            color: #007BFF;
        }
        h2 {
            color: #333;
        }
        p {
            color: #555;
            line-height: 1.6;
        }
        img {
            max-width: 100%;
            height: auto;
            border-radius: 10px;
        }
        .btn {
            display: inline-block;
            margin: 10px;
            padding: 12px 20px;
            font-size: 16px;
            font-weight: bold;
            text-decoration: none;
            color: white;
            background-color: #007BFF;
            border-radius: 8px;
            transition: 0.3s;
        }
        .btn:hover {
            background-color: #0056b3;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Welcome to ESP32 Web Server!</h1>
        <img src="https://img.freepik.com/free-vector/illustration-circuit_53876-5583.jpg?t=st=1740318210~exp=1740321810~hmac=edbcc8c99a2e55db2a58d5d0207a09ace3bd26b732d1e1218df1a065e841f12f&w=900" alt="ESP32 Circuit">
        
        <h2>What is ESP32?</h2>
        <p>The ESP32 is a powerful microcontroller with built-in Wi-Fi and Bluetooth, designed for IoT applications. It is widely used in smart home devices, automation systems, robotics, and more.</p>
        
        <h2>Key Features</h2>
        <p>✔️ Dual-core processor<br>
           ✔️ Wi-Fi & Bluetooth connectivity<br>
           ✔️ Low power consumption<br>
           ✔️ Multiple GPIOs for sensors & actuators<br>
           ✔️ Ideal for IoT and embedded projects</p>

        <h2>Learn More & Explore</h2>
        <a class="btn" href="https://kasun-dev.github.io/portfolio-web/" target="_blank">Visit My Portfolio</a>
        <a class="btn" href="https://www.espressif.com/en/products/socs/esp32" target="_blank">ESP32 Official Page</a>

        <p>Kasun Jayamaha © </p>
    </div>
</body>
</html>
)rawliteral";

// Function to handle root page request
void handleRoot() {
    server.send(200, "text/html", htmlPage);
}

void setup() {
    Serial.begin(115200);
    
    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wi-Fi...");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    
    Serial.println("\nConnected!");
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP()); // Print IP Address

    // Define server routes
    server.on("/", handleRoot);

    // Start the server
    server.begin();
}

void loop() {
    server.handleClient();
}
