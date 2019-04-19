#include "Grid.h"
#include "Player.h"
#include "EmptyCell.h"
#include "PlayerCell.h"

#include <iostream>
using namespace std;
Grid::Grid() 
{
	// initializes all the GameObject pointer of the List to NULL
	for (int i = 0; i < NumRowCells; i++)
	{
		for (int j = 0; j < NumColumnCells; j++)		{
			
			GameCells[i][j] = new EmptyCell(i,j);	//start with all cells as empty cells
		}
	}
	pGUI = new GUI;
	DrawAllCells();
	
}



/////////////////////////////////////////////////////////////////////////////////////////////
// Moves the entered position in the passed direction if possible
// updates currentCell if move takes place
// Note: It returns bool indicating if a move occured or not
bool Grid::MoveIfPossible(Cell* pCurrentCell, ActionType dir) 
{
	
	//Calculate the row and col of the cell to move to
	//First initialize them as current row and col
	int playerOldRow = pCurrentCell->getRow();
	int playerOldCol = pCurrentCell->getCol();

	int TargetRow = playerOldRow;
	int TargetCol = playerOldCol ; 

	
	switch(dir)
	{
	case MOVE_UP:
		TargetRow--;
		break;
	case MOVE_DOWN:
		TargetRow++;
		break;
	case MOVE_LEFT:
		TargetCol--;
		break;
	case MOVE_RIGHT:
		TargetCol++;;
		break;
	}

	if( !IsValidCell(TargetRow, TargetCol))
		return false;

	//get the target cell
	Cell *pTargetCell = GameCells[TargetRow][TargetCol];

	if(!pTargetCell->ActOn(player))	//The target cell is an obstacle ==> no action should be taken
		return false;

	//The target cell is not an obstacle and has acted on the player
	//The player position should be updated to the target cell
	
	//Player should be moved to the target cell
	//1-delete the target cell and make it point to the PlayerCell
	setCell(TargetRow,TargetCol, pCurrentCell);

	//2- update player position
	pCurrentCell->SetRow(TargetRow);
	pCurrentCell->SetCol(TargetCol);

	//3- Convert the old player position to an Empty cell
	GameCells[playerOldRow][playerOldCol] = new EmptyCell(playerOldRow,playerOldCol);

	//4- Redraw both cells
	pGUI->DrawCell(GameCells[TargetRow][TargetCol]);
	pGUI->DrawCell(GameCells[playerOldRow][playerOldCol]);

	return true;
}

void Grid::DrawAllCells() const	
{
	// Draw Empty Grid
	pGUI->ClearGridArea();

	// Draw all the GameObject of the Grid
	for (int i = 0; i < NumRowCells; i++)
		for (int j = 0; j < NumColumnCells; j++)
			pGUI->DrawCell(GameCells[i][j]);
}

void Grid::setCell(int row, int col, Cell* pCell)
{	
	if(GameCells[row][col])
		delete GameCells[row][col];	//if cell ptr is not null, delete it first
	
	GameCells[row][col] = pCell;

}


GUI * Grid::getGUI() const
{
	return pGUI;
}
///////////////////////////////////////////////////////////////////
//				Action-related functions
///////////////////////////////////////////////////////////////////

ActionType Grid::GetUserAction() const
{
	// Ask the input to get the action from the user.
	return pGUI->GetUserAction( ); 
}

////////////////////////////////////////////////////////////////////////////////////

// Creates an action and executes it
void Grid::ExecuteAction(ActionType ActType) 
{
	// According to Action Type, create the corresponding action object	
	switch (ActType)
	{
	case LOAD:
		// Call Load function to load game from a file
		break;

	case SAVE:
		// Call Save function to Save current game to a file
		break;

	case START:	//start game 
		pGUI->setInterfaceMode(MODE_GAME);
		break;

	case PAUSE:	//pause game
		pGUI->setInterfaceMode(MODE_MENU);
		break;
	case EDIT:
		pGUI->setInterfaceMode(MODE_MENU);
		pGUI->PrintMessage("Choose the cell to EDIT");
		//pGUI->GetPointClicked();
		break;

	///TODO: Add a case for EACH Action type
	case MOVE_UP:
	case MOVE_DOWN:
	case MOVE_RIGHT:
	case MOVE_LEFT:
		player->Move(this,ActType);
		break;		

	case STATUS:	// a click on the status bar ==> no action
		return;
	}

}


Grid::~Grid()
{
	delete pGUI;
}

//////////////////////////////////////////////////////////////////////////////////


// Runs the application

void Grid::RunApp()
{

	int row = 7, col=12;	
	PlayerCell* pCell = new PlayerCell(row,col);
	setCell(row,col, pCell);
	player = new Player(*pCell);
	pGUI->DrawCell(pCell);
	
	while (1)
	{
		ActionType act = GetUserAction();
		if(act == EXIT)
			return;
		if (act == SAVE){
			cout << "Saving! File name: ";
			//cin >> FileName;
			getData(pCell);
		}
		
		ExecuteAction(act);
		
		Sleep(100);
	}

}

////////////////////////////////////////////////////////////////////////////////////
bool Grid::IsValidCell(int row, int col) const
{
	return (row >= 0 && row < NumRowCells) 
			&& (col >=0 && col < NumColumnCells);
}
/////////////////////////////////////////////////////////////////////////////////////
void Grid::getData(Cell* pCell){
	// string f = FileName + ".txt";
	 // myfile.open(f.c_str());
	myfile.open("test.txt");
	int myRow=pCell->getRow();
	int myColumn = pCell->getCol();
	cout << "Row= " << myRow << "& Column= " << myColumn;
	myfile << "Row= " << myRow << "& Column= " << myColumn;
	myfile.close();
}
