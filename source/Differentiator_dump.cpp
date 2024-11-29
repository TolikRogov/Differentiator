#include "Differentiator_dump.hpp"
#include "GraphUtils.hpp"

static long AMOUNT_OF_LAST_SYMBOLS = 0;

BinaryTreeStatusCode BinaryTreeHtmlDumpStart() {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	tree_status = BinaryTreeCreateDumpDir();
	TREE_ERROR_CHECK(tree_status);

	FILE* html_file = fopen(HTML_FILE_, "w");
	if (!html_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

	tree_status = BinaryTreeCssFile();
	TREE_ERROR_CHECK(tree_status);

#define HTML_PRINTF(...) fprintf(html_file, __VA_ARGS__);

	HTML_PRINTF("<!DOCTYPE HTML PUBLIC>\n");
	HTML_PRINTF("<html lang=\"ru\">\n");

	HTML_PRINTF("\t<head>\n");
	HTML_PRINTF("\t\t<title>Binary Tree Dump</title>\n");
	HTML_PRINTF("\t\t<meta charset=\"utf-8\" />\n");
	HTML_PRINTF("\t\t<link rel=\"stylesheet\" type=\"text/css\" href=\"styles.css\" />\n");
	HTML_PRINTF("\t</head>\n");

	HTML_PRINTF("\t<body><tt><pre>\n");
	HTML_PRINTF("\t\t\t\t\t\t\t\t\t\t\t<a class='head'>MEGA DUMP</a><br>\n");

	HTML_PRINTF("\t<div class='dump'>\n");
	HTML_PRINTF("\tColors meanings: \n");
	HTML_PRINTF("\t\t<div class='circle' style='background-color: %.7s; border-color: %.7s;'>variable node</div>", color.left_node + 1, color.left_node_border + 1);
	HTML_PRINTF("\t\t<span style='color: %.7s'>&#11153; left subtree edge</span>\n", color.left_edge + 1);
	HTML_PRINTF("\t\t<div class='circle' style='background-color: %.7s; border-color: %.7s;'>number node</div>", color.right_node + 1, color.right_node_border + 1);
	HTML_PRINTF("\t\t\t<span style='color: %.7s'>&#11153; right subtree edge</span>\n", color.right_edge + 1);
	HTML_PRINTF("\t\t<div class='circle' style='background-color: %.7s; border-color: %.7s;'>operation node</div>", color.root_node + 1, color.root_node_border + 1);
	HTML_PRINTF("\t\t<span style='color: %.7s'>&#11153; unknown what edge</span>\n", color.unknown_what_edge + 1);
	HTML_PRINTF("\t\t<div class='circle' style='background-color: %.7s; border-color: %.7s;'>pointed node</div>\n", color.new_node + 1, color.new_node_border + 1);
	HTML_PRINTF("\t\t<div class='circle' style='background-color: %.7s; border-color: %.7s;'>unknown what node</div>", color.unknown_what_node + 1, color.unknown_what_node_border + 1);
	HTML_PRINTF("\t</div>\n\n");

#undef HTML_PRINTF

	if (fclose(html_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode NameTablePrint() {

	FILE* html_file = fopen(HTML_FILE_, "a");
	if (!html_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

#define HTML_PRINTF(...) fprintf(html_file, __VA_ARGS__);

	HTML_PRINTF("\t<div class='dump'>\n");
	HTML_PRINTF("\tNameTable:\n\t<table class='name_table'>\n");
	HTML_PRINTF("\t\t<tr>\n\t\t\t<td>Number</td>\n\t\t\t<td>Variable</td>\n\t\t\t<td>Value</td>\n\t\t\t<td>Status</td>\n\t\t\t<td>State</td>\n\t\t</tr>\n");
	for (size_t i = 0; i < AMOUNT_OF_VARIABLES; i++) {
		HTML_PRINTF("\t\t<tr>\n\t\t\t<td>%d</td>\n\t\t\t<td>%s</td>\n\t\t\t<td>%lg</td>\n\t\t\t<td>%s</td>\n\t\t\t<td>%s</td>\n\t\t</tr>\n",
					var_name_table[i].num, var_name_table[i].symbol, var_name_table[i].value,
					(var_name_table[i].status == VAR_STATUS_USING ? RET_STRING(VAR_STATUS_USING) : RET_STRING(VAR_STATUS_DISUSING)),
					(var_name_table[i].state == VAR_DIFF_STATUS_NUM ? RET_STRING(VAR_DIFF_STATUS_NUM) : RET_STRING(VAR_DIFF_STATUS_VAR)));
	}
	HTML_PRINTF("\t</table>\n");
	HTML_PRINTF("\t<table class='name_table'>\n");
	HTML_PRINTF("\t\t<tr>\n\t\t\t<td>Number</td>\n\t\t\t<td>Operation</td>\n\t\t\t<td>LaTexSymbol</td>\n\t\t</tr>\n");
	for (size_t i = 0; i < AMOUNT_OF_OPERATIONS; i++) {
		HTML_PRINTF("\t\t<tr>\n\t\t\t<td>%d</td>\n\t\t\t<td>%s</td>\n\t\t\t<td>%s</td>\n\t\t</tr>\n",
					op_name_table[i].num, op_name_table[i].math_symbol, op_name_table[i].tex_symbol);
	}
	HTML_PRINTF("\t</table></div>\n\n");

#undef HTML_PRINTF

	if (fclose(html_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode BinaryTreeCssFile() {

	FILE* css_file = fopen(CSS_FILE_, "w");
	if (!css_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

#define CSS_PRINTF(...) fprintf(css_file, __VA_ARGS__);

	CSS_PRINTF("body {\n"\
			   "\tbackground-color: %.7s;\n"\
			   "}\n", color.html_background + 1);

	CSS_PRINTF("h1 {\n"\
			   "\ttext-align: center;\n"\
			   "}\n");

	CSS_PRINTF("b {\n"\
			   "\ttext-decoration: underline;\n"\
			   "}\n");

	CSS_PRINTF(".circle {\n"\
		"\tdisplay: inline-block;\n"\
		"\tborder-radius: 50%%;\n"\
		"\tborder: 1px solid;\n"\
		"\tpadding: 10px;\n"\
		"}\n");

	CSS_PRINTF(".head {\n"\
			   "\tfont-size: 2em;\n"\
    		   "\tfont-weight: bold;\n"\
			   "}\n");

	CSS_PRINTF(".addr {\n"\
			   "\tcolor: %.7s;\n"\
			   "}\n", color.address + 1);

	CSS_PRINTF(".path {\n"\
			   "\tcolor: %.7s;\n"\
			   "}\n", color.path + 1);

	CSS_PRINTF(".line {\n"\
			   "\tcolor: %.7s;\n"\
			   "}\n", color.line + 1);

	CSS_PRINTF(".name {\n"\
			   "\tcolor: %.7s;\n"\
			   "}\n", color.name + 1);

	CSS_PRINTF(".dump {\n"\
			   "\tbackground-color: %.7s;\n"\
			   "\tborder: 1px solid %.7s;\n"\
			   "\tborder-radius: 20px;\n"\
			   "\tpadding: 25px 0px;\n"\
			   "\tmargin: 20px 0px;\n"\
			   "\twidth: 90%%;"\
			   "\tdisplay: inline-block;\n"\
			   "}\n", color.dump_background + 1, color.dump_border + 1);

	CSS_PRINTF(".num {\n"\
			   "\tcolor: %.7s;\n"\
			   "}\n", color.number + 1);

	CSS_PRINTF(".func {\n"\
			   "\tcolor: %.7s;"\
			   "}\n", color.function + 1);

	CSS_PRINTF(".img {\n"\
			   "\twidth: 90%%;\n"\
			   "\theight: 70%%;\n"\
			   "\tobject-fit: contain;\n"\
			   "\tpadding: 10px 0px 0px 0px;\n"\
			   "}\n");

	CSS_PRINTF(".name_table {\n"\
			"\twidth: 90%%;\n"\
			"\tborder: 15px solid %.7s;\n"\
			"\tborder-collapse: collapse;\n"\
			"\tmargin: auto;\n"\
			"\ttable-layout: auto;\n"\
			"\ttext-align: center;\n"\
			"\tmargin-bottom: 20px;\n"\
			"\tborder-top: 5px solid %.7s;\n"\
			"}\n", color.name_table_background + 1, color.name_table_background + 1);

	CSS_PRINTF(".name_table th {\n"\
			   "\tfont-weight: bold;\n"\
			   "\tpadding: 5px;\n"\
			   "\tbackground: %.7s;\n"\
			   "\tborder: none;\n"\
			   "\tborder-bottom: 5px solid %.7s;\n"\
			   "}\n", color.name_table_background + 1, color.name_table_background + 1);

	CSS_PRINTF(".name_table td {\n"\
			   "\tpadding: 10px;\n"\
			   "\tborder: none;\n"\
			   "\tborder-bottom: 5px solid %.7s;\n"\
			   "}\n", color.name_table_background + 1);

	CSS_PRINTF(".name_table tbody tr:nth-child(odd) {\n"\
			   "\tbackground: %.7s;\n"\
			   "}\n", color.name_table_odd_tr + 1);

	CSS_PRINTF(".name_table tbody tr:nth-child(even) {\n"\
			   "\tbackground: %.7s;\n"\
			   "}\n", color.name_table_even_tr + 1);

#undef CSS_PRINTF

	if (fclose(css_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode BinaryTreeCreateDumpDir() {

	DIR* dump_dir = opendir(DUMP_DIR_);
	if (dump_dir) {
		if (closedir(dump_dir))
			TREE_ERROR_CHECK(TREE_DIR_CLOSE_ERROR);
		system("rm -rf " DUMP_DIR_);
	}
	system("mkdir " DUMP_DIR_);

	DIR* img_dir = opendir(DUMP_DIR_ IMG_DIR_);
	if (!img_dir)
		system("mkdir " DUMP_DIR_ IMG_DIR_);
	else {
		if (closedir(img_dir))
			TREE_ERROR_CHECK(TREE_DIR_CLOSE_ERROR);
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode BinaryTreeHtmlDumpFinish() {

	FILE* html_file = fopen(HTML_FILE_, "a");
	if (!html_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

	AMOUNT_OF_LAST_SYMBOLS = 0;

#define HTML_PRINTF(...) AMOUNT_OF_LAST_SYMBOLS += fprintf(html_file, __VA_ARGS__);

	HTML_PRINTF("\t</pre></tt></body>\n");
	HTML_PRINTF("</html>\n");

#undef HTML_PRINTF

	if (fclose(html_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode BinaryTreeBashScript(Tree* tree, DumpLogInfo* dump_info) {

	static size_t script_num = 1;
	static struct PrevLogInfo {
		const char* file_name;
		size_t line;
		const char* var_name;
	} prev_info;

	FILE* bash_script = fopen(BASH_FILE_, "w");
	if (!bash_script)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

#define BASH_PRINTF(...) fprintf(bash_script, __VA_ARGS__);

	BASH_PRINTF("#!/bin/bash\n");

	BASH_PRINTF("script_num=%zu;\\\n", script_num);
	BASH_PRINTF("dot " DOT_FILE_ " -T" IMG_EXTENSION " -o " DUMP_DIR_ IMG_FILE_ "$script_num" "." IMG_EXTENSION " || read huishe;\n");

#undef BASH_PRINTF

	if (fclose(bash_script))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	system("chmod +x " BASH_FILE_ "; ./" BASH_FILE_);

	FILE* html_file = fopen(HTML_FILE_, "r+");
	if (!html_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

	fseek(html_file, 0, SEEK_END);

#define HTML_PRINTF(...) fprintf(html_file, __VA_ARGS__);

	if (AMOUNT_OF_LAST_SYMBOLS)
		fseek(html_file, -AMOUNT_OF_LAST_SYMBOLS, SEEK_END);

	if (StrCmp(prev_info.file_name, tree->info.file_name) != 0 || prev_info.line != tree->info.line || StrCmp(prev_info.var_name, tree->info.name) != 0)
		HTML_PRINTF("\t\t\t\t\t\t<b>Binary Tree[%p] born at \"%s\": %zu, name '%s'</b>\n", \
					tree, (prev_info.file_name = tree->info.file_name), (prev_info.line = (size_t)tree->info.line), (prev_info.var_name = tree->info.name));

	HTML_PRINTF("\t<div class='dump'>\n");
	HTML_PRINTF("\tDUMP #%zu: function %s was called from %s: %zu\n", script_num, dump_info->func, dump_info->file, dump_info->line);
	HTML_PRINTF("\t<img src='%s%zu.%s' class='img'></div>\n\n", IMG_FILE_, script_num++, IMG_EXTENSION);

#undef HTML_PRINTF

	if (fclose(html_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode BinaryTreeGraphDump(Tree* tree, DumpLogInfo dump_info) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	FILE* dot_file = fopen(DOT_FILE_, "w");
	if (!dot_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

#define DOT_PRINTF(...) fprintf(dot_file, __VA_ARGS__);

	DOT_PRINTF("digraph {\n");

	DOT_PRINTF("\tfontname = \"UbuntuMono\";\n");
	DOT_PRINTF("\tbgcolor = %s;\n\n", color.dot_background);

	NodeGraphDump(tree->root, dot_file, &dump_info);

	DOT_PRINTF("}\n");

#undef DOT_PRINTF

	if (fclose(dot_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	tree_status = BinaryTreeBashScript(tree, &dump_info);
	TREE_ERROR_CHECK(tree_status);

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode NodeGraphDump(Node_t* cur_root, FILE* dot_file, DumpLogInfo* dump_info) {

	if (!cur_root)
		return TREE_NULL_POINTER;

#define DOT_PRINTF(...) fprintf(dot_file, __VA_ARGS__);

	DOT_PRINTF("\tnode%p [ shape = Mrecord; style = filled; ", cur_root);

#define NODE_COLOR_PRINT(...) DOT_PRINTF("fillcolor = %s; color = %s; fontcolor = %s; label = \" { ", __VA_ARGS__);

	switch (cur_root->type) {
		case UNW: { NODE_COLOR_PRINT(color.unknown_what_node, color.unknown_what_node_border, color.node_font); break; }
		case NUM: { NODE_COLOR_PRINT(color.number_node, 	  color.number_node_border,    	  color.node_font); break; }
		case VAR: { NODE_COLOR_PRINT(color.variable_node,  	  color.variable_node_border,  	  color.node_font); break; }
		case OP:  { NODE_COLOR_PRINT(color.operation_node, 	  color.operation_node_border, 	  color.node_font); break; }
		default: return TREE_INVALID_TYPE;
	}

#define SPECIFIER_LABEL_PRINT(node)											 	 			 				 \
	do {																									\
		if (!(node)) { DOT_PRINTF("%s\\n%p ", "NONE", node); continue; } 									\
		switch (node->type) {																				\
			case UNW: { DOT_PRINTF("%s\\n%p ", "UNKNOWN WHAT", node); break; } 								\
			case NUM: { DOT_PRINTF("%lg\\n%p ", node->data.val_num, node); break; }							\
			case VAR: { DOT_PRINTF("%s\\n%p ", VarNameTableGetSymbol(node->data.val_var), node); break; }	\
			case OP:  { DOT_PRINTF("%s\\n%p ", OpNameTableGetMathSymbol(node->data.val_op), node); break; } \
			default: return TREE_INVALID_TYPE;																\
		}																									\
	} while(0)

	SPECIFIER_LABEL_PRINT(cur_root);
	DOT_PRINTF("| { <left> Left\\n");
	SPECIFIER_LABEL_PRINT(cur_root->left);
	DOT_PRINTF("| Parent\\n");
	SPECIFIER_LABEL_PRINT(cur_root->parent);
	DOT_PRINTF("| <right> Right\\n");
	SPECIFIER_LABEL_PRINT(cur_root->right);
	DOT_PRINTF("} } \"; ];\n");

	if (cur_root->left) {
		if (!IsRootUnknownWhat(cur_root->left)) {
			DOT_PRINTF("\tnode%p:<left> -> node%p [ style = \"bold\"; color = %s; label = %s; ];\n", cur_root, cur_root->left, color.left_edge, labels.left_sub_arrow);
		}
		NodeGraphDump(cur_root->left, dot_file, dump_info);
	}
	if (cur_root->right) {
		NodeGraphDump(cur_root->right, dot_file, dump_info);
		if (!IsRootUnknownWhat(cur_root->right))
			DOT_PRINTF("\tnode%p:<right> -> node%p [ style = \"bold\"; color = %s; label = %s; ];\n", cur_root, cur_root->right, color.right_edge, labels.right_sub_arrow);
	}

	if (dump_info->pointer)
		DOT_PRINTF("\tnode%p [ fillcolor = %s; color = %s; fontcolor = %s; ];\n", dump_info->pointer, color.new_node, color.new_node_border, color.node_font);

	if (IsRootUnknownWhat(cur_root)) {
		if (cur_root->parent && cur_root->parent->right == cur_root) {
			DOT_PRINTF("\tnode%p:<right> -> node%p [ style = \"bold\"; color = %s; label = %s; ];\n",
						cur_root->parent, cur_root, color.unknown_what_edge, labels.unknown_what_arrow);
		}
		else if (cur_root->parent && cur_root->parent->left == cur_root){
			DOT_PRINTF("\tnode%p:<left> -> node%p [ style = \"bold\"; color = %s; label = %s; ];\n",
						cur_root->parent, cur_root, color.unknown_what_edge, labels.unknown_what_arrow);
		}
	}

#undef NODE_COLOR_PRINT
#undef SPECIFIER_LABEL_PRINT
#undef DOT_PRINTF

	return TREE_NO_ERROR;
}
