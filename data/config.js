let config = {};


function onBtnSaveClick() {
    setConfig();
}

function onBtnRebootClick() {
    api_rest_reboot()
        .then((response) => {
            toastInfoShow("Reboot in progress: " + response);
        })
        .catch((error) => {
            toastErrorShow(error);
        });
}

function onBtnResetClick() {
    api_rest_reset_configuration()
        .then((response) => {
            toastInfoShow("Reboot in progress: " + response);
        })
        .catch((error) => {
            toastErrorShow(error);
        });
}

function setConfig() {
    // Get all information
    let config = {
        'parameters': {
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
    api_rest_set_configuration(config)
        .then((response) => {
            toastInfoShow("Save configuration successfully");
        })
        .catch((error) => {
            toastErrorShow(error);
        });
}

function getConfig() {
    console.log("try to read informations...");
    api_rest_get_informations()
        .then((info) => {
            if (info != null) {
                // Get Version
                if (info.version != null)
                    document.getElementById('version-text').value = info.version;
                // Get Build Date
                if (info.buildDate != null)
                    document.getElementById('buildDate-text').value = info.buildDate;
            }
        });

    console.log("try to read config...");
    api_rest_read_configuration()
        .then((config) => {
            if (config != null && config.parameters != null) {
                // Get Data
                let params = config.parameters;

                // Get Hostname
                if (params.hostname != null)
                    document.getElementById('hostname-text').value = params.hostname;

                // Get led Model
                if (params.ledModel != null)
                    document.getElementById('ledModel-select').select = params.ledModel;
                // Get Led Number
                if (params.ledPerTriangle != null)
                    document.getElementById('led-number').value = params.ledPerTriangle;

                // Get MQTT informations
                if (params.mqttEnable != null)
                    document.getElementById('mqtt-enable').checked = params.mqttEnable;
                if (params.mqttEnable == true)
                    document.getElementById('card-mqtt').setAttribute('collapse', 'show');
                else
                    document.getElementById('card-mqtt').setAttribute('collapse', 'hide');
                if (params.mqttIpServer != null)
                    document.getElementById('mqtt-hostname').value = params.mqttIpServer;
                if (params.mqttPortServer != null)
                    document.getElementById('mqtt-port').value = params.mqttPortServer;
                if (params.mqttUsername != null)
                    document.getElementById('mqtt-username').value = params.mqttUsername;
                if (params.mqttPassword != null)
                    document.getElementById('mqtt-password').value = params.mqttPassword;
                if (params.mqttTopic != null)
                    document.getElementById('mqtt-topic').value = params.mqttTopic;
            }
        }).catch((error) => {
            toastErrorShow("Unable to read the configuration !");
            console.error(error);
            sleep(5000).then(() => getConfig());
        });
}

document.addEventListener('DOMContentLoaded', (function () {
    console.log("config ready !");

    getConfig();
}));