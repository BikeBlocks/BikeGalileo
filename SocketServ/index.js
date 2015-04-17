var app = require('express')();
var server = require('http').Server(app);
var io = require('socket.io')(server);
var m = require("mraa");
var moment = require('moment');
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
var ltime = moment();
var diameter = 55; // centimeters
setInterval(function() {
  
	if(button.read() == 0 && previousread != 0) {
    var duration = ltime.subtract(moment());
    var seconds = duration.asSeconds();
    var milliduration = duration.asMillisecond();
    var speed = (3.14 * diameter) / (milliduration * 3600 * 100) /*convert to hours*/ ; 
      
		io.emit('speed',{value:speed,duration:seconds});
      
    console.log("Event speed emitted");
    ltime = moment.duration(process.hrtime()/1000);
	}
    previousread = button.read();  
}, 1);
