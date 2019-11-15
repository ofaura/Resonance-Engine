#include "Application.h"
#include "EditorManager.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "Brofiler/Brofiler.h"
#include "ModuleSceneIntro.h"
#include "Application.h"
#include "GameObject.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module("Camera", start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

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

				//Move camera to Point Reference of the Object(Pivot)
				vec3 dist = { App->scene_intro->goSelected->box.CenterPoint().x - Reference.x, App->scene_intro->goSelected->box.CenterPoint().y - Reference.y, App->scene_intro->goSelected->box.CenterPoint().z - Reference.z };
				Reference += dist;

				//Focus Camera distance
				float3 points[8];
				App->scene_intro->goSelected->box.GetCornerPoints(points);
				vec3 maximum = { points[0].At(0),points[0].At(1),points[0].At(2) }; //set first point

				for (int i = 0; i < 8 - 1; ++i)
				{
					vec3 point = { points[i].At(0), points[i].At(1), points[i].At(2) };
					if (length(maximum) < length(point))
					{
						maximum = point;
					}
				}

				double radius = length(maximum) / 2; //radius of sphere
				double fov = 60 * DEGTORAD;
				double DistCamera = (radius * 2.0) / tan(fov / 2.0);
				Position = Reference + Z * DistCamera;

			}
		}

		if (MouseInsideWindow())
		{
			vec3 newPos(0, 0, 0);
			float speed = 3.0f * dt;
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
				speed = 8.0f * dt;

			/*if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
			if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;*/

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

			Position += newPos;
			Reference += newPos;


			// Mouse motion ----------------

			if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
			{
				int dx = -App->input->GetMouseXMotion();
				int dy = -App->input->GetMouseYMotion();

				float Sensitivity = 0.25f;

				Position -= Reference;

				if (dx != 0)
				{
					float DeltaX = (float)dx * Sensitivity;

					X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
					Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
					Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				}

				if (dy != 0)
				{
					float DeltaY = (float)dy * Sensitivity;

					Y = rotate(Y, DeltaY, X);
					Z = rotate(Z, DeltaY, X);

					if (Y.y < 0.0f)
					{
						Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
						Y = cross(Z, X);
					}
				}

				Position = Reference + Z * length(Position);
			}

			if (App->input->GetMouseZ() != 0)
			{
				newPos = (0, 0, 0);
				float wheelSensitivity = scrollSensitivity;
				vec3 distance = Reference - Position;

				if (length(distance) < zoomSensitivity)
				{
					wheelSensitivity = length(distance) / zoomSensitivity;
				}

				if (App->input->GetMouseZ() > 0)
				{
					newPos -= Z * wheelSensitivity;
				}
				else
				{
					newPos += Z * wheelSensitivity;
				}

				Position += newPos;
			}

			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			//Left Button

			if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
			{
				if (dx != 0 || dy != 0)
				{
					newPos.y -= dy * MidButtonSensitivity;
					newPos += X * dx * MidButtonSensitivity;
				}

				Position += newPos;
				Reference += newPos;
			}
			// Recalculate matrix -------------
			CalculateViewMatrix();
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

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

bool ModuleCamera3D::MouseInsideWindow()
{
	ImVec2 mouse_pos = { (float)App->input->GetMouseX(), (float)App->input->GetMouseY() };
	ImVec4 game_window = App->editor->GetGameWindowRect();

	return ((mouse_pos.x > game_window.x) && (mouse_pos.x < game_window.x + game_window.w) && (mouse_pos.y > game_window.y) && (mouse_pos.y < game_window.y + game_window.z));
}
