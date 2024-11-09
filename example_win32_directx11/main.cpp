#include "main.h"

int main(int, char**)
{

    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_POPUP, 0, 0, 1920, 1080, nullptr, nullptr, wc.hInstance, nullptr);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    

    ImFontConfig cfg;
    ImFontConfig cfg_regular;

    cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags_LoadColor | ImGuiFreeTypeBuilderFlags_Bitmap;
    cfg_regular.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags_LoadColor;

    font::calibri_bold = io.Fonts->AddFontFromMemoryTTF(calibri_bold, sizeof(calibri_bold), 14.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::calibri_bold_hint = io.Fonts->AddFontFromMemoryTTF(calibri_bold, sizeof(calibri_bold), 12.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    font::calibri_regular = io.Fonts->AddFontFromMemoryTTF(calibri_regular, sizeof(calibri_regular), 12.f, &cfg_regular, io.Fonts->GetGlyphRangesCyrillic());

    for (int i = 14; i < 45; i++)
    font::icomoon[i] = io.Fonts->AddFontFromMemoryTTF(icomoon, sizeof(icomoon), i, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);

    D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };
    if (texture::background == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, background, sizeof(background), &info, pump, &texture::background, 0);
    if (texture::roughness == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, roughness, sizeof(roughness), &info, pump, &texture::roughness, 0);
    if (texture::logo == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, logo, sizeof(logo), &info, pump, &texture::logo, 0);

    bool done = false;
    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done) break;

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            ImGuiStyle* style = &ImGui::GetStyle();

            static float color[4] = { 221 / 255.f, 255 / 255.f, 11 / 255.f, 1.f };
            c::accent = { color[0], color[1], color[2], 1.f };

            style->WindowPadding = ImVec2(0, 0);
            style->ItemSpacing = ImVec2(20, 20);
            style->WindowBorderSize = 0;
            style->ScrollbarSize = 10.f;

            ImGui::GetBackgroundDrawList()->AddImage(texture::background, ImVec2(0, 0), ImVec2(1920, 1080), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 200));

            ImGui::SetNextWindowSize(ImVec2(c::bg::size));

            ImGui::Begin("ImGui Menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus);
            {
                const ImVec2& pos = ImGui::GetWindowPos();
                const ImVec2& region = ImGui::GetContentRegionMax();
                const ImVec2& spacing = style->ItemSpacing;

                ImGui::GetBackgroundDrawList()->AddRectFilled(pos, pos + ImVec2(region), ImGui::GetColorU32(c::bg::background), c::bg::rounding);
                ImGui::GetBackgroundDrawList()->AddImageRounded(texture::roughness, pos, pos + ImVec2(region), ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(c::bg::roughness), c::bg::rounding);
                ImGui::PushFont(font::icomoon[30]);
                //50
                // 
                // centered logo
                ImGui::GetWindowDrawList()->AddText(pos + (ImVec2(200, 50) - ImGui::CalcTextSize("L")) / 2, ImGui::GetColorU32(c::accent), "L");
                ImGui::PopFont();
                
                //ImGui::GetWindowDrawList()->AddText(pos + ImVec2((50 - ImGui::CalcTextSize("L").y) / 2, (50 - ImGui::CalcTextSize("L").y) / 2), ImGui::GetColorU32(c::accent), "L");

                ImGui::PushFont(font::icomoon[21]);
                //drawtablogo
                //ImGui::GetWindowDrawList()->AddText(pos + ImVec2(200 + (50 - ImGui::CalcTextSize("a").y) / 2, (50 - ImGui::CalcTextSize("a").y) / 2), ImGui::GetColorU32(c::accent), "a");


                //ImGui::GetWindowDrawList()->AddText(pos + ImVec2(region.x - (50 + ImGui::CalcTextSize("b").y) / 2, (50 - ImGui::CalcTextSize("b").y) / 2), ImGui::GetColorU32(c::text::text), "b");

                ImGui::PopFont();

               // ImGui::GetWindowDrawList()->AddText(pos + ImVec2(200 + 45, (50 - ImGui::CalcTextSize("Aimb0t").y) / 2 + 1), ImGui::GetColorU32(c::text::text_active), "Aimb0t");
                //ImGui::GetWindowDrawList()->AddText(pos + ImVec2(45, (50 - ImGui::CalcTextSize("GEAR5.AC").y) / 2 + 1), ImGui::GetColorU32(c::text::text_active), "GEAR5.AC");

                ImGui::GetBackgroundDrawList()->AddLine(pos + ImVec2(200, 0), pos + ImVec2(200, region.y), ImGui::GetColorU32(c::bg::outline), 1.f);
                ImGui::GetBackgroundDrawList()->AddLine(pos + ImVec2(0, 50), pos + ImVec2(region.x, 50), ImGui::GetColorU32(c::bg::outline), 1.f);

                ImGui::SetCursorPos(ImVec2(0, 50 + spacing.y));
                ImGui::BeginGroup();//define tabs
                {
                    //dqsg
                    if (ImGui::Tabs(0 == tabs, "a", "Aimbot", "Help with Aiming.", ImVec2(200, 50))) tabs = 0;
            
                    if (ImGui::Tabs(1 == tabs, "b", "Visuals", "U can seen more.", ImVec2(200, 50))) tabs = 1;

                    if (ImGui::Tabs(2 == tabs, "d", "World", "World modification.", ImVec2(200, 50))) tabs = 2;
            
                    if (ImGui::Tabs(3 == tabs, "c", "Misc", "Other options.", ImVec2(200, 50))) tabs = 3;

                    if (ImGui::Tabs(4 == tabs, "e", "Settings", "Setup your cheat.", ImVec2(200, 50))) tabs = 4;
                }
                ImGui::EndGroup();

                ImGui::SetCursorPos(ImVec2(200 + spacing.x, 0));
                if (tabs == 0) { //aimbot tab
                    static int tabsHor = 0;
                    ImGui::BeginGroup();
                    {
                        //settings
                        if (ImGui::TabsHor(0 == tabsHor, "c", "general", "Just a description of this tab", ImVec2(100, 50))) tabsHor = 0;
                        ImGui::SameLine();
                        //finished
                        if (ImGui::TabsHor(1 == tabsHor, "b", "accuracy", "Just a description of this tab", ImVec2(100, 50))) tabsHor = 1;
                        ImGui::SameLine();
                        //finished
                        if (ImGui::TabsHor(2 == tabsHor, "c", "triggerbot", "Just a description of this tab", ImVec2(100, 50))) tabsHor = 2;
                        ImGui::SameLine();
                        //finished
                        if (ImGui::TabsHor(3 == tabsHor, "b", "hitscan", "Just a description of this tab", ImVec2(100, 50))) tabsHor = 3;
                        ImGui::SameLine();
                        //finished
                    }
                    ImGui::EndGroup();

                    ImGui::SetCursorPos(ImVec2(200, 50) + spacing);
                    ImGui::BeginChild("Child", ImVec2(region.x - 200, region.y - 50) - spacing);
                    switch (tabsHor)
                    {
                    case 0:

                        
                        {
                            ImGui::BeginGroup();
                            {
                                ImGui::CustomBeginChild("dah child", ImVec2((region.x - (200 + spacing.x * 3)) / 2, 0));
                                {

                                    ImGui::Checkbox("Checkbox", &checkbox);

                                    ImGui::Checkbox("Checkbox ", &checkbox_false);

                                    ImGui::SliderInt("Drag slider", &slider_int, 1, 100, "%d%%");

                                    ImGui::Combo("Combobox", &select_combo, items, IM_ARRAYSIZE(items), ARRAYSIZE(items));

                                    ImGui::InputTextEx("Textfield", "blah blah blah", field, 46, ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30), NULL);

                                }
                                ImGui::CustomEndChild();

                                ImGui::CustomBeginChild("sibling smh", ImVec2((region.x - (200 + spacing.x * 3)) / 2, 0));
                                {

                                    ImGui::MultiCombo("Multicombo", multi_num, multi_items, ARRAYSIZE(multi_items));


                                }
                                ImGui::CustomEndChild();
                            }
                            ImGui::EndGroup();

                            ImGui::SameLine();

                            ImGui::BeginGroup();
                            {
                                ImGui::CustomBeginChild("dah children", ImVec2(region.x - (200 + spacing.x * 3), 0) / 2);
                                {

                                    ImGui::ColorEdit4("Colorpicker", color, picker_flags);

                                    ImGui::Keybind("Keybind", &key, &m);

                                }
                                ImGui::CustomEndChild();

                                ImGui::CustomBeginChild("aimbooty", ImVec2(region.x - (200 + spacing.x * 3), 0) / 2);
                                {

                                    if (ImGui::Button("Button", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30))) {
                                        ImGui::InsertNotification({ ImGuiToastType_Error, 1500, field });
                                        text_add++;
                                    }
                                    ImGui::SliderFloat("Drag slider", &slider_float, 0.0, 1.0f, "%0.1fF");


                                }
                                ImGui::CustomEndChild();

                                ImGui::CustomBeginChild("Test child", ImVec2((region.x - (200 + spacing.x * 3)) / 2, 0));
                                {
                                    static int int_knob = 0;
                                    static float float_knob = 0;
                                    static float knob_test = 0;

                                    ImGui::KnobInt("Drag int", &int_knob, 0, 200, "%d%%", NULL);

                                    ImGui::KnobFloat("Drag Float", &float_knob, 0.f, 25.f, "%0.3fF", NULL);

                                    ImGui::KnobFloat("Drag Tick", &knob_test, 0, 4500.f, "%0.1fF", NULL);


                                }
                                ImGui::CustomEndChild();
                            }
                            ImGui::EndGroup();

                            ImGui::Spacing();

                        }
                        
                        break;

                    case 1:
                    {
                        ImGui::BeginGroup();
                        {
                            ImGui::CustomBeginChild("another one", ImVec2((region.x - (200 + spacing.x * 3)) / 2, 0));
                            {
                                static bool checkbook = 0;

                                ImGui::Checkbox("Checkbox", &checkbook);

                                ImGui::Combo("Combobox", &select_combo, items, IM_ARRAYSIZE(items), ARRAYSIZE(items));

                            }
                            ImGui::CustomEndChild();
                        }
                        ImGui::EndGroup();

                        ImGui::SameLine();

                        ImGui::BeginGroup();
                        {
                            ImGui::CustomBeginChild("just a child haha", ImVec2(region.x - (200 + spacing.x * 3), 0) / 2);
                            {


                                ImGui::Keybind("Keybind", &key, &m);

                            }
                            ImGui::CustomEndChild();
                        }
                        ImGui::EndGroup();
                        break;
                    }
                    case 2:
                        break;
                    case 3:
                        break;

                    default:
                        break;

                    }
                    ImGui::EndChild();
                }

                

            }
            ImGui::End();

            ImGui::Begin("Popupbox", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
            {
                ImGui::CustomBeginChild("antiaim!!?!", ImVec2(300, 0), ImGuiWindowFlags_NoBringToFrontOnFocus);
                {
                    static int value = 10;
                    static float vertical = 50.f;
                    static bool checkbox = false;

                    ImGui::Checkbox("enable", &checkbox);

                    if (checkbox) {
                        ImGui::KnobInt("pitch", &value, 1, 100, "%d%%", NULL);

                        ImGui::KnobFloat("yaw", &vertical, 0.f, 100.f, "%.2fF", NULL);
                    }
                }
                ImGui::CustomEndChild();

            }
            ImGui::End();

            ImGui::RenderNotifications();

        }
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);

    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam);
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
