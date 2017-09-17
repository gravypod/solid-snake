include("drawing.js");
include("location.js");

var game = {}; // Game Object
var up = 87, left = 65, down = 83, right = 68; // KeyCodes end
var ROWS = 100, COLS = 100;


game.init = function () {
    game.keypressed = null;

    game.num_dots = 3;
    game.dots = [];
    game.dots.get_head = function () {
        return game.dots[game.dots.length - 1];
    };
    game.dots.is_inside = function (loc) {
        return game.dots.filter(function (dot) { return is_same_location(dot, loc); }).length !== 0;
    };
    game.dots.find_empty_location = function () {
        while (true) {
            var loc = random_location();
            if (!game.dots.is_inside(loc))
                return loc;
        }
    };

    game.direction = null;

    game.food = null;

    game.dots.push(game.dots.find_empty_location());
    game.food = game.dots.find_empty_location();

    game.direction_changed = false;

    game.total_wait = 0;
};

game.update = function (delta) {

    game.total_wait += delta * 100;
    if (game.total_wait <= 5 || game.dots.length === 0) {
        return;
    }
    game.total_wait = 0;

    var last_dot = game.dots.get_head();

    if (is_same_location(last_dot, game.food)) {
        game.num_dots += 1;
        game.food = game.dots.find_empty_location();
    }

    // Movement
    if (game.direction !== null) {
        var next = make_location_to(game.direction, last_dot);

        // Fail states
        if (next.x < 0 || next.x > ROWS || next.y < 0 || next.y > COLS || game.dots.is_inside(next)) {
            game.init();
            return;
        }

        // If we didn't die, take this new location and push it into the snake.
        game.dots.push(next);
        game.direction_changed = false;
    }

    // Remove oldest dots from snake.
    while (game.dots.length > game.num_dots)
        game.dots.shift();

    game.updating = false;
};

game.render = function () {
    // Draw the snake
    game.dots.forEach(draw_snake_dot);
    if (game.food)
        draw_food_dot(game.food)
};

game.keypress = function (key, pressed) {

    if (pressed !== true)
        return;

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

    // Protect against people who type too fast.
    if (game.direction_changed)
        return;

    // Update the direction
    game.direction = key;
    game.direction_changed = true;
};