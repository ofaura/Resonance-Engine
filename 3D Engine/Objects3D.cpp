#include "Objects3D.h"

Objects3D::Objects3D(SHAPE_TYPE type, vec3 &position, vec3 &size)
{
	switch (type)
	{
		case SHAPE_TYPE::TETRAHEDRON:
			mesh = par_shapes_create_tetrahedron();
			break;
		case SHAPE_TYPE::CUBE:
			mesh = par_shapes_create_cube();
			break;
		case SHAPE_TYPE::OCTOHEDRON:
			mesh = par_shapes_create_octahedron();
			break;
		case SHAPE_TYPE::DODECAHEDRON:
			mesh = par_shapes_create_dodecahedron();
			break;
		case SHAPE_TYPE::ICOSAHEDRON:
			mesh = par_shapes_create_icosahedron();
			break;
		case SHAPE_TYPE::SPHERE:
			mesh = par_shapes_create_parametric_sphere(24, 12);
			break;
		case SHAPE_TYPE::CYLINDER:
			radius = 1.0f;
			slices = 20;
			stacks = 20;
			mesh = par_shapes_create_cylinder(slices, stacks);
			disk1 = par_shapes_create_disk(radius, slices, center, normal);
			par_shapes_merge(mesh, disk1);
			par_shapes_free_mesh(disk1);
			disk2 = par_shapes_create_disk(radius, slices, center2, normal);
			par_shapes_rotate(disk2, M_PI, rotate);
			par_shapes_merge(mesh, disk2);
			par_shapes_free_mesh(disk2);
			par_shapes_rotate(mesh, M_PI_2, rotate);
			break;
		case SHAPE_TYPE::CONE:
			radius = 1.0f;
			slices = 20;
			stacks = 10;
			mesh = par_shapes_create_cone(slices, stacks);
			disk2 = par_shapes_create_disk(radius, slices, center2, normal);
			par_shapes_rotate(disk2, -M_PI, rotate);
			par_shapes_merge(mesh, disk2);
			par_shapes_free_mesh(disk2);
			par_shapes_rotate(mesh, -M_PI_2, rotate);
			break;
		case SHAPE_TYPE::PLANE:
			slices = 3;
			stacks = 3;
			mesh = par_shapes_create_plane(slices, stacks);
			par_shapes_rotate(mesh, -M_PI_2, rotate);
			break;
		case SHAPE_TYPE::TORUS:
			radius = 0.2f;
			slices = 20;
			stacks = 20;
			mesh = par_shapes_create_torus(slices, stacks, radius);
			par_shapes_rotate(mesh, -M_PI_2, rotate);
			break;
	}

	if (type == SHAPE_TYPE::TETRAHEDRON || type == SHAPE_TYPE::CUBE || type == SHAPE_TYPE::OCTOHEDRON || type == SHAPE_TYPE::DODECAHEDRON || type == SHAPE_TYPE::ICOSAHEDRON)
		par_shapes_compute_normals(mesh);
	
	
	par_shapes_scale(mesh, size.x, size.y, size.z);
	par_shapes_translate(mesh, position.x, position.y, position.z);

	TupleListToFlatList(mesh);
	par_shapes_free_mesh(mesh);

	glGenBuffers(1, &vertex_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
	
	//void glBufferData(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
	//target --> Specifies the target buffer object.The symbolic constant must be GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER.
	//size --> Specifies the size in bytes of the buffer object's new data store.
	//data --> Specifies a pointer to data that will be copied into the data store for initialization, or NULL if no data is to be copied.
	//usage --> Specifies the expected usage pattern of the data store. The symbolic constant must be GL_STREAM_DRAW, GL_STATIC_DRAW, or GL_DYNAMIC_DRAW.

	glBufferData(GL_ARRAY_BUFFER, triangle_vec.size() * sizeof(GL_FLOAT), &triangle_vec[0], GL_STATIC_DRAW); 

	glGenBuffers(1, &normal_id);
	glBindBuffer(GL_ARRAY_BUFFER, normal_id);
	glBufferData(GL_ARRAY_BUFFER, normal_vec.size() * sizeof(GL_FLOAT), &normal_vec[0], GL_STATIC_DRAW);
}


Objects3D::~Objects3D() 
{
	glDeleteBuffers(1, &vertex_id);
	glDeleteBuffers(1, &normal_id);
}

void Objects3D::Draw()
{
	//enable vertices array
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
	
	//glVertexAttribPointer --> define an array of generic vertex attribute data
	//glVertexAttribPointer(index - size - type - normalized - stride - pointer)
	//index --> Specifies the index of the generic vertex attribute to be modified.
	//size --> Specifies the number of components per generic vertex attribute.Must be 1, 2, 3, or 4. The initial value is 4.
	//type --> Specifies the data type of each component in the array.Symbolic constants GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_FIXED, or GL_FLOAT are accepted.The initial value is GL_FLOAT.
	//normalized --> Specifies whether fixed - point data values should be normalized(GL_TRUE) or converted directly as fixed - point values(GL_FALSE) when they are accessed.
	//stride --> Specifies the byte offset between consecutive generic vertex attributes.If stride is 0, the generic vertex attributes are understood to be tightly packed in the array.The initial value is 0.
	//pointer --> Specifies a pointer to the first component of the first generic vertex attribute in the array.The initial value is 0.

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//enable normals array
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normal_id);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, triangle_vec.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

// Function to convert the list PAR_SHAPES_T (tuple-list) into a flat list
void Objects3D::TupleListToFlatList(par_shapes_mesh * mesh)
{
	// Triangle --> 3 vertices 
	for (int i = 0; i < mesh->ntriangles * 3; ++i) 
	{
		PAR_SHAPES_T indices = mesh->triangles[i];
		float* vertices = mesh->points + indices * 3;
		float* normals = mesh->normals + indices * 3;
		
		// {X, Y, Z} = 3 values per vertex
		for (int j = 0; j < 3; ++j) 
		{
			triangle_vec.push_back(*vertices++);
			normal_vec.push_back(*normals++);
		}
	}
}
