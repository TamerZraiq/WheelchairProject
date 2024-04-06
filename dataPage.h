String datapage = F(R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta http-equiv="refresh" content="5">
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>User Data</title>
  
   <style>
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 0;
      display: flex;
      flex-direction: column; /* Change to column for navbar */
      justify-content: center;
      align-items: center;
      min-height: 100vh;
      background: linear-gradient(135deg, #3498db, #2ecc71); /* Gradient from light blue to green */
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
      text-align: center;
      max-width: 800px;
      padding: 20px;
    }

    header {
      margin-bottom: 20px;
    }

    h1 {
      margin-bottom: 20px;
    }

    h3 {
      margin-bottom: 10px;
    }

    p {
      margin-bottom: 20px;
    }

    a {
      color: #fff; 
      text-decoration: none;
      font-weight: bold;
    }
    a:hover {
      color: #2ecc71; /* Lighter green when clicked */
    }
    a:active {
       color: #e74c3c; /* Darker red on hover */
    }

    .sensor-container {
      display: flex;
      justify-content: space-between;
      margin-bottom: 20px;
    }

    .sensor {
      flex: 1;
      text-align: center;
      padding: 20px;
      background-color: rgba(255, 255, 255, 0.1); /* Transparent white background */
      border-radius: 8px;
    }

    @media (max-width: 576px) {
      .sensor-container {
        flex-direction: column;
      }
    }
  </style>
</head>

<body>
  <div class="overlay"></div>
  <div class="navbar">
        <a href="homepage2.html">Home</a>
        <a href="info.html">Project Information</a>
        <a href="controls.html">Control</a>
  </div>
  <div class="content">
    <header>
        <h1>Sensor Data</h1>
    </header>
    <p><a href="homepage2.html">Go back</a></p>
    <section class="options-section">
      <div class="sensor-container">
        <div class="sensor">
          <h3>Temperature</h3>
          )=====");
          String datapage1 = F(R"=====(
          <p>°C</p>
          <h3>Heart Rate</h3>
          <p>-- BPM</p>
        </div>
      </div>
    </section>
    
    <section class="options-section">
      <div class="sensor-container">
        <div class="sensor">
          <h3>Wheelchair Status</h3>
          )=====");
          String datapage3 = F(R"=====(        
          <h3>Wheelchair Condition</h3>
          <p>Stable</p>
        </div>
      </div>
    </section>
  </div>
</body>
</html>
)=====");
