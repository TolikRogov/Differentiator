#pragma once

#include "Differentiator.hpp"

#define DUMP_DIR_ 		"dump/"
#define IMG_DIR_		"img/"

#define DOT_FILE_ 		DUMP_DIR_ "graph.dot"
#define IMG_FILE_  		IMG_DIR_  "graph"
#define HTML_FILE_ 		DUMP_DIR_ "tree.html"
#define CSS_FILE_		DUMP_DIR_ "styles.css"
#define BASH_FILE_		DUMP_DIR_ "script.bash"

#define IMG_EXTENSION 	"svg"

#define OPEN_HTML_FILE() system(OPEN HTML_FILE_);

#ifdef UNKNOWN_WHAT_IN_TREE
	#define UNKNOWN_WHAT_(...) __VA_ARGS__
#else
	#define UNKNOWN_WHAT_(...) NULL
#endif

BinaryTreeStatusCode BinaryTreeCssFile();
BinaryTreeStatusCode BinaryTreeCreateDumpDir();
BinaryTreeStatusCode BinaryTreeBashScript(Tree* tree, DumpLogInfo* dump_info);
BinaryTreeStatusCode NodeGraphDump(Node_t* cur_root, FILE* dot_file, DumpLogInfo* dump_info);
BinaryTreeStatusCode NameTablePrint();
