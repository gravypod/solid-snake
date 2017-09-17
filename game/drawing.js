var CUBE_W = 0.01, CUBE_H = 0.01;


function draw_snake_dot(location) {
    cube(location.x / ROWS, location.y / COLS, CUBE_W, CUBE_H, false);
}

function draw_food_dot(location) {
    cube(location.x / ROWS, location.y / COLS, CUBE_W, CUBE_H, true);
}
