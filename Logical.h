#pragma once
#include <string>
#include <iostream>
#include <array>
#include <set>
#include <vector>
#include <map>


namespace Logical {
	struct Variable {
		char var;
		bool isNegative;
		Variable(char var, bool isNegative) : var(var), isNegative(isNegative) {}
		Variable() : var('0'), isNegative(false) {}
		bool operator==(const Variable& other) const {
			if (var == other.var && isNegative == other.isNegative) {
				return true;
			}
			return false;
		}
		bool operator!=(const Variable& other) const {
			return !(*this == other);
		}
		std::string ToString() const {
			if (isNegative) {
				return "!" + std::string(1, var);
			}
			else {
				return std::string(1, var);
			}
		}
	};
	using variableExpression = std::vector<std::vector<Variable>>;
	int priority(const char operation);
	bool calculate(const std::string& RPN, std::map<char, bool> inputVars);
	std::string RPN(const std::string& str, std::set<char>& vars);
	std::vector<std::vector<bool>> getTable(const std::string& RPN, const std::set<char>& variables);
	std::string getPCNF(std::vector<std::vector<bool>> table, const std::set<char>& variables);
	std::string getPDNF(std::vector<std::vector<bool>> table, const std::set<char>& variables);
	std::string getDecimalFormC(const std::vector<std::vector<bool>>& table);
	std::string getDecimalFormD(const std::vector<std::vector<bool>>& table);
	std::pair<long long, std::string> getIndexForm(const std::vector<std::vector<bool>>& table);
	std::vector<std::vector<Variable>> getVariableExpression(const std::string& expression, char separator, std::set<char>& set);
	variableExpression getSimplifiedForm(variableExpression completeForm, const std::set<char> variables);
	variableExpression McCluskeyMethod(variableExpression original, variableExpression simplified);
	variableExpression algebraicMethodPDNF(variableExpression simplified, char delim);
	std::string varExprToString(const variableExpression& expr, char delim);
	std::string McCluskeyMethodStr(std::string original, char delim);
	std::string KMapStr(std::string original, char delim);
	std::string AlgebraicMethodStr(std::string original, char delim);
	std::vector<std::vector<bool>> fillTable(const std::set<char>& variables);
	std::string CodeToPlus9();
	std::string SummatorFunctions();
	std::string Triggers();
	std::string ToBasis();
}

namespace utils {
	void printVariableExpression(const Logical::variableExpression& expr);
}