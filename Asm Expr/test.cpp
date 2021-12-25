void parse_test() {

    if (2 == 1 || 2 == 2) {
        
    }

    Arena expr_arena;
    Expr* e = parse(&expr_arena,"example.asm");
    std::cout << e;
}