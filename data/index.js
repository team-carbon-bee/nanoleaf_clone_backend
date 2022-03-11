let config =
{
    "assembly": {
        "shape": {
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
    },
    "parameters": {
        "ledPerTriangle": 21,
        "ledModel": "rgb",
        "hostname": "nanoleaf_clone"

    }
};

let size_of_triangle = 100;

function appendTriangle(parent) {
    let triangle = document.createElementNS("http://www.w3.org/2000/svg", "polygon");

    let h = (Math.sqrt(3) * size_of_triangle / 2).toFixed(2);
    let points = `${-(size_of_triangle / 2)} 0 ${size_of_triangle / 2} 0 0 ${-h}`

    // Add Point
    triangle.setAttribute("points", points);
    // Add Class
    triangle.setAttribute("class", "triangle");
    // Append Triangle
    parent.appendChild(triangle);

    // Create group for child 1
    let child1 = document.createElementNS("http://www.w3.org/2000/svg", "g");
    // Add Rotation and set the origin of the child 1
    child1.setAttribute("transform", `translate(${-size_of_triangle / 4 - 2}, ${-h / 2 - 2}), rotate(-60)`);
    // Append child 1
    parent.appendChild(child1);

    // Create group for child 2
    let child2 = document.createElementNS("http://www.w3.org/2000/svg", "g");
    // Add Rotation and set the origin of the child 2
    child2.setAttribute("transform", `translate(${size_of_triangle / 4 + 2}, ${-h / 2 - 2}), rotate(60)`);
    // Append child 2
    parent.appendChild(child2);

}

function appendBase(parent) {
    let base = document.createElementNS("http://www.w3.org/2000/svg", "polygon");

    let h = (Math.sqrt(3) * size_of_triangle / 2 / 4).toFixed(2);
    let points = `${-(size_of_triangle / 2)} 2 ${-(size_of_triangle / 2) + (h * Math.tan(60*Math.PI/360))} ${h} ${(size_of_triangle / 2) - (h * Math.tan(60*Math.PI/360))} ${h} ${size_of_triangle / 2} 2`

    console.log(size_of_triangle * Math.tan(60*Math.PI/360));

    // Add Point
    base.setAttribute("points", points);
    // Add Class
    base.setAttribute("class", "base");

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
        parseShape(shape.connections[0], parent.childNodes[1]);
    }
    // If second Child exist
    if (shape.connections[1] != null && shape.connections[1].length != 0) {
        console.log("second child");
        parseShape(shape.connections[1], parent.childNodes[2]);
    }
}

function parseConfig(config) {
    if (config.assembly.shape != null) {
        let parent = document.createElementNS("http://www.w3.org/2000/svg", "g");

        let c = document.getElementById("mySvg");

        // Add Rotation and set the origin of the group
        parent.setAttribute("transform", `translate(${c.getAttribute('width')/2}, ${c.getAttribute('height')*5/6}), rotate(0)`);

        parseShape(config.assembly.shape, parent);

        appendBase(parent);

        c.appendChild(parent);
    }
}


// Call it when document is ready
document.addEventListener('DOMContentLoaded', (function () {
    console.log("ready !");

    parseConfig(config);
}));