
function is_same_location(a, b) {
    return a.x === b.x && a.y === b.y;
}

function make_location_to(direction, from_location) {
    var location = {x: from_location.x, y: from_location.y};
    switch (direction) {
        case up:
            location.y += 1;
            break;
        case down:
            location.y -= 1;
            break;
        case left:
            location.x -= 1;
            break;
        case right:
            location.x += 1;
            break;
    }
    return location;
}


function random_location() {
    return {
        x: MIN_ROW + Math.floor(Math.random() * (MAX_ROW - 1)),
        y: MIN_COL + Math.floor(Math.random() * (MAX_COL - 1))
    };
}