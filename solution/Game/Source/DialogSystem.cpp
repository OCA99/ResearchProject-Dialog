#include "DialogSystem.h"
#include "DialogNode.h"

#include "App.h"
#include "Render.h"
#include "Input.h"
#include "Fonts.h"

#include <utility>

#include "ToUpperCase.h"

#include "Log.h"

DialogSystem::DialogSystem()
{
	name.Create("dialog");
}

DialogSystem::~DialogSystem()
{

}

bool DialogSystem::Awake(pugi::xml_node& config)
{
	folder = config.child("folder").child_value();

	return true;
}

bool DialogSystem::Start()
{
	LoadDialog("test.xml");
	StartDialog("TEST");

	return true;
}

bool DialogSystem::PreUpdate()
{
	return true;
}

bool DialogSystem::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN) {
		NextDialog();
	}

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KeyState::KEY_DOWN) {
		selectedOption += 1;
		if (selectedOption == currentDialog->children->size())
			selectedOption = currentDialog->children->size() - 1;
	}

	if (app->input->GetKey(SDL_SCANCODE_UP) == KeyState::KEY_DOWN) {
		selectedOption -= 1;
		if (selectedOption < 0) selectedOption = 0;
	}

	return true;
}

bool DialogSystem::PostUpdate()
{
	if (currentDialog == nullptr) return true;

	DrawDialog();

	return true;
}

void DialogSystem::DrawDialog()
{
	app->render->DrawRectangle(SDL_Rect({ 0, (app->render->camera.h / 3) * 2, app->render->camera.w, app->render->camera.h / 3 }), 255, 255, 255, 255, true, false);

	std::string text = ToUpperCase(currentDialog->attributes->at("value"));

	app->fonts->BlitText(10, (app->render->camera.h / 3) * 2 + 10, 0, text.c_str());

	if (currentDialog->type == DialogNode::NodeType::OPTIONS)
	{
		std::vector<DialogNode*>::iterator i = currentDialog->children->begin();
		int y = 0;
		for (i; i < currentDialog->children->end(); i++)
		{
			text = ToUpperCase((*i)->attributes->at("value"));
			app->fonts->BlitText(30, (app->render->camera.h / 3) * 2 + 30 + (18 * y), 0, text.c_str());
			y++;
		}
		SDL_Rect selectedRectangle = SDL_Rect({ 20, (app->render->camera.h / 3) * 2 + 30 + (18 * selectedOption), 6, 6 });
		app->render->DrawRectangle(selectedRectangle, 0, 0, 0, 255, true, false);
	}
}

bool DialogSystem::CleanUp()
{
	nodeRoutes.clear();

	return true;
}

bool DialogSystem::LoadDialog(const char* filename)
{
	bool ret = true;

	std::string path = folder + filename;

	pugi::xml_parse_result result = dialogFile.load_file(path.c_str());

	if (result == NULL) {
		LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}

	if (ret == true) {
		/* Start TODO 1 */

		pugi::xml_node dialogRoot = dialogFile.child("dialog");
		std::string id = dialogRoot.attribute("id").as_string();
		DialogNode* dialog = ParseDialogXML(dialogRoot);
		dialogues.insert(std::make_pair(id, dialog));

		/* End TODO 1 */
	}

	return ret;
}

void DialogSystem::StartDialog(const char* id)
{
	if (dialogues.find(id) == dialogues.end()) return;
	currentDialog = dialogues.at(id);
	NextDialog();
}

void DialogSystem::EndDialog()
{
}

void DialogSystem::NextDialog()
{
	if (currentDialog == nullptr) return;

	if (currentDialog->type == DialogNode::NodeType::DIALOG)
	{
		currentDialog = *currentDialog->children->begin();
	}
	else if (currentDialog->type == DialogNode::NodeType::OPTIONS)
	{
		currentDialog = *currentDialog->children->at(selectedOption)->children->begin();
	}
	else
	{
		currentDialog = currentDialog->Next();
	}

	selectedOption = 0;

	if (currentDialog == nullptr)
	{
		EndDialog();
		return;
	}

	if (currentDialog->type == DialogNode::NodeType::GOTO) {
		std::string route = currentDialog->attributes->at("route");
		currentDialog = nodeRoutes.at(route);
	}
}

DialogNode* DialogSystem::ParseDialogXML(pugi::xml_node currentNode)
{
	DialogNode* dialogNode = new DialogNode();

	/* Start TODO 2 */

	std::string type = currentNode.name();
	if (type == "dialog") {}
	else if (type == "line")
	{
		dialogNode->type = DialogNode::NodeType::LINE;
	}
	else if (type == "options")
	{
		dialogNode->type = DialogNode::NodeType::OPTIONS;
	}
	else if (type == "option")
	{
		dialogNode->type = DialogNode::NodeType::OPTION;
	}
	else if (type == "goto")
	{
		dialogNode->type = DialogNode::NodeType::GOTO;
	}
	else
		return nullptr;


	Attributes* attributes = new Attributes();

	pugi::xml_attribute_iterator aIt = currentNode.attributes_begin();
	for (aIt; aIt != currentNode.attributes_end(); aIt++)
	{
		attributes->insert(std::make_pair(aIt->name(), aIt->value()));
	}

	dialogNode->SetAttributes(attributes);

	if (dialogNode->attributes->find("id") != dialogNode->attributes->end())
	{
		nodeRoutes.insert(std::make_pair(dialogNode->attributes->at("id"), dialogNode));
	}

	if (dialogNode->type != DialogNode::NodeType::LINE)
	{
		pugi::xml_node_iterator cIt = currentNode.begin();
		for (cIt; cIt != currentNode.end(); cIt++)
		{
			DialogNode* child = ParseDialogXML(*cIt);
			child->parent = dialogNode;
			if (dialogNode->children->size() != 0)
			{
				child->prev = *(dialogNode->children->end() - 1);
				(*(dialogNode->children->end() - 1))->next = child;
			}
			dialogNode->AddChild(child);
		}
	}

	/* End TODO 2 */

	return dialogNode;
}