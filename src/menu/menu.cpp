#include "menu.h"

inline ImGuiDir arrow = ImGuiDir_Down;

void RenderAimbotTab()
{
	ImGui::PushFont(fonts::Verdana_15);

	ImVec2 startPos = ImGui::GetCursorScreenPos();
	ImVec2 availableRegion = ImGui::GetContentRegionAvail();
	float groupWidth = availableRegion.x / 2;
	float groupHeight = (availableRegion.y / 2) - 4;

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddRectFilled(
		ImVec2(startPos.x - 2, startPos.y),
		ImVec2(startPos.x + availableRegion.x + 2, startPos.y + availableRegion.y),
		ImGui::GetColorU32(globals::config::colorScene),
		3.0f
	);

	ImVec2 pos = ImVec2(startPos.x, startPos.y);

	if (ImGui::BeginTable("Rage", 2, ImGuiTableFlags_SizingStretchProp))
	{
		ImGui::TableNextColumn();
		ImGui::SetCursorScreenPos(pos);
		ImGui::BeginChild("Features", ImVec2(groupWidth, groupHeight), false, ImGuiWindowFlags_NoScrollbar);
		ImGui::SeparatorText("Features");

		pos.y += 40;
		pos.x += 10;
		render::DrawButton("Enable Aimbot", &globals::config::aimbotEnabled, pos);

		if (globals::config::aimbotEnabled)
		{
			pos.y += 30;
			render::DrawButton("Silent Aim", &globals::config::silentAimEnabled, pos);
			pos.y += 30;
			render::DrawButton("Resolver", &globals::config::aimbotResolver, pos);
			pos.y += 30;
			render::DrawButton("Autofire", &globals::config::aimbotAutoShoot, pos);
			pos.y += 30;
			render::DrawButton("Autoscope", &globals::config::aimbotAutoscope, pos);
			pos.y += 30;
			render::DrawButton("Autostop", &globals::config::aimbotAutostop, pos);
			pos.y += 30;
			render::DrawButton("Quick Peek", &globals::config::aimbotQuickPeek, pos);
		}
		ImGui::EndChild();

		pos = startPos;
		pos.y += groupHeight;
		ImGui::SetCursorScreenPos(pos);
		ImGui::BeginChild("Exploits", ImVec2(groupWidth, groupHeight), false, ImGuiWindowFlags_NoScrollbar);
		ImGui::SeparatorText("Exploits");
		pos.x += 10;
		pos.y += 50;
		render::DrawKeybind("Doubletap", globals::config::DoubleTapKey, pos, 0.f, 0.f, 0.f, 10.f);
		pos.y += 30;
		render::DrawKeybind("Hideshots", globals::config::HideshotsKey, pos, 0.f, 0.f, 0.f, 10.f);
		pos.y += 30;
		render::DrawKeybind("Ideal Tick", globals::config::IdealTickKey, pos, 0.f, 0.f, 0.f, 10.f);

		ImGui::EndChild();

		ImGui::TableNextColumn();
		ImGui::BeginChild("Targeting", ImVec2(groupWidth, (groupHeight * 2)), false, ImGuiWindowFlags_NoScrollbar);
		ImGui::SeparatorText("Targeting");

		pos = ImGui::GetCursorScreenPos();
		pos.y += 15;
		pos.x += 10;

		static const char* targetOptions[] = { "Distance", "Health", "Hitchance" };
		render::DrawCombo("Target Selection", targetOptions, pos, 250.f, &globals::config::aimbotTargetSelection, 3, 1);
		static const char* HitboxOptions[] = { "Head", "Neck", "Chest", "Stomach", "Arms", "Legs", "Feet" };
		pos.y += 50;
		render::DrawCombo("Hitbox Selection", HitboxOptions, pos, 250.f, globals::config::hitboxTable, 7);
		pos.y += 50;
		render::DrawCombo("Multipoint Selection", HitboxOptions, pos, 250.f, globals::config::multipointTable, 7);
		static const char* AutostopOptions[] = { "Early", "Between Shots", "Aggressive", "Passive", "Smart", "Crouch", "In Air" };
		pos.y += 50;
		render::DrawCombo("Autostop Selection", AutostopOptions, pos, 250.f, globals::config::autostopTable, 6);
		if (globals::config::multipointTable[0].isEnabled)
		{
			pos.y += 75;
			ImGui::Text("Multipoint");
			pos.y += 15;
			render::DrawSlider("Head Scale", &globals::config::multipointHead, 0.0f, 100.0f, pos);
			pos.y += 30;
			render::DrawSlider("Body Scale", &globals::config::multipointBody, 0.0f, 100.0f, pos);
		}

		pos.y += 30;
		render::DrawButton("Prefer Safe Point", &globals::config::preferSafepoint, pos);
		pos.y += 50;
		render::DrawSlider("Min. Hitchance", &globals::config::aimbotHitchance, 1.0f, 100.0f, pos);
		pos.y += 30;
		render::DrawSlider("Min. Damage", &globals::config::aimbotMinDamage, 1.0f, 110.0f, pos);
		pos.y += 30;
		render::DrawKeybind("Damage Override", globals::config::MinDamageOverrideKey, pos, globals::config::MinDamageOverride, 1.0f, 100.0f, 10.f);
		pos.y += 30;
		render::DrawKeybind("Force Body Aim", globals::config::forceBaimKey, pos, 10.f);
		pos.y += 30;
		render::DrawKeybind("Hitchance Override", globals::config::HitchanceOverrideKey, pos, globals::config::HitchanceOverride, 1.0f, 100.0f, 10.f);

		ImGui::EndChild();

		ImGui::EndTable();
	}

	ImGui::PopFont();
}


void RenderAntiAimTab()
{
	ImGui::PushFont(fonts::Verdana_15);

	ImVec2 startPos = ImGui::GetCursorScreenPos();
	ImVec2 availableRegion = ImGui::GetContentRegionAvail();
	float groupWidth = availableRegion.x / 2;
	float groupHeight = availableRegion.y - 20;

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddRectFilled(
		ImVec2(startPos.x - 2, startPos.y),
		ImVec2(startPos.x + availableRegion.x + 2, startPos.y + availableRegion.y),
		ImGui::GetColorU32(globals::config::colorScene),
		3.0f
	);

	ImVec2 pos = ImVec2(startPos.x, startPos.y);
	if (ImGui::BeginTable("AntiAimTab", 2, ImGuiTableFlags_SizingStretchProp))
	{
		ImGui::TableNextColumn();

		ImGui::BeginChild("AntiAim", ImVec2(groupWidth, groupHeight), false, ImGuiWindowFlags_NoScrollbar);
		ImGui::SeparatorText("Anti-Aim");

		pos.x += 10;
		pos.y += 40;
		render::DrawButton("Enable Anti-Aim", &globals::config::antiAimEnabled, pos);

		if (globals::config::antiAimEnabled)
		{
			pos.y += 50;
			render::DrawSlider("Pitch", &globals::config::antiAimPitch, -89.f, 89.f, pos);
			pos.y += 30;

			static const char* PitchModeOptions[] = { "Off", "Static", "Down", "Up", "Random", "Alternate", "Fake Down", "Fake Up" };
			render::DrawCombo("Pitch Mode", PitchModeOptions, pos, 250.0f, &globals::config::antiAimPitchMode, 8, 1);
			pos.y += 50;

			render::DrawSlider("Yaw", &globals::config::antiAimYaw, -180.f, 180.f, pos);
			pos.y += 30;
			static const char* YawModeOptions[] = { "Off", "Static", "180", "Spin", "180Z", "Crosshair" };
			render::DrawCombo("Yaw Mode", YawModeOptions, pos, 250.0f, &globals::config::antiAimYawMode, 6, 1);
			pos.y += 50;

			render::DrawSlider("Yaw Modifier", &globals::config::antiAimYawModifier, -180.f, 180.f, pos);
			pos.y += 30;
			static const char* YawModifierOptions[] = { "Off", "Static", "Jitter Shots", "Spin", "Distort", "Dynamic" };
			render::DrawCombo("Yaw Modifier", YawModifierOptions, pos, 250.0f, &globals::config::antiAimYawModifierMode, 6, 1);
			pos.y += 50;

			static const char* YawBaseOptions[] = { "View Angle", "At Targets" };
			render::DrawCombo("Yaw Base", YawBaseOptions, pos, 250.0f, &globals::config::antiAimYawBase, 2, 1);
			pos.y += 50;
		}

		ImGui::EndChild();
		ImGui::TableNextColumn();
	}

	ImGui::EndTable();
	ImGui::PopFont();
}

void RenderVisualsTab()
{
	ImGui::PushFont(fonts::Verdana_15);

	ImVec2 startPos = ImGui::GetCursorScreenPos();
	ImVec2 availableRegion = ImGui::GetContentRegionAvail();
	float groupWidth = availableRegion.x / 2;
	float groupHeight = (availableRegion.y / 2) - 4;

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddRectFilled(
		ImVec2(startPos.x - 2, startPos.y),
		ImVec2(startPos.x + availableRegion.x + 2, startPos.y + availableRegion.y),
		ImGui::GetColorU32(globals::config::colorScene),
		3.0f
	);

	ImVec2 pos = ImVec2(startPos.x + 25, startPos.y);

	render::DrawButton("Enable Visuals", &globals::config::visualsEnabled, pos);
	if (globals::config::visualsEnabled)
	{
		pos.y += 30;
		render::DrawButton("ESP", &globals::config::espEnabled, pos);
		if (globals::config::espEnabled)
		{
			pos.y += 30;
			render::DrawButton("Enemy ESP", &globals::config::espConfig.enemy.enabled, pos);
			if (globals::config::espConfig.enemy.enabled)
			{
				pos.y += 30;
				render::DrawButton("Enemy Boxes", &globals::config::espConfig.enemy.box, pos);
				pos.y += 30;
				render::PopupColorPicker("Enemy Box Primary", &globals::config::espConfig.enemy.boxPrimary, pos);
				pos.y += 30;
				render::PopupColorPicker("Enemy Box Secondary", &globals::config::espConfig.enemy.boxSecondary, pos);
				pos.y += 30;
				render::DrawButton("Enemy Health Bar", &globals::config::espConfig.enemy.healthBar, pos);
				pos.y += 30;
				render::PopupColorPicker("Enemy Health Primary", &globals::config::espConfig.enemy.HealthPrimary, pos);
				pos.y += 30;
				render::DrawButton("Enemy Weapon", &globals::config::espConfig.enemy.weapon, pos);
				pos.y += 30;
				render::PopupColorPicker("Enemy Weapon Primary", &globals::config::espConfig.enemy.WeaponPrimary, pos);
				pos.y += 30;
				render::DrawButton("Enemy Skeleton", &globals::config::espConfig.enemy.skeleton, pos);
				pos.y += 30;
				render::PopupColorPicker("Enemy Skeleton Primary", &globals::config::espConfig.enemy.SkeletonPrimary, pos);
				pos.y += 30;
				render::DrawSlider("Enemy ESP Distance", &globals::config::espConfig.enemy.distance, 1, 1000, pos);
			}

			pos.y += 40;
			render::DrawButton("Teammate ESP", &globals::config::espConfig.teammate.enabled, pos);
			if (globals::config::espConfig.teammate.enabled)
			{
				pos.y += 30;
				render::DrawButton("Teammate Boxes", &globals::config::espConfig.teammate.box, pos);
				pos.y += 30;
				render::PopupColorPicker("Teammate Primary", &globals::config::espConfig.teammate.boxPrimary, pos);
				pos.y += 30;
				render::PopupColorPicker("Teammate Secondary", &globals::config::espConfig.teammate.boxSecondary, pos);
				pos.y += 30;
				render::DrawButton("Teammate Health Bar", &globals::config::espConfig.teammate.healthBar, pos);
				pos.y += 30;
				render::PopupColorPicker("Teammate Health Primary", &globals::config::espConfig.teammate.HealthPrimary, pos);
				pos.y += 30;
				render::DrawButton("Teammate Weapon", &globals::config::espConfig.teammate.weapon, pos);
				pos.y += 30;
				render::PopupColorPicker("Teammate Weapon Primary", &globals::config::espConfig.teammate.WeaponPrimary, pos);
				pos.y += 30;
				render::DrawButton("Teammate Skeleton", &globals::config::espConfig.teammate.skeleton, pos);
				pos.y += 30;
				render::PopupColorPicker("Teammate Skeleton Primary", &globals::config::espConfig.teammate.SkeletonPrimary, pos);
				pos.y += 30;
				render::DrawSlider("Teammate ESP Distance", &globals::config::espConfig.teammate.distance, 1, 1000, pos);
			}
		}

		pos.y += 40;
		render::DrawButton("Chams", &globals::config::chamsEnabled, pos);
		if (globals::config::chamsEnabled)
		{
			pos.y += 30;
			render::DrawButton("Enemy Chams", &globals::config::chamsTable[0].enabled, pos);
			if (globals::config::chamsTable[0].enabled)
			{
				pos.y += 30;
				render::PopupColorPicker("Primary Enemy Color", &globals::config::chamsTable[0].primary, pos);
				pos.y += 30;
				if (globals::config::chamsTable[0].type == 9)
				{
					render::PopupColorPicker("Secondary Enemy Color", &globals::config::chamsTable[0].secondary, pos);
					pos.y += 30;
				}
				static const char* enemyChamsOptions[] = { "Texture", "Flat", "Wireframe", "Platinum", "Glass", "Crystal", "Gold", "Chrome", "Plastic", "Glow" };
				render::DrawCombo("Enemy Material", enemyChamsOptions, pos, 150, &globals::config::chamsTable[0].type, 10, 1);
			}
		}

		pos.y += 40;
		render::DrawButton("Glow", &globals::config::glowEnabled, pos);
		if (globals::config::glowEnabled)
		{
			pos.y += 30;
			render::DrawButton("Enemy Glow", &globals::config::glowTable[0].enabled, pos);
			if (globals::config::glowTable[0].enabled)
			{
				pos.y += 30;
				render::PopupColorPicker("Enemy Glow Color", &globals::config::glowTable[0].primary, pos);
				static const char* glowOptions[] = { "Static", "Inner Pulse", "Thin", "Pulse" };
				render::DrawCombo("Enemy Glow Mode", glowOptions, pos, 150, &globals::config::glowTable[0].type, 4, 1);
			}
		}

		pos.y += 40;
		render::DrawSlider("World FOV", &globals::config::worldFov, 60.f, 130.f, pos);
		pos.y += 30;
		render::DrawButton("Night Mode", &globals::config::nightMode, pos);
		if (globals::config::nightMode)
		{
			pos.y += 30;
			render::DrawSlider("Brightness Scale", &globals::config::nightScale, 0.01f, 1.0f, pos);
		}
		pos.y += 30;
		render::PopupColorPicker("Sky Color", &globals::config::skyColor, pos);
	}

	ImGui::PopFont();
}

void RenderMiscTab()
{
	ImGui::PushFont(fonts::Verdana_15);

	ImVec2 startPos = ImGui::GetCursorScreenPos();
	ImVec2 availableRegion = ImGui::GetContentRegionAvail();
	float groupWidth = availableRegion.x / 2;
	float groupHeight = (availableRegion.y / 2) - 4;

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddRectFilled(
		ImVec2(startPos.x - 2, startPos.y),
		ImVec2(startPos.x + availableRegion.x + 2, startPos.y + availableRegion.y),
		ImGui::GetColorU32(globals::config::colorScene),
		3.0f
	);

	ImVec2 pos = ImVec2(startPos.x + 25, startPos.y);

	ImGui::BeginGroup();
	ImGui::BeginChild("MiscTab", ImVec2(groupWidth, groupHeight), false, ImGuiWindowFlags_NoScrollbar);
	ImGui::SeparatorText("Miscellaneous");
	pos.y += 40;
	pos.x -= 15;
	render::DrawButton("Bunny Hop", &globals::config::bunnyHop, pos);
	pos.y += 25;
	render::DrawButton("Auto-Strafer", &globals::config::autoStrafe, pos);
	pos.y += 25;
	render::DrawKeybind("Third Person", globals::config::ThirdPersonKey, pos, 0.f, 0.f, 0.f, 20);
	pos.y += 50;
	render::DrawSlider("Distance", &globals::config::thirdPersonDistance, 50.f, 300.f, pos);
	pos.y += 30;
	render::DrawButton("Disable Recoil", &globals::config::disableRecoil, pos);
	pos.y += 25;
	render::DrawButton("Disable Post Processing", &globals::config::disablePostProcessing, pos);
	pos.y += 25;
	ImGui::EndChild();
	ImGui::EndGroup();
	ImGui::PopFont();
}

void RenderConfigurationTab()
{
	ImGui::PushFont(fonts::Verdana_15);
	ImGui::BeginGroup();
	static char buffer[128];
	ImGui::SeparatorText("Config");
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.f, 1.f, 1.f, 0.f));
	ImGui::InputText("##cfg", buffer, sizeof(buffer));
	render::DrawBorder(globals::config::colorScene, 2.f, 4.f);
	ImGui::PopStyleColor();
	if (ImGui::Button("save"))
	{
		globals::config::configName = buffer;
		cfg::save();
	}
	ImGui::SameLine();
	if (ImGui::Button("load"))
	{
		cfg::load();
	}
	ImGui::EndGroup();
	ImGui::PopFont();
}

void c_menu::on_paint() {
	if (!(input::m_blocked = globals::config::menuKey.Toggled))
		return;

	ImGuiStyle& style = ImGui::GetStyle();

	style.FrameBorderSize = 1.0f;
	style.WindowRounding = 3.0f;
	style.FrameRounding = 3.0f;
	style.PopupRounding = 3.0f;
	style.ScrollbarRounding = 2.0f;
	style.GrabRounding = 2.0f;

	ImVec4* colors = style.Colors;
	colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);  
	colors[ImGuiCol_Button] = globals::config::backgroundColor;
	colors[ImGuiCol_ButtonHovered] = globals::config::secondaryColor;
	colors[ImGuiCol_ButtonActive] = globals::config::secondaryColor;
	colors[ImGuiCol_Border] = globals::config::secondaryColor;

	ImGui::SetNextWindowPos(ImGui::GetIO().DisplaySize / 2.f, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(800, 700));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
	ImGui::Begin("PRIMEVAL", &globals::config::menuKey.Toggled,
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoDecoration);
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 windowSize = ImGui::GetWindowSize();

	ImVec2 original = ImGui::GetCursorScreenPos();

	drawList->AddRectFilled(ImVec2(windowPos.x + 80.f, windowPos.y),
		ImVec2(windowPos.x + windowSize.x - 10.f, windowPos.y + 50),
		ImColor(globals::config::backgroundColor), 5.0f, ImDrawFlags_RoundCornersAll);

	ImGui::PushFont(fonts::TitleFont);

	ImVec2 textSizePri = ImGui::CalcTextSize("pri");
	ImVec2 textSizeMeval = ImGui::CalcTextSize("meval");
	float totalTextWidth = textSizePri.x + textSizeMeval.x;
	float textHeight = textSizePri.y;

	float textX = windowPos.x + (windowSize.x + 75.f - totalTextWidth) / 2.0f;
	float textY = windowPos.y + (50 - textHeight) / 2.0f;

	drawList->AddText(ImVec2(textX, textY), ImColor(globals::config::textColor), "pri");
	drawList->AddText(ImVec2(textX + textSizePri.x, textY), ImColor(globals::config::secondaryColor), "meval");

	ImGui::SetCursorScreenPos(ImVec2(textX + totalTextWidth + 5.f, textY + 5.f));
	ImGui::Image(textures::logo, ImVec2(25, 25), ImVec2(0, 0), ImVec2(1, 1), globals::config::secondaryColor);

	ImGui::SetCursorScreenPos(original);
	ImGui::PopFont();

	ImGui::Dummy(ImVec2(0, 40));
	ImGui::PushFont(fonts::Verdana_15);

	static int selectedTab = 0;
	const char* tabNames[] = { "Rage", "Anti-Aim", "Visuals", "Misc", "Config" };
	ImTextureID iconNames[] = { textures::ragetab, textures::aaTab, textures::visualsTab, textures::miscTab, textures::configTab };

	ImVec2 sidebarSize = ImVec2(70, windowSize.y - 105);
	float contentWidth = windowSize.x - sidebarSize.x - 30;

	drawList->AddRectFilled(ImVec2(windowPos.x, windowPos.y + 67), ImVec2(windowPos.x + sidebarSize.x, windowPos.y + windowSize.y - 12),
		ImColor(globals::config::backgroundColor), 5.0f, ImDrawFlags_RoundCornersAll);

	ImGui::SetCursorPos(ImVec2(0, 100));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);

	ImGui::BeginChild("Container", sidebarSize, false);

	for (int i = 0; i < IM_ARRAYSIZE(tabNames); i++) {
		bool isSelected = (selectedTab == i);
		ImVec4 tintColor = isSelected ? globals::config::secondaryColor : globals::config::textColor;

		ImVec2 size = ImVec2(50, 50);
		float xOffset = 5.f;
		float ySpacing = 30;

		if (iconNames[i]) {
			IDirect3DTexture9* texture = (IDirect3DTexture9*)iconNames[i];
			D3DSURFACE_DESC desc;
			if (texture) {
				texture->GetLevelDesc(0, &desc);
				size = ImVec2((float)desc.Width * 0.9f, (float)desc.Height * 0.9f);
			}

			float scaleFactor = 0.75f;
			size.x *= scaleFactor;
			size.y *= scaleFactor;

			float maxWidth = 70.0f * scaleFactor;
			xOffset = (maxWidth - size.x) / 2.0f + 5.f;

			if (i == 4)
				xOffset += 3.f;

			interfaces::m_d3d_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
			interfaces::m_d3d_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
			interfaces::m_d3d_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
			interfaces::m_d3d_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

			ImGui::SetCursorPosX(xOffset);

			if (ImGui::ImageButton(tabNames[i], iconNames[i], size, ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), tintColor)) {
				selectedTab = i;
			}

			float cursorY = ImGui::GetCursorPosY();
			ImGui::SetCursorPosY(cursorY + ySpacing);
		}
	}

	ImGui::EndChild();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor(4);

	ImGui::SameLine();
	ImGui::Dummy(ImVec2(15, 0));
	ImGui::SameLine();

	ImVec2 FrameStart = ImVec2(windowPos.x + sidebarSize.x + 10.f, windowPos.y + 75 - 10.f);
	ImVec2 FrameEnd = ImVec2(windowPos.x + windowSize.x - 10.f, windowPos.y + windowSize.y - 10.f);

	drawList->AddRectFilled(FrameStart, FrameEnd, ImColor(globals::config::backgroundColor), 5.0f, ImDrawFlags_RoundCornersAll);

	contentWidth = FrameEnd.x - FrameStart.x - (2 * 10.f);
	float contentHeight = FrameEnd.y - FrameStart.y - (2 * 10.f);
	ImVec2 contentPos = ImVec2(FrameStart.x + 10.f, FrameStart.y + 10.f);

	drawList->AddRectFilled(contentPos,
		ImVec2(contentPos.x + contentWidth, contentPos.y + contentHeight),
		ImColor(globals::config::backgroundColor), 5.0f, ImDrawFlags_RoundCornersAll);

	ImGui::SetCursorPos(ImVec2(contentPos.x - windowPos.x, contentPos.y - windowPos.y));

	ImGui::BeginChild("Content", ImVec2(contentWidth, contentHeight), false);

	switch (selectedTab) {
	case 0: RenderAimbotTab(); break;
	case 1: RenderAntiAimTab(); break;
	case 2: RenderVisualsTab(); break;
	case 3: RenderMiscTab(); break;
	case 4: RenderConfigurationTab(); break;
	}

	ImGui::EndChild();
	render::DrawBorder(globals::config::secondaryColor);

	ImGui::PopFont();
	ImGui::End();
}