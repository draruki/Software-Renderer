#define SDL_MAIN_HANDLED

#include <iostream>
#include <map>
#include <vector>
#include "SDL.h"
#include "Vector.h"
#include "AssimpLoader.h"
#include "Camera.h"
#include "Canvas.h"
#include "Transform.h"
#include "MathUtils.h"
#include "Model.h"
#include "Color.h"
#include "Texture.h"
#include "Bresenham.h"
#include "PrimitiveMeshes.h"

#define NEAR_PLANE_INCREMENTAL_CHANGE 1.0f
#define PERSPECTIVE_FIELD_OF_VIEW 90.f
#define MAX_VIEWPORT_SCALE 8
#define SUN_ROTATION_SPEED 1
#define ORTHOGRAPHIC_SCENE_WIDTH 400
#define ORTHOGRAPHIC_SCENE_FAR -100
#define MODEL_ROTATION_DAMPENING 0.004f
#define POV_INCREMENTAL_CHANGE 3
#define MODEL_MOVE_SPEED 0.1f

struct SceneObject
{
	Model* model;
	Transform* transform;
};

struct Frustum
{
	float left;
	float right;
	float bottom;
	float top;
	float near;
	float far;
};

struct SceneModel
{
	Model* model;
	Transform transform;
	Transform cameraTransform;
};

struct Light
{
	vec3 direction;
	Color color;

	Light(vec3 direction)
	{
		this->direction = direction.normalized();
		color.r = 255;
		color.g = 255;
		color.b = 255;
		color.a = 255;
	}
};

struct Material
{
	vec3 diffuse;
	vec3 emissive;
	vec3 specular;
	enum ShadingType shading;
	int texture;
};

struct PointLight
{
	vec3 position;
	Color color;

	PointLight(vec3 position, Color color)
	{
		this->position = position;
		this->color = color;
	}
};

struct Point
{
	int x;
	int y;

	Point(int x = 0, int y = 0)
	{
		this->x = x;
		this->y = y;
	}
};

enum ShadingType { FLAT_SHADING = 0, GOURAUD_SHADING, PHONG_SHADING, MAX_SHADING };
enum DRAW_MODE { DRAW_POINTS, DRAW_TRIANGLES_WIREFRAME, DRAW_TRIANGLES_FILLED, DRAW_LINES };

std::string WINDOW_TITLE = "3D Software Renderer";
int CLIP_X_VERTICES = 1;
int CLIP_Y_VERTICES = 1;
int CLIP_Z_VERTICES = 1;
float PERSPECTIVE_NEAR_PLANE = -0.1f;
float PERSPECTIVE_FAR_PLANE = -340.f;
int REALTIME_REDRAW = 1;
int SLOW_PIXEL_PLOTTING = 0;
int COMMENT_PLOTTING = 0;
bool WIREFRAME_MODE = true;
unsigned int WINDOW_WIDTH = 640;
unsigned int WINDOW_HEIGHT = 480;
bool PSX_FIXED_POINT_VERTICES_ENABLED = false;
bool CULL_BACKFACE = true;

void displayBackBuffer();
void drawVertexArray();
void pollInputEvents();

std::string assetsFolderPath = "C:/Programming/C++/Rasterizer/assets/";
std::string path;
std::map<const char*, Model> modelMap;
std::map<std::string, Texture> textureCache;
struct Camera camera;
Color ambientLighting(0, 0, 0, 255);
std::vector<SceneModel> sceneModels;
SDL_Surface* windowSurface;
SDL_Window* window;
int viewportScale = 1;
int currentModelIndex = 0;
int discardedTriangles;
Canvas canvas;
AssimpLoader assimpLoader;
SDL_Event event;
bool quit = false;
float fov;
bool resized;
bool modelRotationEnabled = false;
Texture randomTex;
SceneObject currentSceneObject;
struct Frustum perspectiveFrustum;
struct Frustum orthoFrustum;
int SHADING_MODE = PHONG_SHADING;
Light directionalLight(vec3(0, -1, -1.3f));
Model triangleModel;
Model officeModel;
Model bighosroomModel;
Model cubeModel;
Model dragonslayerModel;
Model steelmaceModel;
Model woodsideModel;
Model alessasRoomModel;
Model heathersroomModel;
Transform triangleTransform;
Transform officeTransform;
Transform bighosroomTransform;
Transform cubeTransform;
Transform dragonslayerTransform;
Transform woodsideTransform;
Transform executionerTransform;

PointLight pointLights[] =
{
	PointLight(vec3(200, 30, 130), Color(255, 0, 0, 255)),
	PointLight(vec3(-200, 30, 130), Color(255, 0, 0, 255)),
	PointLight(vec3(-200, 30, -130), Color(255, 0, 0, 255)),
	PointLight(vec3(50, 30, 100), Color(0, 0, 255, 255)),
	PointLight(vec3(260, 20, 20), Color(0, 255, 0, 255)),
	PointLight(vec3(-260, 20, 20), Color(0, 255, 0, 255)),
	PointLight(vec3(50, 30, 240), Color(215, 155, 90, 255))
};

void modelRotationUpdate(Transform* modelTransform, SDL_Event* event)
{
	if (modelTransform != NULL && modelRotationEnabled)
	{
		modelTransform->rotation.y += -event->motion.xrel * MODEL_ROTATION_DAMPENING;
		modelTransform->rotation.x += -event->motion.yrel * MODEL_ROTATION_DAMPENING;
	}
}

void modelMovementUpdate(Transform* modelTransform, SDL_Event* event)
{
	if (modelTransform != NULL)
	{
		if(event->key.keysym.sym == SDLK_LEFT)
			modelTransform->position += -vec3_right * MODEL_MOVE_SPEED;
		if(event->key.keysym.sym == SDLK_RIGHT)
			modelTransform->position += vec3_right * MODEL_MOVE_SPEED;
		if(event->key.keysym.sym == SDLK_UP)
			modelTransform->position += -vec3_forward * MODEL_MOVE_SPEED;
		if(event->key.keysym.sym == SDLK_DOWN)
			modelTransform->position += vec3_forward * MODEL_MOVE_SPEED;
	}
}

struct Frustum createFrustum(float width, float near, float far)
{
	struct Frustum frustum;

	float aspectRatio = WINDOW_WIDTH * 1.f / WINDOW_HEIGHT;
	float screenWidth = width;
	float screenHeight = (screenWidth / aspectRatio);
	frustum.left = -(screenWidth / 2);
	frustum.right = screenWidth / 2;
	frustum.bottom = -(screenHeight / 2);
	frustum.top = screenHeight / 2;
	frustum.near = near;
	frustum.far = far;

	return frustum;
}

void printFrustum(Frustum frustum, float fov_deg, float aspect)
{
	printf("frustum aspect ratio is %f\n", aspect);
	printf("frustum fov is %f\n", fov_deg);
	printf("frustum right is %f. frustum top is %f\n", frustum.right, frustum.top);
	printf("frustum left is %f. frustum bottom is %f\n", frustum.left, frustum.bottom);
	printf("frustum near is %f. frustum far is %f\n", frustum.near, frustum.far);
	puts("");
}

struct Frustum createPerspectiveFrustum(float fov_deg, float aspect, float near, float far)
{
	struct Frustum frustum;

	float fov = deg_to_rad(fov_deg);
    float tangent = tanf(fov / 2.0f);
    float height = abs(near) * tangent;
    float width = height * aspect;
	frustum.left = -width;
	frustum.right = width;
	frustum.top = height;
	frustum.bottom = -height;
	frustum.near = near;
	frustum.far = far;

	return frustum;
}

void initializeOrthographicBoundingBox()
{
	float aspectRatio = WINDOW_WIDTH * 1.f / WINDOW_HEIGHT;
	float sceneWidth = ORTHOGRAPHIC_SCENE_WIDTH;
	float sceneHeight = roundf(sceneWidth / aspectRatio);
	orthoFrustum.left = -(sceneWidth / 2);
	orthoFrustum.right = sceneWidth / 2;
	orthoFrustum.bottom = -(sceneHeight / 2);
	orthoFrustum.top = sceneHeight / 2;
	orthoFrustum.near = 0;
	orthoFrustum.far = ORTHOGRAPHIC_SCENE_FAR;
}

vec3 transformVertexFromViewSpaceToClipSpace(vec3 vertex)
{
	float width = orthoFrustum.right - orthoFrustum.left + 1;
	float height = orthoFrustum.top - orthoFrustum.bottom + 1;
	float depth = orthoFrustum.far - orthoFrustum.near;
	vertex.x = vertex.x / width * 2;
	vertex.y = vertex.y / height * 2;
	vertex.z = vertex.z / depth * 2;

	return vertex;
}

void setPixel(int x,int y, Uint32 color)
{
	canvas.setPixel(x, y, color);

	if (SLOW_PIXEL_PLOTTING)
	{
		displayBackBuffer();
		pollInputEvents();
	}
}

vec3 calculateBarycentricCoordinates(float px, float py, vec3 v1, vec3 v2, vec3 v3)
{
	float b1 = ((py - v3.y) * (v2.x - v3.x)  +  (v2.y - v3.y) * (v3.x - px))   /   ((v1.y - v3.y) * (v2.x - v3.x)  +  (v2.y - v3.y) * (v3.x - v1.x));
	float b2 = ((py - v1.y) * (v3.x - v1.x)  +  (v3.y - v1.y) * (v1.x - px))   /   ((v1.y - v3.y) * (v2.x - v3.x)  +  (v2.y - v3.y) * (v3.x - v1.x));
	float b3 = ((py - v2.y) * (v1.x - v2.x)  +  (v1.y - v2.y) * (v2.x - px))   /   ((v1.y - v3.y) * (v2.x - v3.x)  +  (v2.y - v3.y) * (v3.x - v1.x));

	return vec3(b1, b2, b3);
}

bool isPixelInsideTriangle(vec3 b)
{
	float b1 = b.x;
	float b2 = b.y;
	float b3 = b.z;
	
	if (b1 < 0 || b1 > 1) return false;
	if (b2 < 0 || b2 > 1) return false;
	if (b3 < 0 || b3 > 1) return false;

	return true;
}

void rasterizeTriangle(Point p1, Point p2, Point p3, vec3 n1, vec3 n2, vec3 n3, vec2 uv1, vec2 uv2, vec2 uv3, vec3 depth, vec3 w1, vec3 w2, vec3 w3, Texture& tex)
{
	int start = SDL_GetTicks();
	int minX = min(min(p1.x, p2.x), p3.x);
	int maxX = max(max(p1.x, p2.x), p3.x);
	int minY = min(min(p1.y, p2.y), p3.y);
	int maxY = max(max(p1.y, p2.y), p3.y);
	
	static Color c1(255, 0, 0, 255);
	static Color c2(0, 255, 0, 255);
	static Color c3(0, 0, 255, 255);
	static Color pixelColor;
	static vec3 pixelNormal;
	static float pixelDepth;
	static vec2 pixelUV;
	static float pixelLighting;
	static UintZ fragmentZ;
	static vec3 pixelWorldPosition;

	Color materialColor(255, 255, 255, 255);
	vec3 flatTriangleNormal = (n1 + n2 + n3) / 3;
	float triangleLightIntensity = clamp(dot(flatTriangleNormal, -directionalLight.direction), 0, 1);
	vec3 gouraudLighting;
	gouraudLighting.x = (clamp(dot(n1, -directionalLight.direction), 0, 1));
	gouraudLighting.y = (clamp(dot(n2, -directionalLight.direction), 0, 1));
	gouraudLighting.z = (clamp(dot(n3, -directionalLight.direction), 0, 1));
	
	vec3 v1(p1.x, p1.y);
	vec3 v2(p2.x, p2.y);
	vec3 v3(p3.x, p3.y);

	float divider = ((v1.y - v3.y) * (v2.x - v3.x)  +  (v2.y - v3.y) * (v3.x - v1.x));
	if (divider == 0) divider = 1;

	for (int px = minX; px < maxX; ++px)
	{
		for (int py = minY; py < maxY; ++py)
		{
			float b1 = ((py - v3.y) * (v2.x - v3.x)  +  (v2.y - v3.y) * (v3.x - px))   /   divider;
			float b2 = ((py - v1.y) * (v3.x - v1.x)  +  (v3.y - v1.y) * (v1.x - px))   /   divider;
			float b3 = ((py - v2.y) * (v1.x - v2.x)  +  (v1.y - v2.y) * (v2.x - px))   /   divider;
			vec3 b(b1, b2, b3);

			if (b1 < 0 || b1 > 1  ||  b2 < 0 || b2 > 1  ||  b3 < 0 || b3 > 1) continue;
			
			pixelDepth = depth.x * b.x + depth.y * b.y + depth.z * b.z;
			fragmentZ = (UintZ)(MAX_DEPTH_VALUE * ((pixelDepth + 1) / 2) - 1);

			if (fragmentZ > canvas.getDepth(px, py)) continue;

			canvas.setDepth(px, py, fragmentZ);
			pixelColor = c1 * b.x + c2 * b.y + c3 * b.z;
			pixelNormal = n1 * b.x + n2 * b.y + n3 * b.z;
			pixelLighting = gouraudLighting.x * b.x + gouraudLighting.y * b.y + gouraudLighting.z * b.z;
			pixelWorldPosition = b.x * w1 + b.y * w2 + b.z * w3;
			pixelUV = b.x * uv1 + b.y * uv2 + b.z * uv3;
			Color fragmentColor = materialColor;
			
			if (SHADING_MODE == PHONG_SHADING || SHADING_MODE == FLAT_SHADING)
			{
				Color diffuse(0, 0, 0, 0);
				for(int i = 0; i < sizeof(pointLights)/sizeof(PointLight); ++i)
				{
					vec3 pointLightDirection = (pointLights[i].position - pixelWorldPosition).normalized();
					float pointLightDistance = (pointLights[i].position - pixelWorldPosition).magnitude();
					float brightness = clamp(clamp(dot(pixelNormal, pointLightDirection), 0, 1) / (pointLightDistance * pointLightDistance * 0.0003f), 0, 1);
					if (SHADING_MODE == FLAT_SHADING)
						diffuse = diffuse + (materialColor * pointLights[i].color * brightness);
					if (SHADING_MODE == PHONG_SHADING)
						diffuse = diffuse + (tex.sample(pixelUV.x, pixelUV.y) * brightness);
				}

				fragmentColor = diffuse + ambientLighting;
			}
			else if (SHADING_MODE == FLAT_SHADING && false)
			{
				Color diffuse = materialColor * directionalLight.color * triangleLightIntensity;
				fragmentColor = diffuse + ambientLighting;
			}
			else if (SHADING_MODE == GOURAUD_SHADING)
			{
				Color diffuse = materialColor * directionalLight.color * pixelLighting;
				fragmentColor = diffuse + ambientLighting;
			}

			setPixel(px, py, fragmentColor.get());
		}
	}
}

void drawWireframeTriangle(Point p1, Point p2, Point p3)
{
	bresenham(p1.x, p1.y, p2.x, p2.y, 0xFFFFFFFF);
	bresenham(p2.x, p2.y, p3.x, p3.y, 0xFFFFFFFF);
	bresenham(p3.x, p3.y, p1.x, p1.y, 0xFFFFFFFF);
}

vec3 transformFromViewportToClip(vec2 point)
{
	vec3 p;

	point.x /= (WINDOW_WIDTH-1);
	point.x *= 2;
	point.x -= 1;
	
	point.y = (WINDOW_HEIGHT-1) - point.y;
	point.y /= (WINDOW_HEIGHT-1);
	point.y *= 2;
	point.y -= 1;
	
	p.x = point.x;
	p.y = point.y;
	p.z = 0;

	return p;
}

vec3 transformFromClipToViewport(vec3 point)
{
	point.x += 1;
	point.x = point.x / 2;
	point.x = point.x * (canvas.width);
	
	point.y += 1;
	point.y = point.y / 2;
	point.y = point.y * (canvas.height);
	point.y = (canvas.height) - point.y;

	return point;
}

vec3 transformPointFromPerspectiveViewSpaceToClipSpace(vec3 vertex, struct Frustum frustum)
{
	if (vertex.z > frustum.near) vertex.z = frustum.far - 10;

	vertex.x = (vertex.x * abs(frustum.near)) / (vertex.z);
	vertex.y = (vertex.y * abs(frustum.near)) / (vertex.z);

	float width = frustum.right - frustum.left;
	float height = frustum.top - frustum.bottom;
	float depth = frustum.far - frustum.near;

	vertex.x = vertex.x / width * 2;
	vertex.y = vertex.y / height * 2;
	vertex.z = vertex.z / depth * 2 - 1;

	vertex.x = ((1 - ((vertex.x + 1) / 2)) * 2) - 1;
	vertex.y = ((1 - ((vertex.y + 1) / 2)) * 2) - 1;

	return vertex;
}

vec3 transformVertexFromModelToViewport(vec3 vertex, Transform &worldTransform)
{
	vertex = transformVertex(vertex, worldTransform);
	vertex -= camera.transform.position;
	vertex = rotateVector(vertex, camera.transform);

	if(PSX_FIXED_POINT_VERTICES_ENABLED)
	{
		vertex.x = roundf(vertex.x);
		vertex.y = roundf(vertex.y);
		vertex.z = roundf(vertex.z);
	}

	vertex = transformPointFromPerspectiveViewSpaceToClipSpace(vertex, perspectiveFrustum);
	vertex = transformFromClipToViewport(vertex);

	return vertex;
}

void drawVertexArray(const vec3 *vertices, const vec3 *normals, const vec2 *uvs, int count, std::string textureKey, Uint32 color, enum DRAW_MODE drawMode, Transform &worldTransform)
{
	Uint32 start = SDL_GetTicks();
	static Texture emptyTexture;
	emptyTexture.generateRandom(10, 10);
	Texture* texture;

	if (strcmp(textureKey.c_str(),"") != 0)
	{
		texture = &textureCache[textureKey];
	}
	else
	{
		texture = &emptyTexture;
	}

	if(drawMode == DRAW_POINTS)
	{
		for (int i = 0; i < count; i += 1)
		{
			vec3 v = transformVertexFromModelToViewport(vertices[i], worldTransform);
			
			bool clip1 = (v.z < -1 || v.z > 1);
			if (clip1)
			{
				++discardedTriangles;
				continue;
			}

			setPixel(v.x, v.y, color);
		}
	}
	else if (drawMode == DRAW_LINES)
	{
		for (int i = 0; i < count; i += 2)
		{
			vec3 v1 = transformVertexFromModelToViewport(vertices[i], worldTransform);
			vec3 v2 = transformVertexFromModelToViewport(vertices[i + 1], worldTransform);
			bool clip1 = (v1.z < -1 || v1.z > 1);
			bool clip2 = (v2.z < -1 || v2.z > 1);

			if (clip1 || clip2)
			{
				++discardedTriangles;
				continue;
			}

			bresenham(v1.x, v1.y, v2.x, v2.y, color);
		}
	}
	else if (drawMode == DRAW_TRIANGLES_FILLED || drawMode == DRAW_TRIANGLES_WIREFRAME)
	{
		for (int i = 0; i < count; i += 3)
		{
			vec3 v1 = transformVertexFromModelToViewport(vertices[i], worldTransform);
			vec3 v2 = transformVertexFromModelToViewport(vertices[i + 1], worldTransform);
			vec3 v3 = transformVertexFromModelToViewport(vertices[i + 2], worldTransform);

			if (CULL_BACKFACE)
			{
				int area = v1.x * (v2.y - v3.y) + v2.x * (v3.y - v1.y) + v3.x * (v1.y - v2.y);
				if (area > 0) continue;
			}

			bool clip_v1_x = v1.x < 0 || v1.x >= WINDOW_WIDTH;
			bool clip_v1_y = v1.y < 0 || v1.y >= WINDOW_HEIGHT;
			bool clip_v1_z = v1.z < -1 || v1.z > 1;
			bool clip_v2_x = v2.x < 0 || v2.x >= WINDOW_WIDTH;
			bool clip_v2_y = v2.y < 0 || v2.y >= WINDOW_HEIGHT;
			bool clip_v2_z = v2.z < -1 || v2.z > 1;
			bool clip_v3_x = v3.x < 0 || v3.x >= WINDOW_WIDTH;
			bool clip_v3_y = v3.y < 0 || v3.y >= WINDOW_HEIGHT;
			bool clip_v3_z = v3.z < -1 || v3.z > 1;
			
			if (CLIP_X_VERTICES &&  (clip_v1_x && clip_v2_x && clip_v3_x)) continue;
			if (CLIP_Y_VERTICES &&  (clip_v1_y && clip_v2_y && clip_v3_y)) continue;
			if (CLIP_Z_VERTICES &&  (clip_v1_z || clip_v2_z || clip_v3_z)) continue;
			
			vec3 n1 = normals[i];
			vec3 n2 = normals[i + 1];
			vec3 n3 = normals[i + 2];

			if (normals == NULL)
			{
				n1 = n2 = n3 = cross(v2-v1, v3-v1).normalized();
			}
			else
			{
				n1 = normals[i];
				n2 = normals[i + 1];
				n3 = normals[i + 2];
			}

			n1 = rotateVector(n1, worldTransform);
			n2 = rotateVector(n2, worldTransform);
			n3 = rotateVector(n3, worldTransform);
			
			if (drawMode == DRAW_TRIANGLES_WIREFRAME)
			{
				drawWireframeTriangle(Point(v1.x, v1.y),  Point(v2.x, v2.y),  Point(v3.x, v3.y));
			}
			else if (drawMode == DRAW_TRIANGLES_FILLED)
			{
				vec2 uv1;
				vec2 uv2;
				vec2 uv3;
				if(uvs == NULL)
				{
					uv1 = vec2(0, 0);
					uv2 = vec2(1, 0);
					uv3 = vec2(1, 1);
				}
				else
				{
					uv1 = uvs[i];
					uv2 = uvs[i + 1];
					uv3 = uvs[i + 2];
				}

				vec3 w1 = transformVertex(vertices[i], worldTransform);
				vec3 w2 = transformVertex(vertices[i + 1], worldTransform);
				vec3 w3 = transformVertex(vertices[i + 2], worldTransform);

				rasterizeTriangle(Point(v1.x, v1.y),  Point(v2.x, v2.y),  Point(v3.x, v3.y), n1, n2, n3, uv1, uv2, uv3, vec3(v1.z, v2.z, v3.z), w1, w2, w3, *texture);
			}
		}
	}
}

void drawModel(Model *model, Uint32 color, enum DRAW_MODE drawMode, Transform &worldTransform)
{
	if (model->hasMeshes())
	{
		for (int i = 0; i < model->mesh_count; ++i)
		{
			drawVertexArray(model->mesh_array[i].vertex_array, model->mesh_array[i].normal_array, model->mesh_array[i].uv_array, model->mesh_array[i].vertex_count, model->mesh_array[i].textureKey, color, drawMode, worldTransform);
		}
	}
}

void displayBackBuffer()
{
	static SDL_Rect target;
	target.x = 0;
	target.y = 0;
	target.w = WINDOW_WIDTH;
	target.h = WINDOW_HEIGHT;
    SDL_BlitScaled(canvas.framebuffer, NULL, windowSurface, &target);
	SDL_UpdateWindowSurface(window);
}

void createCanvas()
{
	canvas.initialize(WINDOW_WIDTH / max(1, viewportScale), WINDOW_HEIGHT / max(1, viewportScale));
}

void onMouseDown(int x, int y, int button)
{
	if(button == SDL_BUTTON_LEFT)
	{
		modelRotationEnabled = true;
	}
}

void onMouseUp(int x, int y, int button)
{
	if(event.button.button == SDL_BUTTON_LEFT)
	{
		modelRotationEnabled = false;
	}
}

void onKeyDown(int keysym)
{
	if(keysym == SDLK_ESCAPE)
	{
		quit = true;
	}
	if(keysym == SDLK_SPACE)
	{
		REALTIME_REDRAW = !REALTIME_REDRAW;
		COMMENT_PLOTTING = false;
		displayBackBuffer();
	}
	if(keysym == SDLK_r)
	{
		if(SLOW_PIXEL_PLOTTING)
		{
			SLOW_PIXEL_PLOTTING = 0;
			COMMENT_PLOTTING = 0;
		}
		else
		{
			SLOW_PIXEL_PLOTTING = 1;
			COMMENT_PLOTTING = 1;
			canvas.resetDepthBuffer();
			SDL_FillRect(canvas.framebuffer, NULL, 0x000000);
			drawModel(sceneModels[currentModelIndex].model, 0xFFFFFFFF, WIREFRAME_MODE ? DRAW_TRIANGLES_WIREFRAME : DRAW_TRIANGLES_FILLED, sceneModels[currentModelIndex].transform);
			displayBackBuffer();
			SLOW_PIXEL_PLOTTING = 0;
			COMMENT_PLOTTING = 0;
		}
	}
	if (keysym == SDLK_1)
	{
		fov = clamp(fov - POV_INCREMENTAL_CHANGE, 0, 180);
		perspectiveFrustum = createPerspectiveFrustum(fov, WINDOW_WIDTH * 1.f / WINDOW_HEIGHT, PERSPECTIVE_NEAR_PLANE, PERSPECTIVE_FAR_PLANE);
	}
	if (keysym == SDLK_2)
	{
		fov = clamp(fov + POV_INCREMENTAL_CHANGE, 0, 180);
		perspectiveFrustum = createPerspectiveFrustum(fov, WINDOW_WIDTH * 1.f / WINDOW_HEIGHT, PERSPECTIVE_NEAR_PLANE, PERSPECTIVE_FAR_PLANE);
	}
	if(keysym == SDLK_3)
	{
		PERSPECTIVE_NEAR_PLANE += NEAR_PLANE_INCREMENTAL_CHANGE;
		if (PERSPECTIVE_NEAR_PLANE > -0.001f) PERSPECTIVE_NEAR_PLANE = -0.001f;

		perspectiveFrustum = createPerspectiveFrustum(fov, WINDOW_WIDTH * 1.f / WINDOW_HEIGHT, PERSPECTIVE_NEAR_PLANE, PERSPECTIVE_FAR_PLANE);
	}
	if(keysym == SDLK_4)
	{
		PERSPECTIVE_NEAR_PLANE -= NEAR_PLANE_INCREMENTAL_CHANGE;
		perspectiveFrustum = createPerspectiveFrustum(fov, WINDOW_WIDTH * 1.f / WINDOW_HEIGHT, PERSPECTIVE_NEAR_PLANE, PERSPECTIVE_FAR_PLANE);
	}
	if(keysym == SDLK_5)
	{
		PERSPECTIVE_FAR_PLANE += NEAR_PLANE_INCREMENTAL_CHANGE * 10;
		if (PERSPECTIVE_FAR_PLANE > 0) PERSPECTIVE_FAR_PLANE = -0.001f;

		perspectiveFrustum = createPerspectiveFrustum(fov, WINDOW_WIDTH * 1.f / WINDOW_HEIGHT, PERSPECTIVE_NEAR_PLANE, PERSPECTIVE_FAR_PLANE);
	}
	if(keysym == SDLK_6)
	{
		PERSPECTIVE_FAR_PLANE -= NEAR_PLANE_INCREMENTAL_CHANGE * 10;
		perspectiveFrustum = createPerspectiveFrustum(fov, WINDOW_WIDTH * 1.f / WINDOW_HEIGHT, PERSPECTIVE_NEAR_PLANE, PERSPECTIVE_FAR_PLANE);
	}
	if (keysym == SDLK_f)
	{
		WIREFRAME_MODE = !WIREFRAME_MODE;
	}
	if (keysym == SDLK_n)
	{
		++currentModelIndex;
		if (currentModelIndex >= sceneModels.size()) currentModelIndex = 0;
	}
	if (keysym == SDLK_b)
	{
		--currentModelIndex;
		if (currentModelIndex < 0) currentModelIndex = sceneModels.size() - 1;
	}

	modelMovementUpdate(currentSceneObject.transform, &event);

	if (keysym == SDLK_z)
	{
		CLIP_Z_VERTICES = !CLIP_Z_VERTICES;
	}
	if (keysym == SDLK_x)
	{
		CLIP_X_VERTICES = !CLIP_X_VERTICES;
	}
	if (keysym == SDLK_y)
	{
		CLIP_Y_VERTICES = !CLIP_Y_VERTICES;
	}

	if(keysym == SDLK_w)
		camera.movingForward = true;
	if(keysym == SDLK_s)
		camera.movingBackward = true;
	if(keysym == SDLK_a)
		camera.movingLeft = true;
	if(keysym == SDLK_d)
		camera.movingRight = true;
	if(keysym == SDLK_e)
		camera.movingUp = true;
	if(keysym == SDLK_q)
		camera.movingDown = true;
	if(keysym == SDLK_c)
	{
		CULL_BACKFACE = !CULL_BACKFACE;
	}
	if (keysym == SDLK_l)
	{
		++SHADING_MODE;
		if (SHADING_MODE >= MAX_SHADING)
			SHADING_MODE = 0;
	}
	if (keysym == SDLK_k)
	{
		--SHADING_MODE;
		if (SHADING_MODE < 0)
			SHADING_MODE = MAX_SHADING - 1;;
	}
	if(keysym == SDLK_p)
	{
		PSX_FIXED_POINT_VERTICES_ENABLED = !PSX_FIXED_POINT_VERTICES_ENABLED;
	}
	if (keysym == SDLK_PLUS)
	{
		viewportScale = min(viewportScale + 1, MAX_VIEWPORT_SCALE);
		createCanvas();
	}
	if (keysym == SDLK_MINUS)
	{
		viewportScale = max(viewportScale - 1, 1);
		createCanvas();
	}
}

void onKeyUp(int keysym)
{
	if(keysym == SDLK_w)
		camera.movingForward = false;
	if(keysym == SDLK_s)
		camera.movingBackward = false;
	if(keysym == SDLK_a)
		camera.movingLeft = false;
	if(keysym == SDLK_d)
		camera.movingRight = false;
	if(keysym == SDLK_e)
		camera.movingUp = false;
	if(keysym == SDLK_q)
		camera.movingDown = false;
}

void onMouseWheelEvent()
{
	if (event.wheel.y > 0)
	{
		camera.orthoScale += CAMERA_SCALE_SPEED;
	}
	if (event.wheel.y < 0)
	{
		camera.orthoScale -= CAMERA_SCALE_SPEED;
		if (camera.orthoScale < 0) camera.orthoScale = 0;

	}
}

void pollInputEvents()
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
			{
				quit = true;
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				onMouseDown(event.button.x, event.button.y, event.button.button);
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				onMouseUp(event.button.x, event.button.y, event.button.button);
				break;
			}
			case SDL_KEYDOWN:
			{
				onKeyDown(event.key.keysym.sym);
				break;
			}
			case SDL_KEYUP:
			{
				onKeyUp(event.key.keysym.sym);
				break;
			}
			case SDL_MOUSEMOTION:
			{
				modelRotationUpdate(&camera.transform, &event);
				break;
			}
			case SDL_MOUSEWHEEL:
			{
				onMouseWheelEvent();
				break;
			}
			case SDL_WINDOWEVENT:
			{
				switch (event.window.event)
				{
					case SDL_WINDOWEVENT_RESIZED:
					{
						break;
					}
					case SDL_WINDOWEVENT_SIZE_CHANGED:
					{
						windowSurface = SDL_GetWindowSurface(window);
						WINDOW_WIDTH = (unsigned int) event.window.data1;
						WINDOW_HEIGHT = (unsigned int) event.window.data2;
						perspectiveFrustum = createPerspectiveFrustum(fov, WINDOW_WIDTH * 1.f / WINDOW_HEIGHT, PERSPECTIVE_NEAR_PLANE, PERSPECTIVE_FAR_PLANE);
						createCanvas();
					}
					case SDL_WINDOWEVENT_MINIMIZED:
					{
						break;
					}
					case SDL_WINDOWEVENT_MAXIMIZED:
					{
						break;
					}
				}

				break;
			}
		}
	}
}

void initialize()
{
	initializeOrthographicBoundingBox();
	perspectiveFrustum = createPerspectiveFrustum(PERSPECTIVE_FIELD_OF_VIEW, WINDOW_WIDTH * 1.f / WINDOW_HEIGHT, PERSPECTIVE_NEAR_PLANE, PERSPECTIVE_FAR_PLANE);
	fov = PERSPECTIVE_FIELD_OF_VIEW;
    bool quit = false;
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	SDL_SetWindowResizable(window, SDL_TRUE);
	windowSurface = SDL_GetWindowSurface(window);
    Uint32 wpf = SDL_GetWindowPixelFormat(window);
    WINDOW_PIXEL_FORMAT = SDL_AllocFormat(wpf);
	createCanvas();
}

void terminate()
{
	SDL_FreeFormat(WINDOW_PIXEL_FORMAT);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void tick()
{	
	directionalLight.direction = rotate(directionalLight.direction, vec3_up, SUN_ROTATION_SPEED);
	camera.update();
	canvas.resetDepthBuffer();
	SDL_FillRect(canvas.framebuffer, NULL, 0x000000);
	drawModel(sceneModels[currentModelIndex].model, 0xFFFFFFFF, WIREFRAME_MODE ? DRAW_TRIANGLES_WIREFRAME : DRAW_TRIANGLES_FILLED, sceneModels[currentModelIndex].transform);
	displayBackBuffer();
}

void setupModels()
{
	vec3 triangleVerticesClassic[] = 
	{
		vec3(-1, -1, 0),
		vec3(0, 1, 0),
		vec3(1, -1, 0)
	};

	modelMap["triangle"].name = "triangle model";
	modelMap["triangle"].createMeshes(1);
	modelMap["triangle"].mesh_array[0].setPositions(triangleVerticesClassic, sizeof(triangleVerticesClassic) / sizeof(vec3));
	modelMap["triangle"].mesh_array[0].name = "triangle first (and only) mesh";
	triangleTransform.scale.setAll(1);
	triangleTransform.position.set(0, 0, -2.4f);
	Color triangleColor(225, 225, 255, 255);

	Mesh* cubeMesh = PrimitiveMeshes::Cube();
	modelMap["cube"].name = "cube model";
	modelMap["cube"].createMeshes(1);
	modelMap["cube"].mesh_array[0].setPositions(cubeMesh->vertex_array, cubeMesh->vertex_count);
	modelMap["cube"].mesh_array[0].setNormals(cubeMesh->normal_array, cubeMesh->normal_count);
	modelMap["cube"].mesh_array[0].setUVs(cubeMesh->uv_array, cubeMesh->uv_count);
	modelMap["cube"].mesh_array[0].name = "cube first (and only) mesh";
	delete cubeMesh;
	cubeTransform.scale.setAll(4);
	cubeTransform.position.z = -1.05f;
	cubeTransform.position.y = 0;

	officeTransform.scale.setAll(1);
	officeTransform.position.z = -7.15f;
	path = "Silent Hill/office/";
	assimpLoader.importFile((assetsFolderPath + path).c_str(), "office.obj", 0, &modelMap["office"]);

	bighosroomTransform.scale.setAll(1);
	bighosroomTransform.position.z = -3.15f;;
	path = "Silent Hill/brookhaven/";
	assimpLoader.importFile((assetsFolderPath + path).c_str(), "lounge.obj", 0,  &modelMap["lounge"]);

	path = "berzerk/Steel Mace/";
	assimpLoader.importFile((assetsFolderPath + path).c_str(), "steelmace02.obj", 0, &modelMap["steel mace"]);

	path = "Silent Hill/greenmotel/";
	assimpLoader.importFile((assetsFolderPath + path).c_str(), "SH_BUILDING_13.obj", 0, &modelMap["green motel"]);

	path = "berzerk/The Executioner/";
	assimpLoader.importFile((assetsFolderPath + path).c_str(), "TheExecutioner.dae", 0, &modelMap["executioner"]);
	
	path = "Silent Hill/alley/";
	assimpLoader.importFile((assetsFolderPath + path).c_str(), "alley.obj", 0, &modelMap["alley"]);
	
	woodsideTransform.scale.setAll(11);
	woodsideTransform.rotation.x = 0;

	currentSceneObject.model = &modelMap["green motel"];
	currentSceneObject.transform = &cubeTransform;
	currentSceneObject.transform = &woodsideTransform;

	executionerTransform.scale.setAll(0.05f);
	executionerTransform.rotation.x = PI / 2;

	camera.transform.position.set(-7, 7, 20);

	SceneModel sm;
	sm.model = &modelMap["green motel"];
	sm.transform.position.set(-60, 0, -60);
	sm.transform.rotation.set(0, 0, 0);
	sm.transform.scale.setAll(15);
	sceneModels.push_back(sm);
	
	sm.model = &modelMap["office"];
	sm.transform.position.set(0, 0, 0);
	sm.transform.rotation.set(0, 0, 0);
	sm.transform.scale.setAll(20);
	sceneModels.push_back(sm);
	
	sm.model = &modelMap["lounge"];
	sm.transform.position.set(0, 0, 0);
	sm.transform.rotation.set(0, 0, 0);
	sm.transform.scale.setAll(25);
	sceneModels.push_back(sm);
	
	sm.model = &modelMap["alley"];
	sm.transform.position.set(0, 0, 0);
	sm.transform.rotation.set(0, 0, 0);
	sm.transform.scale.setAll(10);
	sceneModels.push_back(sm);
	
	sm.model = &modelMap["gas station"];
	sm.transform.position.set(0, 0, 0);
	sm.transform.rotation.set(0, 0, 0);
	sm.transform.scale.setAll(12);
	sceneModels.push_back(sm);

	std::map<std::string, Texture>::iterator it;
}

int main(int argc, char** argv)
{
	initialize();
	setupModels();

	tick();
    while (!quit)
    {
		pollInputEvents();

		if (REALTIME_REDRAW)
		{
			tick();

			pointLights[3].position = camera.transform.position;
		}
    }
	
	sceneModels.clear();

    return 0;
}