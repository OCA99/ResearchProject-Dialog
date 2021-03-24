#include "DialogSystem.h"
#include "DialogNode.h"

#include "App.h"
#include "Render.h"
#include "Input.h"
#include "Fonts.h"

#include <utility>

#include "ToUpperCase.h"
#include "Callbacks.h"

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
	/* Start TODO 5 */

	// Improve the dialog rendering system!

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

	/* End TODO 5 */
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

		// Get the root of the XML document (the "dialog" node).
		// Get the id of the dialog from the "id" attribute.
		// Parse the dialog tree using ParseDialogXML.
		// Insert the parsed dialog into the "dialogues" dictionary.

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

void DialogSystem::NextDialog()
{
	/* Start TODO 4 */

	// If currentDialog == nullptr, return.

	// If currentDialog->type == DIALOG, set currentDialog to its first child.
	// Else If currentDialog->type == OPTIONS, set currentDialog to the child at "selectedOption" position. Then set currentDialog to its first child.
	// Else, set currentDialog to currentDialog->Next()

	// Set selectedOption to 0

	// It currentDialog == nullptr, return.

	// If currentDialog has a "callback", execute it.

	// If currentDialog->type == GOTO, get the attribute "route" and set currentDialog to "nodeRoutes.at(route)"

	/* End TODO 4 */
}

DialogNode* DialogSystem::ParseDialogXML(pugi::xml_node currentNode)
{
	DialogNode* dialogNode = new DialogNode();

	/* Start TODO 2 */

	// Get the name of the currentNode.
	// Set the type of dialogNode according to the name of the node.

	Attributes* attributes = new Attributes();

	// Store all the attributes of the currentNode into the "attributes" dictionary.

	dialogNode->SetAttributes(attributes);

	// If the "id" attribute exists insert the pair (id, dialogNode) into the "nodeRoutes" dictionary.


	// If dialogNode->type != LINE, call ParseDialogXML recursively and add the results to dialogNode->children
	// Remember to assign the "parent", "next" and "previous" variables!

	/* End TODO 2 */

	return dialogNode;
}