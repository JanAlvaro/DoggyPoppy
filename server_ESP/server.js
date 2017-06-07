/*
*   Serveur de test pour librairies ESP8266WiFi et ESP8266HTTPClient
*   Test server for ESP8266WiFi and ESP8266HTTPClient libraries
*   http://www.projetsdiy.fr - 2017
*/
var express = require('express');
//var moment = require('moment');
var app = express();
var fs = require('fs');
//var server = require('http').Server(app);
//var io = require('socket.io')(server);

app.get('/map.html', function (req, res) {
   res.sendFile( __dirname + "/" + "map.html" );
})

app.get('/GeoJSONP.js', function (req, res) {
   res.sendFile( __dirname + "/" + "GeoJSONP.js" );
})

app.get('/', function (req, res) {
   res.sendFile( __dirname + "/" + "home.html" );
})


//app.get('/', function(req, res) {
//  res.send('HTTP ESP8266 Test Server')
//});

app.use('/location', function (req, res, next) {

  var lat = req.param('lat');
  var longi = req.param('long');
//  var t = moment.duration(parseInt(req.param('uptime')), 'milliseconds')
  var _message = "lat: " + lat + " long: " + longi ;
  console.log("Your dog is at: " + _message);
  res.send("You are alive!");

  fs.writeFile("./GeoJSONP.js",
              "eqfeed_callback(\n"+
              "{\"type\":\"FeatureCollection\",\n"+
              "\"features\":[{\n"+
              "\"type\":\"Feature\",\n"+
              "\"geometry\":{    \"type\":\"Point\",\n"+
              "\"coordinates\":["+longi+","+lat+"]},\n"+
              "\"id\":\"usc000csx3\"},\n"+
              "{\"type\":\"Feature\",\n"+
              "\"geometry\":{\n"+
              "\"type\":\"Point\",\n"+
              "\"coordinates\":[]},\n"+
              "\"id\":\"usc000csw5\"}]});\n"


              , function(err) {
              if(err) {
                  return console.log(err);
              }

  console.log("The file was saved!");

  });
});

app.listen(8080);
