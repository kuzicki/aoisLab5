#pragma once
#include "Logical.h"
#include <set>
#include <vector>
#include <array>


struct Cell {
	Cell(bool isTrue) : _isIncluded(false), _isTrue(isTrue) { }
	Cell() : _isIncluded(false), _isTrue(false) { }
	const std::vector<Logical::Variable>& GetVars() const { return vars; }
	bool IsIncluded() const { return _isIncluded; }
	void MakeIncluded() { _isIncluded = true; }
	bool IsTrue() { return _isTrue; }
	void AddVar(Logical::Variable newVar);
	int GetSize() { return vars.size(); }
	const Logical::Variable& operator [](int i) { return vars[i]; }
private:
	std::vector<Logical::Variable> vars;
	bool _isTrue;
	bool _isIncluded;
};

class Figure {
public:
	void AddCell(const Cell& newCell);
	std::vector<Logical::Variable> GetVariablePart(bool isPCNF = false);
private:
	std::vector<Cell> _cells;
};

class KMap {
public:
	KMap(const std::vector<std::vector<bool>>& truthTable, const std::set<char>& variables);
	void PrintTable();
	std::vector<Figure> GetAllFiguresPDNF();
	std::vector<Figure> GetAllFiguresPCNF();
	std::string GetMinimalPDNF();
	std::string GetMinimalPCNF();
private:
	std::vector<Figure> GetAllFigures(bool isTrue);
	void fill1by1(const std::vector<std::vector<bool>>& truthTable, const std::vector<char> variables);
	void fill1by2(const std::vector<std::vector<bool>>& truthTable, const std::vector<char> variables);
	void fill2by2(const std::vector<std::vector<bool>>& truthTable, const std::vector<char> variables);
	void fill5Vars(const std::vector<std::vector<bool>>& truthTable, const std::vector<char> variables);
	Cell& accessElement(int x, int y);
	Cell& accessElementCorresponding(int x, int y);
	bool findFigure(int x, int y, int sizeX, int sizeY, bool isTrue);
	bool findFigure3d(int x, int y, int sizeX, int sizeY, bool isTrue);
	Figure getFigure(int x, int y, int sizeX, int sizeY);
	Figure getFigure3d(int x, int y, int sizeX, int sizeY);
	std::vector<Figure> getAllFigure(int sizeX, int sizeY, bool isTrue);
	std::vector<Figure> getAllFigure3d(int sizeX, int sizeY, bool isTrue);
	bool isCompatibleFigure(int index);
	int getIndex(const std::vector<std::vector<bool>>& truthTable, int row, int offset, int size);
	static const std::array<std::array<int, 2>, 12> FigureSizes;
	int area;
	bool is5Vars;
	std::vector<std::vector<Cell>> _cellGrid;
};
