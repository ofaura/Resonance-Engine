#include "Application.h"
#include "EditorManager.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "Application.h"
#include "GameObject.h"
#include "C_Camera.h"

#include "Brofiler/Brofiler.h"

#include "mmgr/mmgr.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module("Camera", start_enabled)
{

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	editorcamera = new C_Camera( COMPONENT_TYPE::CAMERA, nullptr, true);
	editorcamera->SetPos(Position);
	editorcamera->SetFOV(60);
	editorcamera->SetPlanes(1, 10000);
}

ModuleCamera3D::~ModuleCamera3D()
{
	delete editorcamera;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG_CONSOLE("Setting up the camera");
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");
	
	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	BROFILER_CATEGORY("Camera3D Update", Profiler::Color::Orchid)

		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			if (App->scene_intro->goSelected != nullptr)
			{
				GameObject* gameObject = App->scene_intro->goSelected;
				if (gameObject != nullptr) {
					//Move camera to Point Reference of the Object(Pivot)
					vec3 distance = { gameObject->Globalbbox.CenterPoint().x - Reference.x,
									gameObject->Globalbbox.CenterPoint().y - Reference.y,
									gameObject->Globalbbox.CenterPoint().z - Reference.z };

					Reference += distance;

					//Focus Camera distance
					float3 points[8];
					gameObject->Globalbbox.GetCornerPoints(points);
					vec3 max_ = { points[0].At(0),points[0].At(1),points[0].At(2) }; //set first point
					for (int i = 0; i < 8 - 1; ++i) {
						vec3 point_ = { points[i].At(0), points[i].At(1), points[i].At(2) };
						if (length(max_) < length(point_))
							max_ = point_;
					}
					double radius = length(max_) / 2; //radius of sphere

					double fov = editorcamera->GetFOV() * DEGTORAD;

					double camDistance = (radius * 2.0) / tan(fov / 2.0);

					Position = Reference + to_vec3(-editorcamera->frustum.front) * camDistance;
					editorcamera->SetPos(Position);
				}
			}
		}

		if (MouseInsideWindow())
		{
			vec3 newPos(0, 0, 0);
			float speed = 3.0f * dt;
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
				speed = 8.0f * dt;

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += to_vec3(editorcamera->frustum.front) * speed;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= to_vec3(editorcamera->frustum.front) * speed;

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= to_vec3(editorcamera->frustum.WorldRight()) * speed;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += to_vec3(editorcamera->frustum.WorldRight()) * speed;

			Position += newPos;
			Reference += newPos;
			editorcamera->SetPos(Position);


			// Mouse motion ----------------

			if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
			{
				int dx = -App->input->GetMouseXMotion();
				int dy = -App->input->GetMouseYMotion();

				float3 vector = editorcamera->frustum.pos - to_float3(Reference);

				Quat quat_y(editorcamera->frustum.up, dx * 0.005);
				Quat quat_x(editorcamera->frustum.WorldRight(), dy * 0.005);

				vector = quat_x.Transform(vector);
				vector = quat_y.Transform(vector);

				Position = to_vec3(vector) + Reference;
				editorcamera->SetPos(Position);
				LookAt(Reference);
			}

			//Left Button

			if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
			{
				int dx = -App->input->GetMouseXMotion();
				int dy = -App->input->GetMouseYMotion();
				vec3 distance = (Position - Reference);
				float Sensitivity = 0.10;

				if (dx != 0)
				{
					newPos += to_vec3(editorcamera->frustum.WorldRight()) * speed * dx * length(distance) * Sensitivity;
				}

				if (dy != 0)
				{
					newPos -= to_vec3(editorcamera->frustum.up) * speed * dy * length(distance) * Sensitivity;
				}

				Position += newPos;
				Reference += newPos;
				editorcamera->SetPos(Position);
			}

			if ( App->input->GetMouseZ() != 0) {
				vec3 distance = (Position - Reference);
				vec3 nPos = { 0,0,0 };
				if ((App->input->GetMouseZ() > 0 && length(distance) > 0.5) || App->input->GetMouseZ() < 0) {
					nPos -= to_vec3(-editorcamera->frustum.front) * App->input->GetMouseZ() * length(distance) / 10;
					Position += nPos;
					editorcamera->SetPos(Position);
				}
			}

		}

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;
	float3 position = { Spot.x,Spot.y,Spot.z };
	editorcamera->Look(position);

}

bool ModuleCamera3D::MouseInsideWindow()
{
	ImVec2 mouse_pos = { (float)App->input->GetMouseX(), (float)App->input->GetMouseY() };
	ImVec4 game_window = App->editor->GetGameWindowRect();

	return ((mouse_pos.x > game_window.x) && (mouse_pos.x < game_window.x + game_window.w) && (mouse_pos.y > game_window.y) && (mouse_pos.y < game_window.y + game_window.z));
}


float3 ModuleCamera3D::to_float3(vec3 vec) {
	return float3(vec.x, vec.y, vec.z);
}
vec3 ModuleCamera3D::to_vec3(float3 vec) {
	return vec3(vec.x, vec.y, vec.z);
}