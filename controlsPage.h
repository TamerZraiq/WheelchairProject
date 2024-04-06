String controlspage = F(R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Wheelchair Navigation</title>
    
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
      color: #2ecc71; /* Lighter green */
    }
    a:active {
       color: #e74c3c; /* Darker red */
    }
    .control-section {
      display: flex;
      flex-direction: column;
      align-items: center;
    }

    .flex-keypad {
        display: flex;
        flex-direction: row;
        align-items: center;
        justify-content: center;
    }

    .myButton {
      display: block;
      width: 20%;
      padding: 10px;
      margin: 10px;
      font-size: 16px;
      background-color: #4CAF50;
      color: white;
      border: none;
      border-radius: 4px;
      cursor: pointer;
    }
    .emergencyButton {
      display: block;
      background-color: #4CAF50;
      color: white;
      font-size: 20px;
      padding: 15px 25px;
      border: none;
      border-radius: 8px;
      cursor: pointer;
      margin: 8px;
    }
    @media (max-width: 576px) {

    }
</style>
</head>

<body>
    <div class="overlay"></div>
    <div class="navbar">
        <a href="homepage2.html">Home</a>
        <a href="info.html">Project Information</a>
        <a href="data.html">Data</a>
  </div>
    <div class="content">
    <header>
        <h1>Wheelchair Control</h1>
    </header>
    <p><a href="homepage2.html">Go back</a></p>
    <!-- Your content for the controls page goes here -->
    <section class="control-section">
        <div class = "flex-keypad">
          <button class="emergencyButton" onclick="fetch('/sentKeyPressToWebServer?button=F')"> Fwd</button> 
        </div>
          <div class = "flex-keypad">
            <button class="emergencyButton" onclick="fetch('/sentKeyPressToWebServer?button=L')"> Left</button> 
            <button class="emergencyButton" onclick="fetch('/sentKeyPressToWebServer?button=S')"> Stop</button> 
            <button class="emergencyButton" onclick="fetch('/sentKeyPressToWebServer?button=R')"> Right</button> 
          </div>
          <div class = "flex-keypad">
            <button class="emergencyButton" onclick="fetch('/sentKeyPressToWebServer?button=B')"> Back</button> 
          </div>
        </div>
    </section>
</div>
</body>
</html>
)=====");