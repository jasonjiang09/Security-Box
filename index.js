var express = require('express');
var app = express();
var multer  = require('multer');
var upload = multer({ dest: __dirname + '/public/uploads/' });
var type = upload.single('data');
var exec = require('child_process').exec;
var uuid = require('uuid/v1');
const bodyParser = require('body-parser');
const fs = require('fs');
var SerialPort = require("serialport")
const Readline = require('@serialport/parser-readline')
var serialPort = new SerialPort("/dev/ttyS8", {
  baudRate: 9600
});

var last = "none";
var lastSend = 0;

app.use(bodyParser.urlencoded({limit: '50mb', extended: false }))

var PORT = 9876;

app.get('/', function (req, res) {
   res.sendFile(__dirname + '/html/index.html');
});

app.post('/image/', type, function (req, res) {
   //console.log((new Date()) - lastSend);
   if ((new Date()) - lastSend <= 1000) {
      res.send(JSON.stringify({uploaded: false, classification: null}));
   } else {
      res.setHeader("Access-Control-Allow-Origin", "*");
      const id = uuid();
      // console.log(req.body.image);
      let json = {
         "payload": { 
            "image": {
                  "imageBytes": req.body.image.split(',')[1]
               }
            }
      }
      //console.log('HIT');
      fs.writeFile(__dirname + '/public/uploads/' + id + "-req.json.no-demon", JSON.stringify(json), function(err) {
         if (err) console.log(err);
         exec(`export GOOGLE_APPLICATION_CREDENTIALS="/gcloud_cred.json" && curl -X POST -H "Content-Type: application/json" \
              -H "Authorization: Bearer $(gcloud auth application-default print-access-token)" \
              https://automl.googleapis.com/v1beta1/projects/tamshacks-security-box/locations/us-central1/models/ICN1393255613021862030:predict -d @"${__dirname}/public/uploads/${id}-req.json.no-demon"`, function(e, stdout, stderr) {
            console.log(stdout);
            console.log(stderr);
            let data = JSON.parse(stdout);
            if (!data.payload) {
               res.send(JSON.stringify({uploaded: false, classification: null}));
               return;
            }
            let type = data.payload[0].displayName;
            last = type;
            console.log(last)
           
            if ((new Date()) - lastSend > 1000) {
               lastSend = new Date();
               if (last == 'Jason') {
                  serialPort.write('j')
               } else if (last == 'Tanner') {
                  serialPort.write('t')
               } else if (last == 'enviroment') {
                  serialPort.write('e')
               }
               else {
                  serialPort.write('*');
               }
            } else {
               //console.log('Wait');
            }
            res.send(JSON.stringify({uploaded: true, classification: type}));
         });
      });
   }
});

app.get('/last/', function (req, res) {
   res.send(last);
});

const parser = new Readline()
serialPort.pipe(parser);

serialPort.on("open", function () {
  console.log('Communication is on!');
});

parser.on('data', line => console.log(`> ${line}`))
//serialPort.write('ROBOT POWER ON\n')

// setInterval(() => {
//    serialPort.write(Math.random() < 0.5 ? 'w' : 'p');
// }, 5000)

app.listen(PORT, function () {
   console.log(`Listening on port ${PORT}`);
   serialPort.write('x')
});