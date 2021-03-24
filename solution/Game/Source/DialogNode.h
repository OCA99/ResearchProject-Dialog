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

		if (type == NodeType::OPTION) return parent->Next();
		if (next != nullptr) return next;
		if (parent != nullptr) return parent->Next();
		return nullptr;

		/* END TODO 3 */
	}

	NodeType type;
	std::vector<DialogNode*>* children;
	std::map<std::string, std::string>* attributes;

	DialogNode* prev = nullptr;
	DialogNode* next = nullptr;
	DialogNode* parent = nullptr;
};