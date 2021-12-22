
class Expr {
    friend class ExprArena;
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
    friend std::ostream& operator << (std::ostream& o, const Expr* b);
    virtual ~Expr() {};
};

inline std::ostream& operator<< (std::ostream& o, const Expr* b) {
    b->print(o);
    return o;
}

class Value : public Expr {
    int val;
    friend class ExprArena;
protected:
    virtual void print(std::ostream& o) const {
        o << val;
    }
public:
    Value() : Expr() {}
    Value(int val) : Expr() {
        this->val = val;
    }
    void operator=(int val) {
        this->val = val;
    }
    virtual ~Value() {}
};

class Sign : public Expr {
    char sign;
    Expr* exprSignOrExprInt;
    friend class ExprArena;
protected:
    virtual void print(std::ostream& o) const {
        if (sign) 
            o << sign << " " << exprSignOrExprInt;
        else 
            o << " " << exprSignOrExprInt;
    }
public:
    Sign(char sign,Expr* expr) : Expr() {
        this->sign = sign;
        this->exprSignOrExprInt = expr;
    }
    Sign(Expr* expr) : Expr() {
        this->sign = 0;
        this->exprSignOrExprInt = expr;
    }
    virtual ~Sign() {}
};

class ExprList : public Expr {
    Expr* head;
    Expr* tail;
protected:
    virtual void print(std::ostream& o) const = 0;
private:
    inline bool is_empty() { return (head == nullptr) && (tail == nullptr); }
public:
    ExprList(std::initializer_list<Expr*> exprs) : Expr() {
        this->head = this->tail = 0;
        for (auto expr : exprs) {
            this->push(expr);
        }
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
    virtual ~ExprList() {};
};

class Div : public ExprList {
    friend class ExprArena;
protected:
    virtual void print(std::ostream& o) const {
        Expr* node = ExprList::begin();
        o << node;
        node = node->next();
        for (; node != ExprList::end(); node = node->next()) {
            std::cout << '/' << node;
        }
    }
public:
    Div (std::initializer_list<Expr*> exprs) : ExprList(exprs) {}
    virtual ~Div() {}
};

class Mul : public ExprList {
    friend class ExprArena;
protected:
    virtual void print(std::ostream& o) const {
        Expr* node = ExprList::begin();
        o << node;
        node = node->next();
        for (; node != ExprList::end(); node = node->next()) {
            std::cout << '*' << node;
        }
    }
public:
    Mul(std::initializer_list<Expr*> exprs) : ExprList(exprs) {}
    virtual ~Mul() {}
};

class Sub : public ExprList {
    friend class ExprArena;
protected:
    virtual void print(std::ostream& o) const {
        Expr* node = ExprList::begin();
        o << "(";
        o << node;
        node = node->next();
        for (; node != ExprList::end(); node = node->next()) {
            std::cout << " " << '-' << " " << node;
        }
        o << ")";
    }
public:
    Sub(std::initializer_list<Expr*> exprs) : ExprList(exprs) {}
    virtual ~Sub() {}
};

class Add : public ExprList {
    friend class ExprArena;
protected:
    virtual void print(std::ostream& o) const {
        Expr* node = ExprList::begin();
        o << "(";
        o << node;
        node = node->next();
        for (; node != ExprList::end(); node = node->next()) {
            std::cout << " " << '+' << " " << node;
        }
        o << ")";
    }
public:
    Add(std::initializer_list<Expr*> exprs) : ExprList(exprs) {}
    virtual ~Add() {}
};



