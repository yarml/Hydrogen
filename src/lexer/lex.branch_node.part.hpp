// A single branch is when two tokens have the same first character, but one is of size 2 while the other is a single character
#define SINGLE_BRANCH_NODE(FIRST, SECOND, TOKF, TOKS)                                             \
            else if(m_c == FIRST)                                                                  \
            {                                                                                       \
                int line = m_line, cpos = m_cpos;                                                    \
                readnext();                                                                           \
                if(m_c == SECOND)                                                                      \
                {                                                                                       \
                    out.push_back(token(std::string(1, FIRST) + SECOND, token::type::TOKS, line, cpos)); \
                    readnext();                                                                           \
                }                                                                                          \
                else                                                                                        \
                    out.push_back(token(std::string(1, FIRST), token::type::TOKF, line, cpos));              \
            }
// A complex branch node is when a 4 tokens start with the same character
// But one of them is only 1 character long
// Two are two character long
// And one is three characters long with a second character similar to one of the two with two characters
#define CPLX_BRANCH_NODE(FIRST, SECOND1, SECOND2, THIRD, TOK1, TOK2, TOK3, TOK4)                                               \
            else if(m_c == FIRST)                                                                                              \
            {                                                                                                                  \
                int line = m_line, cpos = m_cpos;                                                                              \
                readnext();                                                                                                    \
                switch(m_c)                                                                                                    \
                {                                                                                                              \
                    case SECOND1:                                                                                              \
                        readnext();                                                                                            \
                        switch(m_c)                                                                                            \
                        {                                                                                                      \
                            case THIRD:                                                                                        \
                                out.push_back(token((std::string(1, FIRST) + SECOND1) + THIRD, token::type::TOK1, line, cpos));\
                                readnext();                                                                                    \
                                break;                                                                                         \
                            default:                                                                                           \
                                out.push_back(token(std::string(1, FIRST) + SECOND1, token::type::TOK2, line, cpos));          \
                                break;                                                                                         \
                        }                                                                                                      \
                        break;                                                                                                 \
                    case SECOND2:                                                                                              \
                        out.push_back(token(std::string(1, FIRST) + SECOND2, token::type::TOK3, line, cpos));                  \
                        readnext();                                                                                            \
                        break;                                                                                                 \
                    default:                                                                                                   \
                        out.push_back(token(std::string(1, FIRST), token::type::TOK4, line, cpos));                            \
                        break;                                                                                                 \
                }                                                                                                              \
            }
// And equal single branch is where three tokens start with the same character
// But two of them are two characters long
// while the other one is one character long
#define EQ_SINGLE_BRANCH(FIRST, SECOND1, SECOND2, TOK1, TOK2, TOK3)                                       \
            else if(m_c == FIRST)                                                                         \
            {                                                                                             \
                int line = m_line, cpos = m_cpos;                                                         \
                readnext();                                                                               \
                switch(m_c)                                                                               \
                {                                                                                         \
                case SECOND1:                                                                             \
                    out.push_back(token(std::string(1, FIRST) + SECOND1, token::type::TOK1, line, cpos)); \
                    readnext();                                                                           \
                    break;                                                                                \
                case SECOND2:                                                                             \
                    out.push_back(token(std::string(1, FIRST) + SECOND2, token::type::TOK2, line, cpos)); \
                    readnext();                                                                           \
                    break;                                                                                \
                default:                                                                                  \
                    out.push_back(token(std::string(1, FIRST), token::type::TOK3, line, cpos));           \
                    break;                                                                                \
                }                                                                                         \
            }
