void parse_test() {
    Arena expr_arena;
    Expr* e = parse(&expr_arena,"example.asm");
    std::cout << e;
}