#include <iostream>
#include <string>
#include <sstream>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#pragma comment(lib, "ws2_32.lib")

// ================= BACKEND: C++ =================

struct Point {
    int x, y;
};

// Original triangle
Point triangle[3] = {
    {2, 1},
    {5, 1},
    {3, 4}
};

// Perform reflection
Point reflect(Point p, string axis) {
    Point r;

    if (axis == "x") {
        r = {p.x, -p.y};
    }
    else if (axis == "y") {
        r = {-p.x, p.y};
    }
    else if (axis == "origin") {
        r = {-p.x, -p.y};
    }
    else if (axis == "yx") {
        r = {p.y, p.x};
    }
    else if (axis == "negative") {
        r = {-p.y, -p.x};
    }
    else {
        r = p;
    }

    return r;
}

// Generate JSON response from C++ backend
string getReflectionData(string axis) {
    stringstream json;

    json << "{\"axis\":\"" << axis << "\",\"points\":[";

    for (int i = 0; i < 3; i++) {
        Point p = reflect(triangle[i], axis);

        if (i > 0)
            json << ",";

        json << "{\"x\":" << p.x
             << ",\"y\":" << p.y << "}";
    }

    json << "]}";

    return json.str();
}

// ================= FRONTEND: HTML + CSS + JS =================

string webpage = R"HTML(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<title>2D Reflection Simulator</title>

<style>
* {
    box-sizing: border-box;
}

body {
    margin: 0;
    font-family: Arial, sans-serif;
    background: #0b1020;
    color: #edf2ff;
}

header {
    padding: 25px 8%;
    border-bottom: 1px solid #26324d;
    background: #10182b;
}

header h1 {
    margin: 0;
    color: #72e5ff;
    font-size: 28px;
}

header p {
    color: #9eabc5;
    margin-bottom: 0;
}

.container {
    width: 90%;
    max-width: 1150px;
    margin: 30px auto;
}

.layout {
    display: grid;
    grid-template-columns: 300px 1fr;
    gap: 22px;
}

.card {
    background: #121b30;
    border: 1px solid #293650;
    border-radius: 15px;
    padding: 22px;
    margin-bottom: 20px;
}

h2 {
    font-size: 18px;
    margin-top: 0;
    color: #ffffff;
}

label {
    display: block;
    margin: 20px 0 8px;
    color: #aebbd3;
    font-size: 14px;
}

select, button {
    width: 100%;
    padding: 12px;
    border-radius: 8px;
    border: 1px solid #344461;
    background: #0b1222;
    color: white;
    font-size: 14px;
}

button {
    margin-top: 15px;
    background: #176d91;
    border: none;
    cursor: pointer;
    font-weight: bold;
}

button:hover {
    background: #218db6;
}

.stats {
    display: grid;
    grid-template-columns: repeat(3, 1fr);
    gap: 12px;
    margin-bottom: 20px;
}

.stat {
    padding: 16px;
    border-radius: 10px;
    background: #17233b;
    border: 1px solid #293650;
}

.stat span {
    display: block;
    color: #9eabc5;
    font-size: 12px;
    margin-bottom: 8px;
}

.stat strong {
    font-size: 19px;
    color: #72e5ff;
}

canvas {
    width: 100%;
    height: 400px;
    background: #0c1425;
    border-radius: 10px;
    display: block;
}

.legend {
    display: flex;
    gap: 20px;
    margin-top: 15px;
    font-size: 13px;
    color: #b9c5db;
}

.dot {
    display: inline-block;
    width: 10px;
    height: 10px;
    border-radius: 50%;
    margin-right: 6px;
}

.original {
    background: #72e5ff;
}

.reflected {
    background: #ff83b5;
}

table {
    width: 100%;
    border-collapse: collapse;
    font-size: 14px;
}

th, td {
    padding: 12px 8px;
    text-align: center;
    border-bottom: 1px solid #293650;
}

th {
    color: #72e5ff;
}

.muted {
    color: #9eabc5;
    line-height: 1.7;
    font-size: 14px;
}

footer {
    text-align: center;
    padding: 25px;
    color: #7786a4;
    font-size: 13px;
}

@media (max-width: 750px) {
    .layout {
        grid-template-columns: 1fr;
    }

    .stats {
        grid-template-columns: 1fr;
    }

    canvas {
        height: 320px;
    }
}
</style>
</head>

<body>

<header>
    <h1>2D Reflection Simulator</h1>
    <p>Explore geometric transformations on the Cartesian plane.</p>
</header>

<div class="container">

<div class="layout">

    <div>
        <div class="card">
            <h2>Transformation Controls</h2>

            <p class="muted">
                Select a reflection axis to transform the triangle.
                The C++ backend calculates the new coordinates.
            </p>

            <label for="axis">Choose Reflection</label>

            <select id="axis">
                <option value="x">Reflection across X-axis</option>
                <option value="y">Reflection across Y-axis</option>
                <option value="origin">Reflection across Origin</option>
                <option value="yx">Reflection across y = x</option>
                <option value="negative">Reflection across y = -x</option>
            </select>

            <button onclick="updateReflection()">
                Apply Reflection
            </button>

            <button onclick="resetView()">
                Reset Original Triangle
            </button>
        </div>

        <div class="card">
            <h2>Reflection Formula</h2>

            <p class="muted" id="formula">
                X-axis: (x, y) → (x, -y)
            </p>

            <h2>How it works</h2>

            <p class="muted">
                1. Select a reflection axis.<br><br>
                2. The webpage sends your selection to the C++ backend.<br><br>
                3. C++ calculates the reflected coordinates.<br><br>
                4. The webpage displays the transformed triangle.
            </p>
        </div>
    </div>

    <div>
        <div class="stats">
            <div class="stat">
                <span>Shape</span>
                <strong>Triangle</strong>
            </div>

            <div class="stat">
                <span>Vertices</span>
                <strong>3</strong>
            </div>

            <div class="stat">
                <span>Transformation</span>
                <strong id="status">X-axis</strong>
            </div>
        </div>

        <div class="card">
            <h2>Cartesian Coordinate Plane</h2>

            <canvas id="graph"></canvas>

            <div class="legend">
                <span>
                    <i class="dot original"></i>Original triangle
                </span>

                <span>
                    <i class="dot reflected"></i>Reflected triangle
                </span>
            </div>
        </div>

        <div class="card">
            <h2>Coordinate Results</h2>

            <table>
                <thead>
                    <tr>
                        <th>Vertex</th>
                        <th>Original</th>
                        <th>Reflected</th>
                    </tr>
                </thead>

                <tbody id="results"></tbody>
            </table>
        </div>
    </div>

</div>
</div>

<footer>
    2D Reflection Simulator | C++ Backend + Web Frontend
</footer>

<script>
const original = [
    {x: 2, y: 1},
    {x: 5, y: 1},
    {x: 3, y: 4}
];

let reflected = [];
let currentAxis = "x";

const formulas = {
    x: "X-axis: (x, y) → (x, -y)",
    y: "Y-axis: (x, y) → (-x, y)",
    origin: "Origin: (x, y) → (-x, -y)",
    yx: "y = x: (x, y) → (y, x)",
    negative: "y = -x: (x, y) → (-y, -x)"
};

const names = {
    x: "X-axis",
    y: "Y-axis",
    origin: "Origin",
    yx: "y = x",
    negative: "y = -x"
};

async function updateReflection() {
    currentAxis = document.getElementById("axis").value;

    document.getElementById("status").textContent =
        names[currentAxis];

    document.getElementById("formula").textContent =
        formulas[currentAxis];

    try {
        // Request data from the C++ backend
        const response = await fetch(
            "/api/reflect?axis=" + currentAxis
        );

        if (!response.ok) {
            throw new Error("Backend request failed");
        }

        const data = await response.json();

        reflected = data.points;

        showResults();
        drawGraph();

    } catch (error) {
        alert("Cannot connect to the C++ backend. Restart the program.");
        console.error(error);
    }
}

function resetView() {
    document.getElementById("axis").value = "x";
    updateReflection();
}

function showResults() {
    const table = document.getElementById("results");
    table.innerHTML = "";

    for (let i = 0; i < original.length; i++) {
        const row = document.createElement("tr");

        row.innerHTML =
            "<td>" + String.fromCharCode(65 + i) + "</td>" +
            "<td>(" + original[i].x + ", " +
                      original[i].y + ")</td>" +
            "<td>(" + reflected[i].x + ", " +
                      reflected[i].y + ")</td>";

        table.appendChild(row);
    }
}

function drawGraph() {
    const canvas = document.getElementById("graph");
    const rect = canvas.getBoundingClientRect();

    const dpr = window.devicePixelRatio || 1;

    canvas.width = rect.width * dpr;
    canvas.height = rect.height * dpr;

    const ctx = canvas.getContext("2d");
    ctx.scale(dpr, dpr);

    const width = rect.width;
    const height = rect.height;

    ctx.clearRect(0, 0, width, height);

    const unit = Math.min(width / 14, height / 12);
    const ox = width / 2;
    const oy = height / 2;

    function px(x) {
        return ox + x * unit;
    }

    function py(y) {
        return oy - y * unit;
    }

    // Grid
    ctx.strokeStyle = "#202d45";
    ctx.lineWidth = 1;

    for (let x = -10; x <= 10; x++) {
        ctx.beginPath();
        ctx.moveTo(px(x), 0);
        ctx.lineTo(px(x), height);
        ctx.stroke();
    }

    for (let y = -10; y <= 10; y++) {
        ctx.beginPath();
        ctx.moveTo(0, py(y));
        ctx.lineTo(width, py(y));
        ctx.stroke();
    }

    // Coordinate axes
    ctx.strokeStyle = "#899bb9";
    ctx.lineWidth = 1.5;

    ctx.beginPath();
    ctx.moveTo(0, oy);
    ctx.lineTo(width, oy);
    ctx.stroke();

    ctx.beginPath();
    ctx.moveTo(ox, 0);
    ctx.lineTo(ox, height);
    ctx.stroke();

    // Axis labels
    ctx.fillStyle = "#b9c5db";
    ctx.font = "12px Arial";

    ctx.fillText("X", width - 17, oy - 8);
    ctx.fillText("Y", ox + 8, 15);

    // Draw reflection line
    ctx.strokeStyle = "#526684";
    ctx.setLineDash([6, 6]);

    ctx.beginPath();

    if (currentAxis === "x") {
        ctx.moveTo(0, oy);
        ctx.lineTo(width, oy);
    } else if (currentAxis === "y") {
        ctx.moveTo(ox, 0);
        ctx.lineTo(ox, height);
    } else if (currentAxis === "yx") {
        ctx.moveTo(px(-8), py(-8));
        ctx.lineTo(px(8), py(8));
    } else if (currentAxis === "negative") {
        ctx.moveTo(px(-8), py(8));
        ctx.lineTo(px(8), py(-8));
    } else {
        ctx.arc(ox, oy, 5, 0, Math.PI * 2);
    }

    ctx.stroke();
    ctx.setLineDash([]);

    function drawTriangle(points, color) {
        ctx.beginPath();

        points.forEach((p, i) => {
            if (i === 0) {
                ctx.moveTo(px(p.x), py(p.y));
            } else {
                ctx.lineTo(px(p.x), py(p.y));
            }
        });

        ctx.closePath();

        ctx.fillStyle = color + "22";
        ctx.fill();

        ctx.strokeStyle = color;
        ctx.lineWidth = 2.5;
        ctx.stroke();

        points.forEach((p, i) => {
            ctx.beginPath();
            ctx.arc(px(p.x), py(p.y), 4, 0, Math.PI * 2);

            ctx.fillStyle = color;
            ctx.fill();

            ctx.fillText(
                String.fromCharCode(65 + i) +
                "(" + p.x + "," + p.y + ")",
                px(p.x) + 7,
                py(p.y) - 8
            );
        });
    }

    drawTriangle(original, "#72e5ff");
    drawTriangle(reflected, "#ff83b5");
}

window.addEventListener("resize", drawGraph);

updateReflection();
</script>

</body>
</html>
)HTML";

// ================= WEB SERVER =================

string httpResponse(string content, string type) {
    stringstream response;

    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: " << type << "\r\n";
    response << "Access-Control-Allow-Origin: *\r\n";
    response << "Content-Length: " << content.size() << "\r\n";
    response << "Connection: close\r\n\r\n";
    response << content;

    return response.str();
}

int main() {
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        cout << "Winsock initialization failed.\n";
        return 1;
    }

    SOCKET serverSocket = socket(
        AF_INET, SOCK_STREAM, IPPROTO_TCP
    );

    if (serverSocket == INVALID_SOCKET) {
        cout << "Could not create server socket.\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    serverAddress.sin_port = htons(8080);

    if (bind(
        serverSocket,
        (sockaddr*)&serverAddress,
        sizeof(serverAddress)
    ) == SOCKET_ERROR) {
        cout << "Could not use port 8080. Close any other "
             << "program using this port and try again.\n";

        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cout << "Server could not start listening.\n";

        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "2D Reflection Simulator is running!\n";
    cout << "Website: http://127.0.0.1:8080\n";
    cout << "Keep this terminal open while using the website.\n";
    cout << "Press Ctrl+C to stop the server.\n";

    // Open the frontend in the browser
    system("start \"\" \"http://127.0.0.1:8080\"");

    while (true) {
        SOCKET clientSocket = accept(
            serverSocket, nullptr, nullptr
        );

        if (clientSocket == INVALID_SOCKET)
            continue;

        char buffer[8192] = {0};

        int received = recv(
            clientSocket, buffer,
            sizeof(buffer) - 1, 0
        );

        if (received > 0) {
            string request(buffer, received);
            string body;
            string contentType = "text/html; charset=utf-8";

            if (request.find("GET /api/reflect?axis=") == 0) {
                size_t start = request.find("axis=") + 5;
                size_t end = request.find_first_of(" &\r\n", start);

                string axis = request.substr(
                    start,
                    end == string::npos
                        ? string::npos
                        : end - start
                );

                body = getReflectionData(axis);
                contentType = "application/json; charset=utf-8";
            }
            else {
                body = webpage;
            }

            string response = httpResponse(body, contentType);

            const char* data = response.c_str();
            int remaining = static_cast<int>(response.size());

            while (remaining > 0) {
                int sent = send(
                    clientSocket, data, remaining, 0
                );

                if (sent <= 0)
                    break;

                data += sent;
                remaining -= sent;
            }
        }

        closesocket(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}