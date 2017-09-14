var game = {}; // Game Object
var up = 119, left = 97, down = 115, right = 100; // KeyCodes end
var ROWS = 100, COLS = 100;

function draw_snake_dot(location) {
    cube(location.x, location.y, false);
}

function draw_food_dot(location) {
    cube(location.x, location.y, true);
}

function is_same_dot(a, b) {
    return a.x === b.x && a.y === b.y;
}

function is_location_in_snake(loc) {
    return game.dots.filter(function (dot) {
        return is_same_dot(dot, loc);
    }).length > 0;
}

function make_location() {
    while (true) {
        var location = {
            x: Math.floor(Math.random() * (ROWS)),
            y: Math.floor(Math.random() * (COLS))
        };

        if (!is_location_in_snake(location))
            return location;
    }
}

// Limit update speed
var total_d = 0;

function is_ready_for_tick(delta) {
    total_d -= delta;
    if (total_d > 100) {
        total_d = 0;
        return true;
    }
    return false;
}

game.init = function () {
    game.keypressed = null;

    game.num_dots = 3;
    game.dots = [];

    game.direction = null;

    game.food = null;

    game.dots.push(make_location());
    game.food = make_location();

};

game.update = function (delta) {
    if (!is_ready_for_tick(delta) || game.dots.length === 0)
        return;

    var last_dot = game.dots[game.dots.length - 1];

    if (is_same_dot(last_dot, game.food)) {
        game.num_dots += 1;
        game.food = make_location();
    }

    // Movement
    if (game.direction !== null) {
        var next = {x: last_dot.x, y: last_dot.y};
        //print("Adding next dot");
        switch (game.direction) {
            case up:
                next.y += 1;
                break;
            case down:
                next.y -= 1;
                break;
            case left:
                next.x -= 1;
                break;
            case right:
                next.x += 1;
                break;
        }


        // Fail states
        if (next.x < 0 || next.x > ROWS || next.y < 0 || next.y > COLS || is_location_in_snake(next)) {
            game.init();
            return;
        }

        // If we didn't die, take this new location and push it into the snake.
        game.dots.push(next);

    }

    // Remove oldest dots from snake.
    while (game.dots.length > game.num_dots)
        game.dots.shift();
};

game.render = function () {
    // Draw the snake
    game.dots.forEach(draw_snake_dot);
    if (game.food)
        draw_food_dot(game.food)
};

game.keypress = function (key) {
    // Skip directions that don't make sense
    var is_conflicting = function (a, b) {
        return game.direction === a && key === b || game.direction === b && key === a;
    };
    // Make sure this direction makes sense
    if (is_conflicting(up, down) || is_conflicting(left, right))
        return;
    // Make sure they clicked a real button
    if (up !== key && down !== key && left !== key && right !== key)
        return;

    // Update the direction
    game.direction = key;
}
