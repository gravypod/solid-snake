
function draw_snake_dot(location) {
    cube(location.x, location.y, false);
}

function draw_food_dot(location) {
    cube(location.x, location.y, true);
}
