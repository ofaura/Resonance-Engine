#include "Hierarchy.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"

Hierarchy::Hierarchy(bool is_visible) : EditorElement(is_visible) {}

Hierarchy::~Hierarchy() {}

void Hierarchy::Start()
{
}

void Hierarchy::Draw()
{
	if (!active) return;
	if (ImGui::Begin("Hierarchy", &active, ImGuiWindowFlags_AlwaysAutoResize))
	{
		for (uint i = 0; i < App->scene_intro->gameObjects.size(); ++i)
		{
			if (ImGui::Selectable(App->scene_intro->gameObjects.at(i)->name.data(), App->scene_intro->id_goSelected == i))
			{
				App->scene_intro->goSelected = App->scene_intro->gameObjects.at(i);
				App->scene_intro->id_goSelected = i;
			}
		}

		//ImGui::Indent();

		static TreeNode* root = NULL;
		if (root == NULL)
		{
			// Initialize tree with sample data
			char name_buffer[32];
			const char* colors[] = { "Red", "Green", "Blue", "Black" };
			const char* animals[] = { "Seal", "Wolf", "Crow", "Horse" };
			root = new TreeNode("Root");
			for (int i = 0; i < 4; i++)
			{
				TreeNode* parent = root->AddChild(colors[i]);
				for (int j = 0; j < 4; j++)
				{
					sprintf(name_buffer, "%s %s", colors[i], animals[j]);
					parent->AddChild(name_buffer);
				}
			}
		}
		RenderReorderTree(root);

		//ImGui::Unindent();
	}

	ImGui::End();

	
}

void Hierarchy::CleanUp()
{
}

void Hierarchy::RenderReorderTree(TreeNode * node)
{
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	bool open = ImGui::TreeNode(node->Name);

	if (node->Parent != NULL)   // Can not move root node itself
	{
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			ImGui::SetDragDropPayload("animal", &node, sizeof(node));
			ImGui::TextUnformatted(node->Name);
			ImGui::EndDragDropSource();
		}
	}

	// Prevent dropping a parent node on to one of it's children.
	bool acceptable = false;
	TreeNode* dropped = NULL;
	const ImGuiPayload* payload = ImGui::GetDragDropPayload();
	if (payload != NULL && payload->IsDataType("animal"))
	{
		memcpy(&dropped, payload->Data, sizeof(dropped));
		acceptable = !node->IsDescendantOf(dropped);
	}

	if (acceptable && ImGui::BeginDragDropTarget())
	{
		// Drop directly on to node and append to the end of it's children list.
		if (ImGui::AcceptDragDropPayload("animal"))
		{
			if (dropped->Parent->Children.find_erase(dropped))
			{
				node->Children.push_back(dropped);
				dropped->Parent = node;
			}
		}
		ImGui::EndDragDropTarget();
	}

	if (open)
	{
		// Drop at specific position within a tree.
		for (int i = -1; i < node->Children.Size; i++)
		{
			if (i >= 0)
				RenderReorderTree(node->Children[i]);

			if (acceptable && ImGui::AcceptReorderDropPayload("animal"))
			{
				// This offset compensates for i starting at -1, however if we are moving a node within same parent
				// and it is located at or before our destination position, deletion of this node does compensate
				// for this offset already.
				int offset = 1;
				if (node->Children.contains(dropped))
				{
					int prev_i = node->Children.index_from_ptr(node->Children.find(dropped));
					if (prev_i <= i)
						offset = 0;
				}

				dropped->Parent->Children.find_erase(dropped);
				node->Children.insert(node->Children.begin() + i + offset, dropped);
				dropped->Parent = node;
			}
		}

		ImGui::TreePop();
	}
}
