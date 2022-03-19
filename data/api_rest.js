
function api_rest_reboot() {
    fetch("http://" + URI + "/reboot", {
        method: 'PUT'
    }).then((response) => {
        return response.json()
    }).then((res) => {
        if (res.status === 200) {
            toastInfoShow("Restart in progress...");
        }
        else {
            toastErrorShow("Unable to send the restart command !");
        }
    }).catch((error) => {
        console.log(error)
        /* Display Toast Error */
        toastErrorShow("Unable to send the restart command !");
    });
}

function api_rest_reset_configuration() {
    fetch("http://" + URI + "/resetConfiguration", {
        method: 'PUT'
    }).then((response) => {
        return response.json()
    }).then((res) => {
        if (res.status === 200) {
            toastInfoShow("Reset the configuration to factory");
        }
        else {
            toastErrorShow("Unable to reset the configuration !");
        }
    }).catch((error) => {
        console.log(error)
        /* Display Toast Error */
        toastErrorShow("Unable to reset the configuration !");
    });
}

function api_rest_set_animation_configuration(config) {
    fetch("http://" + URI + "/setAnimationConfiguration", {
        method: 'PUT',
        body: JSON.stringify(config),
        headers: {
            'Content-Type': 'application/json'
        }
    }).then((response) => {
        return response.json()
    }).then((res) => {
        if (res.status === 200) {
            toastInfoShow("Save the configuration successfully !");
        }
        else {
            /* Display Toast Error */
            toastErrorShow("Unable to save the configuration !");
        }
    }).catch((error) => {
        console.log(error)

        /* Display Toast Error */
        toastErrorShow("Unable to save the configuration !");
    });
}

function api_rest_set_assembly_configuration(config) {
    fetch("http://" + URI + "/setAssemblyConfiguration", {
        method: 'PUT',
        body: JSON.stringify(config),
        headers: {
            'Content-Type': 'application/json'
        }
    }).then((response) => {
        return response.json()
    }).then((res) => {
        if (res.status === 200) {
            toastInfoShow("Save the configuration successfully !");
        }
        else {
            /* Display Toast Error */
            toastErrorShow("Unable to save the configuration !");
        }
    }).catch((error) => {
        console.log(error)

        /* Display Toast Error */
        toastErrorShow("Unable to save the configuration !");
    });
}

function api_rest_set_general_configuration(config) {
    fetch("http://" + URI + "/setGeneralConfiguration", {
        method: 'PUT',
        body: JSON.stringify(config),
        headers: {
            'Content-Type': 'application/json'
        }
    }).then((response) => {
        return response.json()
    }).then((res) => {
        if (res.status === 200) {
            toastInfoShow("Save the configuration successfully !");
        }
        else {
            /* Display Toast Error */
            toastErrorShow("Unable to save the configuration !");
        }
    }).catch((error) => {
        console.log(error)

        /* Display Toast Error */
        toastErrorShow("Unable to save the configuration !");
    });
}

async function api_rest_read_configuration() {
    const result = await fetch("http://" + URI + "/configuration.json", {
        method: 'GET',
    }).then((response) => {
        if (response.status === 200) {
            return response.json()
        }
        else {
            return null;
        }
    }).catch((error) => {
        throw new Error(error);
    });

    return result;
}

// **************
// *** Others ***
// **************
function toastInfoShow(text) {
    /* Display Toast Info */
    document.getElementById('toastInfoText').textContent = text;
    new bootstrap.Toast(document.getElementById('toastInfo')).show();
}

function toastErrorShow(text) {
    /* Display Toast Error */
    document.getElementById('toastErrorText').textContent = text;
    new bootstrap.Toast(document.getElementById('toastError')).show();
}

function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}