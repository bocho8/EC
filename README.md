# EC
open-source EC


bhop only works in usermode and efi at the moment and to use bhop just rebind your jump key in settings (you have to do it while your alive and on the ground)

	switch (crosshair_alpha)
	{
	case 200:
		config::visuals_enabled = 0;
		config::aimbot_enabled = 0;
		break;
	case 201:
		config::spotted_esp = 1;
		config::aimbot_enabled = 1;
		config::triggerbot_visible_check = 1;
		config::aimbot_visible_check = 1;
		break;
	case 202:
		config::visuals_enabled = 1;
		config::aimbot_enabled = 0;
		break;
	case 203:
		config::spotted_esp = 1;
		config::aimbot_enabled = 0;
		break;




	default:
		config::spotted_esp = 0;
		config::aimbot_visible_check = 1;
		config::triggerbot_visible_check = 0;
		config::bhop = 0;
		config::trigger_aim	  = 1;
		config::aimbot_button     = 317;
		config::triggerbot_button = 320;
		config::incrosstriggerbot_button = 82;
		config::aimbot_fov        = 2.0f;
		config::aimbot_smooth     = 2.0f;
		config::visuals_enabled   = 1;
		config::visualize_hitbox  = 0;
		break;
	}
