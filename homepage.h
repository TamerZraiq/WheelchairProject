// To create literal strings stored in flash memory enclose your HTML code between 
// F(R"=====( HTML code here )=====");
// If you have 1 reading then you probably have 2 literal strings
// If you have 2 readings then you probably have 3 literal strings etc.

String homePagePart0 = F(R"=====(
<!DOCTYPE html>
<html lang="en" > <!-- Needed for Screenreaders !-->
<head>
<meta http-equiv="refresh" content="10">
<!-- UTF-8 character set covers most characters in the world -->
 <meta charset="utf-8">
 <!-- Make page respond to screen size !-->
 <meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
<!--Include a Title. Used by Search Engines -->
<title> Heart Rate and Temperature Reading WebServer </title>
<style>
 
 <!--choose good contrast between background and foreground colours -->
 body { 
background-color: DodgerBlue;
}
.flex-Container{
 display: flex;
 flex-direction: column;
 background-color: DodgerBlue;
 align-items: center;
 }
 h1{
 font: bold;
 font-size: 40px;
 font-family: Arial;
 color: navy;
text-align: center;
 }
 p{
 font-size: 25px;
 font-family: Arial;
 color: navy;
text-align: center;
 } 
 th, td {
 font-size: 25px;
 padding: 8px;
 text-align: left;
2
 border-bottom: 1px solid #ddd;
}
</style>
</head>
<body>
<div class="flex-Container">
<h1> Smart Wheelchair </h1>
 
<p>Welcome to my website displaying the body temperature and heart rate</p>
<p>Wheelchair Status: </p>
<p>
)=====");
String homePagePart1 = F(R"=====(
  </p>
<table>
 <tr>
 <th>Sensor</th>
 <th>Value</th>
 <th>Unit</th>
 </tr>
 <tr>
 <td>Temperature</td> 
 <td>
)=====");
String homePagePart2 = F(R"=====(
  </td>
 <td>Degree Cel</td>
 </tr>
 <tr>
 <td>Heart Rate</td>
 <td>
 )=====");
String homePagePart3 = F(R"=====(
  </td>
 <td>bpm</td>
 </tr>
 </table>
 </div>
</body>
</html>
)=====");
