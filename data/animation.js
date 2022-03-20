let all_animations = {};

new Sortable(document.getElementById('all-animations-list'), {
    group: {
        name: 'shared',
        pull: 'clone',
        put: false
    },
    animation: 150,
    handle: '.handle',
    sort: false,
    onEnd: function (evt) {
        // Add trash icons only if drag and drop on other list
        if (evt.from != evt.to) {
            add_trash_icons_to_animation_item(evt.item);
        }
    }
});

new Sortable(document.getElementById('custom-animations-list'), {
    group: 'shared',
    animation: 150,
    handle: '.handle'
});

function onBtnSaveClick() {
    setConfig();
}

function onBtnClearClick() {
    clear_list_of_custom_animations();
}

function create_list_from_custom_animation_list() {
    const regex = new RegExp('([0-9]+) *-?');

    let list = document.getElementById('custom-animations-list');
    let json = [];

    list.childNodes.forEach(element => {
        let match = element.innerText.match(regex);
        if (match) {
            json.push(match[1]);
        }
    });

    return json;
}

function clear_list_of_custom_animations() {
    let list = document.getElementById('custom-animations-list');
    while (list.lastChild) {
        list.removeChild(list.lastChild);
    }
}

function play_animation_item(child_els) {
    const regex = new RegExp('([0-9]+) *-?');

    let parent = child_els.closest(".list-group-item");
    let match = parent.innerText.match(regex);
    if (match) {
        api_rest_preview_animation(match[1]);
    }
}

function remove_animation_item(child_els) {
    child_els.closest(".list-group-item").remove();
}

function update_list_of_all_animations() {
    // Clear all animations
    clear_list_of_all_animations();

    // for each animation add it to list
    for (let key in all_animations) {
        let name = all_animations[key].name;
        append_animation_to_all_animations_list(key + " - " + name);
    }
}

function clear_list_of_all_animations() {
    let list = document.getElementById('all-animations-list');
    while (list.lastChild) {
        list.removeChild(list.lastChild);
    }
}

function clear_list_of_custom_animations() {
    let list = document.getElementById('custom-animations-list');
    while (list.lastChild) {
        list.removeChild(list.lastChild);
    }
}

function add_play_icons_to_animation_item(item) {
    // Create trash icons for remove it
    let svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
    svg.setAttribute("width", "28");
    svg.setAttribute("height", "28");
    let path = document.createElementNS("http://www.w3.org/2000/svg", "path");
    path.setAttribute("d", "m11.596 8.697-6.363 3.692c-.54.313-1.233-.066-1.233-.697V4.308c0-.63.692-1.01 1.233-.696l6.363 3.692a.802.802 0 0 1 0 1.393z");
    path.setAttribute("transform", "scale(1.8, 1.8)");
    path.setAttribute("class", "play_icons");
    path.setAttribute("onclick", "play_animation_item(this)");
    svg.appendChild(path);

    // Add trash icons to item
    item.lastChild.insertAdjacentElement("afterbegin", svg);
}

function add_trash_icons_to_animation_item(item) {
    // Create trash icons for remove it
    let svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
    svg.setAttribute("width", "28");
    svg.setAttribute("height", "28");
    let path = document.createElementNS("http://www.w3.org/2000/svg", "path");
    path.setAttribute("d", "M 10.806641 2 C 10.289641 2 9.7956875 2.2043125 9.4296875 2.5703125 L 9 3 L 4 3 A 1.0001 1.0001 0 1 0 4 5 L 20 5 A 1.0001 1.0001 0 1 0 20 3 L 15 3 L 14.570312 2.5703125 C 14.205312 2.2043125 13.710359 2 13.193359 2 L 10.806641 2 z M 4.3652344 7 L 5.8925781 20.263672 C 6.0245781 21.253672 6.877 22 7.875 22 L 16.123047 22 C 17.121047 22 17.974422 21.254859 18.107422 20.255859 L 19.634766 7 L 4.3652344 7 z");
    path.setAttribute("class", "del_button");
    path.setAttribute("onclick", "remove_animation_item(this)");
    svg.appendChild(path);

    // Add trash icons to item
    item.lastChild.insertAdjacentElement("afterbegin", svg);
}

function create_animation_item(animation_name) {
    // Create div for all icons
    let icons = document.createElement("div");

    // Create handle icons for drag and drop
    let svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
    svg.setAttribute("class", "handle ms-2");
    svg.setAttribute("width", "28");
    svg.setAttribute("height", "28");
    let path = document.createElementNS("http://www.w3.org/2000/svg", "path");
    path.setAttribute("transform", "scale(1.8, 1.8)");
    path.setAttribute("d", "M2 8a1 1 0 1 1 0 2 1 1 0 0 1 0-2zm0-3a1 1 0 1 1 0 2 1 1 0 0 1 0-2zm3 3a1 1 0 1 1 0 2 1 1 0 0 1 0-2zm0-3a1 1 0 1 1 0 2 1 1 0 0 1 0-2zm3 3a1 1 0 1 1 0 2 1 1 0 0 1 0-2zm0-3a1 1 0 1 1 0 2 1 1 0 0 1 0-2zm3 3a1 1 0 1 1 0 2 1 1 0 0 1 0-2zm0-3a1 1 0 1 1 0 2 1 1 0 0 1 0-2zm3 3a1 1 0 1 1 0 2 1 1 0 0 1 0-2zm0-3a1 1 0 1 1 0 2 1 1 0 0 1 0-2z");
    svg.appendChild(path);
    icons.appendChild(svg);

    // Create group item
    let item = document.createElement("div");
    item.setAttribute("class", "list-group-item d-flex justify-content-between");

    // Insert text of animation
    item.innerHTML = animation_name;

    // Add icons to item
    item.appendChild(icons);

    return item;
}

function append_animation_to_all_animations_list(animation_name) {
    let list = document.getElementById('all-animations-list');

    // Create animation item
    let item = create_animation_item(animation_name);

    // Add play icons
    add_play_icons_to_animation_item(item);

    // Append item to list
    list.appendChild(item);
}

function append_animation_to_custom_list(animation_id) {
    let list = document.getElementById('custom-animations-list');

    // Get the name of animation on all animation list
    let name = "Unknown"
    if (all_animations[animation_id] != null)
        name = all_animations[animation_id].name;

    // Create animation item
    let item = create_animation_item(animation_id + ' - ' + name);

    // Add play icons
    add_play_icons_to_animation_item(item);

    // Add trash icons (for remove it)
    add_trash_icons_to_animation_item(item);

    // Append item to list
    list.appendChild(item);
}

function setConfig() {
    // Get all information
    let config = {
        'animation': {
            'animationDuration': document.getElementById('duration-number').value,
            'animationMethod': document.getElementById('method-select').value,
            'animationList': create_list_from_custom_animation_list(),
        }
    }

    // Send config
    api_rest_set_configuration(config)
        .then(() => {
            toastInfoShow("Save animation successfully");
        })
        .catch((error) => {
            toastErrorShow(error);
        });
}

async function getAnimations() {
    console.log("try to get all animations...");
    await api_rest_get_animations()
        .then((animations) => {
            // Save all animations
            all_animations = animations;

            console.log("Received list of all animations:");
            console.log(all_animations);

            // Update list of animations
            update_list_of_all_animations();
        })
        .catch((error) => {
            toastErrorShow(error);
            throw new Error(error);
        });
}

async function getConfig() {
    console.log("try to read config...");
    await api_rest_read_configuration()
        .then((config) => {
            if (config != null && config.animation != null) {
                /* Get Data */
                let params = config.animation;

                console.log(params);

                if (params.animationDuration != null)
                    document.getElementById('duration-number').value = params.animationDuration;
                if (params.animationMethod != null)
                    document.getElementById('method-select').value = params.animationMethod;
                if (params.animationList != null) {
                    clear_list_of_custom_animations();
                    params.animationList.forEach(anim_id => {
                        append_animation_to_custom_list(anim_id);
                    });
                }
            }
        }).catch((error) => {
            toastErrorShow(error);
            throw new Error(error);
            // sleep(5000).then(() => getConfig));
        });
}

document.addEventListener('DOMContentLoaded', (function () {
    console.log("animation ready !");

    getAnimations()
        .then(() => {
            getConfig();
        });

}));