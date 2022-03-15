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

let size_of_triangle = 200;
let padding_between_triangle = 1.5;

function appendTriangle(parent) {
    let triangle = document.createElementNS("http://www.w3.org/2000/svg", "polygon");

    let h = (Math.sqrt(3) * size_of_triangle / 2);
    let l = size_of_triangle / 8;
    let hyp = (l / Math.cos(30 * Math.PI / 180));
    let points = `${-(size_of_triangle / 2) + hyp}                                        0
                    ${-(size_of_triangle / 2) + (hyp / 2)}                                  ${-l}
                    ${-hyp / 2}                                                             ${-h + l}
                    ${hyp / 2}                                                              ${-h + l}
                    ${(size_of_triangle / 2) - (hyp / 2)}                                   ${-l}
                    ${(size_of_triangle / 2) - hyp}                                         0`

    // Add Point
    triangle.setAttribute("points", points);
    // Add Class
    triangle.setAttribute("class", "triangle");
    // Append Triangle
    parent.appendChild(triangle);

    var connection = document.createElementNS("http://www.w3.org/2000/svg", "polygon");
    points =   `${-size_of_triangle/32}         ${-size_of_triangle/40}
                ${-size_of_triangle/32}         ${size_of_triangle/40 + padding_between_triangle}
                ${size_of_triangle/32}          ${size_of_triangle/40 + padding_between_triangle}
                ${size_of_triangle/32}          ${-size_of_triangle/40}`

    // Add Point
    connection.setAttribute("points", points);
    // Add Class
    connection.setAttribute("class", "connection");

    // Append connection
    parent.appendChild(connection);

    console.log("plop")
    // Create group for child 1
    let child1 = document.createElementNS("http://www.w3.org/2000/svg", "g");
    // Add Rotation and set the origin of the child 1
    child1.setAttribute("transform", `translate(${-size_of_triangle / 4 - (padding_between_triangle / Math.tan(30 * Math.PI / 180))}, ${-h / 2 - padding_between_triangle}), rotate(-60)`);
    // Append child 1
    parent.appendChild(child1);

    // Create group for child 2
    let child2 = document.createElementNS("http://www.w3.org/2000/svg", "g");
    // Add Rotation and set the origin of the child 2
    child2.setAttribute("transform", `translate(${size_of_triangle / 4 + (padding_between_triangle / Math.tan(30 * Math.PI / 180))}, ${-h / 2 - padding_between_triangle}), rotate(60)`);
    // Append child 2
    parent.appendChild(child2);
}

function appendBase(parent) {
    let base = document.createElementNS("http://www.w3.org/2000/svg", "g");

    base.setAttribute("transform", `rotate(180) translate(0, ${-padding_between_triangle})`);

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

    base.append(triangle);

    // Append Base
    parent.appendChild(base);
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
    if (shape.connections[0] != null && shape.connections[0].length != 0) {
        console.log("first child");
        parseShape(shape.connections[0], parent.childNodes[2]);
    }
    // If second Child exist
    if (shape.connections[1] != null && shape.connections[1].length != 0) {
        console.log("second child");
        parseShape(shape.connections[1], parent.childNodes[3]);
    }
}

function parseConfig(config) {
    if (config.assembly != null) {
        let parent = document.createElementNS("http://www.w3.org/2000/svg", "g");

        let c = document.getElementById("mySvg");

        // Add Rotation and set the origin of the group
        parent.setAttribute("transform", `translate(${c.getAttribute('width') / 2}, ${c.getAttribute('height') * 5 / 6}), rotate(0)`);

        parseShape(config.assembly, parent);

        appendBase(parent);

        c.appendChild(parent);
    }
}

const svgContainer = document.getElementById("mySvg");
let viewBox = { x: 0, y: 0, w: 1500, h: 750 };
let svgSize = { w: 1500, h: 750 };
let isPanning = false;
let startPoint = {x:0,y:0};
let endPoint = {x:0,y:0};
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


// Call it when document is ready
document.addEventListener('DOMContentLoaded', (function () {
    console.log("ready !");

    parseConfig(config);
}));