
#include <sstream>
#include "../utils.h"
#include "../../sdk/interfaces.h"
#include "../../features/features.h"

ImVec2 LastSwitchPos;
float adjustment = 40.f;

namespace render {
	void init() {
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(input::m_hwnd);
		ImGui_ImplDX9_Init(interfaces::m_d3d_device);

		auto& io = ImGui::GetIO();
		auto& style = ImGui::GetStyle();

		ImFontConfig config;

		if (!fonts::Lucon_22 || !fonts::Verdana_15) {
			config.OversampleH = 2;
			config.OversampleV = 1;
			fonts::Verdana_15 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 15.0f);
			fonts::Lucon_22 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\lucon.ttf", 22.0f);
		}

		io.Fonts->Build();

	}

	void DrawBorder(ImVec4 color, float thickness, float adjustment, float rounding)
	{
		ImVec2 item_min = ImGui::GetItemRectMin();
		ImVec2 item_max = ImGui::GetItemRectMax();

		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		draw_list->AddRect(
			ImVec2(item_min.x - thickness + adjustment, item_min.y - thickness + adjustment),
			ImVec2(item_max.x + thickness - adjustment, item_max.y + thickness - adjustment),
			ImGui::GetColorU32(color),
			rounding,
			ImDrawFlags_RoundCornersAll,
			thickness
		);
	}

	bool DrawSwitch(const char* label, bool* v, bool bullet)
	{
		ImGui::PushID(label);
		ImGui::Dummy(ImVec2(0.f, 1.5f));
		ImVec2 pos = ImGui::GetCursorScreenPos();
		LastSwitchPos = pos;
		float height = ImGui::GetFrameHeight() / 2;
		float width = height * 1.f;

		ImGui::InvisibleButton(label, ImVec2(width, height));
		bool toggle = ImGui::IsItemClicked();
		if (toggle)
			*v = !(*v);

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImVec2 rect_end = ImVec2(pos.x + width, pos.y + height);

		ImU32 color = *v ? ImGui::GetColorU32(globals::config::secondaryColor) : ImGui::GetColorU32(globals::config::backgroundColor);

		draw_list->AddRectFilled(pos, rect_end, color, height * 0.01f);
		DrawBorder(globals::config::secondaryColor, 1.5f, 1.f, 0.0f);

		ImGui::SameLine();

		float text_width = ImGui::CalcTextSize(label).x;
		float available_width = ImGui::GetContentRegionAvail().x - 15.f;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (available_width - text_width));
		LastSwitchPos = ImGui::GetCursorScreenPos();
		ImGui::Text(label);

		ImGui::PopID();

		adjustment = 40.f;

		return toggle;
	}

	bool drawSlider(const char* label, float* v, float v_min, float v_max, bool bullet, int precision)
	{
		ImGui::PushID(label);

		ImGui::Text(label);

		ImVec2 pos = ImGui::GetCursorScreenPos();

		float width = ImGui::CalcItemWidth() + ImGui::CalcItemWidth() / 5;
		float height = ImGui::GetFrameHeight() / 5;

		/* if (bullet)
		{
			ImGui::Bullet();
			width = ImGui::CalcItemWidth() + ImGui::CalcItemWidth() / 8;
			pos.x += ImGui::GetStyle().ItemSpacing.x;
		} */

		ImU32 color_bg = ImGui::GetColorU32(globals::config::backgroundColor);
		ImVec4 color_start = globals::config::secondaryColor;
		color_start.x = ImClamp(color_start.x - 1.0f, 0.0f, 1.0f);
		color_start.y = ImClamp(color_start.y - 1.0f, 0.0f, 1.0f);
		color_start.z = ImClamp(color_start.z - 1.0f, 0.0f, 1.0f);
		ImVec4 color_end = globals::config::secondaryColor;

		ImVec4 interpolated_color = {
			color_start.x + (*v - v_min) / (v_max - v_min) * (color_end.x - color_start.x),
			color_start.y + (*v - v_min) / (v_max - v_min) * (color_end.y - color_start.y),
			color_start.z + (*v - v_min) / (v_max - v_min) * (color_end.z - color_start.z),
			color_start.w + (*v - v_min) / (v_max - v_min) * (color_end.w - color_start.w)
		};

		ImU32 final_fill_color = ImGui::GetColorU32(interpolated_color);

		ImGui::InvisibleButton(label, ImVec2(width, height));
		bool is_active = ImGui::IsItemActive();
		bool clicked = ImGui::IsItemClicked();

		if (is_active)
		{
			float mouse_pos = ImGui::GetMousePos().x - pos.x;
			*v = v_min + (v_max - v_min) * (mouse_pos / width);
			*v = ImClamp(*v, v_min, v_max);
		}

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->AddRectFilled(pos, ImVec2(pos.x + width, pos.y + height), color_bg, height * 0.3f);

		float fill_width = (*v + v_min) / (v_max + v_min) * width;
		draw_list->AddRectFilled(pos, ImVec2(pos.x + fill_width, pos.y + height), final_fill_color, height * 0.3f);

		float handle_x = pos.x + fill_width;
		draw_list->AddCircleFilled(ImVec2(handle_x - 2.0f, pos.y + height * 0.5f), height * 1.3f, ImGui::GetColorU32(globals::config::secondaryColor));

		int intValue;
		std::ostringstream strValueStream;
		std::string strValue;

		if (precision <= 0)
		{
			intValue = static_cast<int>(*v);
			strValue = std::to_string(intValue);
		}
		else
		{
			strValueStream.precision(2);
			strValueStream << std::fixed << *v;
			strValue = strValueStream.str();
		}

		float widthAddition = 17.0f;

		/* if (bullet)
			widthAddition = 15.0f;
		*/

		ImVec2 textPos = ImVec2(pos.x + width + widthAddition, pos.y - 4.99);
		draw_list->AddText(textPos, ImGui::GetColorU32(globals::config::secondaryColor), strValue.c_str());

		ImGui::PopID();

		return clicked;
	}

	void drawKeyBind(const char* label, KeyBind& parent, float v, float v_min, float v_max)
	{
		ImGui::PushID(label);
		ImGui::Text(label);
		ImGui::SameLine();

		static bool waitingForKey = false;
		static std::unordered_map<const char*, int> placeholders;

		if (placeholders.find(label) == placeholders.end())
			placeholders[label] = static_cast<int>(v);

		char buffer[64];

		if (parent.Key && parent.Key != 0)
			snprintf(buffer, sizeof(buffer), "%s", input::GetKeyNameFromVK(parent.Key).c_str());
		else
			snprintf(buffer, sizeof(buffer), "?");

		ImVec2 buttonSize = ImVec2(75, 25);
		ImGui::PushStyleColor(ImGuiCol_Button, globals::config::backgroundColor);
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));

		if (ImGui::Button(buffer, buttonSize))
		{
			ImGui::OpenPopup(label);
		}

		ImGui::PopStyleColor(2);

		if (ImGui::BeginPopupModal(label, nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
		{
			ImGui::PushStyleColor(ImGuiCol_WindowBg, globals::config::backgroundColor);
			ImGui::Spacing();

			if (waitingForKey)
			{
				ImGui::Text("Press a key to bind...");
				for (int vkKey = 0x01; vkKey <= 0xFE; vkKey++)  // Loop through possible VK keys
				{
					if (GetAsyncKeyState(vkKey) & 0x8000)  // Check if the key is pressed
					{
						parent.Key = vkKey;
						waitingForKey = false;
					}
				}
			}
			else
			{
				ImGui::Text("Current key: %s", input::GetKeyNameFromVK(parent.Key).c_str());

				if (ImGui::Button("Bind Key", ImVec2(150, 30)))
				{
					waitingForKey = true;
				}
			}

			if (v != 0.0f && v_min != 0.0f && v_max != 0.0f)
			{
				ImGui::SameLine();
				ImGui::SetNextItemWidth(100);

				if (ImGui::InputInt("##value", &placeholders[label], 1, 10))
				{
					placeholders[label] = ImClamp(placeholders[label], static_cast<int>(v_min), static_cast<int>(v_max));
					v = static_cast<float>(placeholders[label]);
				}
			}

			ImGui::Spacing();
			const char* bindOptions[] = { "Always Off", "Hold", "Toggle", "Key Release", "Always On" };
			ImGui::PushStyleColor(ImGuiCol_FrameBg, globals::config::colorScene);
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, globals::config::colorScene);
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(globals::config::backgroundColor.x + 0.05f, globals::config::backgroundColor.y + 0.05f, globals::config::backgroundColor.z + 0.05f, globals::config::backgroundColor.w));
			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(globals::config::colorScene.x - 0.05f, globals::config::colorScene.y - 0.05f, globals::config::colorScene.z - 0.05f, globals::config::colorScene.w));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(globals::config::backgroundColor.x + 0.05f, globals::config::backgroundColor.y + 0.05f, globals::config::backgroundColor.z + 0.05f, globals::config::backgroundColor.w));
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(globals::config::backgroundColor.x - 0.05f, globals::config::backgroundColor.y - 0.05f, globals::config::backgroundColor.z - 0.05f, globals::config::backgroundColor.w));
			ImGui::BulletText("Bind Mode");
			ImGui::Combo("##bindmode", &parent.Mode, bindOptions, IM_ARRAYSIZE(bindOptions));
			ImGui::PopStyleColor(6);
			ImGui::Spacing();

			if (ImGui::Button("Done", ImVec2(150, 30)))
			{
				waitingForKey = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::PopStyleColor();
			ImGui::EndPopup();
		}

		ImGui::PopID();
	}

	void DrawColorPicker(const char* label, ImColor* color)
	{
		ImVec2 pos = ImVec2(LastSwitchPos.x - adjustment, LastSwitchPos.y);

		ImGui::SetCursorScreenPos(pos);

		ImGui::SetNextItemWidth(30.f);
		ImGui::ColorEdit4(label, (float*)color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoLabel);

		adjustment = adjustment + 40.f;
	}
}

namespace fonts {
	ImFont* Verdana_15 = nullptr;
	ImFont* Lucon_22 = nullptr;
}
