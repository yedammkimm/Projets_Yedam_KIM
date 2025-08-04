// function run by Game.html and Player.html
function updateWebSocket(){
    // when received update about a Game, just display it
    socket.on('update{{ GameName }}', async function (msg) {
       msg = JSON.parse(msg);

        if (!maze && msg.hasOwnProperty('lab')){
            maze = new Maze(canvas, ctx, msg.sizeX, msg.sizeY);
            maze.labyrinth = msg.lab;
            maze.extraTile = msg.extra;
            maze.history = msg.history;
            maze.canvas.width = (maze.dimensions.x+2)*64;
            maze.canvas.height = (maze.dimensions.y+2)*64;
            await loadImages();
            maze.draw();
            maze.isPaused = false;
        }

        maze.history.push(msg.lastInsert);
        // update comments
        if (msg.comments) {
            let comments = document.getElementById('comments');
            comments.innerHTML += msg.comments + "</br>";
            comments.scrollTop = comments.scrollHeight;
        }
    });
}

/* register to endOfGame and display it when received*/
function displayEndOfGame(){
    socket.on('endOfGame', function(msg){
        document.getElementById('endOfGame').innerHTML = msg;
    });
}


function createVariants(image) {
    let variants = [];
    for (let i = 1; i < 4; i++) {
        let variant_canvas = document.createElement("canvas");
        let variant_ctx = variant_canvas.getContext("2d");
        variant_canvas.width = 64;
        variant_canvas.height = 64;
        variant_ctx.translate(32, 32);
        variant_ctx.rotate((Math.PI/2)*i);
        variant_ctx.drawImage(image, -32, -32, 64, 64);
        variants.push(variant_canvas);
    }
    return variants;
}

function loadImages () {
    let counter = 0;
    return new Promise((resolve, reject) => {
        for (let i in images) {
            let image = new Image();
            image.onload = function() {
                if (!maze.sprites[i]) maze.sprites[i] = [];

                maze.sprites[i][0] = image;
                maze.sprites[i].push(...createVariants(image));

                // When all images are loaded, resolve promise
                if (++counter === images.length) resolve();
            };
            image.crossOrigin="anonymous"
            image.src = images[i];
        }
    });
}

let canvas = document.getElementById("maze");
let ctx = canvas.getContext("2d");

let maze = false;

let images = [
    "/data/game/images/L.png",
    "/data/game/images/I.png",
    "/data/game/images/T.png",
    "/data/game/images/Crown.png",
    "/data/game/images/Player_1.png",
    "/data/game/images/Player_2.png"
];


document.onkeydown = async function (e) {

    e = e || window.event;

    if (maze.isAnimating) return;
    if (e.keyCode == '32') {
        maze.isPaused = false;
    }
    else if (e.keyCode == '37') {
        if (maze.historyPointer < 0) return;
        maze.isPaused = true;
        maze.isAnimating = true;
        await maze.insert(maze.opposite(maze.history[maze.historyPointer].insert), maze.history[maze.historyPointer].number);
        maze.players[0].x = maze.history[maze.historyPointer].playerPos[0][0];
        maze.players[0].y = maze.history[maze.historyPointer].playerPos[0][1];
        maze.players[0].item = maze.history[maze.historyPointer].itemPos[0];

        maze.players[1].x = maze.history[maze.historyPointer].playerPos[1][0];
        maze.players[1].y = maze.history[maze.historyPointer].playerPos[1][1];
        maze.players[1].item = maze.history[maze.historyPointer].itemPos[1];
        maze.draw();
        maze.historyPointer--;
        maze.isAnimating = false;
    }
    else if (e.keyCode == '39') {
        if (maze.historyPointer >= maze.history.length-1) return;
        maze.isPaused = true;
        maze.isAnimating = true;
        maze.historyPointer++;
        maze.extraTile = maze.history[maze.historyPointer].inserted;
        await maze.insert(maze.history[maze.historyPointer].insert, maze.history[maze.historyPointer].number);
        maze.players[0].x = maze.history[maze.historyPointer].playerPos[0][0];
        maze.players[0].y = maze.history[maze.historyPointer].playerPos[0][1];
        maze.players[0].item = maze.history[maze.historyPointer].itemPos[0];

        maze.players[1].x = maze.history[maze.historyPointer].playerPos[1][0];
        maze.players[1].y = maze.history[maze.historyPointer].playerPos[1][1];
        maze.players[1].item = maze.history[maze.historyPointer].itemPos[1];
        maze.draw();
        maze.isAnimating = false;
    }

}

setInterval(async function() {
    if (maze && !maze.isAnimating && maze.historyPointer !== maze.history.length-1 && !maze.isPaused) {
        maze.isAnimating = true;
        for (let i = maze.historyPointer+1; i < maze.history.length; i++) {
            maze.extraTile = maze.history[i].inserted;
            await maze.insert(maze.history[i].insert, maze.history[i].number);
            maze.players[0].x = maze.history[i].playerPos[0][0];
            maze.players[0].y = maze.history[i].playerPos[0][1];
            maze.players[0].item = maze.history[i].itemPos[0];

            maze.players[1].x = maze.history[i].playerPos[1][0];
            maze.players[1].y = maze.history[i].playerPos[1][1];
            maze.players[1].item = maze.history[i].itemPos[1];
            maze.draw();
            maze.historyPointer++;
        }
        maze.isAnimating = false;
    }
}, 100);