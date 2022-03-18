let config = {};

new Sortable(document.getElementById('all-animation-list'), {
    group: {
        name: 'shared',
        pull: 'clone',
        put: false
    },
    animation: 150,
    handle: '.handle',
    sort: false,
});

new Sortable(document.getElementById('custom-animation-list'), {
    group: 'shared',
    animation: 150,
    handle: '.handle'
});

function onBtnSaveClick() {
    setConfig();
}

function append_animation_to_list(animation_name) {
    let list = document.getElementById('all-animation-list');

    // Create icons for handle move
    let svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
    svg.setAttribute("class", "handle me-2");
    svg.setAttribute("width", "24");
    svg.setAttribute("height", "24");
    svg.setAttribute("viewBox", "0 0 16 16");
    let path = document.createElementNS("http://www.w3.org/2000/svg", "path");
    path.setAttribute("d", "M2 8a1 1 0 1 1 0 2 1 1 0 0 1 0-2zm0-3a1 1 0 1 1 0 2 1 1 0 0 1 0-2zm3 3a1 1 0 1 1 0 2 1 1 0 0 1 0-2zm0-3a1 1 0 1 1 0 2 1 1 0 0 1 0-2zm3 3a1 1 0 1 1 0 2 1 1 0 0 1 0-2zm0-3a1 1 0 1 1 0 2 1 1 0 0 1 0-2zm3 3a1 1 0 1 1 0 2 1 1 0 0 1 0-2zm0-3a1 1 0 1 1 0 2 1 1 0 0 1 0-2zm3 3a1 1 0 1 1 0 2 1 1 0 0 1 0-2zm0-3a1 1 0 1 1 0 2 1 1 0 0 1 0-2z");
    svg.appendChild(path);

    // Create group item
    let item = document.createElement("div");
    item.setAttribute("class", "list-group-item d-flex justify-content-between");

    // Insert text of animation
    item.innerText = animation_name;

    // Add svg to item
    item.appendChild(svg);

    // Add item to list
    list.appendChild(item);
}

function setConfig() {
    // Get all information
    let config = {
        'animation' : {
            'animationDuration': document.getElementById('duration-number').value,
            'animationMethod': document.getElementById('method-select').select,
            'animationList': [],
        }
    }

    // Send config
    api_rest_set_general_configuration(config);
}

function getConfig() {
    console.log("try to read config...")

    let config = api_rest_read_configuration();

    console.log(config);

    if (config != null && config.animation != null) {
        // Exemple of configuration
        // "animation": {
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
        let params = config.animation;

        document.getElementById('duration-number').value = params.animationDuration;
        document.getElementById('method-select').select = params.animationMethod;
        // document.getElementById('buildDate-text').value = params.animationList;
    }
    else {
        // getConfig();
    }
}

document.addEventListener('DOMContentLoaded', (function () {
    console.log("animation ready !");

    append_animation_to_list("coucou les amis");
    append_animation_to_list("c'était pas simple...");
    append_animation_to_list("mais j'ai réussi");
    append_animation_to_list("Je suis trop fort !");
    // getConfig();
}));