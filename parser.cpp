/*
-> Grammar rules
-> Parsing table (FIRST, FOLLOW, nonterminals, terminals = tokans!!!!)
-> Stack -> tree (getting tokens from lexical)
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


/*
keyword, literal, num
num NO3O EHH
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
E'   -> + T E' | #
T    -> F T2
T2   -> * F T2 | #
F    -> ( E ) | id

nonTerminal[] F = [[id], [(, E, )]]
*/

//-------------------------------------------

string token;
<identifier, int> -> token = "int"
<PUNCTUATION, "{"> -> token = "{"

// Statements
stmt-sequence -> statement; stmt-seq
stmt-seq -> stmt-sequence | #

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

op_stmt -> stmt-sequence | "#"
op_exp -> exp | "#"



//assignment
assignment-statement -> integral_types integral_id | numeric_types numeric_id | string_types string_id | id_op rest
id_op -> "identifier" | "#"

assignment-operator  -> "=" 
assignment-operator1 -> "+=" | "-=" | "*=" | "/=" | "%=" 
assignment-operator2 -> "&=" | "|=" | "^=" 
assignment-operator3 -> "<<=" | ">>="

integral_types -> "int" | "unsigned" "int" | "short" | "long" long_integeral | "char"
long_integeral -> "long" | "#" 
numeric_types ->  "float" | "double" | "long" "double" 
string_types -> "string"

rest -> "identifier" all_operators exp
integral_id -> "identifier" all_operators exp 
all_operators -> assignment-operator | assignment-operator1 | assignment-operator2 | assignment-operator3 

numeric_id -> "identifier" operators_numeric exp 
operators_numeric -> assignment-operator | assignment-operator1

string_id -> identifier assignment-operator exp


//loops
for_loop -> "for" "(" init_expr ";" condtions ";" conditions ")" compound_statement
init_expr -> assignment_expr init_expr_cont | "#"
init_expr_cont -> "," init_expr 
assignment_expr -> "int" "id" "=" exp | "id" "=" exp

conditions -> exp conditions_cont | "#"
conditions_cont -> "," conditions

compound_statement -> "{" op_stmt "}" | statement | ";"       

while_loop -> "while" "(" conditions ")" compound_statement

do_while_loop -> "do" compound_statement "while" "(" conditions ")" ";"



// if 
if-stm -> if (conditions) compound_statement else-part
else-part -> else compound_statement | "#"


        
    
//switch
switch_stm -> "switch" '(' exp ')' switch_body
switch_body -> '{' first_case '}' ";" | one_case_d | "break"";"| ";" | stmt_sequence

first_case -> case_stm_d | stmt-sequence case_stm_d

case_body -> "{" op_stmt break_stm "}" | op_stmt


case_stm_d  -> "case" constant_exp ':' case_body break_stm case_stm_d 
            |  "default" ':' case_body break_stm case_stm
            |  "#"

case_stm  -> "case" constant_exp ':' case_body break_stm case_stm
            | "#"

one_case_d -> "case" constant_exp ':' stmt_part_d 
            |  "default" ':' stmt_part
                
one_case -> "case" constant_exp ':' stmt_part | "#"


stmt_part_d -> one_case_d | stmt-sequence | "break" ";" | "{" op_stmt break_stm "}"
stmt_part -> one_case | stmt-sequence | "break" ";" | "{" op_stmt break_stm "}"

constant_exp -> number | literal                 
break_stm -> "break" ";" | "#"



//////OTHERS//////
    // numbers [number]
    number -> DEC | BIN | OCT | HEX

    //type
    type               -> string_types | numeric_types | pointer_type | user_defined_type | "auto"
    pointer_type       -> type '*'
    user_defined_type  -> struct_type | typedef_type //enum_type - union
    struct_type        -> 'struct' identifier
    typedef_type       -> identifier

    ///////Expressions [exp, exp2, addop, term, term2, mulop, factor]
    exp -> term exp2   //no "#" 3shan 5ater el func w kda
    exp2 -> addop term exp2 | #
    addop   -> + | -
    term -> factor term2
    term2 -> mulop factor term2 | #
    mulop   -> * | /
    factor  -> (exp) | number

    //////expressionsPRO 

    exp               -> term exp2
    exp2              -> addop term exp2 | #
    term              -> factor term2
    term2             -> mulop factor term2 | #
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








//Functions - versions: proto/not void/not
function_start -> type identifier '(' function_stm
function_stm -> op_parameter ')' {func_body} | op_parameter_proto ')' ";"

func_start_void -> "void" identifier '(' function_stm_void
function_stm_void ->  op_parameter ')' {func_body_void} | op_parameter_proto ')'";"


func_body ->  op_stmt "return" exp ";" end_
end_ -> op_stmt maybe_return | "#"
maybe_return -> func_body | "#";

func_body_void -> op_stmt op_return
op_return -> "return" ";" end_void | "#"
end_void ->  func_body_void | "#"

op_parameter -> parameter_list | "#"
parameter_list       -> parameter parameter_list_cont
parameter_list_cont ->  ',' parameter_list | "#"
parameter            -> type identifier

op_parameter_proto -> parameter_list_proto | "#"
parameter_list_proto       -> parameter_proto parameter_list_cont_proto
parameter_list_cont_proto ->  ',' parameter_list_proto | "#"
parameter_proto -> type id_op
id_op -> identifier | "#"


function_call_statement  -> identifier '(' op_argument ')' ';'
op_argument -> argument_list | "#"
argument_list -> exp argument_list_cont
argument_list_cont -> ',' argument_list | "#"







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
                                if(vect[i] == "#")
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


