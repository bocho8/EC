# EC
open-source EC


bhop only works in usermode and efi at the moment and to use bhop just rebind your jump key in settings (you have to do it while your alive and on the ground)

	//
	// default global settings
	//
	//standlone rcs is still weird but better than default, improve it or don't use it
 	//config::triggerbot_smooth = 1; 
	//config::triggerbot_fov = 1; 
	config::triggerbot_multibone = 0; 
	config::standalone_rcs = 0; //probably dont use
	config::aimbot_enabled = 1;
	config::aimbot_multibone = 1; 
	config::spotted_esp = 0; //visible only esp
	config::aimbot_visible_check = 1; //aimbot visible check default on
	config::triggerbot_visible_check = 0;
	config::bhop = 0;
	config::trigger_aim = 1;
	config::aimbot_button = 317; mouse1
	config::triggerbot_button = 320; mouse5
	config::incrosstriggerbot_button = 82; //left alt
	config::aimbot_fov = 1.5f;
	config::aimbot_smooth = 3.0f;
	config::visuals_enabled = 1;
	config::visualize_hitbox = 0; //show aim location

 
	cl_crosshairalpha 200:
		config::visuals_enabled = 0;
		config::aimbot_enabled = 0;
		break;
	cl_crosshairalpha 201:
		config::aimbot_fov = 1.5;
		config::aimbot_smooth = 3;
		config::spotted_esp = 1;
		config::aimbot_enabled = 1;
		config::triggerbot_visible_check = 1;
		config::aimbot_visible_check = 1;
		break;
	cl_crosshairalpha 202:
		config::aimbot_fov = 1.5;
		config::aimbot_smooth = 3;
		break;
	cl_crosshairalpha 203:
		config::aimbot_fov = 2;
		config::aimbot_smooth = 2;
		break;
	cl_crosshairalpha 204:
		config::aimbot_fov = 4;
		config::aimbot_smooth = 2;
		break;
	cl_crosshairalpha 205:
		config::visuals_enabled = 1;
		config::aimbot_enabled = 0;
		break;
	cl_crosshairalpha 206:
		config::spotted_esp = 1;
		config::triggerbot_visible_check = 1;
		config::aimbot_enabled = 0;
		break;
	cl_crosshairalpha 207:
		config::aimbot_multibone = 0;
		config::aimbot_fov = 45;
		config::aimbot_smooth = 1.5;
		config::visualize_hitbox = 1;
		break;
	cl_crosshairalpha 208:
		config::aimbot_fov = 2;
		config::aimbot_smooth = 4;
		config::triggerbot_fov = .75;
		break;
	cl_crosshairalpha 209:
		config::aimbot_fov = 2;
		config::aimbot_smooth = 4;
		config::triggerbot_fov = .75;
		config::triggerbot_multibone = 1;
		break;
	cl_crosshairalpha 210:
		config::aimbot_fov = 1;
		config::aimbot_smooth = 2;
		break;
	
