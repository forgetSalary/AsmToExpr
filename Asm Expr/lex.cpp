enum TokenKind {
    TOKEN_EOF = 0,
    TOKEN_CMD = 128,
    TOKEN_NEW_LINE,
    TOKEN_NUMBER,
    TOKEN_COMMA,
    TOKEN_REG,
    // ...
};

enum CmdKind {
    CMD_NONE,
    CMD_MOV = 1,
    CMD_ADD,
    CMD_INC,
    CMD_SUB,
    CMD_DEC,
    CMD_MUL,
    CMD_DIV,
    CMD_PUSH,
    CMD_POP,
    CMD_OUT,
};

enum RegKind {
    REG_NONE,
    REG_EAX = 1,
    REG_EBX,
    REG_ECX,
    REG_EDX,
};

const int reg_count = REG_EDX;

class Lex {
    struct Token {
        TokenKind kind;
        const char* start;
        const char* end;
        union {
            uint32_t num_val;
            CmdKind cmd_kind;
            RegKind reg_kind;
        };
    };

    char token_name_buff[64];
    Token token;
    uint8_t char_to_digit[256];
    std::map<std::string, CmdKind> cdm_name_to_kind_map;
    std::map<std::string, RegKind> reg_name_to_kind_map;
    const char* initial_stream;
    char* stream;
private:
    void scan_int() {
        uint64_t base = 10;
        uint64_t val = 0;
        for (;;) {
            uint64_t digit = char_to_digit[*stream];
            if (digit == 0 && *stream != '0') {
                break;
            }
            if (digit >= base) {
                fatal("Digit '%c' out of range for base %", *stream, base);
                digit = 0;
            }
            if (val > (UINT64_MAX - digit) / base) {
                fatal("Integer literal overflow");
                while (isdigit(*stream)) {
                    stream++;
                }
                val = 0;
                break;
            }
            val = val * base + digit;
            stream++;
        }
        token.kind = TOKEN_NUMBER;
        token.num_val = val;
    }

public:
    void fatal(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        printf("FATAL: ");
        vprintf(fmt, args);
        va_end(args);
        exit(1);
    }
    void next_token() {
    begin:
        switch (*stream) {
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': {
            bool negative;
            if (*stream == '-') {
                negative = true;
                stream++;
            }
            else negative = false;

            char* num_start = stream;
            while (isdigit(*stream)) {
                stream++;
            }
            char c = *stream;
            stream = num_start;
            scan_int();
            break;
        }
        case 'a': case 'b': case 'c': case 'd':case 'e': case 'f':case 'g': case 'h':case 'i':case 'j':
        case 'k': case 'l': case 'm': case 'n':case 'o': case 'p':case 'q': case 'r':case 's':case 't':
        case 'u': case 'v': case 'w': case 'x':case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D':case 'E': case 'F':case 'G': case 'H':case 'I':case 'J':
        case 'K': case 'L': case 'M': case 'N':case 'O': case 'P':case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X':case 'Y': case 'Z': {
            token.start = stream;
            int char_count = 0;
            int i = 0;
            for (; i < 64 && (isalnum(*stream)); i++) {
                token_name_buff[i] = *stream++;
            }
            token_name_buff[i + 1] = 0;
            int kind;
            if (kind = cdm_name_to_kind_map[token_name_buff]) {
                token.kind = TOKEN_CMD;
                token.cmd_kind = static_cast<CmdKind>(kind);
            }else if (kind = reg_name_to_kind_map[token_name_buff]){
                token.kind = TOKEN_REG;
                token.reg_kind = static_cast<RegKind>(kind);
            }
            break;
        }
        case '\n':
            token.kind = TOKEN_NEW_LINE;
            stream++;
            break;
        case '\t':case ' ':case '\r':case '\v':case '\b':case '\a': {
            stream++;
            while (*stream == ' ' || *stream == '\t') {
                stream++;
            }
            goto begin;
        }
        case ',': {
            token.kind = TOKEN_COMMA;
            stream++;
            break;
        }
        case ';':
            while (*stream != '\n') {
                stream++;
            }
            token.kind = TOKEN_NEW_LINE;
            stream++;
            break;
        case '\0':
            token.kind = TOKEN_EOF;
        default:
            token.kind = static_cast<TokenKind>(*stream++);
        }
        token.end = stream;
    }
    inline bool is_token(TokenKind kind) {
        return token.kind == kind;
    }
    inline bool match_token(TokenKind kind) {
        if (is_token(kind)) {
            next_token();
            return true;
        }
        else {
            return false;
        }
    }
    inline bool expect_token(TokenKind kind) {
        if (is_token(kind)) {
            next_token();
            return true;
        }
        else {
            fatal("expected token 0x%x, got 0x%x", token.kind, kind);
            return false;
        }
    }
    inline int number() {
        return token.num_val;
    }
    inline RegKind reg_kind() {
        return token.reg_kind;
    }
    inline CmdKind cmd_kind() {
        return token.cmd_kind;
    }
public:
    Lex() {
        char_to_digit['0'] = 0;
        char_to_digit['1'] = 1;
        char_to_digit['2'] = 2;
        char_to_digit['3'] = 3;
        char_to_digit['4'] = 4;
        char_to_digit['5'] = 5;
        char_to_digit['6'] = 6;
        char_to_digit['7'] = 7;
        char_to_digit['8'] = 8;
        char_to_digit['9'] = 9;
        char_to_digit['a'] = 10; char_to_digit['A'] = 10;
        char_to_digit['b'] = 11; char_to_digit['B'] = 11;
        char_to_digit['c'] = 12; char_to_digit['C'] = 12;
        char_to_digit['d'] = 13; char_to_digit['D'] = 13;
        char_to_digit['e'] = 14; char_to_digit['E'] = 14;
        char_to_digit['f'] = 15; char_to_digit['F'] = 15;

        cdm_name_to_kind_map = {
            {"mov",CMD_MOV},
            {"add",CMD_ADD},
            {"inc",CMD_INC},
            {"sub",CMD_SUB},
            {"dec",CMD_DEC},
            {"mul",CMD_MUL},
            {"div",CMD_DIV},
            {"push",CMD_PUSH},
            {"pop",CMD_POP},
            {"out",CMD_OUT},
        };

        reg_name_to_kind_map = {
            {"eax",REG_EAX},
            {"ebx",REG_EBX},
            {"ecx",REG_ECX},
            {"edx",REG_EDX},
        };
    }    
    void init_stream(const char* src) {
        initial_stream = stream = const_cast<char*>(src);
        next_token();
    }
    ~Lex() { }
};
