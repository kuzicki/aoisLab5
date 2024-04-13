#include "KMap.h"
#include <stdexcept>
#include <bitset>

const std::array<std::array<int, 2>, 12> KMap::FigureSizes = { {
	{1, 8},
	{8, 1},
	{4, 4},
	{2, 4},
	{4, 2},
	{2, 2},
	{1, 4},
	{4, 1},
	{1, 2},
	{2, 1},
	{1, 1}
	} };

void Cell::AddVar(Logical::Variable newVar) {
	vars.push_back(newVar);
}

std::vector<Logical::Variable> Figure::GetVariablePart(bool isPCNF) {
	std::vector<Logical::Variable> part;
	for (int i = 0; i < _cells[0].GetSize(); i++) {
		const Logical::Variable& var = _cells[0][i];
		bool isCompatible = true;
		for (int j = 1; j < _cells.size(); j++) {

			if (var != _cells[j][i]) {
				isCompatible = false;
				break;
			}
		}
		if (isCompatible) {
			Logical::Variable newVar = var;
			if (!isPCNF)
				newVar.isNegative = !newVar.isNegative;
			part.push_back(newVar);
		}
	}
	return part;
}

KMap::KMap(const std::vector<std::vector<bool>>& truthTable, const std::set<char>& variables) {
	std::vector<char> vectorVars(variables.begin(), variables.end());
	int variablesSize = vectorVars.size();
	if (variablesSize == 2) {
		fill1by1(truthTable, vectorVars);
	}
	else if (variablesSize == 3) {
		fill1by2(truthTable, vectorVars);
	}
	else if (variablesSize == 4) {
		fill2by2(truthTable, vectorVars);
	}
	else if (variablesSize == 5) {
		fill5Vars(truthTable, vectorVars);
	}
	else {
		throw std::invalid_argument("The class only takes 2, 3, 4 variables!");
	}
}

void KMap::fill5Vars(const std::vector<std::vector<bool>>& truthTable, const std::vector<char> variables) {
	_cellGrid.resize(4, std::vector<Cell>(8));
	is5Vars = true;
	int lastColumn = truthTable[0].size() - 1;
	for (int i = 0; i < truthTable.size(); i++) {
		int x = getIndex(truthTable, i, 1, 2);
		int y = getIndex(truthTable, i, 3, 2);
		Logical::Variable var1(variables[0], truthTable[i][0]);
		if (var1.isNegative)
			y += 4;
		Logical::Variable var2(variables[1], truthTable[i][1]);
		Logical::Variable var3(variables[2], truthTable[i][2]);
		Logical::Variable var4(variables[3], truthTable[i][3]);
		Logical::Variable var5(variables[4], truthTable[i][4]);
		Cell newCell(truthTable[i][lastColumn]);
		newCell.AddVar(var1);
		newCell.AddVar(var2);
		newCell.AddVar(var3);
		newCell.AddVar(var4);
		newCell.AddVar(var5);
		_cellGrid[x][y] = newCell;
	}
}

void KMap::fill1by1(const std::vector<std::vector<bool>>& truthTable, const std::vector<char> variables) {
	_cellGrid.resize(2, std::vector<Cell>(2));
	is5Vars = false;
	int lastColumn = truthTable[0].size() - 1;
	for (int i = 0; i < truthTable.size(); i++) {
		int x = getIndex(truthTable, i, 0, 1);
		int y = getIndex(truthTable, i, 1, 2);
		Logical::Variable var1(variables[0], truthTable[i][0]);
		Logical::Variable var2(variables[1], truthTable[i][1]);
		Cell newCell(truthTable[i][lastColumn]);
		newCell.AddVar(var1);
		newCell.AddVar(var2);
		_cellGrid[x][y] = newCell;
	}
}

void KMap::fill1by2(const std::vector<std::vector<bool>>& truthTable, const std::vector<char> variables) {
	_cellGrid.resize(2, std::vector<Cell>(4));
	is5Vars = false;
	int lastColumn = truthTable[0].size() - 1;
	for (int i = 0; i < truthTable.size(); i++) {
		int x = getIndex(truthTable, i, 0, 1);
		int y = getIndex(truthTable, i, 1, 2);
		Logical::Variable var1(variables[0], truthTable[i][0]);
		Logical::Variable var2(variables[1], truthTable[i][1]);
		Logical::Variable var3(variables[2], truthTable[i][2]);
		Cell newCell(truthTable[i][lastColumn]);
		newCell.AddVar(var1);
		newCell.AddVar(var2);
		newCell.AddVar(var3);
		_cellGrid[x][y] = newCell;
	}
}

void KMap::fill2by2(const std::vector<std::vector<bool>>& truthTable, const std::vector<char> variables) {
	_cellGrid.resize(4, std::vector<Cell>(4));
	is5Vars = false;
	int lastColumn = truthTable[0].size() - 1;
	for (int i = 0; i < truthTable.size(); i++) {
		int x = getIndex(truthTable, i, 0, 2);
		int y = getIndex(truthTable, i, 2, 2);
		Logical::Variable var1(variables[0], truthTable[i][0]);
		Logical::Variable var2(variables[1], truthTable[i][1]);
		Logical::Variable var3(variables[2], truthTable[i][2]);
		Logical::Variable var4(variables[3], truthTable[i][3]);
		Cell newCell(truthTable[i][lastColumn]);
		newCell.AddVar(var1);
		newCell.AddVar(var2);
		newCell.AddVar(var3);
		newCell.AddVar(var4);
		_cellGrid[x][y] = newCell;
	}
}


int KMap::getIndex(const std::vector<std::vector<bool>>& truthTable, int row, int offset, int size) {
	std::string bitsString = "";
	for (int i = offset; i < offset + size; i++) {
		bitsString += std::to_string(truthTable[row][i]);
	}
	if (bitsString == "11")
		return 2;
	if (bitsString == "10")
		return 3;
	std::bitset<4> bits(bitsString);
	return bits.to_ulong();
}

void KMap::PrintTable() {
	for (int i = 0; i < _cellGrid.size(); i++) {
		for (int j = 0; j < _cellGrid[i].size(); j++) {
			std::cout << _cellGrid[i][j].IsTrue() << " ";
		}
		std::cout << std::endl;
	}
}

void Figure::AddCell(const Cell& newCell) {
	_cells.push_back(newCell);
}

bool KMap::isCompatibleFigure(int index) {
	if (FigureSizes[index][0] <= _cellGrid.size() && FigureSizes[index][1] <= _cellGrid[0].size()) {
		return true;
	}
	return false;
}

Figure KMap::getFigure(int x, int y, int sizeX, int sizeY) {
	Figure newFigure;
	for (int i = x; i < x + sizeX; i++) {
		for (int j = y; j < y + sizeY; j++) {
			Cell& cell = accessElement(i, j);
			cell.MakeIncluded();
			newFigure.AddCell(cell);
		}
	}
	return newFigure;
}

Figure KMap::getFigure3d(int x, int y, int sizeX, int sizeY) {
	Figure newFigure;
	for (int i = x; i < x + sizeX; i++) {
		for (int j = y; j < y + sizeY; j++) {
			Cell& cell1 = accessElement(i, j);
			Cell& cell2 = accessElementCorresponding(i, j);
			cell1.MakeIncluded();
			newFigure.AddCell(cell1);
			cell2.MakeIncluded();
			newFigure.AddCell(cell2);
		}
	}
	return newFigure;
}

bool KMap::findFigure3d(int x, int y, int sizeX, int sizeY, bool isTrue) {
	bool areAllIncluded = true;
	for (int i = x; i < x + sizeX; i++) {
		for (int j = y; j < y + sizeY; j++) {
			Cell& cell1 = accessElement(i, j);
			Cell& cell2 = accessElementCorresponding(i, j);
			if (cell1.IsTrue() != isTrue || cell2.IsTrue() != isTrue) {
				return false;
			}
			if (!cell1.IsIncluded() || !cell2.IsIncluded()) {
				areAllIncluded = false;
			}
		}
	}
	if (!areAllIncluded) {
		return true;
	}
	return false;
}

bool KMap::findFigure(int x, int y, int sizeX, int sizeY, bool isTrue) {
	bool areAllIncluded = true;
	for (int i = x; i < x + sizeX; i++) {
		for (int j = y; j < y + sizeY; j++) {
			Cell& cell = accessElement(i, j);
			if (cell.IsTrue() != isTrue) {
				return false;
			}
			if (!cell.IsIncluded()) {
				areAllIncluded = false;
			}
		}
	}
	if (!areAllIncluded) {
		return true;
	}
	return false;
}

Cell& KMap::accessElement(int x, int y) {
	if (x >= _cellGrid.size()) {
		x %= _cellGrid.size();
	}
	if (y >= _cellGrid[0].size()) {
		y %= _cellGrid[0].size();
	}

	return _cellGrid[x][y];
}

Cell& KMap::accessElementCorresponding(int x, int y) {
	y += 4;
	if (x >= _cellGrid.size()) {
		x %= _cellGrid.size();
	}
	if (y >= _cellGrid[0].size()) {
		y %= _cellGrid[0].size();
	}

	return _cellGrid[x][y];
}

std::vector<Figure> KMap::getAllFigure(int sizeX, int sizeY, bool isTrue) {
	std::vector<Figure> figures;
	for (int x = 0; x < _cellGrid.size(); x++) {
		for (int y = 0; y < _cellGrid[0].size(); y++) {
			if (findFigure(x, y, sizeX, sizeY, isTrue)) {
				figures.push_back(getFigure(x, y, sizeX, sizeY));
			}
		}
	}
	return figures;
}

std::vector<Figure> KMap::getAllFigure3d(int sizeX, int sizeY, bool isTrue) {
	std::vector<Figure> figures;
	for (int x = 0; x < _cellGrid.size(); x++) {
		for (int y = 0; y < _cellGrid[0].size(); y++) {
			if (findFigure3d(x, y, sizeX, sizeY, isTrue)) {
				figures.push_back(getFigure3d(x, y, sizeX, sizeY));
			}
		}
	}
	return figures;
}

std::vector<Figure> KMap::GetAllFigures(bool isTrue) {
	std::vector<Figure> foundFigures;
	if (is5Vars) {
		for (int i = 0; i < FigureSizes.size(); i++) {
			if (!isCompatibleFigure(i)) {
				continue;
			}
			std::vector<Figure> figures = getAllFigure3d(FigureSizes[i][0], FigureSizes[i][1], isTrue);
			foundFigures.insert(foundFigures.end(), figures.begin(), figures.end());

		}
	}
	for (int i = 0; i < FigureSizes.size(); i++) {
		if (!isCompatibleFigure(i)) {
			continue;
		}
		std::vector<Figure> figures = getAllFigure(FigureSizes[i][0], FigureSizes[i][1], isTrue);
		foundFigures.insert(foundFigures.end(), figures.begin(), figures.end());

	}
	return foundFigures;
}

std::vector<Figure> KMap::GetAllFiguresPDNF() {
	return GetAllFigures(true);
}
std::vector<Figure> KMap::GetAllFiguresPCNF() {
	return GetAllFigures(false);
}

std::string KMap::GetMinimalPDNF() {
	auto figures = GetAllFiguresPDNF();
	Logical::variableExpression expression;
	for (auto& figure : figures) {
		expression.push_back(figure.GetVariablePart());
	}

	return Logical::varExprToString(expression, '|');
}

std::string KMap::GetMinimalPCNF() {
	auto figures = GetAllFiguresPCNF();
	Logical::variableExpression expression;
	for (auto& figure : figures) {
		expression.push_back(figure.GetVariablePart(true));
	}

	return Logical::varExprToString(expression, '&');
}