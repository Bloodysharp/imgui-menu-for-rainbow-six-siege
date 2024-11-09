#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
#include "imgui_freetype.h"

//#include <imgui_knobs.h>
#include <d3d11.h>
#include <tchar.h>

#include "notify.h"

#include <D3DX11tex.h>
#pragma comment(lib, "D3DX11.lib")

#include "texture.h"
#include "font.h"

static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int tabs = 0;

int text_add = 0;
DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;

bool checkbox = true;
bool checkbox_false = false;
int slider_int = 50;
float slider_float = 0.5f;
char field[46] = { "" };

int select_combo = 0;
const char* items[3]{ "dah one", "dah other one", "dah other other one" };

bool multi_num[5] = { false, false, false, false, false };
const char* multi_items[5] = { "One", "Two", "Three", "Four", "Five" };

std::vector<std::string> words;


static int key, m;

namespace font
{
	inline ImFont* calibri_bold = nullptr;
	inline ImFont* calibri_bold_hint = nullptr;
	inline ImFont* calibri_regular = nullptr;
	inline ImFont* icomoon[14];
}

namespace texture
{
	inline ID3D11ShaderResourceView* background = nullptr;
	inline ID3D11ShaderResourceView* roughness = nullptr;
	inline ID3D11ShaderResourceView* logo = nullptr;
	inline ID3D11ShaderResourceView* arrow = nullptr;
}