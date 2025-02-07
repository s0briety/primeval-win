#include "menu.h"

inline ImGuiDir arrow = ImGuiDir_Down;

void RenderAimbotTab()
{
	ImGui::PushFont(fonts::Verdana_15);

	ImVec2 tableStartPos = ImGui::GetCursorScreenPos();
	ImVec2 availableRegion = ImGui::GetContentRegionAvail();
	float groupWidth = availableRegion.x / 2;
	float groupHeight = (availableRegion.y / 2) - 4;

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddRectFilled(
		ImVec2(tableStartPos.x - 2, tableStartPos.y),
		ImVec2(tableStartPos.x + availableRegion.x + 2, tableStartPos.y + availableRegion.y),
		ImGui::GetColorU32(globals::config::colorScene),
		3.0f
	);

	if (ImGui::BeginTable("Rage", 2, ImGuiTableFlags_SizingStretchProp))
	{
		ImGui::TableNextColumn();
		ImGui::BeginChild("Features", ImVec2(groupWidth, groupHeight), true, ImGuiWindowFlags_NoScrollbar);
		ImGui::SeparatorText("Features");
		render::DrawSwitch("Enable Aimbot", &globals::config::aimbotEnabled, false);

		if (globals::config::aimbotEnabled)
		{
			ImGui::Indent();
			render::DrawSwitch("Silent Aim", &globals::config::silentAimEnabled);
			render::DrawSwitch("Resolver", &globals::config::aimbotResolver);
			render::DrawSwitch("Autofire", &globals::config::aimbotAutoShoot);
			render::DrawSwitch("Autoscope", &globals::config::aimbotAutoscope);
			render::DrawSwitch("Autostop", &globals::config::aimbotAutostop);
			render::DrawSwitch("Quick Peek", &globals::config::aimbotQuickPeek);
			ImGui::Unindent();
		}
		ImGui::EndChild();

		ImGui::BeginChild("Exploits", ImVec2(groupWidth, groupHeight), true, ImGuiWindowFlags_NoScrollbar);
		ImGui::SeparatorText("Exploits");
		render::drawKeyBind("Doubletap", globals::config::DoubleTapKey);
		render::drawKeyBind("Hideshots", globals::config::HideshotsKey);
		render::drawKeyBind("Ideal Tick", globals::config::IdealTickKey);
		ImGui::EndChild();

		ImGui::TableNextColumn();
		ImGui::BeginChild("Targeting", ImVec2(groupWidth, groupHeight * 2), true, ImGuiWindowFlags_NoScrollbar);
		ImGui::SeparatorText("Targeting");

		ImGui::PushStyleColor(ImGuiCol_FrameBg, globals::config::backgroundColor);
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, globals::config::backgroundColor);
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(globals::config::backgroundColor.x + 0.05f, globals::config::backgroundColor.y + 0.05f, globals::config::backgroundColor.z + 0.05f, globals::config::backgroundColor.w));
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(globals::config::backgroundColor.x - 0.05f, globals::config::backgroundColor.y - 0.05f, globals::config::backgroundColor.z - 0.05f, globals::config::backgroundColor.w));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(globals::config::backgroundColor.x + 0.05f, globals::config::backgroundColor.y + 0.05f, globals::config::backgroundColor.z + 0.05f, globals::config::backgroundColor.w));
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(globals::config::backgroundColor.x - 0.05f, globals::config::backgroundColor.y - 0.05f, globals::config::backgroundColor.z - 0.05f, globals::config::backgroundColor.w));

		ImGui::Text("Target Selection");
		std::string targLabel = "##combo" + std::string("targ");

		if (ImGui::BeginCombo(targLabel.c_str(), globals::config::aimbotTargetSelection == 0 ? "Distance" : globals::config::aimbotTargetSelection == 1 ? "Health" : "Hitchance", ImGuiComboFlags_HeightLarge))
		{
			for (int i = 0; i < 3; i++)
			{
				bool Selected = (globals::config::aimbotTargetSelection == i);

				if (ImGui::Selectable(i == 0 ? "Distance" : i == 1 ? "Health" : "Hitchance", Selected))
				{
					globals::config::aimbotTargetSelection = i;
				}

			}
			ImGui::EndCombo();
		}

		ImGui::Text("Hitbox Selection");
		std::string selectedHitboxes = "";
		for (int i = 0; i < 8; i++)
		{
			if (globals::config::hitboxTable[i].isEnabled)
			{
				if (!selectedHitboxes.empty())
					selectedHitboxes += ", ";
				selectedHitboxes += globals::config::hitboxTable[i].label;
			}
		}

		const char* previewText = selectedHitboxes.empty() ? "Select Hitboxes" : selectedHitboxes.c_str();

		if (ImGui::BeginCombo("##hitbox", previewText, ImGuiComboFlags_HeightLarge))
		{
			for (int i = 0; i < 8; i++)
			{
				bool selected = globals::config::hitboxTable[i].isEnabled;

				if (ImGui::Selectable(globals::config::hitboxTable[i].label, selected, ImGuiSelectableFlags_DontClosePopups))
				{
					globals::config::hitboxTable[i].isEnabled = !selected;
				}
			}
			ImGui::EndCombo();
		}

		ImGui::Text("Multipoint Selection");
		std::string selectedMultipoints = "";
		for (int i = 0; i < 8; i++)
		{
			if (globals::config::multipointTable[i].isEnabled)
			{
				if (!selectedMultipoints.empty())
					selectedMultipoints += ", ";
				selectedMultipoints += globals::config::multipointTable[i].label;
			}
		}

		const char* mpText = selectedMultipoints.empty() ? "Select Multipoints" : selectedMultipoints.c_str();

		if (ImGui::BeginCombo("##multipoints", mpText, ImGuiComboFlags_HeightLarge))
		{
			for (int i = 0; i < 8; i++)
			{
				bool selected = globals::config::multipointTable[i].isEnabled;

				if (ImGui::Selectable(globals::config::multipointTable[i].label, selected, ImGuiSelectableFlags_DontClosePopups))
				{
					globals::config::multipointTable[i].isEnabled = !selected;
				}
			}
			ImGui::EndCombo();
		}

		render::drawSlider("Min. Hitchance", &globals::config::aimbotHitchance, 1.0f, 100.0f);
		render::drawKeyBind("Hitchance Override", globals::config::HitchanceOverrideKey, globals::config::HitchanceOverride, 1.0f, 100.0f);
		render::drawSlider("Min. Damage", &globals::config::aimbotMinDamage, 1.0f, 110.0f);
		render::drawKeyBind("Damage Override", globals::config::MinDamageOverrideKey, globals::config::MinDamageOverride, 1.0f, 100.0f);

		if (!selectedMultipoints.empty())
		{
			ImGui::Text("Multipoint");
			ImGui::Indent();
			render::drawSlider("Head Scale", &globals::config::multipointHead, 0.0f, 100.0f, true);
			render::drawSlider("Body Scale", &globals::config::multipointBody, 0.0f, 100.0f, true);
			ImGui::Unindent();
			ImGui::Spacing();
		}

		render::DrawSwitch("Prefer Safe Point", &globals::config::preferSafepoint, false);
		render::drawKeyBind("Force Body Aim", globals::config::forceBaimKey);

		ImGui::PopStyleColor(6);
		ImGui::EndChild();

		ImGui::EndTable();
	}

	ImGui::PopFont();
}


void RenderAntiAimTab()
{
	ImGui::PushFont(fonts::Verdana_15);
	render::DrawSwitch("Enable Anti-Aim", &globals::config::antiAimEnabled);
	if (globals::config::antiAimEnabled)
	{
		ImGui::SliderInt("Pitch", &globals::config::antiAimPitch, -89, 89, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Yaw", &globals::config::antiAimYaw, -180, 180, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::Combo("Yaw Mode", &globals::config::antiAimYawMode, "Static\0Jitter\0Spin\0Random\0Distort\0Dynamic\0");
		ImGui::Combo("Pitch Mode", &globals::config::antiAimPitchMode, "Static\0Down\0Up\0Random\0Alternate\0Fake Down\0Fake Up\0");
	}
	ImGui::PopFont();
}

void RenderVisualsTab()
{
	ImGui::PushFont(fonts::Verdana_15);
	render::DrawSwitch("Enable Visuals", &globals::config::visualsEnabled);
	if (globals::config::visualsEnabled)
	{
		ImGui::BeginGroup();
		render::DrawSwitch("ESP", &globals::config::espEnabled);
		if (globals::config::espEnabled)
		{
			// Enemy Settings
			render::DrawSwitch("Enemy ESP", &globals::config::espConfig.enemy.enabled);
			if (globals::config::espConfig.enemy.enabled)
			{
				render::DrawSwitch("Enemy Boxes", &globals::config::espConfig.enemy.box);
				render::DrawColorPicker("Enemy Box Primary Color", &globals::config::espConfig.enemy.boxPrimary);
				render::DrawColorPicker("Enemy Box Secondary Color", &globals::config::espConfig.enemy.boxSecondary);
				render::DrawSwitch("Enemy Health Bar", &globals::config::espConfig.enemy.healthBar);
				render::DrawColorPicker("Enemy Health Primary Color", &globals::config::espConfig.enemy.HealthPrimary);
				render::DrawSwitch("Enemy Weapon", &globals::config::espConfig.enemy.weapon);
				render::DrawColorPicker("Enemy Weapon Primary Color", &globals::config::espConfig.enemy.WeaponPrimary);
				render::DrawSwitch("Enemy Skeleton", &globals::config::espConfig.enemy.skeleton);
				render::DrawColorPicker("Enemy Skeleton Primary Color", &globals::config::espConfig.enemy.SkeletonPrimary);
				render::drawSlider("Enemy ESP Distance", &globals::config::espConfig.enemy.distance, 1, 1000);
			}

			// Teammate Settings
			render::DrawSwitch("Teammate ESP", &globals::config::espConfig.teammate.enabled);
			if (globals::config::espConfig.teammate.enabled)
			{
				render::DrawSwitch("Teammate Boxes", &globals::config::espConfig.teammate.box);
				render::DrawColorPicker("Teammate Primary Color", &globals::config::espConfig.teammate.boxPrimary);
				render::DrawColorPicker("Teammate Secondary Color", &globals::config::espConfig.teammate.boxSecondary);
				render::DrawSwitch("Teammate Health Bar", &globals::config::espConfig.teammate.healthBar);
				render::DrawColorPicker("Teammate Health Primary Color", &globals::config::espConfig.teammate.HealthPrimary);
				render::DrawSwitch("Teammate Weapon", &globals::config::espConfig.teammate.weapon);
				render::DrawColorPicker("Teammate Weapon Primary Color", &globals::config::espConfig.teammate.WeaponPrimary);
				render::DrawSwitch("Teammate Skeleton", &globals::config::espConfig.teammate.skeleton);
				render::DrawColorPicker("Teammate Skeleton Primary Color", &globals::config::espConfig.teammate.SkeletonPrimary);
				render::drawSlider("Teammate ESP Distance", &globals::config::espConfig.teammate.distance, 1, 1000);
			}

			// Self Settings
			render::DrawSwitch("Self ESP", &globals::config::espConfig.self.enabled);
			if (globals::config::espConfig.self.enabled)
			{
				render::DrawSwitch("Self Boxes", &globals::config::espConfig.self.box);
				render::DrawColorPicker("Self Primary Color", &globals::config::espConfig.self.boxPrimary);
				render::DrawColorPicker("Self Secondary Color", &globals::config::espConfig.self.boxSecondary);
				render::DrawSwitch("Self Health Bar", &globals::config::espConfig.self.healthBar);
				render::DrawColorPicker("Self Health Primary Color", &globals::config::espConfig.self.HealthPrimary);
				render::DrawSwitch("Self Weapon", &globals::config::espConfig.self.weapon);
				render::DrawColorPicker("Self Weapon Primary Color", &globals::config::espConfig.self.WeaponPrimary);
				render::DrawSwitch("Self Skeleton", &globals::config::espConfig.self.skeleton);
				render::DrawColorPicker("Self Skeleton Primary Color", &globals::config::espConfig.self.SkeletonPrimary);
				render::drawSlider("Self ESP Distance", &globals::config::espConfig.self.distance, 1, 1000);
			}
		}
		ImGui::EndGroup();

		ImGui::BeginGroup();
		render::DrawSwitch("Chams", &globals::config::chamsEnabled);
		if (globals::config::chamsEnabled)
		{

			render::DrawSwitch("Enemy Chams", &globals::config::chamsTable[0].enabled);

			if (globals::config::chamsTable[0].enabled)
			{
				ImGui::ColorEdit4("Primary Enemy Color", (float*)&globals::config::chamsTable[0].primary);

				if (globals::config::chamsTable[0].type == 9)
				{
					ImGui::ColorEdit4("Secondary Enemy Color", (float*)&globals::config::chamsTable[0].secondary);
				}

				ImGui::PushStyleColor(ImGuiCol_FrameBg, globals::config::backgroundColor);
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, globals::config::backgroundColor);
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(globals::config::backgroundColor.x + 0.05f, globals::config::backgroundColor.y + 0.05f, globals::config::backgroundColor.z + 0.05f, globals::config::backgroundColor.w));
				ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(globals::config::backgroundColor.x - 0.05f, globals::config::backgroundColor.y - 0.05f, globals::config::backgroundColor.z - 0.05f, globals::config::backgroundColor.w));
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(globals::config::backgroundColor.x + 0.05f, globals::config::backgroundColor.y + 0.05f, globals::config::backgroundColor.z + 0.05f, globals::config::backgroundColor.w));
				ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(globals::config::backgroundColor.x - 0.05f, globals::config::backgroundColor.y - 0.05f, globals::config::backgroundColor.z - 0.05f, globals::config::backgroundColor.w));
				ImGui::Combo("##EnemyMaterial", &globals::config::chamsTable[0].type, "Texture\0Flat\0Wireframe\0Platinum\0Glass\0Crystal\0Gold\0Chrome\0Plastic\0Glow\0\0");
				ImGui::PopStyleColor(6);
			}

			render::DrawSwitch("Teammate Chams", &globals::config::chamsTable[1].enabled);

			if (globals::config::chamsTable[1].enabled)
			{
				ImGui::ColorEdit4("Primary Teammate Color", (float*)&globals::config::chamsTable[1].primary);

				if (globals::config::chamsTable[1].type == 9)
				{
					ImGui::ColorEdit4("Secondary Teammate Color", (float*)&globals::config::chamsTable[1].secondary);
				}

				ImGui::PushStyleColor(ImGuiCol_FrameBg, globals::config::backgroundColor);
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, globals::config::backgroundColor);
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(globals::config::backgroundColor.x + 0.05f, globals::config::backgroundColor.y + 0.05f, globals::config::backgroundColor.z + 0.05f, globals::config::backgroundColor.w));
				ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(globals::config::backgroundColor.x - 0.05f, globals::config::backgroundColor.y - 0.05f, globals::config::backgroundColor.z - 0.05f, globals::config::backgroundColor.w));
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(globals::config::backgroundColor.x + 0.05f, globals::config::backgroundColor.y + 0.05f, globals::config::backgroundColor.z + 0.05f, globals::config::backgroundColor.w));
				ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(globals::config::backgroundColor.x - 0.05f, globals::config::backgroundColor.y - 0.05f, globals::config::backgroundColor.z - 0.05f, globals::config::backgroundColor.w));
				ImGui::Combo("##FriendlyMaterial", &globals::config::chamsTable[1].type, "Texture\0Flat\0Wireframe\0Platinum\0Glass\0Crystal\0Gold\0Chrome\0Plastic\0Glow\0\0");
				ImGui::PopStyleColor(6);
			}

			render::DrawSwitch("Self Chams", &globals::config::chamsTable[2].enabled);

			if (globals::config::chamsTable[2].enabled)
			{
				ImGui::ColorEdit4("Primary Self Color", (float*)&globals::config::chamsTable[2].primary);

				if (globals::config::chamsTable[2].type == 9)
				{
					ImGui::ColorEdit4("Secondary Self Color", (float*)&globals::config::chamsTable[2].secondary);
				}

				ImGui::PushStyleColor(ImGuiCol_FrameBg, globals::config::backgroundColor);
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, globals::config::backgroundColor);
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(globals::config::backgroundColor.x + 0.05f, globals::config::backgroundColor.y + 0.05f, globals::config::backgroundColor.z + 0.05f, globals::config::backgroundColor.w));
				ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(globals::config::backgroundColor.x - 0.05f, globals::config::backgroundColor.y - 0.05f, globals::config::backgroundColor.z - 0.05f, globals::config::backgroundColor.w));
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(globals::config::backgroundColor.x + 0.05f, globals::config::backgroundColor.y + 0.05f, globals::config::backgroundColor.z + 0.05f, globals::config::backgroundColor.w));
				ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(globals::config::backgroundColor.x - 0.05f, globals::config::backgroundColor.y - 0.05f, globals::config::backgroundColor.z - 0.05f, globals::config::backgroundColor.w));
				ImGui::Combo("##SelfMaterial", &globals::config::chamsTable[2].type, "Texture\0Flat\0Wireframe\0Platinum\0Glass\0Crystal\0Gold\0Chrome\0Plastic\0Glow\0\0");
				ImGui::PopStyleColor(6);
			}
		}
		ImGui::EndGroup();

		ImGui::BeginGroup();
		render::DrawSwitch("Glow", &globals::config::glowEnabled);
		if (globals::config::glowEnabled)
		{
			render::DrawSwitch("Enemy Glow", &globals::config::glowTable[0].enabled);

			if (globals::config::glowTable[0].enabled)
			{
				ImGui::ColorEdit4("Enemy Glow Color", (float*)&globals::config::glowTable[0].primary);
				ImGui::PushStyleColor(ImGuiCol_FrameBg, globals::config::backgroundColor);
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, globals::config::backgroundColor);
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(globals::config::backgroundColor.x + 0.05f, globals::config::backgroundColor.y + 0.05f, globals::config::backgroundColor.z + 0.05f, globals::config::backgroundColor.w));
				ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(globals::config::backgroundColor.x - 0.05f, globals::config::backgroundColor.y - 0.05f, globals::config::backgroundColor.z - 0.05f, globals::config::backgroundColor.w));
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(globals::config::backgroundColor.x + 0.05f, globals::config::backgroundColor.y + 0.05f, globals::config::backgroundColor.z + 0.05f, globals::config::backgroundColor.w));
				ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(globals::config::backgroundColor.x - 0.05f, globals::config::backgroundColor.y - 0.05f, globals::config::backgroundColor.z - 0.05f, globals::config::backgroundColor.w));
				ImGui::Combo("##EnemyGlow", &globals::config::glowTable[0].type, "Static\0Inner Pulse\0Thin\0Pulse\0\0");
				ImGui::PopStyleColor(6);
			}

			render::DrawSwitch("Teammate Glow", &globals::config::glowTable[1].enabled);

			if (globals::config::glowTable[1].enabled)
			{
				ImGui::ColorEdit4("Teammate Glow Color", (float*)&globals::config::glowTable[1].primary);
				ImGui::PushStyleColor(ImGuiCol_FrameBg, globals::config::backgroundColor);
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, globals::config::backgroundColor);
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(globals::config::backgroundColor.x + 0.05f, globals::config::backgroundColor.y + 0.05f, globals::config::backgroundColor.z + 0.05f, globals::config::backgroundColor.w));
				ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(globals::config::backgroundColor.x - 0.05f, globals::config::backgroundColor.y - 0.05f, globals::config::backgroundColor.z - 0.05f, globals::config::backgroundColor.w));
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(globals::config::backgroundColor.x + 0.05f, globals::config::backgroundColor.y + 0.05f, globals::config::backgroundColor.z + 0.05f, globals::config::backgroundColor.w));
				ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(globals::config::backgroundColor.x - 0.05f, globals::config::backgroundColor.y - 0.05f, globals::config::backgroundColor.z - 0.05f, globals::config::backgroundColor.w));
				ImGui::Combo("##FriendlyGlow", &globals::config::glowTable[1].type, "Static\0Inner Pulse\0Thin\0Pulse\0\0");
				ImGui::PopStyleColor(6);
			}

			render::DrawSwitch("Self Glow", &globals::config::glowTable[2].enabled);

			if (globals::config::glowTable[2].enabled)
			{
				ImGui::ColorEdit4("Self Glow Color", (float*)&globals::config::glowTable[2].primary);
				ImGui::PushStyleColor(ImGuiCol_FrameBg, globals::config::backgroundColor);
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, globals::config::backgroundColor);
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(globals::config::backgroundColor.x + 0.05f, globals::config::backgroundColor.y + 0.05f, globals::config::backgroundColor.z + 0.05f, globals::config::backgroundColor.w));
				ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(globals::config::backgroundColor.x - 0.05f, globals::config::backgroundColor.y - 0.05f, globals::config::backgroundColor.z - 0.05f, globals::config::backgroundColor.w));
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(globals::config::backgroundColor.x + 0.05f, globals::config::backgroundColor.y + 0.05f, globals::config::backgroundColor.z + 0.05f, globals::config::backgroundColor.w));
				ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(globals::config::backgroundColor.x - 0.05f, globals::config::backgroundColor.y - 0.05f, globals::config::backgroundColor.z - 0.05f, globals::config::backgroundColor.w));
				ImGui::Combo("##SelfGlow", &globals::config::glowTable[2].type, "Static\0Inner Pulse\0Thin\0Pulse\0\0");
				ImGui::PopStyleColor(6);
			}

		}
		ImGui::EndGroup();

		ImGui::BeginGroup();
		ImGui::SliderFloat("World FOV", &globals::config::worldFov, 60.f, 130.f, "%.0f", ImGuiSliderFlags_AlwaysClamp);
		render::DrawSwitch("Night mode", &globals::config::nightMode);

		if (globals::config::nightMode)
			render::drawSlider("Brightness Scale", &globals::config::nightScale, 0.01f, 1.0f, true, 2);

		ImGui::ColorEdit4("Sky Color", (float*)&globals::config::skyColor);
		ImGui::EndGroup();
	}
	ImGui::PopFont();
}

void RenderMiscTab()
{
	ImGui::PushFont(fonts::Verdana_15);
	ImGui::BeginGroup();
	render::DrawSwitch("Bunny Hop", &globals::config::bunnyHop, false);
	render::DrawSwitch("Auto-Strafer", &globals::config::autoStrafe, false);

	render::drawKeyBind("Third Person", globals::config::ThirdPersonKey);

	ImGui::BulletText("Distance");
	ImGui::SliderFloat("##tpsDistance", &globals::config::thirdPersonDistance, 50.f, 300.f, "%.0f", ImGuiSliderFlags_AlwaysClamp);

	ImGui::Spacing();

	render::DrawSwitch("Disable Recoil", &globals::config::disableRecoil, false);
	render::DrawSwitch("Disable Post Processing", &globals::config::disablePostProcessing, false);
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
	style.WindowRounding = 6.0f;
	style.FrameRounding = 6.0f;
	style.PopupRounding = 6.0f;
	style.ScrollbarRounding = 6.0f;
	style.GrabRounding = 6.0f;

	style.WindowPadding = ImVec2(10, 10);
	style.FramePadding = ImVec2(6, 6);
	style.ItemSpacing = ImVec2(8, 8);

	ImVec4* colors = style.Colors;
	colors[ImGuiCol_WindowBg] = globals::config::backgroundColor;
	colors[ImGuiCol_FrameBg] = globals::config::colorScene;
	colors[ImGuiCol_FrameBgHovered] = ImVec4(globals::config::colorScene.x + 0.02f, globals::config::colorScene.y + 0.02f, globals::config::colorScene.z + 0.02f, globals::config::colorScene.w);
	colors[ImGuiCol_FrameBgActive] = ImVec4(globals::config::colorScene.x + 0.04f, globals::config::colorScene.y + 0.04f, globals::config::colorScene.z + 0.04f, globals::config::colorScene.w);
	colors[ImGuiCol_SliderGrab] = ImVec4(globals::config::secondaryColor.x + 0.02f, globals::config::secondaryColor.y + 0.02f, globals::config::secondaryColor.z + 0.02f, globals::config::secondaryColor.w);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(globals::config::secondaryColor.x + 0.04f, globals::config::secondaryColor.y + 0.04f, globals::config::secondaryColor.z + 0.04f, globals::config::secondaryColor.w);
	colors[ImGuiCol_CheckMark] = globals::config::secondaryColor;
	colors[ImGuiCol_Text] = globals::config::secondaryColor;
	colors[ImGuiCol_Button] = globals::config::colorScene;
	colors[ImGuiCol_ButtonHovered] = ImVec4(globals::config::colorScene.x + 0.02f, globals::config::colorScene.y + 0.02f, globals::config::colorScene.z + 0.02f, globals::config::colorScene.w);
	colors[ImGuiCol_ButtonActive] = ImVec4(globals::config::colorScene.x + 0.04f, globals::config::colorScene.y + 0.04f, globals::config::colorScene.z + 0.04f, globals::config::colorScene.w);
	colors[ImGuiCol_Tab] = globals::config::colorScene;
	colors[ImGuiCol_TabHovered] = ImVec4(globals::config::colorScene.x + 0.02f, globals::config::colorScene.y + 0.02f, globals::config::colorScene.z + 0.02f, globals::config::colorScene.w);
	colors[ImGuiCol_TabActive] = ImVec4(globals::config::colorScene.x + 0.04f, globals::config::colorScene.y + 0.04f, globals::config::colorScene.z + 0.04f, globals::config::colorScene.w);
	colors[ImGuiCol_Separator] = globals::config::secondaryColor;
	colors[ImGuiTableBgTarget_CellBg] = ImVec4(globals::config::colorScene.x + 0.04f, globals::config::colorScene.y + 0.04f, globals::config::colorScene.z + 0.04f, 0.0f);
	colors[ImGuiCol_Border] = ImVec4(globals::config::colorScene.x + 0.02f, globals::config::colorScene.y + 0.02f, globals::config::colorScene.z + 0.02f, 0.0f);
	colors[ImGuiCol_PopupBg] = globals::config::backgroundColor;

	ImGui::SetNextWindowPos(ImGui::GetIO().DisplaySize / 2.f, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(600, 650));
	ImGui::Begin("Sorority", &globals::config::menuKey.Toggled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 windowSize = ImGui::GetWindowSize();

	ImGui::PushFont(fonts::Lucon_22);
	drawList->AddRectFilled(windowPos, ImVec2(windowPos.x + windowSize.x, windowPos.y + 50), ImColor(globals::config::backgroundColor), 12.0f, ImDrawFlags_RoundCornersTop);
	drawList->AddText(ImVec2(windowPos.x + 10, windowPos.y + 15), ImColor(globals::config::secondaryColor), "PRIMEVAL");
	ImGui::PopFont();

	ImGui::Dummy(ImVec2(0, 100));
	ImGui::PushFont(fonts::Verdana_15);

	// Settings Button
	ImVec2 buttonPos = ImVec2(windowPos.x + windowSize.x - 40, windowPos.y + 10);
	ImGui::SetCursorScreenPos(buttonPos);
	if (ImGui::ArrowButton("settings", arrow))
	{
		arrow = ImGuiDir_Right;
		ImGui::OpenPopup("colorPopup");
	}

	if (ImGui::BeginPopup("colorPopup"))
	{
		ImGui::Text("Color Scheme:");
		ImGui::Separator();
		ImGui::ColorPicker4("Background Color", (float*)&globals::config::backgroundColor);
		ImGui::ColorPicker4("Primary Color", (float*)&globals::config::colorScene);
		ImGui::ColorPicker4("Secondary Color", (float*)&globals::config::secondaryColor);
		ImGui::EndPopup();
	}
	else
	{
		arrow = ImGuiDir_Down;
	}

	// Main Tabs
	if (ImGui::BeginTabBar("MainTabs"))
	{
		if (ImGui::BeginTabItem("rage"))
		{
			RenderAimbotTab();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("anti-aim"))
		{
			RenderAntiAimTab();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("visuals"))
		{
			RenderVisualsTab();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("misc"))
		{
			RenderMiscTab();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("config"))
		{
			RenderConfigurationTab();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::PopFont();
	ImGui::End();
}