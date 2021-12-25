enum class EExprPriority : uint32_t {
    OR = 0,
    AND,
    CMP,
    ADD,
    MUL,
    UNARY,
    VALUE,
};

enum class EExprOperator : uint8_t{
    NONE = 0,
    LOGIC_OR,
    LOGIC_AND,
    LESS,
    MORE,
    EQ,
    NOTEQ,
    LTEQ,
    GTEQ,
    ADD,
    BIN_OR,
    SUB,
    XOR,
    MUL,
    MOD,
    DIV,
    LSH,
    RSH,
    LOGIC_NOT,
    BIN_NOT,
};

const char* expr_operator_strings[] = {
    "",
    "||",
    "&&",
    "<",
    ">",
    "==",
    "!=",
    "<=",
    ">=",
    "+",
    "|",
    "-",
    "^",
    "*",
    "%",
    "/",
    "<<",
    ">>",
    "!",
    "~",
};

inline std::ostream& operator<< (std::ostream& stream, const EExprOperator op) {
    stream << expr_operator_strings[static_cast<size_t>(op)];
    return stream;
}

class Expr {
    friend class ExprList;
    Expr* _next;
protected:
    virtual void print(std::ostream& o) const = 0;
public:
    Expr() {
        _next = 0;
    }
    inline Expr* next() {
        return _next;
    }
    virtual EExprPriority priority() const = 0;
    friend std::ostream& operator << (std::ostream& o, const Expr* b);
    virtual ~Expr() {};
};

inline std::ostream& operator<< (std::ostream& o, const Expr* b) {
    b->print(o);
    return o;
}

class ValueExpr : public Expr {
    int val;
    friend class ExprArena;
protected:
    virtual void print(std::ostream& o) const {
        o << val;
    }
public:
    ValueExpr() : Expr() {}
    ValueExpr(int val) : Expr() {
        this->val = val;
    }
    virtual EExprPriority priority() const { return EExprPriority::VALUE; }
    void operator=(int val) {
        this->val = val;
    }
    virtual ~ValueExpr() {}
};

class UnaryExpr : public Expr {
    EExprOperator op;
    Expr* exprSignOrExprInt;
    friend class ExprArena;
protected:
    virtual void print(std::ostream& o) const {
        if (static_cast<uint8_t>(op)) 
            o << op << " " << exprSignOrExprInt;
        else 
            o << " " << exprSignOrExprInt;
    }
public:
    UnaryExpr(EExprOperator op,Expr* expr) : Expr() {
        this->op = op;
        this->exprSignOrExprInt = expr;
    }
    UnaryExpr(Expr* expr) : Expr() {
        this->op = EExprOperator::NONE;
        this->exprSignOrExprInt = expr;
    }
    virtual EExprPriority priority() const { return EExprPriority::UNARY; }
    virtual ~UnaryExpr() {}
};

class ExprList : public Expr {
    EExprOperator _op;
    Expr* head;
    Expr* tail;
protected:
    virtual void print(std::ostream& o) const {
        Expr* e = ExprList::begin();
        if (this->priority() > e->priority())
            std::cout << '(' << e << ')';
        else
            std::cout << e;

        e = e->next();
        for (; e != ExprList::end(); e = e->next()) {
            if (this->priority() > e->priority())
                std::cout << _op << '(' << e << ')';
            else
                std::cout << _op << e;
        }
    }
private:
    inline bool is_empty() { return (head == nullptr) && (tail == nullptr); }
public:
    ExprList(EExprOperator op,std::initializer_list<Expr*> exprs) : Expr() {
        this->_op = op;
        this->head = this->tail = 0;
        for (auto expr : exprs) {
            this->push(expr);
        }
    }
    virtual ~ExprList() {};
public:
    virtual EExprPriority priority() const { 
        switch (_op){
        case EExprOperator::NONE:
            assert(0);
            return EExprPriority::VALUE;
        case EExprOperator::LOGIC_OR:
            return EExprPriority::OR;
        case EExprOperator::LOGIC_AND:
            return EExprPriority::AND;
        case EExprOperator::LESS:
        case EExprOperator::MORE:
        case EExprOperator::EQ:
        case EExprOperator::NOTEQ:
        case EExprOperator::LTEQ:
        case EExprOperator::GTEQ:
            return EExprPriority::CMP;
        case EExprOperator::ADD:
        case EExprOperator::BIN_OR:
        case EExprOperator::SUB:
        case EExprOperator::XOR:
            return EExprPriority::ADD;
        case EExprOperator::MUL:
        case EExprOperator::MOD:
        case EExprOperator::DIV:
        case EExprOperator::LSH:
        case EExprOperator::RSH:
            return EExprPriority::MUL;
        case EExprOperator::LOGIC_NOT:
        case EExprOperator::BIN_NOT:
            return EExprPriority::UNARY;
        default:
            assert(0);
            return EExprPriority::VALUE;
            break;
        }
    }
    inline EExprOperator op() {
        return this->_op;
    }
    inline Expr* begin() const {return this->head;}
    inline Expr* end() const {return this->tail->_next;}
    void push(Expr* node) {
        if (this->is_empty()) {
            this->head = this->tail = node;
        }
        this->tail->_next = node;
        this->tail = node;
    }

};