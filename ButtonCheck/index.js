var m = require("mraa");

var button = new m.Gpio(7);
button.dir(m.DIR_IN);

setInterval(function() {
	if(button.read() == 0) {
		console.log("OMG WTF BUTTON PRESSED! ZPONIES!");
	}
}, 50);
