var CUBE_W = 0.01, CUBE_H = 0.01;

function init_styles() {
    make_style("snake", CUBE_W, CUBE_H, "shaders/shared.vert", "shaders/snake.frag");
    make_style("apple", CUBE_W, CUBE_H, "shaders/shared.vert", "shaders/apple.frag");
}

function draw_snake_dot(location) {
    draw_style("snake", (location.x / ROWS), (location.y / COLS));
}

function draw_food_dot(location) {
    draw_style("apple", (location.x / ROWS), (location.y / COLS));
}
