#include "dungeon.hpp"
#include<time.h>

string gameState;
Block blocks[xGrid][yGrid];
Player player = {1, 1, 60, 10, {16, 287, 92, 130}, {50, 50, 50, 50}};
int random;
int coronacount =0;
int burgercount = 0;
int wellcount = 0;
int firecount = 0;


// string check_row_col(int row[], int col[], int x, int y){
// 	for (int i=0; i<row.size(); i++){
// 		if (row[i] == x && col[i]==y){
// 			return "YES";
// 		}
// 	}
// 	return "NO";
// }

//This function can be called to set an element in every block. It's complete, don't modify it...
void setBlock(int row, int col, int shape, int life, int health){
	blocks[row][col].shape = shape;
	blocks[row][col].life = life;
	blocks[row][col].health = health;
	blocks[row][col].mover = {row*yJump, col*xJump, xJump, yJump};
	switch (shape)
	{
	case ROCK:
		blocks[row][col].src = {151, 287, 147, 138};
		break;
	case CORONA:
		blocks[row][col].src = {0,0, 123, 122};
		break;
	case FIRE:
		blocks[row][col].src = {11, 134, 105, 109};
		break;
	case BURGER:
		blocks[row][col].src = {145, 9, 112, 94};
		break;
	case WELL:
		blocks[row][col].src = {154, 164, 102, 66};
		break;	
	default:
		blocks[row][col].src = {7, 443, 127, 114};
		break;
	}

}

//This function is drawing all the objects. It's complete, don't modify it.
void update(SDL_Renderer* gRenderer, SDL_Texture* assets, string direction ){
	SDL_RenderClear( gRenderer );
	for(int row=0;row<xGrid;row++){
		for (int col = 0; col < yGrid; col++)
		{
			SDL_RenderCopy( gRenderer, assets, &blocks[row][col].src, &blocks[row][col].mover );
		}		
	}


	if(direction!="")//if default argument is used.
		makeMove(direction);
	player.mover = {player.col*yJump, player.row*xJump, xJump, yJump};
	SDL_RenderCopy( gRenderer, assets, &player.src, &player.mover );
	// display life
	int r=0, c=0;
	for(int i=0;i<player.life;i++){
		if(i%10==0){
			r=0; c++;
		}
		SDL_Rect mover = {600 + r++*12, 100 + c*20, 10, 15};	
		SDL_RenderCopy( gRenderer, assets, &player.src, &mover );
	}

	for(int i=0 ;i<player.health;i++){
		SDL_Rect mover = {600+i*12, 300, 10, 10};	
		SDL_Rect src = {175, 461, 102, 90};	
		SDL_RenderCopy( gRenderer, assets, &src, &mover );		
	}
	SDL_Rect mover, src;
	if (gameState=="Running"){
		src = {367, 23, 281, 75};
		mover = {600, 400, 100, 50};
		SDL_RenderCopy(gRenderer, assets, &src, &mover);
	}
	else if (gameState=="Lost")
	{
		src= {372, 224, 182, 45};
		mover = {600, 400, 100, 50};
		SDL_RenderCopy(gRenderer, assets, &src, &mover);
	}
	else if (gameState=="Won")
	{
		src = {366, 125, 186, 45};
		mover = {600, 400, 100, 50};
		SDL_RenderCopy(gRenderer, assets, &src, &mover);	
	}
		
	SDL_RenderPresent( gRenderer );	
	SDL_Delay(5);	
}
	// fill this function to create dungeon. 
void createDungeon(){
	// To fill a block, call the setBlock function, the example is given here:
	for (int i=0; i<10; i++){
		setBlock(0, i, ROCK, -2, 0);	
	}
	for (int i=0; i<10; i++){
		setBlock(i, 0, ROCK, -4, 5);
	}
	for (int i=0; i<10; i++){
		setBlock(9, i, ROCK, -4, 5);	
	}
	for (int i=0; i<10; i++){
		setBlock(i, 9, ROCK, -4, 5);
	}
	srand(time(NULL));
	int max = 99;
	for (int i =1; i<9; i++){
		for (int j=1; j <9; j++){
			int random = rand()%max;
			if (random < 20){
				setBlock(i, j, CORONA, -20, -2);
			} else if (random < 50){
				setBlock(i, j, BURGER, 5, 0);
			} else if (random < 70){
				setBlock(i, j, FIRE, -10, -4);
			} else {
				setBlock(i, j, WELL, 5, 2);
			}
		}
	}
	setBlock(8, 8, GOLD, 0, 0);
	gameState = "Running";

	// It Sets a fire block at row=3, and column=4, it reduces life by 4, and increases health by 5
	// Other possible values for shape are: ROCK, CORONA, BURGER, WELL, FIRE, GOLD 
}


void makeMove(string direction){
	if (gameState == "Running"){
		if (direction == "right"){
			player.col++;
			if (blocks[player.col][player.row].shape == ROCK){
				player.life = player.life - 2;
				player.col--;
				if (player.health<=0 || player.life <=0){
					gameState = "Lost";
				} else {
					gameState = "Running";
				}
			}
			else if (blocks[player.col][player.row].shape == CORONA){
				cout << "CORONA" << endl;
				player.life -=20;
				player.health -2;
				
				if (player.health<=0 || player.life <=0){
					gameState = "Lost";
				} else {
					gameState = "Running";
				}
			}
			else if (blocks[player.col][player.row].shape == BURGER){
				cout << "BURGER" << endl;
				player.life +=5;
				if (player.health<=0 || player.life <=0){
					gameState = "Lost";
				} else {
					gameState = "Running";
				}
			}
			else if (blocks[player.col][player.row].shape == WELL){
				cout << "WELL" << endl;
				player.life -=5;
				player.health +=2;
				if (player.health<=0 || player.life <=0){
					gameState = "Lost";
				} else {
					gameState = "Running";
				}
			} else if (blocks[player.col][player.row].shape == FIRE){
				cout << "FIRE" << endl;
				player.life -=10;
				player.health-=4;
				if (player.health<=0 || player.life <=0){
					gameState = "Lost";
				} else {
					gameState = "Running";
				}
			} else if (blocks[player.col][player.row].shape == GOLD){
				gameState = "Won";
			}
		}
		if (direction == "left"){
			player.col--;
			if (blocks[player.col][player.row].shape == ROCK){
				player.life = player.life - 2;
				player.col++;
				if (player.health<=0 || player.life <=0){
					gameState = "Lost";
				} else {
					gameState = "Running";
				}
			}
			else if (blocks[player.col][player.row].shape == CORONA){
				player.life -=20;
				player.health -2;
				if (player.health<=0 || player.life <=0){
					gameState = "Lost";
				} else {
					gameState = "Running";
				}
			}
			else if (blocks[player.col][player.row].shape == BURGER){
				player.life +=5;
				if (player.health<=0 || player.life <=0){
					gameState = "Lost";
				} else {
					gameState = "Running";
				}
			}
			else if (blocks[player.col][player.row].shape == WELL){
				player.life -=5;
				player.health +=2;
				if (player.health<=0 || player.life <=0){
					gameState = "Lost";
				} else {
					gameState = "Running";
				}
			} else if (blocks[player.col][player.row].shape == FIRE){
				player.life -=10;
				player.health-=4;
				if (player.health<=0 || player.life <=0){
					gameState = "Lost";
				} else {
					gameState = "Running";
				}
			} else {
				gameState = "Won";

			}
		}
		if (direction == "up"){
			player.row--;
			if (blocks[player.col][player.row].shape == ROCK){
				player.life = player.life - 2;
				player.row++;
				if (player.health<=0 || player.life <=0){
					gameState = "Lost";
				} else {
					gameState = "Running";
				}
			}
			else if (blocks[player.col][player.row].shape == CORONA){
				player.life -=20;
				player.health -2;
				if (player.health<=0 || player.life <=0){
					gameState = "Lost";
				} else {
					gameState = "Running";
				}
			}
			else if (blocks[player.col][player.row].shape == BURGER){
				player.life +=5;
				if (player.health<=0 || player.life <=0){
				gameState = "Lost";
				} else {
					gameState = "Running";
				}
			}
			else if (blocks[player.col][player.row].shape == WELL){
				player.life -=5;
				player.health +=2;
				if (player.health<=0 || player.life <=0){
					gameState = "Lost";
				} else {
					gameState = "Running";
				}
			} else if (blocks[player.col][player.row].shape == FIRE){
				player.life -=10;
				player.health-=4;
				if (player.health<=0 || player.life <=0){
					gameState = "Lost";
				} else {
					gameState = "Running";
				}
			} else {
				gameState = "Won";
			}

		}
		if (direction == "down"){
			player.row++;
			if (blocks[player.col][player.row].shape == ROCK){
				player.life = player.life - 2;
				player.row--;
				if (player.health<=0 || player.life <=0){
					gameState = "Lost";
				} else {
					gameState = "Running";
				}
			}
			else if (blocks[player.col][player.row].shape == CORONA){
				player.life -=20;
				player.health -2;
				if (player.health<=0 || player.life <=0){
					gameState = "Lost";
				} else {
					gameState = "Running";
				}
			}
			else if (blocks[player.col][player.row].shape == BURGER){
				player.life +=5;
				if (player.health <=0 || player.life <=0){
					gameState = "Lost";
				} else {
					gameState = "Running";
				}
			}
			else if (blocks[player.col][player.row].shape == WELL){
				player.life -=5;
				player.health +=2;
				if (player.health<=0 || player.life <=0){
					gameState = "Lost";
				} else{
					gameState = "Running";
				}
			} else if (blocks[player.col][player.row].shape == FIRE){
				player.life -=10;
				player.health-=4;
				if (player.health<=0 || player.life <=0){
					gameState = "Lost";
				} else {
					gameState = "Running";
				}
			} else if (blocks[player.col][player.row].shape == GOLD){
				gameState = "Won";
				player.row++;
			}
			
		}
	}
}
