#ifndef __Hierarchy__H__
#define __Hierarchy__H__

#include "EditorManager.h"
#include "Objects3D.h"

class GameObject;
class TreeNodes;

struct TreeNode
{
	const char* Name;
	TreeNode* Parent;
	ImVector<TreeNode*> Children;

	TreeNode(const char* node_name, TreeNode* parent_node = NULL)
	{
		char* name = (char*)IM_ALLOC(strlen(node_name));
		strcpy(name, node_name);
		Name = name;
		Parent = parent_node;
	}

	~TreeNode()
	{
		IM_FREE((void*)Name);
		Name = NULL;
		Parent = NULL;
		for (int i = 0; i < Children.Size; i++)
			IM_DELETE(Children[i]);
		Children.clear();
	}

	TreeNode* AddChild(const char* name)
	{
		Children.push_back(IM_NEW(TreeNode(name, this)));
		return Children.back();
	}

	bool IsDescendantOf(TreeNode* node)
	{
		bool is_descendant = false;
		for (TreeNode* n = this; !is_descendant && n->Parent != NULL; n = n->Parent)
			is_descendant = n == node;
		return is_descendant;
	}
};

class Hierarchy : public EditorElement
{
public:

	Hierarchy(bool is_visible = true);
	virtual ~Hierarchy();

public:

	void Start();
	void Draw();
	void CleanUp();

	static void RenderReorderTree(TreeNode* node);
};

#endif __Hierarchy__H__
