#pragma once

static struct Colors {
	const char* dot_background 						= "\"#DCDCDC\"";
	const char* html_background 					= "\"#FFFAFA\"";
	const char* address 							= "\"#4B0082\"";
	const char* path 								= "\"#FFA500\"";
	const char* line 								= "\"#20B2AA\"";
	const char* name 								= "\"#7B68EE\"";
	const char* dump_background 					= "\"#F7F7F7\"";
	const char* dump_border							= "\"#C6C1C1\"";
	const char* number 								= "\"#BDB76B\"";
	const char* function 							= "\"#8B008B\"";
	const char* left_node 							= "\"#D5CFE1\"";
	const char* left_node_border					= "\"#8797b2\"";
	const char* node_font 							= "\"#000000\"";
	const char* right_node 							= "\"#BDEF9E\"";
	const char* right_node_border					= "\"#99c2a2\"";
	const char* root_node 							= "\"#FBEE21\"";
	const char* root_node_border					= "\"#D3C70D\"";
	const char* left_edge 							= "\"#187AE8\"";
	const char* right_edge							= "\"#48E818\"";
	const char* new_node 							= "\"#F18805\"";
	const char* new_node_border						= "\"#AF3800\"";
	const char* unknown_what_node					= "\"#BD3458\"";
	const char* unknown_what_node_border			= "\"#7b0d1e\"";
	const char* unknown_what_edge 					= "\"#3d1308\"";
	const char* operation_node 						= "\"#FBEE21\"";
	const char* operation_node_border				= "\"#D3C70D\"";
	const char* number_node 						= "\"#BDEF9E\"";
	const char* number_node_border					= "\"#99c2a2\"";
	const char* variable_node 						= "\"#D5CFE1\"";
	const char* variable_node_border				= "\"#8797b2\"";
} color;

static struct Labels {
	const char* left_sub_arrow 		= "\"\"";
	const char* right_sub_arrow		= "\"\"";
	const char* unknown_what_arrow  = "\"Если б мы знали, что это такое.\\nМы не знаем, что это такое.\"";
} labels;
