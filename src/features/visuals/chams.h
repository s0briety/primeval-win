#pragma once
#include "../../globals.h"

class c_chams : public c_singleton<c_chams> {
private:
	enum e_material_type {
		MATERIAL_TYPE_REGULAR,
		MATERIAL_TYPE_FLAT,
		MATERIAL_TYPE_WIREFRAME,
		MATERIAL_TYPE_PLATINUM,
		MATERIAL_TYPE_GLASS,
		MATERIAL_TYPE_CRYSTAL,
		MATERIAL_TYPE_GOLD,
		MATERIAL_TYPE_DARK_CHROME,
		MATERIAL_TYPE_PLASTIC,
		MATERIAL_TYPE_GLOW
	};

	void override_material(int type, const col_t& clr, bool ignorez);
	i_material* create_material(std::string_view material_name, std::string_view shader_type, std::string_view material_data);

	i_material* m_regular = nullptr;
	i_material* m_regular_z = nullptr;

	i_material* m_flat = nullptr;
	i_material* m_flat_z = nullptr;

	i_material* m_wireframe = nullptr;
	i_material* m_wireframe_z = nullptr;

	i_material* m_platinum = nullptr;
	i_material* m_platinum_z = nullptr;

	i_material* m_glass = nullptr;
	i_material* m_glass_z = nullptr;

	i_material* m_crystal = nullptr;
	i_material* m_crystal_z = nullptr;

	i_material* m_gold = nullptr;
	i_material* m_gold_z = nullptr;

	i_material* m_dark_chrome = nullptr;
	i_material* m_dark_chrome_z = nullptr;

	i_material* m_plastic = nullptr;
	i_material* m_plastic_z = nullptr;

	i_material* m_glow = nullptr;
	i_material* m_glow_z = nullptr;
public:
	c_chams() {
		m_regular = create_material(_("primeval_regular.vmt"), _("VertexLitGeneric"), _(R"#("VertexLitGeneric" {
	"$basetexture" "vgui/white_additive"
	"$ignorez"      "0"
	"$model"		"1"
	"$flat"			"0"
	"$nocull"		"1"
	"$halflambert"	"1"
	"$nofog"		"1"
	"$wireframe"	"0"
})#"));

		m_regular_z = create_material(_("primeval_regular_z.vmt"), _("VertexLitGeneric"), _(R"#("VertexLitGeneric" {
	"$basetexture" "vgui/white_additive"
	"$ignorez"      "1"
	"$model"		"1"
	"$flat"			"0"
	"$nocull"		"1"
	"$halflambert"	"1"
	"$nofog"		"1"
	"$wireframe"	"0"
})#"));

		m_flat = create_material(_("primeval_flat.vmt"), _("UnlitGeneric"), _(R"#("UnlitGeneric" {
	"$basetexture" "vgui/white_additive"
	"$ignorez"      "0"
	"$model"		"1"
	"$flat"			"1"
	"$nocull"		"1"
	"$selfillum"	"1"
	"$halflambert"	"1"
	"$nofog"		"1"
	"$wireframe"	"0"
})#"));

		m_flat_z = create_material(_("primeval_flat_z.vmt"), _("UnlitGeneric"), _(R"#("UnlitGeneric" {
	"$basetexture" "vgui/white_additive"
	"$ignorez"      "1"
	"$model"		"1"
	"$flat"			"1"
	"$nocull"		"1"
	"$selfillum"	"1"
	"$halflambert"	"1"
	"$nofog"		"1"
	"$wireframe"	"0"
})#"));
		m_wireframe = create_material(_("primeval_flat.vmt"), _("UnlitGeneric"), _(R"#("UnlitGeneric" {
	"$basetexture" "vgui/white_additive"
	"$ignorez"      "0"
	"$model"		"1"
	"$flat"			"0"
	"$nocull"		"1"
	"$selfillum"	"1"
	"$halflambert"	"1"
	"$nofog"		"1"
	"$wireframe"	"1"
})#"));

		m_wireframe_z = create_material(_("primeval_flat_z.vmt"), _("UnlitGeneric"), _(R"#("UnlitGeneric" {
	"$basetexture" "vgui/white_additive"
	"$ignorez"      "1"
	"$model"		"1"
	"$flat"			"0"
	"$nocull"		"1"
	"$selfillum"	"1"
	"$halflambert"	"1"
	"$nofog"		"1"
	"$wireframe"	"1"
})#"));
		m_glow = create_material(_("primeval_glow_z.vmt"), _("VertexLitGeneric"), _(R"#("VertexLitGeneric" {
    "$ignorez"  "0"
	"$additive" "1"
	"$envmap" "models/effects/cube_white"
	"$envmaptint" "[1 1 1]"
	"$envmapfresnel" "1"
	"$envmapfresnelminmaxexp" "[0 1 2]"
	"$alpha" "1"
})#"));

		m_glow_z = create_material(_("primeval_glow_z.vmt"), _("VertexLitGeneric"), _(R"#("VertexLitGeneric" {
    "$ignorez"  "1"
	"$additive" "1"
	"$envmap" "models/effects/cube_white"
	"$envmaptint" "[1 1 1]"
	"$envmapfresnel" "1"
	"$envmapfresnelminmaxexp" "[0 1 2]"
	"$alpha" "1"
})#"));

		m_platinum = interfaces::m_material_system->find_material("models/player/ct_fbi/ct_fbi_glass", TEXTURE_GROUP_OTHER);
		m_platinum_z = interfaces::m_material_system->find_material("models/player/ct_fbi/ct_fbi_glass", TEXTURE_GROUP_OTHER);

		m_glass = interfaces::m_material_system->find_material("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER);
		m_glass_z = interfaces::m_material_system->find_material("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER);

		m_crystal = interfaces::m_material_system->find_material("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_OTHER);
		m_crystal_z = interfaces::m_material_system->find_material("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_OTHER);

		m_gold = interfaces::m_material_system->find_material("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER);
		m_gold_z = interfaces::m_material_system->find_material("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER);

		m_dark_chrome = interfaces::m_material_system->find_material("models/gibs/glass/glass", TEXTURE_GROUP_OTHER);
		m_dark_chrome_z = interfaces::m_material_system->find_material("models/gibs/glass/glass", TEXTURE_GROUP_OTHER);

		m_plastic = interfaces::m_material_system->find_material("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
		m_plastic_z = interfaces::m_material_system->find_material("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);

		if (m_platinum_z) m_platinum_z->set_flag(MATERIAL_FLAG_IGNOREZ, true);
		if (m_glass_z) m_glass_z->set_flag(MATERIAL_FLAG_IGNOREZ, true);
		if (m_crystal_z) m_crystal_z->set_flag(MATERIAL_FLAG_IGNOREZ, true);
		if (m_gold_z) m_gold_z->set_flag(MATERIAL_FLAG_IGNOREZ, true);
		if (m_dark_chrome_z) m_dark_chrome_z->set_flag(MATERIAL_FLAG_IGNOREZ, true);
		if (m_plastic_z) m_plastic_z->set_flag(MATERIAL_FLAG_IGNOREZ, true);
	}

	bool on_draw_model(i_model_render* ecx, void* context, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bones);
};
#define chams c_chams::instance()