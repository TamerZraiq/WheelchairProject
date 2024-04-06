String homePagePart2 = F(R"=====(
<!DOCTYPE html>
<html lang="en">
<head>    
    <meta http-equiv="refresh" content="10">
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Smart IoT Wheelchair</title>

  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 0;
      display: flex;
      flex-direction: column; /* Change to column for navbar */
      align-items: center;
      min-height: 100vh;
      width: 100vw; 
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
      width: 25%; /* Four links of equal widths */
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

    a {
      color: #0b214b; /* Default color */
      text-decoration: none;
      font-weight: bold;
    }
    a:hover {
      color: #2ecc71;
    }
    a:active {
      color: #e74c3c;
    }

    .options-section {
      border: 1px solid #ddd;
      border-radius: 8px;
      text-align: center;
      margin-bottom: 20px;
      padding: 20px;
    }
    .options-section h2 {
      margin: 0;
    }

    @media (max-width: 768px) {
      .options-section {
        max-width: 90%;
        margin: 10px;
      }
    }
    @media (max-width: 576px) {
      .options-section {
        max-width: 100%;
      }
      .options-section a {
        font-size: 1.0em;
      }
    }
    .emergency-button {
      background-color: #e74c3c; /* Red */
      color: white;
      font-size: 20px;
      padding: 15px 25px;
      border: none;
      border-radius: 8px;
      cursor: pointer;
      margin-top: 20px; 
    }
    .emergency-button:hover {
      background-color: #c0392b; /* Darker red */
    }
  </style>
</head>

<body>
  <div class="overlay"></div>
  <div class="navbar">
        <a href="homepage2.html">Home</a>
        <a href="info.html">Project Info</a>
  </div>
  <div class="content">
    <header>
      <h1>Smart IoT Wheelchair</h1>
    </header>
    <header>
      <h2>Please choose one of the following</h2>
    </header>        
    <section class="options-section">
      <h2><a href="data.html">Data and Details</a></h2>
    </section>
    <section class="options-section">
      <h2><a href="controls.html">Wheelchair Control</a></h2>
    </section>
    <button onclick="location.href='emergency.html'" class="emergency-button">Emergency Stop</button>
  </div>
</body>
</html>
)=====");