#pragma once

#include <vector>
#include <map>
#include <string>

using Attributes = std::map<std::string, std::string>;

struct DialogNode {
public:
	enum class NodeType {
		DIALOG,
		LINE,
		OPTIONS,
		OPTION,
		GOTO
	};

	DialogNode(Attributes* _attributes = nullptr) {
		type = NodeType::DIALOG;
		children = new std::vector<DialogNode*>();
		attributes = _attributes;
	}

	~DialogNode() {
		delete children;
		delete attributes;
	}

	void SetAttributes(Attributes* _attributes) {
		attributes = _attributes;
	}

	void AddChild(DialogNode* child)
	{
		children->push_back(child);
	}

	DialogNode* Next()
	{
		/* Start TODO 3 */

		// If type == OPTION we should move up the tree
		// If next != nullptr we should move to the next sibling
		// If parent != nullptr we should move up the tree
		// If none of the conditions match, we are at the end of the dialog and should return nullptr

		/* End TODO 3 */
	}

	NodeType type;
	std::vector<DialogNode*>* children;
	std::map<std::string, std::string>* attributes;

	DialogNode* prev = nullptr;
	DialogNode* next = nullptr;
	DialogNode* parent = nullptr;
};