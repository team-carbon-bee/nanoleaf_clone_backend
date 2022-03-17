let config =
{
    "assembly": {
        "type": "triangle",
        "connections":
            [
                {
                    "type": "triangle",
                    "connections":
                        [
                            {
                                "type": "triangle",
                                "connections": [
                                    null,
                                    null
                                ]
                            },
                            null
                        ]
                },
                {
                    "type": "triangle",
                    "connections":
                        [
                            {
                                "type": "triangle",
                                "connections": [
                                    {
                                        "type": "triangle",
                                        "connections": [
                                            {
                                                "type": "triangle",
                                                "connections": [
                                                    null,
                                                    null
                                                ]
                                            },
                                            null
                                        ]
                                    },
                                    {
                                        "type": "triangle",
                                        "connections": [
                                            null,
                                            {
                                                "type": "triangle",
                                                "connections": [
                                                    null,
                                                    {
                                                        "type": "triangle",
                                                        "connections": [
                                                            null,
                                                            null
                                                        ]
                                                    }
                                                ]
                                            }
                                        ]
                                    }
                                ]
                            },
                            {
                                "type": "triangle",
                                "connections": [
                                    null,
                                    null
                                ]
                            }
                        ]
                }
            ]
    },
    "parameters": {
        "ledPerTriangle": 21,
        "ledModel": "rgb",
        "hostname": "nanoleaf_clone",
        "maxBrightness": 0.5,
        "speed": 50,
        "mainColorRandom": true,
        "mainColor": 16711935,
        "backgroundColorRandom": false,
        "backgroundColor": 0,
        "animationDuration": 5000,
        "animationMethod": "random",
        "animationList": [10, 11, 13, 15]
    }
};

function onBtSaveClick() {
    let json = create_json_from_svg();
    console.log(json);
}

let size_of_triangle = 200;
let padding_between_triangle = 1.5;

function create_json_from_svg() {
    let parent = getFirstChildByClassName(svgContainer, "child1");
    let triangle = getFirstChildByClassName(parent, "triangle");
    let json = {};

    // If we have one or more triangle
    if (triangle != null) {
        json = {
            "type": "triangle",
            "connections": []
        };
        parent = triangle.parentElement;
        let child1 = getFirstChildByClassName(parent, "child1");
        if (child1 != null)
            add_child_to_json(json.connections, child1);
        let child2 = getFirstChildByClassName(parent, "child2");
        if (child2 != null)
            add_child_to_json(json.connections, child2);
    }

    return json;
}

function add_child_to_json(json_data, child) {
    let triangle = getFirstChildByClassName(child, "triangle");

    // If we have one or more triangle
    if (triangle != null) {
        json_data.push({
            "type": "triangle",
            "connections": []
        });

        let parent = triangle.parentElement;
        let child1 = getFirstChildByClassName(parent, "child1");
        if (child1 != null)
            add_child_to_json(json_data[json_data.length - 1].connections, child1);
        let child2 = getFirstChildByClassName(parent, "child2");
        if (child2 != null)
            add_child_to_json(json_data[json_data.length - 1].connections, child2);
    }
    else
        json_data.push(null);
}

function add_child(child_els) {
    appendTriangle(child_els.parentElement);
}

function remove_child(child_els) {
    child_els.parentElement.remove();
}

function appendTriangle(parent) {
    let group = document.createElementNS("http://www.w3.org/2000/svg", "g");
    let triangle = document.createElementNS("http://www.w3.org/2000/svg", "polygon");

    let h = (Math.sqrt(3) * size_of_triangle / 2);
    let l = size_of_triangle / 8;
    let hyp = (l / Math.cos(30 * Math.PI / 180));
    let points = `${-(size_of_triangle / 2) + hyp}                                          0
                    ${-(size_of_triangle / 2) + (hyp / 2)}                                  ${-l}
                    ${-hyp / 2}                                                             ${-h + l}
                    ${hyp / 2}                                                              ${-h + l}
                    ${(size_of_triangle / 2) - (hyp / 2)}                                   ${-l}
                    ${(size_of_triangle / 2) - hyp}                                         0`

    // Add Point
    triangle.setAttribute("points", points);
    // Add Class
    triangle.setAttribute("class", "triangle");

    // Append Triangle to group
    group.appendChild(triangle);

    let connection = document.createElementNS("http://www.w3.org/2000/svg", "polygon");
    let offset = size_of_triangle / 50;
    points = `${-size_of_triangle / 32}             ${-size_of_triangle / 40 + offset}
                ${-size_of_triangle / 32}           ${size_of_triangle / 40 + offset}
                ${-size_of_triangle / 32 - 3}       ${size_of_triangle / 40 + offset}
                0                                   ${size_of_triangle / 40 + offset + 10}
                ${size_of_triangle / 32 + 3}        ${size_of_triangle / 40 + offset}
                ${size_of_triangle / 32}            ${size_of_triangle / 40 + offset}
                ${size_of_triangle / 32}            ${-size_of_triangle / 40 + offset}`

    // Add Point
    connection.setAttribute("points", points);
    // Add Class
    connection.setAttribute("class", "connection");

    // Append connection to group
    group.appendChild(connection);

    // Create Del button
    let del_button = document.createElementNS("http://www.w3.org/2000/svg", "path");
    del_button.setAttribute("d", "M11,22A11,11,0,1,0,0,11,11,11,0,0,0,11,22ZM5,10H17v2H5Z");
    del_button.setAttribute("transform", `translate(${-22 * 0.75}, ${22 * 0.75 - size_of_triangle / 2}) scale(1.5, 1.5)`);
    del_button.setAttribute("class", "del_button");
    del_button.setAttribute("onclick", "remove_child(this)");

    // Append connection to group
    group.appendChild(del_button);

    // Create Add button
    let add_button = document.createElementNS("http://www.w3.org/2000/svg", "path");
    add_button.setAttribute("d", "M11 22A11 11 0 1 0 0 11a11 11 0 0 0 11 11zM5 10h5V5h2v5h5v2h-5v5h-2v-5H5z");
    add_button.setAttribute("transform", `translate(${-22 * 0.75}, ${-size_of_triangle / 4}) scale(1.5, 1.5)`);
    add_button.setAttribute("class", "add_button");
    add_button.setAttribute("onclick", "add_child(this)");

    // Create group for child 1
    let child1 = document.createElementNS("http://www.w3.org/2000/svg", "g");
    // Add Rotation and set the origin of the child 1
    child1.setAttribute("transform", `translate(${-size_of_triangle / 4 - (padding_between_triangle / Math.tan(30 * Math.PI / 180))}, ${-h / 2 - padding_between_triangle}), rotate(-60)`);
    child1.setAttribute("class", "child1");

    // Append Add button to child 1
    child1.appendChild(add_button.cloneNode(true));

    // Append child 1 to group
    group.appendChild(child1);

    // Create group for child 2
    let child2 = document.createElementNS("http://www.w3.org/2000/svg", "g");
    // Add Rotation and set the origin of the child 2
    child2.setAttribute("transform", `translate(${size_of_triangle / 4 + (padding_between_triangle / Math.tan(30 * Math.PI / 180))}, ${-h / 2 - padding_between_triangle}), rotate(60)`);
    child2.setAttribute("class", "child2");

    // Append Add button to child 2
    child2.appendChild(add_button.cloneNode(true));

    // Append child 2 to group
    group.appendChild(child2);

    // Append group to parent
    parent.appendChild(group);
}

function appendBase(parent) {
    let group = document.createElementNS("http://www.w3.org/2000/svg", "g");

    group.setAttribute("transform", `rotate(180) translate(0, ${-padding_between_triangle * 2})`);

    let triangle = document.createElementNS("http://www.w3.org/2000/svg", "polygon");

    let h = (Math.sqrt(3) * size_of_triangle / 2);
    let l = size_of_triangle / 8;
    let hyp = (l / Math.cos(30 * Math.PI / 180));
    let l2 = size_of_triangle / 1.7;
    let hyp2 = (l2 / Math.cos(30 * Math.PI / 180));
    let points = `${-(size_of_triangle / 2) + hyp}                                        0
                    ${-(size_of_triangle / 2) + (hyp / 2)}                                  ${-l}
                    ${-hyp2 / 2}                                                            ${-h + l2}
                    ${hyp2 / 2}                                                             ${-h + l2}
                    ${(size_of_triangle / 2) - (hyp / 2)}                                   ${-l}
                    ${(size_of_triangle / 2) - hyp}                                         0`

    // Add Point
    triangle.setAttribute("points", points);
    // Add Class
    triangle.setAttribute("class", "base");

    // Append triangle to group
    group.appendChild(triangle);

    // Create Add button
    let add_button = document.createElementNS("http://www.w3.org/2000/svg", "path");
    add_button.setAttribute("d", "M11 22A11 11 0 1 0 0 11a11 11 0 0 0 11 11zM5 10h5V5h2v5h5v2h-5v5h-2v-5H5z");
    add_button.setAttribute("transform", `translate(${-22 * 0.75}, ${-size_of_triangle / 4}) scale(1.5, 1.5)`);
    add_button.setAttribute("class", "add_button");
    add_button.setAttribute("onclick", "add_child(this)");

    // Create group for child 1
    let child1 = document.createElementNS("http://www.w3.org/2000/svg", "g");
    // Add Rotation and set the origin of the child 1
    child1.setAttribute("transform", `translate(0, ${padding_between_triangle * 2}), rotate(-180)`);
    child1.setAttribute("class", "child1");

    // Append Add button to child 1
    child1.appendChild(add_button);

    // Append child 1 to group
    group.appendChild(child1);

    // Append goup to parent
    parent.appendChild(group);
}

function parseShape(shape, parent) {
    // Check if it is a triangle
    if (shape.type != "triangle")
        return;

    // Check if we are 2 connections only
    if (shape.connections.length != 2)
        return;

    appendTriangle(parent);

    // If first Child exist
    let child1 = getFirstChildByClassName(parent, "child1");
    if (child1 != null && shape.connections[0] != null && shape.connections[0].length != 0) {
        parseShape(shape.connections[0], child1);
    }

    // If second Child exist
    let child2 = getFirstChildByClassName(parent, "child2");
    if (child2 != null && shape.connections[1] != null && shape.connections[1].length != 0) {
        parseShape(shape.connections[1], child2);
    }
}

function parseConfig(config) {
    if (config.assembly != null) {
        let parent = document.createElementNS("http://www.w3.org/2000/svg", "g");

        let c = document.getElementById("mySvg");

        // Add Rotation and set the origin of the group
        parent.setAttribute("transform", `translate(${c.clientWidth / 2}, ${c.clientHeight * 5 / 6}), rotate(0)`);

        appendBase(parent);

        let child1 = getFirstChildByClassName(parent, "child1");

        if (child1 != null)
            parseShape(config.assembly, child1);

        c.appendChild(parent);
    }
}

const svgContainer = document.getElementById("mySvg");
let viewBox = { x: 0, y: 0, w: svgContainer.clientWidth, h: svgContainer.clientHeight };
let svgSize = { w: svgContainer.clientWidth, h: svgContainer.clientHeight };
let isPanning = false;
let startPoint = { x: 0, y: 0 };
let endPoint = { x: 0, y: 0 };
let scale = 1;

svgContainer.onmousewheel = function (e) {
    e.preventDefault();
    var w = viewBox.w;
    var h = viewBox.h;
    var mx = e.offsetX;
    var my = e.offsetY;
    var dw = -1 * w * Math.sign(e.deltaY) * 0.1;
    var dh = -1 * h * Math.sign(e.deltaY) * 0.1;
    var dx = dw * mx / svgSize.w;
    var dy = dh * my / svgSize.h;
    viewBox = { x: viewBox.x + dx, y: viewBox.y + dy, w: viewBox.w - dw, h: viewBox.h - dh };
    scale = svgSize.w / viewBox.w;
    svgContainer.setAttribute('viewBox', `${viewBox.x} ${viewBox.y} ${viewBox.w} ${viewBox.h}`);
}

svgContainer.onmousedown = function (e) {
    isPanning = true;
    startPoint = { x: e.x, y: e.y };
}

svgContainer.onmousemove = function (e) {
    if (isPanning) {
        endPoint = { x: e.x, y: e.y };
        var dx = (startPoint.x - endPoint.x) / scale;
        var dy = (startPoint.y - endPoint.y) / scale;
        var movedViewBox = { x: viewBox.x + dx, y: viewBox.y + dy, w: viewBox.w, h: viewBox.h };
        svgContainer.setAttribute('viewBox', `${movedViewBox.x} ${movedViewBox.y} ${movedViewBox.w} ${movedViewBox.h}`);
    }
}

svgContainer.onmouseup = function (e) {
    if (isPanning) {
        endPoint = { x: e.x, y: e.y };
        var dx = (startPoint.x - endPoint.x) / scale;
        var dy = (startPoint.y - endPoint.y) / scale;
        viewBox = { x: viewBox.x + dx, y: viewBox.y + dy, w: viewBox.w, h: viewBox.h };
        svgContainer.setAttribute('viewBox', `${viewBox.x} ${viewBox.y} ${viewBox.w} ${viewBox.h}`);
        isPanning = false;
    }
}

svgContainer.onmouseleave = svgContainer.onmouseup;

function getFirstChildByClassName(parent, className) {
    let child = null;

    if (!parent.hasChildNodes())
        return null;

    let len = parent.childElementCount;
    let i = 0;
    for (i = 0; i < len; ++i) {
        if (parent.childNodes[i].getAttribute("class") == className)
            return parent.childNodes[i];
        else
            child = getFirstChildByClassName(parent.childNodes[i], className);
    }

    return child;
}

// Call it when document is ready
document.addEventListener('DOMContentLoaded', (function () {
    console.log("assembly ready !");

    parseConfig(config);
}));