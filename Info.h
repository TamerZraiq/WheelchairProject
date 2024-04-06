String information = F(R"=====(
<!DOCTYPE html>
<html lang="en">
<head>    
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Smart IoT Wheelchair - Information</title>
    <style>
        body {
            font-family: Arial, sans-serif;
          margin: 0;
          padding: 0;
          display: flex;
          flex-direction: column;
          align-items: center;
          width: 100vw;
          min-height: 100vh;
          background: linear-gradient(135deg, #3498db, #2ecc71);
          color: white;
        }
        .navbar {
          width: 100%;
          background-color: rgba(255, 255, 255, 0.1); 
          overflow: auto;
        }
        .navbar a {
          float: left;
          padding: 12px;
          color: white;
          text-decoration: none;
          font-size: 17px;
          width: 25%; 
          text-align: center;
        }
        .navbar a:hover {
          background-color: #34495e;
        }
        .content {
          flex-grow: 1;
          text-align: center;
          max-width: 800px;
          width: 100%;
          padding: 20px;
          box-sizing: border-box;
        }
        .information-section {
          background-color: rgba(0,0,0,0.1); /* Slightly transparent black */
          margin: 20px;
          padding: 20px;
          border-radius: 8px;
        }
    </style>
</head>
<body>
    <div class="navbar">
        <a href="homepage2.html">Home</a>
        <a href="data.html">Data</a>
        <a href="controls.html">Control</a>
        <a href="emergency.html">Emergency</a>
    </div>
    <div class="content">
        <header>
            <h1>Smart IoT Wheelchair - Information</h1>
        </header>
        <div class="information-section">
            <h2>Welcome to the Smart IoT Wheelchair Information Page</h2>
            <p>This page provides details and insights into the operation and features of the Smart IoT Wheelchair. Designed to enhance mobility and ensure safety, the Smart IoT Wheelchair is equipped with various features.</p>
            <p>Features include:</p>
            <ul>
                <li>Real-time data monitoring</li>
                <li>Remote control capabilities</li>
                <li>Emergency stop function</li>
                <li>Customizable seating and control settings</li>
            </ul>
            <p>The aim is to deliver an unmatched level of convenience and reliability for the users! Thank you for choosing the Smart IoT Wheelchair.</p>
        </div>
    </div>
</body>
</html>
)=====");