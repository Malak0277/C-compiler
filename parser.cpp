/*
-> Grammar rules
-> Parsing table (FIRST, FOLLOW)
-> Stack -> tree
*/


/*
Grammar for:
    loops 
    ifconditions 
    assignment statments 
    switch case 
    arthimatic boolean expression
    function call
    returnstatments 
    function delclarions 
    functions call
    array
*/

//-------------------------------------------
#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef nonTerminal;
vector<nonTerminal> symbols;

//typedef char NonTerminal;
//typedef vector<NonTerminal> NonTerminalVec;

/*
E    -> T E'
E'   -> + T E' | €
T    -> F T2
T2   -> * F T2 | €
F    -> ( E ) | id

nonTerminal[] F = [[id], [(, E, )]]
*/

//-------------------------------------------
//map
string firstTable [SYMBOLS][2];
string followTable [SYMBOLS][2];





const int SYMBOLS = 10;

// Statements
stmt-sequence -> statement; stmt-seq
stmt-seq -> stmt-sequence | €

statement  -> if-stm
            | while-stm
            | do-while-stm
            | for-stm
            | switchcase-stm 
            | assignment 
            | arithmetic-boolean-expression 
            | function-call 
            | return-statement //if not in a function?
            | function-declaration 
            | function-call 
            | array
            //enum + union

stmt_or_empty -> stmt-sequence | "€"

// if [if-stm, else-part]
if-stm -> if (exp) statement else-part
else-part -> else statement | €


//loops
for_loop -> "for" "(" init_expr ; condition_expr ; update_expr ")" compound_statement
init_expr -> assignment_expr_list | "€"
assignment_expr_list -> assignment_expr cont_
cont_ -> assignment_expr_list ,| "€"
assignment_expr -> variable = expression
variable -> ID Cont
Cont-> int | "€"
condition_expr -> expression | "€"
update_expr -> assignment_expr_list | "€"
compound_statement -> "{" stmnt "}"|stmnt;        ///////////////////////to be handelled


while_loop -> "while" ( exp ) loop_statement

do_while_loop -> "do" loop_statement "while" (exp ) ;

loop_statement ->  { stmt-sequence } | statement";" | "€"



//assignment
assignment-type -> type | "€"
assignment -> assignment-type identifier assignment-operator exp
assignment-operator -> "=" | "+=" | "-=" | "*=" | "/=" | "%=" | "&=" | "|=" | "^=" | "<<=" | ">>="

        
    
//switch
switch_stm -> "switch" '(' exp ')' switch_body
switch_body -> '{' first_case '}' ";" | one_case_d | "break"";"| ";" | stmt_sequence

first_case -> case_stm_d | stmt-sequence case_stm_d

case_body -> "{" stmt_or_empty break_stm "}" | stmt_or_empty


case_stm_d  -> "case" constant_exp ':' case_body break_stm case_stm_d 
            |  "default" ':' case_body break_stm case_stm
            |  "€"

case_stm  -> "case" constant_exp ':' case_body break_stm case_stm
            | "€"

one_case_d -> "case" constant_exp ':' stmt_part_d 
            |  "default" ':' stmt_part
                
one_case -> "case" constant_exp ':' stmt_part | "€"


stmt_part_d -> one_case_d | stmt-sequence | "break" ";" | "{" stmt_or_empty break_stm "}"
stmt_part -> one_case | stmt-sequence | "break" ";" | "{" stmt_or_empty break_stm "}"

constant_exp -> number | literal                 
break_stm -> "break" ";" | "€"



//////OTHERS//////
    // numbers [number]
    number -> DEC | BIN | OCT | HEX

    //type
    type               -> integral_types | numeric_types | pointer_type | user_defined_type //long, short
    pointer_type       -> type '*'
    user_defined_type  -> struct_type | typedef_type //enum_type
    struct_type        -> 'struct' identifier
    typedef_type       -> identifier

    ///////Expressions [exp, exp2, addop, term, term2, mulop, factor]
    exp -> term exp2
    exp2 -> addop term exp2 | €
    addop   -> + | -
    term -> factor term2
    term2 -> mulop factor term2 | €
    mulop   -> * | /
    factor  -> (exp) | number

    //////expressionsPRO 

    exp               -> term exp2
    exp2              -> addop term exp2 | €
    term              -> factor term2
    term2             -> mulop factor term2 | €
    factor            -> '(' exp ')' | number | identifier | function_call | array_access | struct_access | pointer_op | type_cast | boolean_exp | string_op | bitwise_op | unary_op
    addop             -> '+' | '-'
    mulop             -> '*' | '/'
    function_call     -> identifier '(' argument_list ')'
    array_access      -> identifier '[' exp ']'
    struct_access     -> identifier '.' identifier
    pointer_op        -> '*' exp
    type_cast         -> '(' type ')' exp
    boolean_exp       -> exp ('&&' | '||') exp
    string_op         -> exp '+' exp
    bitwise_op        -> exp '&' exp | exp '|' exp | exp '^' exp | exp '<<' exp | exp '>>' exp
    unary_op          -> '+' exp | '-' exp | '!' exp | '++' exp | '--' exp | '~' exp
    number            -> [0-9]+
    identifier        -> [a-zA-Z_][a-zA-Z0-9_]*








//Functions
function_stm -> type identifier '(' parameter_list ')' func2 | 'void' identifier '(' parameter_list ')' func3
func2 -> ";" | {stmt-and-return}
stmt-and-return ->  stmt-sequence stmt-and-return | return_stm stmt-and-return
stmt-and-void-return ->  stmt-sequence stmt-and-return | void-return_stm stmt-and-return
func3 -> ";" | {stmt-and-void-return}

parameter_list       -> parameter | parameter ',' parameter_list | "€"
parameter            -> type identifier

function_call_statement  -> identifier '(' argument_list ')' ';'
argument_list           -> exp | exp ',' argument_list | "€"


return-stm     -> 'return' exp ';'
void-return-stm -> 'return' ';'







//assignment_statement -> identifier '=' exp ';'



vector<string> first(nonTerminal k){
    vector<string> firstVect;
    for (int i = 0; i < k.size(); i++)
        if (typeid(k[i][0]).name() == typeid(string).name())
            firstVect.push_back(k[i][0]);
        else 
            firstVect.push_back(first(k[i][0]));
    return removeDuplicates(firstVect); /////
}


// nonTerminal[] F = [[id], [(, E, )]]
vector<string> follow(nonTerminal t){
    vector<string> followVect;
    for (int i = 0; i < symbols.size(); i++) //to loop through the symbols
        for(int j = 0; j < symbols[i].size; j++) // to loop through the grammar in a symbol
            for(int k = 0; k < symbols[i][j].size; k++){ // to loop inside the grammar
                if (symbols[i][j][k] == t)
                    nonTerminal start = symbols[i];
                    if(t == start)
                        followVect.push_back("$");

                    if(k != symbols[i][j].size() -1){
                        auto after = symbols[i][j][k+1];

                        if (typeid(after).name() == typeid(string).name()) //if it is a terminal
                            followVect.push_back(after);
                        else // if it is a non-terminal
                            vector<string> vect = first(after);
                            for(int i = 0; i < vect.size(); i++)
                                if(vect[i] == "€")
                                    followVect.push_back(follow(start));
                                else
                                    followVect.push_back(vect[i])
                    }
                    else //if it is the last element
                        followVect.push_back(follow(start));
            }
    return removeDuplicates(followVect);
}


vector<string> removeDuplicates(const vector<string>& arr) {
    vector<string> uniqueArr;

    // Iterate through the array and add unique elements to the uniqueArr
    for (const T& value : arr) {
        if (find(uniqueArr.begin(), uniqueArr.end(), value) == uniqueArr.end()) {
            uniqueArr.push_back(value);
        }
    }
    return uniqueArr;
}


