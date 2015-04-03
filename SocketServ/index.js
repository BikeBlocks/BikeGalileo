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
  socket.emit('news', { hello: 'world' });
  socket.on('my other event', function (data) {
    console.log(data);
  });
});





var button = new m.Gpio(7);
button.dir(m.DIR_IN);
var previousread;
var ltime = moment();
setInterval(function() {
  
	if(button.read() == 0 && previousread != 0) {
    var duration = ltime.substract(moment());
		io.emit('buttonpress',{time:duration.millisecond()});
    console.log("btnemit");
    ltime = moment();
	}
    previousread = button.read();  
}, 100);
