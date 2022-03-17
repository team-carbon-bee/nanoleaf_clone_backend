let config = {};


function onBtnSaveClick() {
    sendConfig();
}

function onBtnRebootClick() {
    $.post("http://" + URI + "/restart")
        .done(function (res) {
            /* Display Toast Info */
            toastInfoShow("Restart in progress...");
        })
        .fail(function () {
            /* Display Toast Error */
            toastErrorShow("Unable to communicate with device !");
        })
        .always(function () {
        });
}

function onBtnResetClick() {
    $.post("http://" + URI + "/reset")
        .done(function (res) {
            /* Display Toast Info */
            toastInfoShow("Reset to factory mode...");
            getConfig();
        })
        .fail(function () {
            /* Display Toast Error */
            toastErrorShow("Unable to communicate with device !");
        })
        .always(function () {
        });
}

function toastInfoShow(text) {
    /* Display Toast Info */
    document.getElementById('toastInfoText').textContent = text;
    let toast = document.getElementById('toastInfo');
    bootstrap.Toast.getOrCreateInstance(toast).show();
}

function toastErrorShow(text) {
    /* Display Toast Error */
    document.getElementById('toastErrorText').textContent = text;
    let toast = document.getElementById('toastError');
    bootstrap.Toast.getOrCreateInstance(toast).show();
}

function getConfig() {
    console.log("try to read config...")
    fetch("http://" + URI + "/configuration.json", {
        method: 'get',
        // headers: {
        //     'Accept': 'application/json',
        //     'Content-Type': 'application/json'
        // }
    }).then((response) => {
        return response.json()
    }).then((res) => {
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
        if (res.status === 200) {
            console.log("Get successfully !")
            console.log("data receive: " + JSON.stringify(res));

            /* Get Data */
            let params = res.parameters;

            // Get MQTT informations
            document.getElementById('mqtt-enable').checked = params.mqttEnable;
            if (params.mqttEnable == true)
                $('#card-mqtt').collapse('show');
            else
                $('#card-mqtt').collapse('hide');
            document.getElementById('mqtt-hostname').value = params.mqttIpServer;
            document.getElementById('mqtt-port').value = params.mqttPortServer;
            document.getElementById('mqtt-username').value = params.mqttUsername;
            document.getElementById('mqtt-password').value = params.mqttPassword;
            document.getElementById('mqtt-topic').value = params.mqttTopic;

            document.getElementById('version-text').value = params.version;
            document.getElementById('buildDate-text').value = params.buildDate;
        }
    }).catch((error) => {
        console.log(error)
        /* Display Toast Error */
        toastErrorShow("Unable to read configuration !");
        getConfig();
    })

}

document.addEventListener('DOMContentLoaded', (function () {
    console.log("config ready !");

    getConfig();
}));