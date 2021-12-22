Expr* register_expresions[reg_count+1];
std::stack<Expr*>* stack;
Lex* lex;

void parse_cmd(Arena* arena) {
#define NEW_EXPR(T)\
if (!register_expresions[reg_kind_left]) { lex->fatal("Register is empty"); }\
if (typeid(*register_expresions[reg_kind_left]) == typeid(T)) {\
	dynamic_cast<T*>(register_expresions[reg_kind_left])->push(right);\
}\
else {\
	temp = register_expresions[reg_kind_left];\
	register_expresions[reg_kind_left] = new(arena) T({ temp, right });\
}

	CmdKind cmd_kind = lex->cmd_kind();
	RegKind reg_kind_left;
	RegKind reg_kind_right;
	Expr* right = NULL;
	Expr* temp;
	lex->next_token();
	reg_kind_left = lex->reg_kind();
	lex->expect_token(TOKEN_REG);
	if (lex->is_token(TOKEN_COMMA)) {
		lex->next_token();

		if (lex->is_token(TOKEN_NUMBER)) {
			right = new(arena) Value(lex->number());
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
			register_expresions[reg_kind_left] = right;
			break;
		case CMD_ADD:
			NEW_EXPR(Add);
			break;
		case CMD_SUB:
			NEW_EXPR(Sub);
			break;
		case CMD_MUL:
			NEW_EXPR(Mul);
			break;
		case CMD_DIV:
			NEW_EXPR(Div);
			break;
		default:
			break;
		}
	}
	else {
		reg_kind_right = lex->reg_kind();
		right = new (arena) Value(1);
		switch (cmd_kind) {
		case CMD_INC:
			NEW_EXPR(Add);
			break;
		case CMD_DEC:
			NEW_EXPR(Sub);
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