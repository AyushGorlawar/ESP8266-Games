#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "ayush@";       
const char* password = "ayush@123";   

ESP8266WebServer server(80);

// HTML content stored in PROGMEM to save RAM
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>ESP8266 Games</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body {
            font-family: Arial, sans-serif;
            background: #1a1a1a;
            color: #fff;
            min-height: 100vh;
        }
        .container {
            max-width: 800px;
            padding: 20px;
            margin: 0 auto;
        }
        h1 {
            text-align: center;
            color: #4CAF50;
            margin: 20px 0;
            font-size: 2.5em;
        }
        .game-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
            gap: 20px;
            margin-top: 20px;
        }
        .game-card {
            background: #2d2d2d;
            border-radius: 10px;
            padding: 20px;
            text-align: center;
            cursor: pointer;
            transition: all 0.3s ease;
        }
        .game-card:hover {
            transform: translateY(-5px);
            box-shadow: 0 0 20px rgba(76, 175, 80, 0.3);
        }
        .game-card h2 {
            color: #4CAF50;
            margin-bottom: 10px;
        }
        .game-card p {
            color: #bbb;
            font-size: 0.9em;
        }
        #game-area {
            width: 100%;
            min-height: 100vh;
            position: fixed;
            top: 0;
            left: 0;
            background: #1a1a1a;
            display: none;
            overflow-y: auto;
        }
        .back-btn {
            position: fixed;
            top: 20px;
            left: 20px;
            padding: 10px 20px;
            background: #4CAF50;
            border: none;
            border-radius: 5px;
            color: white;
            cursor: pointer;
            z-index: 1000;
        }
        
        /* Game Specific Styles */
        /* Tic Tac Toe */
        .ttt-board {
            display: grid;
            grid-template-columns: repeat(3, 1fr);
            gap: 10px;
            max-width: 300px;
            margin: 20px auto;
        }
        .ttt-cell {
            aspect-ratio: 1;
            background: #2d2d2d;
            border: none;
            border-radius: 5px;
            color: #fff;
            font-size: 2em;
            cursor: pointer;
        }
        
        /* Subway Runner */
        #gameCanvas {
            background: #2d2d2d;
            display: block;
            margin: 20px auto;
            border-radius: 10px;
        }
        .game-stats {
            position: fixed;
            top: 20px;
            right: 20px;
            background: rgba(0,0,0,0.7);
            padding: 10px;
            border-radius: 5px;
        }
        
        /* Sudoku */
        .sudoku-board {
            display: grid;
            grid-template-columns: repeat(9, 1fr);
            gap: 1px;
            max-width: 450px;
            margin: 20px auto;
            background: #2d2d2d;
            padding: 10px;
            border-radius: 10px;
        }
        .sudoku-cell {
            aspect-ratio: 1;
            width: 100%;
            background: #1a1a1a;
            border: none;
            color: #fff;
            font-size: 20px;
            text-align: center;
        }
        .sudoku-cell:focus {
            outline: 2px solid #4CAF50;
        }
        
        /* Memory Game */
        .memory-board {
            display: grid;
            grid-template-columns: repeat(4, 1fr);
            gap: 10px;
            max-width: 400px;
            margin: 20px auto;
        }
        .memory-card {
            aspect-ratio: 1;
            background: #2d2d2d;
            border-radius: 5px;
            cursor: pointer;
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 2em;
            transition: all 0.3s ease;
        }
        .memory-card.flipped {
            background: #4CAF50;
            transform: rotateY(180deg);
        }
        
        /* Common Game Elements */
        .game-status {
            text-align: center;
            margin: 20px 0;
            font-size: 1.5em;
            color: #4CAF50;
        }
        .game-controls {
            text-align: center;
            margin: 20px 0;
        }
        .game-btn {
            padding: 10px 20px;
            background: #4CAF50;
            border: none;
            border-radius: 5px;
            color: white;
            cursor: pointer;
            margin: 0 5px;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>ESP8266 Games</h1>
        <div class="game-grid">
            <div class="game-card" onclick="loadGame('tictactoe')">
                <h2>Tic Tac Toe</h2>
                <p>Classic X's and O's game</p>
            </div>
            <div class="game-card" onclick="loadGame('subway')">
                <h2>Subway Runner</h2>
                <p>Endless runner game</p>
            </div>
            <div class="game-card" onclick="loadGame('sudoku')">
                <h2>Sudoku</h2>
                <p>Number puzzle game</p>
            </div>
            <div class="game-card" onclick="loadGame('memory')">
                <h2>Memory Game</h2>
                <p>Match the pairs</p>
            </div>
        </div>
    </div>
    <div id="game-area"></div>

    <script>
        // Game Management
        let currentGame = null;
        const gameArea = document.getElementById('game-area');

        function showMenu() {
            document.querySelector('.container').style.display = 'block';
            gameArea.style.display = 'none';
            gameArea.innerHTML = '';
            if (currentGame && currentGame.cleanup) {
                currentGame.cleanup();
            }
        }

        function loadGame(game) {
            document.querySelector('.container').style.display = 'none';
            gameArea.style.display = 'block';
            gameArea.innerHTML = '<button class="back-btn" onclick="showMenu()">Back to Menu</button>';
            
            switch(game) {
                case 'tictactoe':
                    initTicTacToe();
                    break;
                case 'subway':
                    initSubwayRunner();
                    break;
                case 'sudoku':
                    initSudoku();
                    break;
                case 'memory':
                    initMemoryGame();
                    break;
            }
        }

        // Tic Tac Toe Implementation
        function initTicTacToe() {
            let board = Array(9).fill('');
            let currentPlayer = 'X';
            let gameActive = true;

            const gameContainer = document.createElement('div');
            gameContainer.innerHTML = `
                <h2 style="text-align: center; color: #4CAF50; margin: 20px 0;">Tic Tac Toe</h2>
                <div class="game-status"></div>
                <div class="ttt-board">
                    ${Array(9).fill('').map((_, i) => `
                        <button class="ttt-cell" data-index="${i}"></button>
                    `).join('')}
                </div>
                <div class="game-controls">
                    <button class="game-btn" onclick="initTicTacToe()">New Game</button>
                </div>
            `;
            gameArea.appendChild(gameContainer);

            const statusDisplay = gameContainer.querySelector('.game-status');
            const cells = gameContainer.querySelectorAll('.ttt-cell');

            statusDisplay.textContent = `Player ${currentPlayer}'s turn`;

            cells.forEach(cell => {
                cell.addEventListener('click', () => {
                    if (board[cell.dataset.index] || !gameActive) return;
                    
                    board[cell.dataset.index] = currentPlayer;
                    cell.textContent = currentPlayer;
                    cell.style.color = currentPlayer === 'X' ? '#4CAF50' : '#ff9800';

                    if (checkWinner()) {
                        statusDisplay.textContent = `Player ${currentPlayer} wins!`;
                        gameActive = false;
                        return;
                    }

                    if (board.every(cell => cell !== '')) {
                        statusDisplay.textContent = "Game is a draw!";
                        gameActive = false;
                        return;
                    }

                    currentPlayer = currentPlayer === 'X' ? 'O' : 'X';
                    statusDisplay.textContent = `Player ${currentPlayer}'s turn`;
                });
            });

            function checkWinner() {
                const winPatterns = [
                    [0,1,2], [3,4,5], [6,7,8],
                    [0,3,6], [1,4,7], [2,5,8],
                    [0,4,8], [2,4,6]
                ];
                return winPatterns.some(([a,b,c]) => 
                    board[a] && board[a] === board[b] && board[a] === board[c]
                );
            }
        }

        // Subway Runner Implementation
        function initSubwayRunner() {
            const gameContainer = document.createElement('div');
            gameContainer.innerHTML = `
                <canvas id="gameCanvas" width="800" height="400"></canvas>
                <div class="game-stats">
                    <div>Score: <span id="score">0</span></div>
                    <div>Distance: <span id="distance">0</span>m</div>
                </div>
            `;
            gameArea.appendChild(gameContainer);

            const canvas = document.getElementById('gameCanvas');
            const ctx = canvas.getContext('2d');
            
            const player = {
                x: 100,
                y: canvas.height - 50,
                width: 30,
                height: 30,
                speed: 5,
                jumpForce: -15,
                gravity: 0.8,
                velocityY: 0,
                isJumping: false
            };

            const obstacles = [];
            let score = 0;
            let distance = 0;
            let gameLoop;
            let gameSpeed = 5;

            function spawnObstacle() {
                if (Math.random() < 0.02) {
                    obstacles.push({
                        x: canvas.width,
                        y: canvas.height - 30,
                        width: 20,
                        height: 30
                    });
                }
            }

            function drawPlayer() {
                ctx.fillStyle = '#4CAF50';
                ctx.fillRect(player.x, player.y, player.width, player.height);
            }

            function drawObstacles() {
                ctx.fillStyle = '#ff0000';
                obstacles.forEach(obstacle => {
                    ctx.fillRect(obstacle.x, obstacle.y, obstacle.width, obstacle.height);
                });
            }

            function updateGame() {
                // Clear canvas
                ctx.clearRect(0, 0, canvas.width, canvas.height);

                // Update player
                if (player.isJumping) {
                    player.velocityY += player.gravity;
                    player.y += player.velocityY;

                    if (player.y > canvas.height - player.height) {
                        player.y = canvas.height - player.height;
                        player.isJumping = false;
                        player.velocityY = 0;
                    }
                }

                // Update obstacles
                obstacles.forEach((obstacle, index) => {
                    obstacle.x -= gameSpeed;
                    if (obstacle.x + obstacle.width < 0) {
                        obstacles.splice(index, 1);
                        score += 10;
                    }

                    // Collision detection
                    if (player.x < obstacle.x + obstacle.width &&
                        player.x + player.width > obstacle.x &&
                        player.y < obstacle.y + obstacle.height &&
                        player.y + player.height > obstacle.y) {
                        gameOver();
                    }
                });

                // Spawn new obstacles
                spawnObstacle();

                // Update score and distance
                distance += gameSpeed / 10;
                document.getElementById('score').textContent = score;
                document.getElementById('distance').textContent = Math.floor(distance);

                // Draw everything
                drawPlayer();
                drawObstacles();

                // Continue game loop
                gameLoop = requestAnimationFrame(updateGame);
            }

            function jump() {
                if (!player.isJumping) {
                    player.isJumping = true;
                    player.velocityY = player.jumpForce;
                }
            }

            function gameOver() {
                cancelAnimationFrame(gameLoop);
                ctx.fillStyle = 'rgba(0, 0, 0, 0.7)';
                ctx.fillRect(0, 0, canvas.width, canvas.height);
                ctx.fillStyle = '#fff';
                ctx.font = '48px Arial';
                ctx.textAlign = 'center';
                ctx.fillText('Game Over!', canvas.width/2, canvas.height/2);
                ctx.font = '24px Arial';
                ctx.fillText(`Score: ${score} Distance: ${Math.floor(distance)}m`, canvas.width/2, canvas.height/2 + 40);
                ctx.fillText('Click to play again', canvas.width/2, canvas.height/2 + 80);
            }

            // Event listeners
            document.addEventListener('keydown', (e) => {
                if (e.code === 'Space') {
                    jump();
                }
            });

            canvas.addEventListener('click', () => {
                if (!gameLoop) {
                    score = 0;
                    distance = 0;
                    obstacles.length = 0;
                    player.y = canvas.height - player.height;
                    player.velocityY = 0;
                    player.isJumping = false;
                    updateGame();
                } else {
                    jump();
                }
            });

            // Start the game
            updateGame();
        }

        // Sudoku Implementation
        function initSudoku() {
            const gameContainer = document.createElement('div');
            gameContainer.innerHTML = `
                <h2 style="text-align: center; color: #4CAF50; margin: 20px 0;">Sudoku</h2>
                <div class="game-status"></div>
                <div class="sudoku-board"></div>
                <div class="game-controls">
                    <button class="game-btn" onclick="initSudoku()">New Game</button>
                    <button class="game-btn" onclick="checkSudoku()">Check Solution</button>
                </div>
            `;
            gameArea.appendChild(gameContainer);

            const board = gameContainer.querySelector('.sudoku-board');
            const statusDisplay = gameContainer.querySelector('.game-status');
            let solution = [];
            let puzzle = [];

            function generateSudoku() {
                // Initialize empty board
                solution = Array(9).fill().map(() => Array(9).fill(0));
                
                // Generate solution
                solveSudoku(solution);
                
                // Create puzzle by removing numbers
                puzzle = solution.map(row => [...row]);
                let cellsToRemove = 40; // Adjust difficulty by changing this number
                
                while (cellsToRemove > 0) {
                    const row = Math.floor(Math.random() * 9);
                    const col = Math.floor(Math.random() * 9);
                    if (puzzle[row][col] !== 0) {
                        puzzle[row][col] = 0;
                        cellsToRemove--;
                    }
                }

                // Render puzzle
                renderBoard();
            }

            function renderBoard() {
                board.innerHTML = '';
                for (let i = 0; i < 9; i++) {
                    for (let j = 0; j < 9; j++) {
                        const input = document.createElement('input');
                        input.type = 'number';
                        input.className = 'sudoku-cell';
                        input.min = 1;
                        input.max = 9;
                        if (puzzle[i][j] !== 0) {
                            input.value = puzzle[i][j];
                            input.readOnly = true;
                        }
                        input.dataset.row = i;
                        input.dataset.col = j;
                        input.addEventListener('input', validateInput);
                        board.appendChild(input);
                    }
                }
            }

            function validateInput(e) {
                const val = e.target.value;
                if (val.length > 1) {
                    e.target.value = val.slice(0, 1);
                }
                if (val < 1 || val > 9) {
                    e.target.value = '';
                }
            }

            function solveSudoku(board) {
                const emptyCell = findEmptyCell(board);
                if (!emptyCell) return true;
                
                const [row, col] = emptyCell;
                for (let num = 1; num <= 9; num++) {
                    if (isValidPlacement(board, row, col, num)) {
                        board[row][col] = num;
                        if (solveSudoku(board)) return true;
                        board[row][col] = 0;
                    }
                }
                return false;
            }

            function findEmptyCell(board) {
                for (let i = 0; i < 9; i++) {
                    for (let j = 0; j < 9; j++) {
                        if (board[i][j] === 0) return [i, j];
                    }
                }
                return null;
            }

            function isValidPlacement(board, row, col, num) {
                // Check row
                for (let x = 0; x < 9; x++) {
                    if (board[row][x] === num) return false;
                }
                
                // Check column
                for (let x = 0; x < 9; x++) {
                    if (board[x][col] === num) return false;
                }
                
                // Check 3x3 box
                const boxRow = Math.floor(row / 3) * 3;
                const boxCol = Math.floor(col / 3) * 3;
                for (let i = 0; i < 3; i++) {
                    for (let j = 0; j < 3; j++) {
                        if (board[boxRow + i][boxCol + j] === num) return false;
                    }
                }
                
                return true;
            }

            function checkSudoku() {
                const currentBoard = Array(9).fill().map(() => Array(9).fill(0));
                const inputs = board.querySelectorAll('.sudoku-cell');
                let isFilled = true;
                
                inputs.forEach(input => {
                    const row = parseInt(input.dataset.row);
                    const col = parseInt(input.dataset.col);
                    currentBoard[row][col] = input.value ? parseInt(input.value) : 0;
                    if (!input.value) isFilled = false;
                });

                if (!isFilled) {
                    statusDisplay.textContent = 'Please fill in all cells!';
                    return;
                }

                let isCorrect = true;
                for (let i = 0; i < 9; i++) {
                    for (let j = 0; j < 9; j++) {
                        if (currentBoard[i][j] !== solution[i][j]) {
                            isCorrect = false;
                            break;
                        }
                    }
                }

                statusDisplay.textContent = isCorrect ? 
                    'Congratulations! You solved it!' : 
                    'Not quite right, keep trying!';
            }

            generateSudoku();
        }

        // Memory Game Implementation
        function initMemoryGame() {
            const symbols = ['★', '♦', '♥', '♠', '♣', '♫', '☺', '☀'];
            let cards = [...symbols, ...symbols];
            let flipped = [];
            let matched = [];
            let canFlip = true;

            // Shuffle cards
            cards.sort(() => Math.random() - 0.5);

            const gameContainer = document.createElement('div');
            gameContainer.innerHTML = `
                <h2 style="text-align: center; color: #4CAF50; margin: 20px 0;">Memory Game</h2>
                <div class="game-status">Match all pairs to win!</div>
                <div class="memory-board">
                    ${cards.map((symbol, index) => `
                        <div class="memory-card" data-index="${index}" data-symbol="${symbol}"></div>
                    `).join('')}
                </div>
                <div class="game-controls">
                    <button class="game-btn" onclick="initMemoryGame()">New Game</button>
                </div>
            `;
            gameArea.appendChild(gameContainer);

            const statusDisplay = gameContainer.querySelector('.game-status');
            const cardElements = gameContainer.querySelectorAll('.memory-card');

            let moves = 0;
            function updateStatus() {
                statusDisplay.textContent = `Moves: ${moves} | Pairs found: ${matched.length/2} of ${symbols.length}`;
            }
            updateStatus();

            cardElements.forEach(card => {
                card.addEventListener('click', () => {
                    if (!canFlip || flipped.includes(card) || matched.includes(card)) return;

                    card.textContent = card.dataset.symbol;
                    card.classList.add('flipped');
                    flipped.push(card);

                    if (flipped.length === 2) {
                        moves++;
                        canFlip = false;
                        const [card1, card2] = flipped;

                        if (card1.dataset.symbol === card2.dataset.symbol) {
                            matched.push(card1, card2);
                            flipped = [];
                            canFlip = true;

                            if (matched.length === cards.length) {
                                statusDisplay.textContent = `Congratulations! You won in ${moves} moves!`;
                            } else {
                                updateStatus();
                            }
                        } else {
                            setTimeout(() => {
                                card1.textContent = '';
                                card2.textContent = '';
                                card1.classList.remove('flipped');
                                card2.classList.remove('flipped');
                                flipped = [];
                                canFlip = true;
                                updateStatus();
                            }, 1000);
                        }
                    }
                });
            });
        }
    </script>
</body>
</html>
)=====";

void setup() {
    Serial.begin(115200);
    
    // Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    // Define web server routes
    server.on("/", handleRoot);
    
    // Start server
    server.begin();
    Serial.println("HTTP server started");
}

void handleRoot() {
    server.send(200, "text/html", MAIN_page);
}

void loop() {
    server.handleClient();
}
