#include "Hierarchy.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"

#include "mmgr/mmgr.h"

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
		isSelected = false;
		int id = 0;
		for (int i = 0; i < App->scene_intro->root->children.size(); ++i)
			HandleHierarchyGO(App->scene_intro->root->children[i], id);
	}

	ImGui::End();
}

void Hierarchy::CleanUp()
{
}

void Hierarchy::HandleHierarchyGO(GameObject * gameObject, int & id)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;

	if (gameObject == App->scene_intro->goSelected)
		flags |= ImGuiTreeNodeFlags_Selected;

	if (gameObject->children.size() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	id++;

	if (ImGui::TreeNodeEx((void*)(intptr_t)id, flags, gameObject->name.data()))
	{
		if (ImGui::IsItemClicked(0) && isSelected == false)
		{
			App->scene_intro->goSelected = gameObject;
			isSelected = true;
		}

		else if (ImGui::IsItemClicked(1) && ImGui::IsWindowHovered() && isSelected == false)
		{
			openNode = true;
			App->scene_intro->goSelected = gameObject;
		}

		if (gameObject != App->scene_intro->root)
		{
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				ImGui::SetDragDropPayload("GAME_OBJECT", gameObject, sizeof(GameObject));
				ImGui::Text(" %s ", gameObject->name.data());
				draggedGO = gameObject;
				ImGui::EndDragDropSource();
			}
		}


		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAME_OBJECT"))
			{
				App->scene_intro->SetParent(gameObject, draggedGO);
			}
			ImGui::EndDragDropTarget();
		}

		if (gameObject->children.size() > 0)
		{
			for (int i = 0; i < gameObject->children.size(); ++i)
			{
				HandleHierarchyGO(gameObject->children[i], id);
			}
		}

		ImGui::TreePop();
	}
}

