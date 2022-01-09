const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style type="text/css">
.button {
  background-color: #4CAF50; /* Green */
  border: none;
  color: white;
  padding: 15px 32px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 16px;
}
/* #bor{
  border-style: solid;
} */
</style>
<body style="background-color: #ccd5ae ">
<center>
<div>
<h1>PH & TURBIDITY LEVEL WEBSERVER</h1>
  <!-- <button class="button" onclick="send(1)">LED ON</button>
  <button class="button" onclick="send(0)">LED OFF</button><BR> -->
</div id="ph">
 <br>
<div><h2>
  PH: <span id="bor"><span id="ph-val">0</span></span>
</h2>
</div>
<div id="turbidity">
  <h2>
    TURBIDITY: <span id="bor"><span id="turb-val">0</span></span> NTU
  </h2>
</div>
<script>
// function send(led_sts) 
// {
//   var xhttp = new XMLHttpRequest();
//   xhttp.onreadystatechange = function() {
//     if (this.readyState == 4 && this.status == 200) {
//       document.getElementById("state").innerHTML = this.responseText;
//     }
//   };
//   xhttp.open("GET", "led_set?state="+led_sts, true);
//   xhttp.send();
// }

setInterval(function() 
{
  getData_ph();
}, 500); 
function getData_ph() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ph-val").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "phread", true);
  xhttp.send();
}

setInterval(function() 
{
  getData_turb();
}, 500); 
function getData_turb() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("turb-val").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "turbread", true);
  xhttp.send();
}
</script>
</center>
</body>
</html>
)=====";
