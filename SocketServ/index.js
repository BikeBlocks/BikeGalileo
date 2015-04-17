var app = require('express')();
var server = require('http').Server(app);
var io = require('socket.io')(server);
var m = require("mraa");

server.listen(8080);

app.get('/', function (req, res) {
  res.sendfile(__dirname + '/index.html');
});

io.on('connection', function (socket) {
  console.log("connection");
  socket.emit('speed', { value: 00 });
});





var button = new m.Gpio(7);
button.dir(m.DIR_IN);
var previousread;
var ltime = process.hrtime();
var diameter = 55; // centimeters
setInterval(function() {
  
	if(button.read() == 0 && previousread != 0) {
      
    var duration = process.hrtime() - ltime;
      
    var seconds = duration / 1000000;

    var speed = (3.14 * diameter) / (seconds / 360); /*convert to km/h */  
      
		io.emit('speed',{value:speed,duration:seconds});
      
    console.log("Event speed emitted");
    ltime = process.hrtime();
	}
    previousread = button.read();  
}, 1);
