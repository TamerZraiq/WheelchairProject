String homePagePart0 = F(R"=====(
<!DOCTYPE html>
<html lang="en">
<head>    
    <meta http-equiv="refresh" content="10">
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Smart IoT Wheelchair</title>
    <link rel="stylesheet" href="styles.css">
    <link rel="stylesheet" href="background.css">

<style>
    body {
    font-family: Arial, sans-serif;
    margin: 0;
    padding: 0;
}
a {
    color: #0b214b; /* Default color */
    text-decoration: none; 
}
a:hover {
    color: #2ecc71; 

}
a:active {
    color: #e74c3c; 

}

.sensor-section,
.control-section {
    max-width: 800px;
    margin: 20px auto;
    padding: 20px;
    border: 1px solid #ddd;
    border-radius: 8px;
}

.options-section {
    max-width: 800px;
    margin: 20px auto;
    padding: 20px;
    border: 1px solid #ddd;
    border-radius: 8px;
    text-align: center;
}

.sensor-container {
    display: flex;
    justify-content: space-between;
}

.sensor,
.controls {
    flex: 1;
    text-align: center;
    padding: 20px;
}
button:hover {
    background-color: #45a049;
}
.flex-Container{
    display: flex;
    flex-direction: column;
    background-color: DodgerBlue;
  }
  .flex-row {
    display: flex;
    flex-direction: column;
    align-items: center;
  }
  .flex-keypad{
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
footer {
    background-color: #333;
    color: #fff;
    text-align: center;
    padding: 1em 0;
    position: fixed;
    width: 100%;
    bottom: 0;
}
body {
    margin: 0;
    padding: 0;
    height: 100vh;
    display: flex;
    align-items: center;
    justify-content: center;
    background: linear-gradient(135deg, #3498db, #2ecc71); /* Gradient from light blue to green */
    overflow: hidden; /* Prevent scrolling beyond the viewport */
    font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
    color: white;
}

.overlay {
    position: absolute;
    width: 100%;
    height: 100%;
    background: rgba(0, 0, 0, 0.3); /* Semi-transparent overlay */
}

.content {
    z-index: 1; /* Place content above the overlay */
    text-align: center;
}

h1 {
    font-size: 3em;
    margin-bottom: 20px;
}

p {
    font-size: 1.5em;
}
</style>
</head>

<body>
    <div class="overlay"></div>
    <div class="content">
    <header>
        <h1>Smart IoT Wheelchair</h1>
    </header>
    <br>
    <header>
    <h2>Welcome to my Website!</h2>
    <h2>Please Scan Your Card </h2>
    </header>        
    <section class="options-section">
        <h2>Status: </h2>
        )=====");
        String AccessStatus = F(R"=====(
        <h2><a href="homepage3.html">Proceed</a></h2>
        <h2> </h2>
    </section>
    
</div>
</body>
</html>
)=====");