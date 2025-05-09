#include "hooks.h"

namespace hooks {
	void init() {
		// // // // // // // // // // // // // // // // // // // // // // //

		m_key_values = std::make_unique<memory::hook_t>(interfaces::m_key_values_system);

		m_key_values->hook(key_values_system::alloc_key_values_memory::index, key_values_system::alloc_key_values_memory::fn);

		// // // // // // // // // // // // // // // // // // // // // // //

		m_d3d_device = std::make_unique<memory::hook_t>(interfaces::m_d3d_device);

		m_d3d_device->hook(d3d_device::reset::index, d3d_device::reset::fn);
		m_d3d_device->hook(d3d_device::present::index, d3d_device::present::fn);

		// // // // // // // // // // // // // // // // // // // // // // //

		m_client_dll = std::make_unique<memory::hook_t>(interfaces::m_client_dll);

		m_client_dll->hook(client_dll::frame_stage_notify::index, client_dll::frame_stage_notify::fn);
		m_client_dll->hook(client_dll::create_move::index, client_dll::create_move::gate);

		// // // // // // // // // // // // // // // // // // // // // // //

		m_client_mode = std::make_unique<memory::hook_t>(interfaces::m_client_mode);

		m_client_mode->hook(client_mode::override_view::index, client_mode::override_view::fn);
		m_client_mode->hook(client_mode::do_post_screen_space_effects::index, client_mode::do_post_screen_space_effects::fn);

		// // // // // // // // // // // // // // // // // // // // // // //

		m_model_render = std::make_unique<memory::hook_t>(interfaces::m_model_render);

		m_model_render->hook(model_render::draw_model_execute::index, model_render::draw_model_execute::fn);

		// // // // // // // // // // // // // // // // // // // // // // //

		m_panel = std::make_unique<memory::hook_t>(interfaces::m_panel);

		m_panel->hook(panel::paint_traverse::index, panel::paint_traverse::fn);

		// // // // // // // // // // // // // // // // // // // // // // //

		m_surface = std::make_unique<memory::hook_t>(interfaces::m_surface);

		m_surface->hook(surface::lock_cursor::index, surface::lock_cursor::fn);

		// // // // // // // // // // // // // // // // // // // // // // //

		m_player = std::make_unique<memory::hook_t>(c_cs_player::get_vtable());

		m_player->hook(player::eye_angles::index, player::eye_angles::fn);

		// // // // // // // // // // // // // // // // // // // // // // //

		m_renderable = std::make_unique<memory::hook_t>(i_client_renderable::get_vtable());

		m_renderable->hook(renderable::setup_bones::index, renderable::setup_bones::fn);

		// // // // // // // // // // // // // // // // // // // // // // //
	}

	void undo() {
		m_renderable->unhook();
		m_player->unhook();	
		m_surface->unhook();
		m_panel->unhook();
		m_model_render->unhook();
		m_client_mode->unhook();
		m_client_dll->unhook();
		m_d3d_device->unhook();
		m_key_values->unhook();
	}

	std::unique_ptr<memory::hook_t> m_d3d_device = nullptr;
	std::unique_ptr<memory::hook_t> m_client_dll = nullptr;
	std::unique_ptr<memory::hook_t> m_client_mode = nullptr;
	std::unique_ptr<memory::hook_t> m_model_render = nullptr;
	std::unique_ptr<memory::hook_t> m_panel = nullptr;
	std::unique_ptr<memory::hook_t> m_surface = nullptr;
	std::unique_ptr<memory::hook_t> m_player = nullptr;
	std::unique_ptr<memory::hook_t> m_renderable = nullptr;
	std::unique_ptr<memory::hook_t> m_key_values = nullptr;
}
