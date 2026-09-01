#include <fmt/format.h>
#include <compiler/token.hpp>
#include <compiler/fileread.hpp>
#include <compiler/ast.hpp>
#include <compiler/lexer.hpp>
#include <string>
#include <iostream>

int main() {
	// 循环
	while (true) {
		for (auto file : FileRead::get_file_list()) {
			std::cout << file << std::endl;      // TODO : use fmt::print
		}
		int n;
		std::cin >> n;
		std::string file_content = FileRead::get_file_content(FileRead::get_file_list()[n]);
		std::cout << "文件内容：" << std::endl << file_content << std::endl << std::endl;

		Lexer lexer = Lexer(file_content);
		lexer.letex_analyzer();
		AstGenerator ast_tree = AstGenerator(lexer.tokens);
		Node* root = ast_tree.geneAstTree();
		display(root);

		std::cout << "If Go on" << std::endl;
		std::cin >> n;
		if (n == 1) {
			break;
		}
	}
}
