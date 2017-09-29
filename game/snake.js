var game = {}; // Game Object
var up = 87, left = 65, down = 83, right = 68; // KeyCodes end
var MIN_ROW = 1, MAX_ROW = 99;
var MIN_COL = 1, MAX_COL = 99;

include("location.js");

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
        var is_off_screen = function (dot) {
            return dot.x < MIN_ROW || dot.x > (MAX_ROW - 1) ||
                   dot.y < MIN_COL || dot.y > (MAX_COL - 1);
        };
        var next = make_location_to(game.direction, last_dot);

        // Fail states
        if (is_off_screen(next) || game.dots.is_inside(next)) {
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

};

game.render = function () {
    var draw_type = function (type) {
        return function (location) { draw(type, location.x / 100, location.y / 100); }
    };
    var draw_snake = draw_type("snake");
    var draw_food  = draw_type("apple");

    [0, 99].forEach(function (x) {
        for (var y = 0; y < 100; y++) {
            draw_snake({x: x, y: y});
            draw_snake({x: y, y: x});
        }
    });

    // Draw the snake
    game.dots.forEach(draw_snake);
    if (game.food)
        draw_food(game.food);
};

game.keypress = function (key, pressed) {

    if (!pressed)
        return;

    if (key === 0) {
        game.init();
        return;
    }

    // Prevent switching the snake's direction twice in the same update loop.
    if (game.direction_changed)
        return;

    // Only handle when they click a key we care about (up, down, left, right).
    if ([up, down, left, right].indexOf(key) === -1)
        return;

    // Make sure the player doesn't move into themselves.
    var is_conflicting = function (a, b) {
        return game.direction === a && key === b || game.direction === b && key === a;
    };
    if (is_conflicting(up, down) || is_conflicting(left, right))
        return;

    // Update the direction
    game.direction = key;
    game.direction_changed = true;
};

game.mouse_move   = function (x, y) {};
game.mouse_click  = function (button, pressed) {};
game.mouse_scroll = function (change_x, change_y) {};