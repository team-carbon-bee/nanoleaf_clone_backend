
async function api_rest_reboot() {
    const response = await fetch("http://" + URI + "/reboot", {
        method: 'PUT',
    }).catch((error) => {
        throw "<b>Unable to send command !</b><br/>" + error;
    });

    if (response.status === 200) {
        if (response.headers.get('Content-Type') == 'application/json')
            return await response.json();
        else
            return await response.text();
    }
    else {
        throw "<b>Response Error !</b><br/>" + "message: " + await response.text() + "<br/>status: " + response.status;
    }
}

async function api_rest_get_informations() {
    const response = await fetch("http://" + URI + "/informations", {
        method: 'GET'
    }).catch((error) => {
        throw "<b>Unable to send command !</b><br/>" + error;
    });

    if (response.status === 200) {
        if (response.headers.get('Content-Type') == 'application/json')
            return await response.json();
        else
            return await response.text();
    }
    else {
        throw "<b>Response Error !</b><br/>" + "message: " + await response.text() + "<br/>status: " + response.status;
    }
}

async function api_rest_reset_configuration() {
    const response = await fetch("http://" + URI + "/resetConfiguration", {
        method: 'GET',
    }).catch((error) => {
        throw "<b>Unable to send command !</b><br/>" + error;
    });

    if (response.status === 200) {
        if (response.headers.get('Content-Type') == 'application/json')
            return await response.json();
        else
            return await response.text();
    }
    else {
        throw "<b>Response Error !</b><br/>" + "message: " + await response.text() + "<br/>status: " + response.status;
    }
}

async function api_rest_set_animation_configuration(config) {
    const response = await fetch("http://" + URI + "/setAnimationConfiguration", {
        method: 'PUT',
        body: JSON.stringify(config),
        headers: {
            'Content-Type': 'application/json'
        }
    }).catch((error) => {
        throw "<b>Unable to send command !</b><br/>" + error;
    });

    if (response.status === 200) {
        if (response.headers.get('Content-Type') == 'application/json')
            return await response.json();
        else
            return await response.text();
    }
    else {
        throw "<b>Response Error !</b><br/>" + "message: " + await response.text() + "<br/>status: " + response.status;
    }
}

async function api_rest_set_assembly_configuration(config) {
    const response = await fetch("http://" + URI + "/setAssemblyConfiguration", {
        method: 'PUT',
        body: JSON.stringify(config),
        headers: {
            'Content-Type': 'application/json'
        }
    }).catch((error) => {
        throw "<b>Unable to send command !</b><br/>" + error;
    });

    if (response.status === 200) {
        if (response.headers.get('Content-Type') == 'application/json')
            return await response.json();
        else
            return await response.text();
    }
    else {
        throw "<b>Response Error !</b><br/>" + "message: " + await response.text() + "<br/>status: " + response.status;
    }
}

async function api_rest_set_general_configuration(config) {
    const response = await fetch("http://" + URI + "/setGeneralConfiguration", {
        method: 'PUT',
        body: JSON.stringify(config),
        headers: {
            'Content-Type': 'application/json'
        }
    }).catch((error) => {
        throw "<b>Unable to send command !</b><br/>" + error;
    });

    if (response.status === 200) {
        if (response.headers.get('Content-Type') == 'application/json')
            return await response.json();
        else
            return await response.text();
    }
    else {
        throw "<b>Response Error !</b><br/>" + "message: " + await response.text() + "<br/>status: " + response.status;
    }
}

async function api_rest_read_configuration() {
    const response = await fetch("http://" + URI + "/configuration.json", {
        method: 'GET',
    }).catch((error) => {
        throw "<b>Unable to send command !</b><br/>" + error;
    });

    if (response.status === 200) {
        if (response.headers.get('Content-Type') == 'application/json')
            return await response.json();
        else
            return await response.text();
    }
    else {
        throw "<b>Response Error !</b><br/>" + "message: " + await response.text() + "<br/>status: " + response.status;
    }
}

// **************
// *** Others ***
// **************
function toastInfoShow(text) {
    /* Display Toast Info */
    document.getElementById('toastInfoText').innerHTML = text;
    new bootstrap.Toast(document.getElementById('toastInfo')).show();
}

function toastErrorShow(text) {
    /* Display Toast Error */
    document.getElementById('toastErrorText').innerHTML = text;
    new bootstrap.Toast(document.getElementById('toastError')).show();
}

function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}