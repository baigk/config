
#ifndef CPPTEMPL_H_
#define CPPTEMPL_H_

#include <string.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <iostream>
using namespace std;

namespace cpptempl {

void SplitString(const std::string &str, const char *delim,
                 std::vector<std::string> *result) {
    char *cstr, *p;
    cstr = new char[str.size() + 1];
    snprintf(cstr, str.size() + 1, "%s", str.c_str());
    p = strtok(cstr, delim);  // NOLINT'
    while (p != NULL) {
        result->push_back(std::string(p));
        p = strtok(NULL, delim);  // NOLINT'
    }
    delete[] cstr;
}

class auto_data {
   public:
    enum class data_type : uint8_t {
        null, string, boolean, number_integer, number_float, map, list
    };

    union data_value {
        std::string *str;
        bool boolean;
        int64_t int_val;
        double f_val;

        data_value() {}
        data_value(std::string *v) { str = v; }
        data_value(bool boolean) : boolean(boolean) {}
        data_value(int64_t v) : int_val(v) {}
        data_value(double v) : f_val(v) {}
        data_value(data_type t) {
            switch (t) {
                case data_type::string: {
                    str = NULL;
                    break;
                }
                case data_type::boolean: {
                    boolean = false;
                    break;
                }
                case data_type::number_integer: {
                    int_val = 0;
                    break;
                }
                case data_type::number_float: { f_val = 0.0; }
                default:
                    break;
            }
        }
    };

   public:
    using string_t = std::string;
    auto_data() { type = data_type::null; }
    auto_data(const string_t &v) : type(data_type::string) {
        std::string *str = new std::string(v);
        value.str = str;
    }
    auto_data(const char *v) : type(data_type::string) {
        std::string *str = new std::string(v);
        value.str = str;
    }

    auto_data(bool v)  // NOLINT
        : type(data_type::boolean),
          value(v) {}
    auto_data(int64_t v)  // NOLINT
        : type(data_type::number_integer),
          value(v) {}
    auto_data(int v)  // NOLINT
        : type(data_type::number_integer),
          value((int64_t)v) {}
    auto_data(size_t v)  // NOLINT
        : type(data_type::number_integer),
          value((int64_t)v) {}
    auto_data(double v)  // NOLINT
        : type(data_type::number_float),
          value(v) {}
    auto_data(const auto_data &data) {
        type = data.type;
        if (data.type == data_type::string) {
            value.str = new std::string(*(data.value.str));
        } else if (data.type == data_type::map) {
            for (auto &item : data.map_data) {
                auto_data d = item.second;
                map_data[item.first] = d;
            }
        } else if (data.type == data_type::list) {
            list_data = data.list_data;
        } else {
            value = data.value;
        }
    }

    ~auto_data() {
        if (type == data_type::string && value.str != NULL) {
            delete value.str;
            value.str = NULL;
        }
    }

    // map
    bool has(std::string key) const {
        if (type == data_type::map) {
            auto iter = map_data.find(key);
            if (iter != map_data.end()) {
                return true;
            }
        }
        return false;
    }
    auto_data &operator[](const std::string &key) {
        auto iter = map_data.find(key);
        if (iter == map_data.end()) {  // find
            map_data[key] = auto_data();
        }
        type = data_type::map;
        return map_data[key];
    }
    auto_data &operator[](const char *key) {
        auto iter = map_data.find(key);
        if (iter == map_data.end()) {  // find
            map_data[key] = auto_data();
        }
        type = data_type::map;
        return map_data[key];
    }

    const auto_data &Get(const std::string &key) const {
        auto iter = map_data.find(key);
        if (iter == map_data.end()) {  // find
            throw new std::out_of_range("out of range is Get method");
            ;
        }
        return map_data.at(key);
    }

    int size() const { return list_data.size(); }
    auto_data operator[](int index) const { return list_data[index]; }
    void push_back(const auto_data &data) {
        type = data_type::list;
        list_data.push_back(data);
    }

    bool operator==(const auto_data &data) {
        if (this->type != data.type) {
            return false;
        }
        switch (type) {
            case data_type::string: {
                return *(this->value.str) == *(data.value.str);
            }
            case data_type::boolean: {
                return value.boolean == data.value.boolean;
            }
            case data_type::number_integer: {
                return value.int_val == data.value.int_val;
            }
            case data_type::number_float: {
                return value.f_val == data.value.f_val;
            }
            case data_type::map: { return false; }
            default:
                return false;
        }
        return false;
    }

    void operator=(const auto_data &data) {
        type = data.type;
        if (data.type == data_type::string) {
            value.str = new std::string(data.value.str->c_str());
        } else if (data.type == data_type::map) {
            map_data = data.map_data;
        } else if (data.type == data_type::list) {
            list_data = data.list_data;
        } else {
            value = data.value;
        }
    }
    operator std::string() const {
        std::string str = "";
        switch (type) {
            case data_type::string: {
                str = std::string((*(value.str)));
                break;
            }
            default:
                break;
        }
        return str;
    }
    operator int() const {
        int64_t v = 0;
        switch (type) {
            case data_type::number_integer: {
                v = value.int_val;
                break;
            }
            default:
                break;
        }
        return v;
    }
    operator int64_t() const {
        int64_t v = 0;
        switch (type) {
            case data_type::number_integer: {
                v = value.int_val;
                break;
            }
            default:
                break;
        }
        return v;
    }
    operator double() const {
        double v = 0.0;
        switch (type) {
            case data_type::number_float: {
                v = value.f_val;
                break;
            }
            default:
                break;
        }
        return v;
    }
    operator bool() const {
        bool v = false;
        switch (type) {
            case data_type::boolean: {
                v = value.boolean;
                break;
            }
            default:
                break;
        }
        return v;
    }

   public:
    std::string type_name() const {
        switch (type) {
            case(data_type::null) : { return "null"; }
            case(data_type::string) : { return "string"; }
            case(data_type::boolean) : { return "boolean"; }
            default: { return "number"; }
        }
    }

    data_type Type() const { return type; }

    bool empty() { return type == data_type::null; }

    bool is_true() {
        switch (type) {
            case data_type::null: { return false; }
            case data_type::boolean: { return value.boolean; }
            case data_type::number_integer: { return value.int_val != 0; }
            case data_type::number_float: { return value.f_val != 0; }
            default:
                return true;
                break;
        }
        return true;
    }

   private:
    data_type type;
    data_value value = data_type::null;
    std::map<std::string, auto_data> map_data;
    std::vector<auto_data> list_data;
};

auto_data parse_val(std::string key, const auto_data &data) {
    if (key[0] == '\"') {
        size_t index = key.substr(1).find_last_of("\"");
        if (index != std::string::npos) {
            return key.substr(1, index);
        }
        return "";
    }
    size_t index = key.find(".");
    if (index == std::string::npos) {
        if (!data.has(key)) {
            return auto_data();
        }
        return data.Get(key);
    }

    std::string sub_key = key.substr(0, index);
    if (!data.has(sub_key)) {
        return auto_data();
    }
    const auto_data &item = data.Get(sub_key);
    return parse_val(key.substr(index + 1), item);
}

typedef enum {
    TOKEN_TYPE_NONE, TOKEN_TYPE_TEXT, TOKEN_TYPE_VAR, TOKEN_TYPE_IF, TOKEN_TYPE_FOR, TOKEN_TYPE_ENDIF, TOKEN_TYPE_ENDFOR, TOKEN_TYPE_VARDEF,
} TokenType;

const char *tm[] = {"TOKEN_TYPE_NONE",   "TOKEN_TYPE_TEXT",
                    "TOKEN_TYPE_VAR",    "TOKEN_TYPE_IF",
                    "TOKEN_TYPE_FOR",    "TOKEN_TYPE_ENDIF",
                    "TOKEN_TYPE_ENDFOR", "TOKEN_TYPE_VARDEF", };

class Token;
using token_vector = std::vector<std::shared_ptr<Token> >;
class Token {
   public:
    token_vector define_vars;
    virtual TokenType gettype() = 0;
    virtual void set_children(const token_vector &) {
        printf("this token can't set child\n");
    }
    virtual std::string get_text(const auto_data &) { return "invalid token"; }

    void set_define_vars(const token_vector &define_vars) {
        this->define_vars.assign(define_vars.begin(), define_vars.end());
    }

    virtual void update_data(auto_data &data) {
        for (auto &i : define_vars) {
            i->update_data(data);
        }
    }

    virtual void dump(int k) {};

};

class TokenText : public Token {
   private:
    std::string m_text;

   public:
    explicit TokenText(std::string text) : m_text(text) {}
    TokenType gettype() { return TOKEN_TYPE_TEXT; }
    std::string get_text(const auto_data &) {
        return m_text;
    }

    void dump(int indent) {
        std::cout << string(indent, ' ') << tm[gettype()] << ":" << m_text << std::endl;
    }
};

class TokenVar : public Token {
   private:
    std::string m_key;

   public:
    explicit TokenVar(std::string key) : m_key(key) {}
    TokenType gettype() { return TOKEN_TYPE_VAR; }
    std::string get_text(const auto_data &data) {
        auto_data ret = parse_val(m_key, data);
        std::string str = "";
        switch (ret.Type()) {
            case auto_data::data_type::string: { return ret; }
            case auto_data::data_type::boolean: {
                bool b = ret;
                str = b ? "true" : "false";
                break;
            }
            case auto_data::data_type::number_integer: {
                char temp[10] = {'\0'};
                int64_t t = ret;
                snprintf(temp, sizeof(temp), "%" PRIu64, t);
                str = std::string(temp);
                break;
            }
            case auto_data::data_type::number_float: {
                char temp[10] = {'\0'};
                double t = ret;
                snprintf(temp, sizeof(temp), "%f", t);
                str = std::string(temp);
                break;
            }
            case auto_data::data_type::null: {
                str = "null";
                break;
            }
            default:
                cout << "inalid type" << ret.type_name() << endl;
                break;
        }
        return str;
    }
    void dump(int indent) {
        std::cout << string(indent, ' ') <<  tm[gettype()] << ":" << m_key << std::endl;
    }
};

class TokenFor : public Token {
   public:
    std::string m_key;
    std::string m_val;
    token_vector m_children;
    
    explicit TokenFor(std::string expr) {
        std::vector<std::string> elements;
        char split[] = " ";
        SplitString(expr, split, &elements);
        if (elements.size() != 4) {
            std::cout << "cpp template string have error syntax 'for'"
                      << ":" << expr << std::endl;
            exit(0);
        }
        m_val = elements[1];
        m_key = elements[3];
    }
    TokenType gettype() { return TOKEN_TYPE_FOR; }
    void set_children(const token_vector &children) {
        m_children.assign(children.begin(), children.end());
    }
    token_vector &get_children() { return m_children; }
    void dump(int indent) {
        std::cout << string(indent, ' ')  << tm[gettype()] << ":" << m_key << ":" << m_val << std::endl;

        indent++;
        for (auto i : m_children) {
            i->dump(indent);
        }
    }
    std::string get_text(const auto_data &data) {
        const auto_data &l = parse_val(m_key, data);
        std::string str = "";

        auto trans = [=, &str](const auto_data & l) {
            auto_data d;
            d[m_val] = l;
            update_data(d);

            for (size_t j = 0; j < m_children.size(); ++j) {
                str += m_children[j]->get_text(d);
            }
        };

        if (l.Type() == auto_data::data_type::list) {
            for (int i = 0; i < l.size(); i++) {
                trans(l[i]);
            }
        } else {
            trans(l);
        }
        return str;
    }
};

class TokenVarDef : public Token {
   public:
    std::string m_val;
    std::string m_key;

    explicit TokenVarDef(std::string expr) {
        std::vector<std::string> elements;
        char split[] = " ";
        SplitString(expr, split, &elements);
        if (elements.size() != 4) {
            std::cout << "cpp template string have error syntax 'for'"
                      << " " << expr << std::endl;
            exit(0);
        }
        m_val = elements[1];
        m_key = elements[3];
    }

    TokenType gettype() { return TOKEN_TYPE_VARDEF; }
    virtual void update_data(auto_data &data) {
        data[m_val] = parse_val(m_key, data);
    }
};

// if block
class TokenIf : public Token {
   public:
    std::string m_expr;
    token_vector m_children;
    explicit TokenIf(std::string expr) : m_expr(expr) {}
    TokenType gettype() { return TOKEN_TYPE_IF; }
    void set_children(const token_vector &children) {
        m_children.assign(children.begin(), children.end());
    }
    token_vector &get_children() { return m_children; }
    std::string get_text(const auto_data &data) {
        std::string str = "";
        if (is_true(data)) {
            for (size_t j = 0; j < m_children.size(); ++j) {
                str += m_children[j]->get_text(data);
            }
        } else {
            // printf("is not true:%s\n", m_expr.c_str());
        }
        return str;
    }
    bool is_true(const auto_data &data) {
        std::vector<std::string> elements;
        char split[] = " ";
        SplitString(m_expr, split, &elements);
        if (elements.size() == 1) {
            return false;
        } else if (elements.size() == 2) {
            return parse_val(elements[1], data).is_true();
        } else if (elements.size() == 3) {
            if (elements[1] == "not") {
                return !parse_val(elements[2], data).is_true();
            }
        } else if (elements.size() == 4) {
            if (elements[2] == "==") {
                auto_data lhs = parse_val(elements[1], data);
                auto_data rhs = parse_val(elements[3], data);
                return lhs == rhs;
            }
        }
        return false;
    }
};

class TokenEnd : public Token {
   private:
    std::string m_type;

   public:
    explicit TokenEnd(std::string text) : m_type(text) {}
    TokenType gettype() {
        return m_type == "endfor" ? TOKEN_TYPE_ENDFOR : TOKEN_TYPE_ENDIF;
    }
};

token_vector tokenize(std::string text) {
    token_vector tokens;
    while (!text.empty()) {
        size_t pos = text.find("{");

        if (pos == std::string::npos) {
            if (!text.empty()) {
                tokens.push_back(std::shared_ptr<Token>(new TokenText(text)));
            }
            return tokens;
        }
        std::string pre_text = text.substr(0, pos);
        if (!pre_text.empty()) {
            tokens.push_back(std::shared_ptr<Token>(new TokenText(pre_text)));
        }
        text = text.substr(pos + 1);
        if (text.empty()) {
            tokens.push_back(std::shared_ptr<Token>(new TokenText("{")));
            return tokens;
        }
        
        if (text[0] == '$') {
            pos = text.find("}");
            if (pos != std::string::npos) {
                tokens.push_back(std::shared_ptr<Token>(
                    new TokenVar(text.substr(1, pos - 1))));
                text = text.substr(pos + 1);
            }
        } else if (text[0] == '%') {  // control statement
            pos = text.find("}");
            if (pos != std::string::npos) {
                std::string expression = text.substr(1, pos - 2);
                size_t spos = expression.find_first_not_of(" ");
                if (spos != 0 && spos != std::string::npos) {
                    expression = expression.substr(spos);
                }
                size_t epos = expression.find_last_not_of(" ");
                if (epos != expression.length() && epos != std::string::npos) {
                    expression = expression.substr(0, epos + 1);
                }
                text = text.substr(pos + 1);
                if (expression.find("for ") != expression.npos) {
                    tokens.push_back(
                        std::shared_ptr<Token>(new TokenFor(expression)));
                } else if (expression.find("if ") != expression.npos) {
                    tokens.push_back(
                        std::shared_ptr<Token>(new TokenIf(expression)));
                } else if (expression.find("set ") != expression.npos) {
                    tokens.push_back(
                        std::shared_ptr<Token>(new TokenVarDef(expression)));
                } else {
                    tokens.push_back(
                        std::shared_ptr<Token>(new TokenEnd(expression)));
                }
            }
        } else {
            tokens.push_back(std::shared_ptr<Token>(new TokenText("{")));
        }
    }
    return tokens;
}

void parse_tree(token_vector *tokens, token_vector *tree, token_vector var,
                TokenType until = TOKEN_TYPE_NONE) {
    while (!tokens->empty()) {
        std::shared_ptr<Token> token = tokens->at(0);
        tokens->erase(tokens->begin());

        if (token->gettype() == TOKEN_TYPE_FOR ||
            token->gettype() == TOKEN_TYPE_IF) {
            cpptempl::TokenType j =
                (token->gettype() == TOKEN_TYPE_FOR ? TOKEN_TYPE_ENDFOR
                                                    : TOKEN_TYPE_ENDIF);
            token_vector children, define_vars;

            parse_tree(tokens, &children, var, j);
            token->set_children(children);
            token->set_define_vars(var);
        }
        else if (token->gettype() == TOKEN_TYPE_VARDEF) {
            var.push_back(token);
            continue;
        } else if (token->gettype() == until) {
            return;
        }
        tree->push_back(token);
    }
}

std::string parse(std::string templ_text, auto_data &data) {
    token_vector tokens;
    tokens = tokenize(templ_text);
    token_vector tree, var;
    parse_tree(&tokens, &tree, var);
    for (auto &i : var) {
        i->update_data(data);
    }
    std::string str = "";
    for (size_t i = 0; i < tree.size(); ++i) {
        str = str + tree[i]->get_text(data);
    }
    return str;
}

}  // namespace cpptempl

#endif  // CPPTEMPL_H_
