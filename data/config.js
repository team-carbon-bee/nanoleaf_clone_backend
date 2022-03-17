let config = {};


function onBtnSaveClick() {
    setConfig();
}

function onBtnRebootClick() {
    api_rest_reboot();
}

function onBtnResetClick() {
    api_rest_reset_configuration();
}

function setConfig() {
    // Get all information
    let config = {
        'parameters' : {
            'ledPerTriangle': document.getElementById('led-number').value,
            'ledModel': document.getElementById('ledModel-select').select,
            'hostname': document.getElementById('hostname-text'),
            'mqtt': {
                'enable': document.getElementById('mqtt-enable').checked,
                'hostname': document.getElementById('mqtt-hostname').value,
                'port': document.getElementById('mqtt-port').value,
                'username': document.getElementById('mqtt-username').value,
                'password': document.getElementById('mqtt-password').value,
                'topic': document.getElementById('mqtt-topic').value,
            }
        }
    }

    // Send config
    api_rest_send_configuration(config);
}

function getConfig() {
    console.log("try to read config...")
    let config = api_rest_read_configuration();
    console.log(config);
    if (config != null) {
        // Exemple of configuration
        // "parameters": {
        //     "ledPerTriangle": 21,
        //     "ledModel": "rgb",
        //     "hostname": "nanoleaf_clone",
        //     "maxBrightness": 0.5,
        //     "speed": 50,
        //     "mainColorRandom": true,
        //     "mainColor": 16711935,
        //     "backgroundColorRandom": false,
        //     "backgroundColor": 0,
        //     "animationDuration": 5000,
        //     "animationMethod": "random",
        //     "animationList": [10, 11, 13, 15]
        // }

        /* Get Data */
        let params = config.parameters;

        // Get Hostname
        document.getElementById('hostname-text').value = params.hostname;

        // Get led Model and Led Number
        document.getElementById('ledModel-select').select = params.ledModel;
        document.getElementById('led-number').value = params.ledPerTriangle;

        // Get MQTT informations
        document.getElementById('mqtt-enable').checked = params.mqttEnable;
        if (params.mqttEnable == true)
            document.getElementById('card-mqtt').setAttribute('collapse', 'show');
            // $('#card-mqtt').collapse('show');
        else
            document.getElementById('card-mqtt').setAttribute('collapse', 'hide');
            // $('#card-mqtt').collapse('hide');
        document.getElementById('mqtt-hostname').value = params.mqttIpServer;
        document.getElementById('mqtt-port').value = params.mqttPortServer;
        document.getElementById('mqtt-username').value = params.mqttUsername;
        document.getElementById('mqtt-password').value = params.mqttPassword;
        document.getElementById('mqtt-topic').value = params.mqttTopic;

        document.getElementById('version-text').value = params.version;
        document.getElementById('buildDate-text').value = params.buildDate;
    }
    else {
        // getConfig();
    }
}

document.addEventListener('DOMContentLoaded', (function () {
    console.log("config ready !");

    getConfig();
}));