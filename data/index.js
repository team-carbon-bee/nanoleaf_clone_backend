
function onBtnStartClick() {
    api_rest_power_on()
        .catch((error) => {
            toastErrorShow(error);
        });
}

function onBtnStopClick() {
    api_rest_power_off()
        .catch((error) => {
            toastErrorShow(error);
        });
}

// Base on real dimension
const base_size_of_triangle = 155;
const cut_size_of_triangle = 16;
const cut_size_of_base = 90;
const padding_between_triangle = 1.5;

const leds_position = {
    '0': {
        'x': 11.5,
        'y': -7,
        'r': 0
    },
    '1': {
        'x': 28.2,
        'y': -7,
        'r': 0
    },
    '2': {
        'x': 44.9,
        'y': -7,
        'r': 0
    },
    '3': {
        'x': 57.2,
        'y': -12,
        'r': -60
    },
    '4': {
        'x': 54.5,
        'y': -25.7,
        'r': -120
    },
    '5': {
        'x': 46.5,
        'y': -39.8,
        'r': -120
    },
    '6': {
        'x': 38.2,
        'y': -54.1,
        'r': -120
    },
    '7': {
        'x': 26.6,
        'y': -74.2,
        'r': -120
    },
    '8': {
        'x': 18.3,
        'y': -88.6,
        'r': -120
    },
    '9': {
        'x': 10.2,
        'y': -102.6,
        'r': -120
    },
    '10': {
        'x': 0,
        'y': -110.8,
        'r': 180
    },
    '11': {
        'x': -10.2,
        'y': -102.6,
        'r': -240
    },
    '12': {
        'x': -18.3,
        'y': -88.6,
        'r': -240
    },
    '13': {
        'x': -26.6,
        'y': -74.2,
        'r': -240
    },
    '14': {
        'x': -38.2,
        'y': -54.1,
        'r': -240
    },
    '15': {
        'x': -46.5,
        'y': -39.8,
        'r': -240
    },
    '16': {
        'x': -54.5,
        'y': -25.7,
        'r': -240
    },
    '17': {
        'x': -57.2,
        'y': -12,
        'r': -300
    },
    '18': {
        'x': -44.9,
        'y': -7,
        'r': 0
    },
    '19': {
        'x': -28.2,
        'y': -7,
        'r': 0
    },
    '20': {
        'x': -11.5,
        'y': -7,
        'r': 0
    }
};

function appendLedToTriangle(led_elm) {

    for (let pos in leds_position) {
        const led = leds_position[pos];
        let circle = document.createElementNS("http://www.w3.org/2000/svg", "circle");
        circle.setAttribute("transform", `translate(${led.x}, ${led.y}), rotate(${led.r})`);
        circle.setAttribute("r", 5);
        circle.setAttribute("id", `led_${pos}`);
        circle.setAttribute("fill-opacity", 0);

        led_elm.appendChild(circle);
    }
}

function appendTriangle(parent) {
    let group = document.createElementNS("http://www.w3.org/2000/svg", "g");
    let triangle = document.createElementNS("http://www.w3.org/2000/svg", "polygon");

    let h = (Math.sqrt(3) * base_size_of_triangle / 2);
    let l = cut_size_of_triangle;
    let hyp = (l / Math.cos(30 * Math.PI / 180));
    let points = `${-(base_size_of_triangle / 2) + hyp}                                     0
                    ${-(base_size_of_triangle / 2) + (hyp / 2)}                             ${-l}
                    ${-hyp / 2}                                                             ${-h + l}
                    ${hyp / 2}                                                              ${-h + l}
                    ${(base_size_of_triangle / 2) - (hyp / 2)}                              ${-l}
                    ${(base_size_of_triangle / 2) - hyp}                                    0`

    // Add Point
    triangle.setAttribute("points", points);
    // Add Class
    triangle.setAttribute("class", "triangle");

    // Append Triangle to group
    group.appendChild(triangle);

    // Create group for LED
    let led = document.createElementNS("http://www.w3.org/2000/svg", "g");
    led.setAttribute("class", "leds");
    appendLedToTriangle(led);

    // Append LED to group
    group.appendChild(led);

    // Create group for child 1
    let child1 = document.createElementNS("http://www.w3.org/2000/svg", "g");
    // Add Rotation and set the origin of the child 1
    child1.setAttribute("transform", `translate(${-base_size_of_triangle / 4 - (padding_between_triangle / Math.tan(30 * Math.PI / 180))}, ${-h / 2 - padding_between_triangle}), rotate(-60)`);
    child1.setAttribute("class", "child1");

    // Append child 1 to group
    group.appendChild(child1);

    // Create group for child 2
    let child2 = document.createElementNS("http://www.w3.org/2000/svg", "g");
    // Add Rotation and set the origin of the child 2
    child2.setAttribute("transform", `translate(${base_size_of_triangle / 4 + (padding_between_triangle / Math.tan(30 * Math.PI / 180))}, ${-h / 2 - padding_between_triangle}), rotate(60)`);
    child2.setAttribute("class", "child2");

    // Append child 2 to group
    group.appendChild(child2);

    // Append group to parent
    parent.appendChild(group);
}

function appendBase(parent) {
    let group = document.createElementNS("http://www.w3.org/2000/svg", "g");

    group.setAttribute("transform", `rotate(180) translate(0, ${-padding_between_triangle * 2})`);

    let triangle = document.createElementNS("http://www.w3.org/2000/svg", "polygon");

    let h = (Math.sqrt(3) * base_size_of_triangle / 2);
    let l = cut_size_of_triangle;
    let hyp = (l / Math.cos(30 * Math.PI / 180));
    let l2 = cut_size_of_base;
    let hyp2 = (l2 / Math.cos(30 * Math.PI / 180));
    let points = `${-(base_size_of_triangle / 2) + hyp}                                     0
                    ${-(base_size_of_triangle / 2) + (hyp / 2)}                             ${-l}
                    ${-hyp2 / 2}                                                            ${-h + l2}
                    ${hyp2 / 2}                                                             ${-h + l2}
                    ${(base_size_of_triangle / 2) - (hyp / 2)}                              ${-l}
                    ${(base_size_of_triangle / 2) - hyp}                                    0`

    // Add Point
    triangle.setAttribute("points", points);
    // Add Class
    triangle.setAttribute("class", "base");

    // Append triangle to group
    group.appendChild(triangle);

    // Create group for child 1
    let child1 = document.createElementNS("http://www.w3.org/2000/svg", "g");
    // Add Rotation and set the origin of the child 1
    child1.setAttribute("transform", `translate(0, ${padding_between_triangle * 2}), rotate(-180)`);
    child1.setAttribute("class", "child1");

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
    if (child1 != null && shape.connections[0] != null && shape.connections[0].shape != null) {
        parseShape(shape.connections[0].shape, child1);
    }

    // If second Child exist
    let child2 = getFirstChildByClassName(parent, "child2");
    if (child2 != null && shape.connections[1] != null && shape.connections[1].shape != null) {
        parseShape(shape.connections[1].shape, child2);
    }
}

function parseConfig(config) {
    let parent = document.createElementNS("http://www.w3.org/2000/svg", "g");

    let c = document.getElementById("mySvg");

    // Add Rotation and set the origin of the group
    parent.setAttribute("transform", `translate(${c.clientWidth / 2}, ${c.clientHeight * 5 / 6}), rotate(0)`);

    appendBase(parent);

    if (config != null && config.assembly != null && config.assembly.shape != null) {
        let child1 = getFirstChildByClassName(parent, "child1");

        if (child1 != null)
            parseShape(config.assembly.shape, child1);
    }

    c.appendChild(parent);
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
    for (let i = 0; i < len; ++i) {
        if (parent.childNodes[i].getAttribute("class") == className)
            return parent.childNodes[i];
        else
            child = getFirstChildByClassName(parent.childNodes[i], className);
    }

    return child;
}

// Call it when document is ready
document.addEventListener('DOMContentLoaded', (function () {
    console.log("dashboard ready !");

    api_rest_read_configuration()
        .then((config) => {
            parseConfig(config);
        })
        .catch((error) => {
            toastErrorShow(error);
        })
}));