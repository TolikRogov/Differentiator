#include "Differentiator_reader.hpp"

BinaryTreeStatusCode ReadFromFileToBuffer() {

	FILE* exp_file = fopen(DIFF_EXPRESSION_FILE_, "r");
	if (!exp_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

	struct stat file_info = {};
	stat(DIFF_EXPRESSION_FILE_, &file_info);

	size_t size = (size_t)file_info.st_size;
	char* buffer = (char*)calloc(size, sizeof(char));
	if (!buffer)
		TREE_ERROR_CHECK(TREE_ALLOC_ERROR);

	size_t read_check = fread(buffer, size, sizeof(char), exp_file);
	if (!read_check)
		TREE_ERROR_CHECK(TREE_READ_ERROR);

	for (size_t i = 0; i < size; i++) {
		printf("%c", buffer[i]);
	}

	if (fclose(exp_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	RecursionReadExpression(&buffer);

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode RecursionReadExpression(char** buffer) {

	static int brackets = 0;
	char word[6] = {};
	static int cnt = 0;

	if (*(*buffer + cnt) == '\n')
		return TREE_NO_ERROR;

	sscanf(*buffer, "%s", word);
	getchar();

	printf("%s\n", word);

	RecursionReadExpression(buffer);

	return TREE_NO_ERROR;
}
