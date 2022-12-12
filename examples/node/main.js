'use strict';

var Protocol = require('azure-iot-device-mqtt').Mqtt;
var ModuleClient = require('azure-iot-device').ModuleClient;
var Message = require('azure-iot-device').Message;

var msg_counter = 0;
const LOG_FREQUENCY = 10;

// The SSL verification step can only be verified if the hostname is provided as
// a domain name, rather than an IP address.
// In the case you provided the IP address when creating the Local node, please
// uncomment the following code

//process.env.IOTEDGE_GATEWAYHOSTNAME = 'edgehub';

ModuleClient.fromEnvironment(Protocol, function (err, client) {
  if (err) {
    console.log('error:' + err);
  } else {
    console.log('got client');
    client.on('error', function (err) {
      console.error(err.message);
    });
    // connect to the edge instance
    client.open(function (err) {
      if (err) {
        console.error('Could not connect: ' + err.message);
      } else {
        console.log('Client connected');
      }
    });

    // Create a message and send it every two seconds
    setInterval(function () {
      var temperature = (Math.random() * 30); // range: [0, 30]
      var message = new Message(String(temperature));
      client.sendOutputEvent('sensoroutput', message, mqttPromise);

      if (msg_counter % LOG_FREQUENCY == 0) {
        console.log('Sent data number ' + msg_counter + ' -> message = ' + message.getData());
      }

      msg_counter++;
    }, 10000);
  }
});

function mqttPromise(op) {
  return function printResult(err, res) {
    if (err) console.log(op + ' error: ' + err.toString());
  };
}