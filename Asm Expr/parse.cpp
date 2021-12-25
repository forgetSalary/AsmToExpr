Expr* register_expresions[reg_count+1];
Expr* last_parsed;
std::stack<Expr*>* stack;
Lex* lex;

void parse_expr(Expr*& left, Expr* right,Arena* arena,EExprOperator _op) {
	if (!left) { lex->fatal("Register is empty"); }
	Expr* temp;
	if (typeid(left) == typeid(ExprList)) {
		if (dynamic_cast<ExprList*>(left)->op() == _op) {
			dynamic_cast<ExprList*>(left)->push(right);
			return;
		}
	}
	temp = left;
	left = new(arena) ExprList(_op, { temp, right });
}

void parse_cmd(Arena* arena) {
	CmdKind cmd_kind = lex->cmd_kind();
	lex->next_token();
	RegKind reg_kind_left = lex->reg_kind();
	lex->expect_token(TOKEN_REG);
	Expr*& left = register_expresions[reg_kind_left];

	RegKind reg_kind_right;
	Expr* right = NULL;
	if (lex->is_token(TOKEN_COMMA)) {
		lex->next_token();

		if (lex->is_token(TOKEN_NUMBER)) {
			right = new(arena) ValueExpr(lex->number());
		}
		else if (lex->is_token(TOKEN_REG)) {
			reg_kind_right = lex->reg_kind();
			right = register_expresions[reg_kind_right];
			if (!right) {
				lex->fatal("Register is empty");
			}
		}else{
			//fatal
		}
		switch (cmd_kind) {
		case CMD_MOV:
			left = right;
			break;
		case CMD_ADD:
			parse_expr(left, right,arena,EExprOperator::ADD);
			break;
		case CMD_SUB:
			parse_expr(left, right, arena, EExprOperator::SUB);
			break;
		case CMD_MUL:
			parse_expr(left, right, arena, EExprOperator::MUL);
			break;
		case CMD_DIV:
			parse_expr(left, right, arena, EExprOperator::DIV);
			break;
		default:
			break;
		}
	}
	else {
		reg_kind_right = lex->reg_kind();
		right = new (arena) ValueExpr(1);
		switch (cmd_kind) {
		case CMD_INC:
			parse_expr(left, right, arena, EExprOperator::ADD);
			break;
		case CMD_DEC:
			parse_expr(left, right, arena, EExprOperator::SUB);
			break;
		/*case CMD_PUSH:
			stack.push(register_expresions[reg_kind_left]);
			break;
		case CMD_POP:
			register_expresions[reg_kind_left] = stack.top();
			stack.pop();
			break;*/
		default:
			break;
		}
	}
	last_parsed = right;
#undef NEW_EXPR
}

Expr* parse(Arena* arena,const char* path) {
	lex = new Lex;
	Expr* e;
	char* src = read_file(path);
	if (!src) {
		return NULL;
	}
	lex->init_stream(src);
	memset(register_expresions,0,sizeof(Expr*)*reg_count);
	while (!lex->is_token(TOKEN_EOF)) {
		while (!lex->is_token(TOKEN_CMD)) {
			lex->next_token();
		}
		if (lex->cmd_kind() == CMD_OUT) {
			break;
		}
		parse_cmd(arena);
		while (!lex->is_token(TOKEN_NEW_LINE)) {
			lex->next_token();
		}
		lex->next_token();
	}
	if (lex->is_token(TOKEN_EOF)) {
		e = register_expresions[REG_EAX];
	}else{
		lex->next_token();
		e = register_expresions[lex->reg_kind()];
		lex->expect_token(TOKEN_REG);
	}
	delete lex;
	free(src);
	return e;
}