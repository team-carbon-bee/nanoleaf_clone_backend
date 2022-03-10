var size_of_triangle = 100;

function createTriangle(origin_x, origin_y, rotation) {
    var triangle = document.createElementNS("http://www.w3.org/2000/svg", "polygon");

    var h = (Math.sqrt(3) * size_of_triangle / 2).toFixed(2);
    var points = `${-(size_of_triangle / 2)} 0 ${size_of_triangle / 2} 0 0 ${h}`

    // Add Point
    triangle.setAttribute("points", points);

    // Add Rotation and set the origin of the triangle
    triangle.setAttribute("transform", `translate(${origin_x}, ${origin_y}), rotate(${rotation})`);

    // Add Class
    triangle.setAttribute("class", "triangle");

    return triangle;
}

function createGroupeOfTriangle() {
    var group = document.createElementNS("http://www.w3.org/2000/svg", "g");
    group.appendChild(createTriangle(250, 50, 0));
    group.appendChild(createTriangle(450, 50, 60));
    group.appendChild(createTriangle(50, 50, -60));
    return group;
}

// Call it when document is ready
document.addEventListener('DOMContentLoaded', (function () {
    console.log("ready !");
    var c = document.getElementById("mySvg");
    var g = createGroupeOfTriangle();
    c.appendChild(g);
    // triangle = createTriangle(50, 50, 0, c);
    // c.appendChild(triangle);
}));