#include "../features.h"

void c_thirdperson::EnterThirdPerson()
{

	if (!globals::config::ThirdPersonKey.Toggled || !globals::m_local->is_alive()) {
		ExitThirdPerson();
		return;
	}

	if (!interfaces::m_input->m_camera_in_third_person)
	{

		interfaces::m_input_system->enable_input(true);

		interfaces::m_input->m_camera_in_third_person = true;

		qangle_t angles;
		interfaces::m_engine->get_view_angles(angles);
		OriginalDistance = angles.z;

		interfaces::m_input->m_camera_offset = vec3_t(angles.x, angles.y, angles.z);
	}

	float* distance = &globals::config::thirdPersonDistance;
	c_cvar* cam_idealdist = interfaces::m_cvar_system->find_var(FNV1A_RT("cam_idealdist"));

	if (cam_idealdist->get_float() != *distance)
	{
		cam_idealdist->set_value(*distance);
	}
}

void c_thirdperson::ExitThirdPerson()
{
	if (!globals::m_local->is_alive())
	{
		if (interfaces::m_input->m_camera_in_third_person)
		{
			qangle_t angles;
			interfaces::m_engine->get_view_angles(angles);
			interfaces::m_input->m_camera_offset = vec3_t(angles.x, angles.y, OriginalDistance);
			interfaces::m_input->m_camera_in_third_person = false;
		}
	}

	if (!interfaces::m_input->m_camera_in_third_person)
		return;

	qangle_t angles;
	interfaces::m_engine->get_view_angles(angles);
	interfaces::m_input->m_camera_offset = vec3_t(angles.x, angles.y, OriginalDistance);
	interfaces::m_input->m_camera_in_third_person = false;
}
