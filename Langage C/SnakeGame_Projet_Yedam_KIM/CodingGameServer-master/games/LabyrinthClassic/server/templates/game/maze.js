class Maze {
    constructor(canvas, context, sizeX, sizeY) {
        this.dimensions = {
            x: sizeX,
            y: sizeY
        }
        this.canvas = canvas;
        this.ctx = context;
        this.labyrinth = [];
        this.extraTile = [];
        this.sprites = [];
        this.shift = 0;
        this.history = [];
        this.historyPointer = -1;
        this.isAnimating = false;
        this.isPaused = true;
        this.players = [
            {
                x: 0,
                y: 0,
                item: 1
            },
            {
                x: this.dimensions.x-1,
                y: this.dimensions.y-1,
                item: 24
            }
        ];
    }

    draw() {
        this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
        for (let i = 0; i < this.dimensions.x; i++) {
            for (let j = 0; j < this.dimensions.y; j++) {
                this.ctx.drawImage(this.getSprite(this.labyrinth[j][i], [j, i]), (i + 1) * 64, (j + 1) * 64, 64, 64);
            }
        }

        let extraTileCanvas = document.getElementById("extra_tile");
        let extraTileCtx = extraTileCanvas.getContext("2d");
        extraTileCtx.drawImage(this.getSprite(this.extraTile, [-1, -1]), 0, 0, 64, 64);
    }

    async insert(insertion, number) {
        console.log(`maze.insert(${insertion}, ${number})`);
        return new Promise(async resolve => {
            this.draw();

            await this.animate(insertion, number);

            let extraTile;
            if (insertion === 0) {
                extraTile = this.labyrinth[number][this.dimensions.x-1];
                for (let i = this.dimensions.x - 2; i >= 0; i--) {
                    this.labyrinth[number][i + 1] = this.labyrinth[number][i];
                }
                this.labyrinth[number][0] = this.extraTile;

                if (this.players[0].y === number) this.players[0].x = (this.players[0].x+1)%this.dimensions.x;
                if (this.players[1].y === number) this.players[1].x = (this.players[1].x+1)%this.dimensions.x;
            } else if (insertion === 1) {
                extraTile = this.labyrinth[number][0];
                for (let i = 1; i < this.dimensions.x; i++) {
                    this.labyrinth[number][i - 1] = this.labyrinth[number][i];
                }
                this.labyrinth[number][this.dimensions.x - 1] = this.extraTile;

                if (this.players[0].y === number) {
                    this.players[0].x = (this.players[0].x-1);
                    if(this.players[0].x < 0) this.players[0].x = this.dimensions.x-1;
                }
                if (this.players[1].y === number) {
                    this.players[1].x = (this.players[1].x-1);
                    if(this.players[1].x < 0) this.players[1].x = this.dimensions.x-1;
                }
            } else if (insertion === 2) {
                extraTile = this.labyrinth[this.dimensions.y-1][number];
                for (let i = this.dimensions.y - 2; i >= 0; i--) {
                    this.labyrinth[i + 1][number] = this.labyrinth[i][number];
                }
                this.labyrinth[0][number] = this.extraTile;

                if (this.players[0].x === number) this.players[0].y = (this.players[0].y+1)%this.dimensions.x;
                if (this.players[1].x === number) this.players[1].y = (this.players[1].y+1)%this.dimensions.x;
            } else if (insertion === 3) {
                extraTile = this.labyrinth[0][number];
                for (let i = 1; i < this.dimensions.y; i++) {
                    this.labyrinth[i - 1][number] = this.labyrinth[i][number];
                }
                this.labyrinth[this.dimensions.y - 1][number] = this.extraTile;

                if (this.players[0].x === number) {
                    this.players[0].y = (this.players[0].y-1);
                    if(this.players[0].y < 0) this.players[0].y = this.dimensions.y-1;
                }
                if (this.players[1].x === number) {
                    this.players[1].y = (this.players[1].y-1);
                    if(this.players[1].y < 0) this.players[1].y = this.dimensions.y-1;
                }
            }

            this.extraTile = extraTile;

            this.draw();
            resolve();
        });
    }

    animate(insertion, number) {
        // Clear the this.canvas
        this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);

        let coord = [];
        if (insertion === 0) {
            coord[0] = 0;
            coord[1] = number;
        } else if (insertion === 1) {
            coord[0] = this.dimensions.x-1;
            coord[1] = number;
        } else if (insertion === 2) {
            coord[0] = number;
            coord[1] = 0;
        } else if (insertion === 3) {
            coord[0] = number;
            coord[1] = this.dimensions.y-1;
        }

        if (insertion < 2) {
            this.ctx.drawImage(this.getSprite(this.extraTile, [-1, -1]), this.shift + (coord[0] + 1) * 64 + ((insertion === 0) ? -64 : 64), (coord[1] + 1) * 64, 64, 64);
        } else {
            this.ctx.drawImage(this.getSprite(this.extraTile, [-1, -1]), (coord[0] + 1) * 64, this.shift + (coord[1] + 1) * 64 + ((insertion === 2) ? -64 : 64), 64, 64);
        }

        // Draw sliding and fixed images
        for (let i = 0; i < this.dimensions.x; i++) {
            for (let j = 0; j < this.dimensions.y; j++) {
                if (insertion < 2) {
                    if (j === number) this.ctx.drawImage(this.getSprite(this.labyrinth[j][i], [j, i]), this.shift + (i + 1) * 64, (j + 1) * 64, 64, 64);
                    else this.ctx.drawImage(this.getSprite(this.labyrinth[j][i], [j, i]), (i + 1) * 64, (j + 1) * 64, 64, 64);
                } else if (insertion >= 2) {
                    if (i === number) this.ctx.drawImage(this.getSprite(this.labyrinth[j][i], [j, i]), (i + 1) * 64, this.shift + (j + 1) * 64, 64, 64);
                    else this.ctx.drawImage(this.getSprite(this.labyrinth[j][i], [j, i]), (i + 1) * 64, (j + 1) * 64, 64, 64);
                }
            }
        }

        // If the images have reached their final destination, end movement
        if ((insertion % 2 === 1 && this.shift < -64) || (insertion % 2 === 0 && this.shift > 64)) {
            this.shift = 0;
            Promise.resolve();
            return;
        }

        // Slide the images
        if (insertion % 2 === 1) this.shift -= 1;
        else if (insertion % 2 === 0) this.shift += 1;

        let self = this;
        return new Promise(resolve => {
            // Request another frame
            requestAnimationFrame(resolve);
        }).then(function () {
            return self.animate(insertion, number);
        });
    }

    getSprite(tile, coordinates) {
        let sprite;
        // L sprites
        if (tile[0] && tile[1] && !tile[2] && !tile[3]) sprite = this.sprites[0][0];
        else if (!tile[0] && tile[1] && tile[2] && !tile[3]) sprite = this.sprites[0][1];
        else if (!tile[0] && !tile[1] && tile[2] && tile[3]) sprite = this.sprites[0][2];
        else if (tile[0] && !tile[1] && !tile[2] && tile[3]) sprite = this.sprites[0][3];

        // I sprites
        else if (!tile[0] && tile[1] && !tile[2] && tile[3]) sprite = this.sprites[1][0];
        else if (tile[0] && !tile[1] && tile[2] && !tile[3]) sprite = this.sprites[1][1];

        // T sprites
        else if (!tile[0] && !tile[1] && tile[2] && !tile[3]) sprite = this.sprites[2][0];
        else if (!tile[0] && !tile[1] && !tile[2] && tile[3]) sprite = this.sprites[2][1];
        else if (tile[0] && !tile[1] && !tile[2] && !tile[3]) sprite = this.sprites[2][2];
        else if (!tile[0] && tile[1] && !tile[2] && !tile[3]) sprite = this.sprites[2][3];

        let temp_canvas = document.getElementById("temp");
        let context = temp_canvas.getContext("2d");
        context.clearRect(0, 0, temp_canvas.width, temp_canvas.height);

        let playerOnTile = false;
        if (this.players[0].y === coordinates[0] && this.players[0].x === coordinates[1]) {
            context.drawImage(sprite, 0, 0, 64, 64);
            context.drawImage(this.sprites[4][0], (!tile[4]) ? 12 : 30, 5, 40, 40);
            sprite = temp_canvas;
            playerOnTile = true;
        }

        if (this.players[1].y === coordinates[0] && this.players[1].x === coordinates[1]) {
            context.drawImage(sprite, 0, 0, 64, 64);
            context.drawImage(this.sprites[5][0], (!tile[4]) ? 12 : 30, 5, 40, 40);
            sprite = temp_canvas;
            playerOnTile = true;
        }

        if (tile[4] !== 0) {
            context.drawImage(sprite, 0, 0, 64, 64);
            context.beginPath();
            context.arc((!playerOnTile) ? 32 : 16, 32, 12, 0, 2 * Math.PI, false);
            if (this.players[0].item === tile[4] && this.players[1].item === tile[4]) {
                context.fillStyle = '#fa37fa';
            } else if (this.players[0].item === tile[4]) {
                context.fillStyle = '#4287f5';
            } else if (this.players[1].item === tile[4]) {
                context.fillStyle = '#37c43e';
            } else {
                context.fillStyle = '#666666';
            }
            context.fill();
            context.lineWidth = 2
            context.strokeStyle = '#000000';
            context.stroke();
            context.font = "12px Arial";
            context.fillStyle = "white";
            context.textAlign = "center";
            context.fillText(tile[4], (!playerOnTile) ? 32 : 16, 36);
            sprite = temp_canvas;
        }

        return sprite;
    }

    opposite(insertion){
        let opposite = [1, 0, 3, 2];
        return opposite[insertion];
    }
}
