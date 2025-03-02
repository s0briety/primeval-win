
#include <sstream>
#include "../utils.h"
#include "../../sdk/interfaces.h"
#include "../../features/features.h"


#pragma warning(disable:4996)

float adjustment = 40.f;

namespace fonts {
	bool initialized = false;
	ImFont* Verdana_15 = nullptr;
	ImFont* TitleFont = nullptr;
	ImFont* smallest_pixel_14 = nullptr;
}

namespace textures {
	bool initialized = false;
	ImTextureID logo = NULL;
	ImTextureID ragetab = NULL;
	ImTextureID aaTab = NULL;
	ImTextureID visualsTab = NULL;
	ImTextureID miscTab = NULL;
	ImTextureID configTab = NULL;
}

namespace render {
	ImTextureID LoadTextureFromMemory(IDirect3DDevice9* device, unsigned char* imageData, int imageSize) {
		IDirect3DTexture9* texture = nullptr;
		HRESULT result = D3DXCreateTextureFromFileInMemoryEx(
			device, imageData, imageSize,
			D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			D3DX_FILTER_LINEAR,
			D3DX_FILTER_LINEAR,
			0, nullptr, nullptr, &texture
		);

		if (FAILED(result)) {
			char text[100];
			sprintf(text, "Failed to load texture from memory! HRESULT: 0x%08X\n", result);
			logging->ConsoleLog(text);
			return NULL;
		}

		return (ImTextureID)texture;
	}

	void init() {
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(input::m_hwnd);
		ImGui_ImplDX9_Init(interfaces::m_d3d_device);

		auto& io = ImGui::GetIO();
		auto& style = ImGui::GetStyle();

		ImFontConfig config;

		if (!fonts::initialized)
		{
			fonts::Verdana_15 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 15.0f);
			fonts::smallest_pixel_14 = io.Fonts->AddFontFromMemoryTTF((void*)fontdata::smallest_pixel, sizeof(fontdata::smallest_pixel), 14.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
			fonts::TitleFont = io.Fonts->AddFontFromMemoryTTF(fontdata::MontserratRegular, sizeof(fontdata::MontserratRegular), 30, NULL, io.Fonts->GetGlyphRangesCyrillic());
			fonts::initialized = true;
		}

		io.Fonts->Build();

		if (!textures::initialized)
		{
			textures::logo = LoadTextureFromMemory(interfaces::m_d3d_device, icondata::logotype, icondata::logotype_size);

			textures::ragetab = LoadTextureFromMemory(interfaces::m_d3d_device, icondata::aimbotIcon, icondata::aimbotIcon_size);

			textures::aaTab = LoadTextureFromMemory(interfaces::m_d3d_device, icondata::shieldIcon, icondata::shieldIcon_size);

			textures::visualsTab = LoadTextureFromMemory(interfaces::m_d3d_device, icondata::visualsIcon, icondata::visualsIcon_size);

			textures::miscTab = LoadTextureFromMemory(interfaces::m_d3d_device, icondata::miscIcon, icondata::miscIcon_size);

			textures::configTab = LoadTextureFromMemory(interfaces::m_d3d_device, icondata::configIcon, icondata::configIcon_size);

			textures::initialized = true;
		}
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

	bool DrawButton(const char* label, bool* value, const ImVec2& pos, ImVec2 size_arg, ImGuiButtonFlags flags)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

		ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);
		const ImRect bb(pos, pos + size);

		if (!ImGui::ItemAdd(bb, id))
			return false;
		ImGui::ItemSize(size, style.FramePadding.y);

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

		if (pressed)
			*value = !(*value);

		bool selected = *value;

		static std::map<ImGuiID, float> anim_progress;
		float& anim = anim_progress[id];

		float speed = 6.0f;
		float target = selected ? 1.0f : 0.0f;
		float delta = g.IO.DeltaTime * speed;

		anim += (target - anim) * (1.0f - exp(-delta));

		float ease_anim = anim * anim * (3 - 2 * anim);

		float fill_width = size.x * ease_anim;
		float fill_height = size.y * ease_anim;

		float rounded_corners = ImClamp(size.y * 0.3f * ease_anim, 0.0f, size.y * 0.3f);

		ImVec2 fill_min = ImVec2(bb.Min.x + (size.x - fill_width) * 0.5f, bb.Min.y + (size.y - fill_height) * 0.5f);
		ImVec2 fill_max = ImVec2(bb.Min.x + (size.x + fill_width) * 0.5f, bb.Min.y + (size.y + fill_height) * 0.5f);

		ImU32 bg_color = ImGui::GetColorU32(selected ? globals::config::secondaryColor : globals::config::textColor);

		window->DrawList->AddRectFilled(fill_min, fill_max, bg_color, rounded_corners);

		window->DrawList->AddRect(bb.Min, bb.Max, IM_COL32(255, 255, 255, 255), 0.0f, 0, 1.0f);

		ImVec2 text_pos = ImVec2(bb.Max.x + 8, bb.Min.y + (size.y - label_size.y) * 0.5f);
		window->DrawList->AddText(text_pos, ImGui::GetColorU32(globals::config::textColor), label);

		ImVec2 underline_start = text_pos + ImVec2(0, label_size.y + 2);
		ImVec2 underline_end = underline_start + ImVec2(label_size.x, 0);
		window->DrawList->AddLine(underline_start, underline_end, IM_COL32(255, 255, 255, 255), 1.5f);

		IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
		return pressed;
	}

	bool ColorSlider(const char* label, ImColor* color, float min, float max, ImVec2 pos)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

		ImVec2 Original = ImGui::GetCursorScreenPos();

		float slider_width = 200.0f;
		ImVec2 slider_size = ImVec2(slider_width, 6.0f);

		if (pos == ImVec2(0, 0))
			pos = window->DC.CursorPos;
		ImRect bb(pos, pos + ImVec2(slider_width, label_size.y + slider_size.y + style.ItemInnerSpacing.y));

		ImGui::ItemSize(bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		bool hovered = ImGui::ItemHoverable(bb, id, ImGuiItemFlags_None);
		bool held = (g.ActiveId == id) || (hovered && g.IO.MouseDown[0]);
		bool changed = false;

		if (hovered && g.IO.MouseClicked[0])
			g.ActiveId = id;

		if (!g.IO.MouseDown[0] && g.ActiveId == id)
			g.ActiveId = 0;

		float alpha = color->Value.w * 255.0f;
		float normalized = (alpha - min) / (max - min);

		static std::map<ImGuiID, float> anim_progress;
		float& anim = anim_progress[id];

		float speed = 8.0f;
		anim += (normalized - anim) * g.IO.DeltaTime * speed;
		float new_alpha = alpha;

		if (held)
		{
			float mouse_pos = (g.IO.MousePos.x - bb.Min.x) / slider_width;
			new_alpha = ImClamp(roundf(min + (mouse_pos * (max - min))), min, max);
			color->Value.w = new_alpha / 255.0f;

			changed = true;
		}

		ImU32 left_color = ImGui::GetColorU32(ImVec4(color->Value.x, color->Value.y, color->Value.z, 0.0f));
		ImU32 right_color = ImGui::GetColorU32(ImVec4(color->Value.x, color->Value.y, color->Value.z, 1.0f));
		window->DrawList->AddRectFilledMultiColor(bb.Min, ImVec2(bb.Max.x, bb.Min.y + slider_size.y), left_color, right_color, right_color, left_color);

		// **Draw Handle**
		float handle_x = bb.Min.x + slider_width * anim;
		ImVec2 handle_pos = ImVec2(handle_x - 4.0f, bb.Min.y);
		window->DrawList->AddCircleFilled(handle_pos + ImVec2(4, 3), 6.0f, ImGui::GetColorU32(globals::config::textColor));

		ImGui::SetCursorScreenPos(Original);
		return changed;
	}


	bool PopupColorPicker(const char* label, ImColor* color, ImVec2 pos)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImVec2 Original = ImGui::GetCursorScreenPos();
		ImGui::SetCursorScreenPos(pos);

		ImGuiID id = ImGui::GetID(label);
		bool value_changed = false;

		ImVec2 button_size(15, 15);
		ImVec4 col_vec4 = color->Value;

		if (ImGui::ColorButton(label, col_vec4, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, button_size))
			ImGui::OpenPopup(label);

		if (ImGui::BeginPopup(label))
		{
			if (ImGui::ColorPicker4(label, (float*)&col_vec4, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs))
			{
				*color = ImColor(col_vec4);
				value_changed = true;
			}

			// **Properly Align Alpha Slider Below Picker**
			ImVec2 ScreenPosition = ImGui::GetCursorScreenPos();
			ImGui::SetCursorScreenPos(ImVec2(ScreenPosition.x, ScreenPosition.y + 10));

			// **Alpha Slider (Now Fully Functional)**
			if (ColorSlider("##alpha", color, 0, 255, ImGui::GetCursorScreenPos()))
			{
				value_changed = true;
			}

			bool inside_picker = ImGui::IsItemActive() || ImGui::IsItemHovered() || ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

			if (!inside_picker && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && ImGui::IsMouseClicked(0))
				ImGui::CloseCurrentPopup();

			ImGui::EndPopup();
		}

		ImGui::SetCursorScreenPos(Original);
		return value_changed;
	}

	bool PopupColorPicker(const char* label, ImVec4* color, ImVec2 pos)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImVec2 Original = ImGui::GetCursorScreenPos();
		ImGui::SetCursorScreenPos(pos);

		ImGuiID id = ImGui::GetID(label);
		bool value_changed = false;

		ImVec2 button_size(20, 20);

		// Convert ImVec4 to ImColor (For rendering & internal use)
		ImColor color_im(*color);

		// **Color Preview Button**
		if (ImGui::ColorButton(label, *color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, button_size))
			ImGui::OpenPopup(label);

		std::string ID = "##picker" + *label;

		if (ImGui::BeginPopup(label))
		{
			// **Color Picker**
			if (ImGui::ColorPicker4(ID.c_str(), (float*)color, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs))
			{
				value_changed = true;
			}

			// **Custom Alpha Slider**
			ImVec2 sliderPos = ImGui::GetCursorScreenPos();
			sliderPos.y += 10; // Adjust position below picker
			ImGui::SetCursorScreenPos(sliderPos);

			if (ColorSlider("##alpha", &color_im, 0.0f, 255.0f, ImGui::GetCursorScreenPos()))
			{
				*color = color_im.Value; // Convert back to ImVec4
				value_changed = true;
			}

			// **Prevent Popup from Closing when Interacting**
			bool inside_picker = ImGui::IsItemActive() || ImGui::IsItemHovered() || ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

			if (!inside_picker && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && ImGui::IsMouseClicked(0))
				ImGui::CloseCurrentPopup();

			ImGui::EndPopup();
		}

		ImGui::SetCursorScreenPos(Original);
		return value_changed;
	}

	bool DrawCombo(const char* label, const char* items[], ImVec2 pos, float width, int selectedIndexes[], int item_count, int max_selected)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiID id = ImGui::GetID(label);
		bool value_changed = false;

		// **Build preview text of selected items**
		char preview_text[128] = "";
		int selected_count = 0;
		std::string inputLabel = std::string("##combo") + label;

		for (int i = 0; i < item_count; i++)
		{
			if (selectedIndexes[i] == 1)
			{
				if (selected_count > 0)
					strcat(preview_text, ", ");
				strcat(preview_text, items[i]);
				selected_count++;
			}
		}

		// **Show selected items or "Select..." if none are picked**
		if (selected_count == 0)
			strcpy(preview_text, "Select...");

		// **Calculate label position at the top-right of the combo box**
		ImVec2 textSize = ImGui::CalcTextSize(label);
		ImVec2 textPos = ImVec2(pos.x + width - textSize.x, pos.y - textSize.y - 3);

		// **Render label above the combo box (Top-Right Aligned)**
		ImGui::SetCursorScreenPos(textPos);
		ImGui::Text(label);

		// **Render Combo Box Below the Label**
		ImGui::SetCursorScreenPos(pos);
		ImGui::SetNextItemWidth(width);

		// **Apply custom styling for the combo**
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetColorU32(globals::config::colorScene));
		ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetColorU32(globals::config::secondaryColor));
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(globals::config::textColor));

		// **Main Combo Box**
		if (ImGui::BeginCombo(inputLabel.c_str(), preview_text, ImGuiComboFlags_NoArrowButton))
		{
			// **Style the popup background**
			ImGui::PushStyleColor(ImGuiCol_PopupBg, ImGui::GetColorU32(globals::config::colorScene));
			ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetColorU32(globals::config::secondaryColor));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImGui::GetColorU32(globals::config::secondaryColor));
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImGui::GetColorU32(globals::config::secondaryColor));

			for (int i = 0; i < item_count; i++)
			{
				bool is_selected = (selectedIndexes[i] == 1);
				bool is_limit_reached = (max_selected > 0 && selected_count >= max_selected && !is_selected);

				// **Disable selections if max limit reached**
				if (is_limit_reached)
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);

				if (ImGui::Selectable(items[i], is_selected, ImGuiSelectableFlags_DontClosePopups))
				{
					if (max_selected == 1)
					{
						// **Single Selection Mode**
						memset(selectedIndexes, 0, item_count * sizeof(int)); // Clear previous selection
						selectedIndexes[i] = 1;
					}
					else
					{
						// **Multi-Selection Mode**
						selectedIndexes[i] = !is_selected;
					}
					value_changed = true;
				}

				// **Un-disable if limit was reached**
				if (is_limit_reached)
					ImGui::PopItemFlag();

				if (is_selected)
					ImGui::SetItemDefaultFocus(); // Highlight selected item
			}

			ImGui::PopStyleColor(4);
			ImGui::EndCombo();
		}

		// **Pop styles back to default**
		ImGui::PopStyleVar(3);
		ImGui::PopStyleColor(3);

		return value_changed;
	}

	bool DrawCombo(const char* label, const char* items[], ImVec2 pos, float width, Hitbox hitboxTable[], int item_count, int max_selected)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiID id = ImGui::GetID(label);
		bool value_changed = false;

		// **Map hitbox selection state to an array of selected indexes**
		int selectedIndexes[8] = { 0 };
		for (int i = 0; i < item_count; i++)
		{
			selectedIndexes[i] = hitboxTable[i].isEnabled ? 1 : 0;
		}

		// **Build preview text for selected items**
		char preview_text[128] = "";
		int selected_count = 0;
		std::string inputLabel = std::string("##combo") + label;

		for (int i = 0; i < item_count; i++)
		{
			if (selectedIndexes[i] == 1)
			{
				if (selected_count > 0)
					strcat(preview_text, ", ");
				strcat(preview_text, items[i]);
				selected_count++;
			}
		}

		if (selected_count == 0)
			strcpy(preview_text, "Select Hitboxes");

		// **Label position at the top-right**
		ImVec2 textSize = ImGui::CalcTextSize(label);
		ImVec2 textPos = ImVec2(pos.x + width - textSize.x, pos.y - textSize.y - 3);
		ImGui::SetCursorScreenPos(textPos);
		ImGui::Text(label);

		// **Combo Box Styling**
		ImGui::SetCursorScreenPos(pos);
		ImGui::SetNextItemWidth(width);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetColorU32(globals::config::colorScene));
		ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetColorU32(globals::config::secondaryColor));
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(globals::config::textColor));

		// **Main Combo Box**
		if (ImGui::BeginCombo(inputLabel.c_str(), preview_text, ImGuiComboFlags_NoArrowButton))
		{
			ImGui::PushStyleColor(ImGuiCol_PopupBg, ImGui::GetColorU32(globals::config::colorScene));
			ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetColorU32(globals::config::secondaryColor));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImGui::GetColorU32(globals::config::secondaryColor));
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImGui::GetColorU32(globals::config::secondaryColor));

			for (int i = 0; i < item_count; i++)
			{
				bool is_selected = hitboxTable[i].isEnabled;
				bool is_limit_reached = (max_selected > 0 && selected_count >= max_selected && !is_selected);

				if (is_limit_reached)
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);

				if (ImGui::Selectable(items[i], is_selected, ImGuiSelectableFlags_DontClosePopups))
				{
					if (max_selected == 1)
					{
						// **Single Selection Mode**
						for (int j = 0; j < item_count; j++)
							hitboxTable[j].isEnabled = false;
						hitboxTable[i].isEnabled = true;
					}
					else
					{
						// **Multi-Selection Mode**
						hitboxTable[i].isEnabled = !hitboxTable[i].isEnabled;
					}
					value_changed = true;
				}

				if (is_limit_reached)
					ImGui::PopItemFlag();
			}

			ImGui::PopStyleColor(4);
			ImGui::EndCombo();
		}

		// **Pop styles back to default**
		ImGui::PopStyleVar(3);
		ImGui::PopStyleColor(3);

		return value_changed;
	}

	bool DrawCombo(const char* label, const char* items[], ImVec2 pos, float width, Combostruct Table[], int item_count, int max_selected)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiID id = ImGui::GetID(label);
		bool value_changed = false;

		// **Map hitbox selection state to an array of selected indexes**
		int selectedIndexes[8] = { 0 };
		for (int i = 0; i < item_count; i++)
		{
			selectedIndexes[i] = Table[i].isEnabled ? 1 : 0;
		}

		// **Build preview text for selected items**
		char preview_text[128] = "";
		int selected_count = 0;
		std::string inputLabel = std::string("##combo") + label;

		for (int i = 0; i < item_count; i++)
		{
			if (selectedIndexes[i] == 1)
			{
				if (selected_count > 0)
					strcat(preview_text, ", ");
				strcat(preview_text, items[i]);
				selected_count++;
			}
		}

		if (selected_count == 0)
			strcpy(preview_text, "Select");

		// **Label position at the top-right**
		ImVec2 textSize = ImGui::CalcTextSize(label);
		ImVec2 textPos = ImVec2(pos.x + width - textSize.x, pos.y - textSize.y - 3);
		ImGui::SetCursorScreenPos(textPos);
		ImGui::Text(label);

		// **Combo Box Styling**
		ImGui::SetCursorScreenPos(pos);
		ImGui::SetNextItemWidth(width);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetColorU32(globals::config::colorScene));
		ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetColorU32(globals::config::secondaryColor));
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(globals::config::textColor));

		// **Main Combo Box**
		if (ImGui::BeginCombo(inputLabel.c_str(), preview_text, ImGuiComboFlags_NoArrowButton))
		{
			ImGui::PushStyleColor(ImGuiCol_PopupBg, ImGui::GetColorU32(globals::config::colorScene));
			ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetColorU32(globals::config::secondaryColor));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImGui::GetColorU32(globals::config::secondaryColor));
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImGui::GetColorU32(globals::config::secondaryColor));

			for (int i = 0; i <= item_count; i++)
			{
				bool is_selected = Table[i].isEnabled;
				bool is_limit_reached = (max_selected > 0 && selected_count >= max_selected && !is_selected);


				if (ImGui::Selectable(items[i], is_selected, ImGuiSelectableFlags_DontClosePopups))
				{
					if (max_selected == 1)
					{
						// **Single Selection Mode**
						for (int j = 0; j <= item_count; j++)
							Table[j].isEnabled = false;
						Table[i].isEnabled = true;
					}
					else
					{
						Table[i].isEnabled = !Table[i].isEnabled;
					}
					value_changed = true;
				}
			}

			ImGui::PopStyleColor(4);
			ImGui::EndCombo();
		}

		// **Pop styles back to default**
		ImGui::PopStyleVar(3);
		ImGui::PopStyleColor(3);

		return value_changed;
	}

	bool DrawSlider(const char* label, int* value, int min, int max, ImVec2 pos, float slider_width)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

		ImVec2 Original = ImGui::GetCursorScreenPos();

		ImVec2 slider_size = ImVec2(slider_width, 6.0f);

		if (pos == ImVec2(0, 0))
			pos = window->DC.CursorPos;
		ImRect bb(pos, pos + ImVec2(slider_width, label_size.y + slider_size.y + style.ItemInnerSpacing.y));

		ImGui::ItemSize(bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		bool hovered = ImGui::ItemHoverable(bb, id, ImGuiItemFlags_None);
		bool held = (g.ActiveId == id) || (hovered && g.IO.MouseDown[0]);
		bool changed = false;

		if (hovered && g.IO.MouseClicked[0])
			g.ActiveId = id;

		if (!g.IO.MouseDown[0] && g.ActiveId == id)
			g.ActiveId = 0;

		float normalized = float(*value - min) / float(max - min);

		static std::map<ImGuiID, float> anim_progress;
		float& anim = anim_progress[id];

		float speed = 8.0f;
		anim += (normalized - anim) * g.IO.DeltaTime * speed;

		int new_value = *value;

		if (held)
		{
			int mouse_pos = static_cast<int>((g.IO.MousePos.x - bb.Min.x) / slider_width);
			new_value = ImClamp(min + (mouse_pos * (max - min)), min, max);

			*value = new_value;

			changed = true;
		}

		ImU32 track_color = ImGui::GetColorU32(globals::config::colorScene);
		ImU32 fill_color = ImGui::GetColorU32(globals::config::secondaryColor);
		ImU32 handle_color = ImGui::GetColorU32(globals::config::textColor);

		window->DrawList->AddRectFilled(bb.Min, ImVec2(bb.Max.x, bb.Min.y + slider_size.y), track_color, 3.0f);

		window->DrawList->AddRectFilled(bb.Min, ImVec2(bb.Min.x + slider_width * anim, bb.Min.y + slider_size.y), fill_color, 3.0f);

		float handle_x = bb.Min.x + slider_width * anim;
		ImVec2 handle_pos = ImVec2(handle_x - 4.0f, bb.Min.y);
		window->DrawList->AddCircleFilled(handle_pos + ImVec2(4, 3), 6.0f, handle_color);

		// Render text label on the top left
		ImVec2 text_pos = ImVec2(bb.Min.x, bb.Min.y - label_size.y - 5);
		window->DrawList->AddText(text_pos, handle_color, label);

		std::string inputLabel = std::string("input") + label;
		ImGuiID input_id = ImGui::GetID(inputLabel.c_str());

		float& anim2 = anim_progress[input_id];

		speed = 6.0f;
		bool is_active = (g.ActiveId == input_id || held);
		float target = is_active ? 1.0f : 0.0f;
		anim2 += (target - anim2) * g.IO.DeltaTime * speed;

		ImVec4 textColor = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(globals::config::textColor));
		ImVec4 secondaryColor = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(globals::config::secondaryColor));

		ImVec4 fadedColor = ImVec4(
			textColor.x + (secondaryColor.x - textColor.x) * anim2,
			textColor.y + (secondaryColor.y - textColor.y) * anim2,
			textColor.z + (secondaryColor.z - textColor.z) * anim2,
			textColor.w + (secondaryColor.w - textColor.w) * anim2
		);

		ImU32 DrawTextColor = ImGui::ColorConvertFloat4ToU32(fadedColor);

		ImGui::SetCursorScreenPos(ImVec2(bb.Max.x + 25.f, bb.Min.y - 5.f));
		ImVec2 input_pos = ImGui::GetCursorScreenPos();
		ImVec2 input_size = ImVec2(30.f, ImGui::GetTextLineHeight() + 4.0f);
		ImVec2 input_center = input_pos + ImVec2(input_size.x * 0.5f, input_size.y * 0.5f);

		window->DrawList->AddRectFilled(input_pos, input_pos + input_size, ImGui::GetColorU32(globals::config::colorScene), 0.0f);

		char buffer[16];
		snprintf(buffer, sizeof(buffer), "%d", new_value);
		ImVec2 text_size = ImGui::CalcTextSize(buffer);
		text_pos = input_center - text_size * 0.5f;

		window->DrawList->AddText(text_pos, DrawTextColor, buffer);

		ImGui::SetNextItemWidth(input_size.x);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0, 0, 0, 0));

		if (ImGui::InputInt(inputLabel.c_str(), &new_value))
		{
			new_value = ImClamp(new_value, min, max);
			*value = new_value;
			changed = true;
		}

		ImGui::PopStyleColor(5);
		ImGui::SetCursorScreenPos(Original);

		return changed;
	}

	bool DrawSlider(const char* label, float* value, float min, float max, ImVec2 pos, float slider_width)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

		ImVec2 Original = ImGui::GetCursorScreenPos();

		// Slider size
		ImVec2 slider_size = ImVec2(slider_width, 6.0f);

		if (pos == ImVec2(0, 0))
			pos = window->DC.CursorPos;
		ImRect bb(pos, pos + ImVec2(slider_width, label_size.y + slider_size.y + style.ItemInnerSpacing.y));

		ImGui::ItemSize(bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		bool hovered = ImGui::ItemHoverable(bb, id, ImGuiItemFlags_None);
		bool held = (g.ActiveId == id) || (hovered && g.IO.MouseDown[0]);
		bool changed = false;

		if (hovered && g.IO.MouseClicked[0])
			g.ActiveId = id;

		if (!g.IO.MouseDown[0] && g.ActiveId == id)
			g.ActiveId = 0;

		float normalized = (*value - min) / (max - min);

		static std::map<ImGuiID, float> anim_progress;
		float& anim = anim_progress[id];

		float speed = 8.0f;
		anim += (normalized - anim) * g.IO.DeltaTime * speed;
		float new_value = *value;

		if (held)
		{
			float mouse_pos = (g.IO.MousePos.x - bb.Min.x) / slider_width;
			new_value = ImClamp(roundf(min + (mouse_pos * (max - min))), min, max);

			*value = new_value;

			changed = true;
		}

		ImU32 track_color = ImGui::GetColorU32(globals::config::colorScene);
		ImU32 fill_color = ImGui::GetColorU32(globals::config::secondaryColor);
		ImU32 handle_color = ImGui::GetColorU32(globals::config::textColor);

		window->DrawList->AddRectFilled(bb.Min, ImVec2(bb.Max.x, bb.Min.y + slider_size.y), track_color, 3.0f);

		window->DrawList->AddRectFilled(bb.Min, ImVec2(bb.Min.x + slider_width * anim, bb.Min.y + slider_size.y), fill_color, 3.0f);

		float handle_x = bb.Min.x + slider_width * anim;
		ImVec2 handle_pos = ImVec2(handle_x - 4.0f, bb.Min.y);
		window->DrawList->AddCircleFilled(handle_pos + ImVec2(4, 3), 6.0f, handle_color);

		ImVec2 text_pos = ImVec2(bb.Min.x, bb.Min.y - label_size.y - 5);
		window->DrawList->AddText(text_pos, handle_color, label);

		std::string inputLabel = std::string("input") + label;
		ImGuiID input_id = ImGui::GetID(inputLabel.c_str());

		float& anim2 = anim_progress[input_id];

		speed = 6.0f;
		bool is_active = (g.ActiveId == input_id || held);
		float target = is_active ? 1.0f : 0.0f;
		anim2 += (target - anim2) * g.IO.DeltaTime * speed;

		ImVec4 textColor = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(globals::config::textColor));
		ImVec4 secondaryColor = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(globals::config::secondaryColor));

		ImVec4 fadedColor = ImVec4(
			textColor.x + (secondaryColor.x - textColor.x) * anim2,
			textColor.y + (secondaryColor.y - textColor.y) * anim2,
			textColor.z + (secondaryColor.z - textColor.z) * anim2,
			textColor.w + (secondaryColor.w - textColor.w) * anim2
		);

		ImU32 DrawTextColor = ImGui::ColorConvertFloat4ToU32(fadedColor);

		ImGui::SetCursorScreenPos(ImVec2(bb.Max.x + 25.f, bb.Min.y - 5.f));

		ImVec2 input_pos = ImGui::GetCursorScreenPos();
		ImVec2 input_size = ImVec2(30.f, ImGui::GetTextLineHeight() + 4.0f);
		ImVec2 input_center = input_pos + ImVec2(input_size.x * 0.5f, input_size.y * 0.5f);

		window->DrawList->AddRectFilled(input_pos, input_pos + input_size, ImGui::GetColorU32(globals::config::colorScene), 0.0f);

		char buffer[16];
		snprintf(buffer, sizeof(buffer), "%.0f", new_value);
		ImVec2 text_size = ImGui::CalcTextSize(buffer);

		text_pos = input_center - text_size * 0.5f;

		window->DrawList->AddText(text_pos, DrawTextColor, buffer);

		ImGui::SetNextItemWidth(input_size.x);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0, 0, 0, 0));

		if (ImGui::InputFloat(inputLabel.c_str(), &new_value, 0.0f, 0.0f, "%.0f"))
		{
			new_value = ImClamp(new_value, min, max);
			*value = new_value;
			changed = true;
		}

		ImGui::PopStyleColor(5);
		ImGui::SetCursorScreenPos(Original);

		return changed;
	}

	void DrawKeybind(const char* label, KeyBind& parent, ImVec2 pos, float v, float v_min, float v_max, int addition)
	{
		ImGui::PushID(label);
		ImGui::SetCursorScreenPos(pos);

		static bool waitingForKey = false;
		static std::unordered_map<const char*, int> placeholders;

		if (placeholders.find(label) == placeholders.end())
			placeholders[label] = static_cast<int>(v);

		char buffer[64];
		if (parent.Key && parent.Key != 0)
			snprintf(buffer, sizeof(buffer), "%s", input::GetKeyNameFromVK(parent.Key).c_str());
		else
			snprintf(buffer, sizeof(buffer), "?");

		ImVec2 buttonSize = ImVec2(40, 20);

		// **Draw Keybind Button First**
		if (ImGui::Button(buffer, buttonSize))
		{
			ImGui::OpenPopup(label);
		}

		// **Properly Align Text to the Right of the Button**
		ImVec2 buttonEnd = ImGui::GetCursorScreenPos();
		ImVec2 textSizeLabel = ImGui::CalcTextSize(label);
		ImVec2 textPos = ImVec2(buttonEnd.x + (50 + addition), pos.y + (buttonSize.y / 2) - (textSizeLabel.y / 2));

		ImGui::SetCursorScreenPos(textPos);
		ImGui::Text(label);

		// **Draw Outline**
		ImGui::GetWindowDrawList()->AddRect(pos, pos + buttonSize, ImGui::GetColorU32(globals::config::secondaryColor), 0.0f, 0, 1.0f);

		// **Keybind Popup**
		if (ImGui::BeginPopupModal(label, nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
		{
			ImGui::Spacing();
			if (waitingForKey)
			{
				ImGui::Text("Press a key to bind...");
				for (int vkKey = 0x01; vkKey <= 0xFE; vkKey++)
				{
					if (GetAsyncKeyState(vkKey) & 0x8000)
					{
						parent.Key = vkKey;
						waitingForKey = false;
					}
				}
			}
			else
			{
				ImGui::Text("Current key: %s", input::GetKeyNameFromVK(parent.Key).c_str());

				ImVec2 bindButtonSize = ImVec2(150, 30);
				if (ImGui::Button("Bind Key", bindButtonSize))
				{
					waitingForKey = true;
				}
			}

			// **Optional Value Input**
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

			// **Bind Mode Combo Box**
			const char* bindOptions[] = { "Always Off", "Hold", "Toggle", "Key Release", "Always On" };

			ImGui::BulletText("Bind Mode");
			ImGui::SetNextItemWidth(150);
			ImGui::Combo("##bindmode", &parent.Mode, bindOptions, IM_ARRAYSIZE(bindOptions));

			ImGui::Spacing();

			// **Done Button**
			ImVec2 doneButtonSize = ImVec2(150, 30);
			if (ImGui::Button("Done", doneButtonSize))
			{
				waitingForKey = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopID();
	}
}

namespace render_game {
	void DrawPlayerBox(int r, int g, int b, int a, int rr, int gg, int bb, int aa, int left, int right, float top, float bottom) {
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();

		ImU32 mainColor = IM_COL32(r, g, b, a);
		ImU32 outlineColor = IM_COL32(rr, gg, bb, aa);

		drawList->AddRect(ImVec2(left, top), ImVec2(right, bottom), mainColor, 0.0f, 0, 2.0f);

		drawList->AddRect(ImVec2(left - 1, top - 1), ImVec2(right + 1, bottom + 1), outlineColor, 0.0f, 0, 1.0f);
		drawList->AddRect(ImVec2(left + 1, top + 1), ImVec2(right - 1, bottom - 1), outlineColor, 0.0f, 0, 1.0f);
	}

	void DrawHealthBar(float healthFrac, float h, int r, int g, int b, int a, int left, int right, float top, float bottom) {
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();

		ImU32 black = IM_COL32(0, 0, 0, 255);
		ImU32 healthColor = IM_COL32(r, g, b, a);

		drawList->AddRect(ImVec2(left - 6, top - 1), ImVec2(left - 3, bottom + 1), black, 0.0f, 0, 1.0f);

		drawList->AddRectFilled(ImVec2(left - 5, bottom - (h * healthFrac)), ImVec2(left - 4, bottom), healthColor);
	}

	void DrawText2D(ImFont* font, float x, float y, int r, int g, int b, int a, const char* text) {
		if (!font || !text) return;

		ImDrawList* drawList = ImGui::GetBackgroundDrawList();

		ImU32 color = IM_COL32(r, g, b, a);

		drawList->PushTextureID(font->ContainerAtlas->TexID);
		drawList->AddText(font, font->FontSize, ImVec2(x, y), color, text);
		drawList->PopTextureID();
	}
}