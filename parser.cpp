

// Statements
stmt-sequence -> statement";" stmt-seq
stmt-seq -> stmt-sequence | "#"

//return stm
//array
//enum + union

statement  -> if-stm
            | while_loop
            | do_while_loop
            | for-loop
            | switch_stm
            | assignment 
            | exp
            | function_call  /////////
            | function_stm    // function_declration 
    
            

stmt_or_empty -> stmt-sequence | "#"

// if 
if-stm -> if (conditions) compound_statement else-part
else-part -> else compound_statement | "#"



//loops
for_loop -> "for" "(" init_expr ";" condition_expr ";" update_expr ")" compound_statement
init_expr -> assignment_expr_list | "#"
assignment_expr_list -> assignment_expr cont_
cont_ -> assignment_expr_list ","| "#"
assignment_expr -> variable "=" exp
variable -> Cont identifier 
Cont-> int | "#"
update_expr -> assignment_expr_list | "#"
compound_statement -> "{" statement "}"|statement";"        ///////////////////////to be handelled


while_loop -> "while" "(" exp ")" loop_statement

do_while_loop -> "do" loop_statement "while" "("exp ")" ";"

loop_statement ->  "{" stmt-sequence "}" | statement";" | "#"



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
 

    //type
    type               -> integral_types | numeric_types | pointer_type | user_defined_type //long, short
    pointer_type       -> type "*"
    user_defined_type  -> struct_type | typedef_type //enum_type
    struct_type        -> "struct" identifier
    typedef_type       -> identifier

    // ///////Expressions [exp, exp2, addop, term, term2, mulop, factor]
    // exp -> term exp2
    // exp2 -> addop term exp2 | "#"
    // addop   -> + | -
    // term -> factor term2
    // term2 -> mulop factor term2 | "#"
    // mulop   -> * | /
    // factor  -> (exp) | number


    //////expressionsPRO 

    exp               -> term exp2
    exp2              -> addop term exp2 | "#"
    term              -> factor term2
    term2             -> mulop factor term2 | "#"
    factor            -> "("exp ")"| number | identifier | function_call | array_access | struct_access | pointer_op | type_cast | boolean_exp | string_op | bitwise_op | unary_op
    addop             -> "+" | "-"
    mulop             -> "*" | "/"
    array_access      -> identifier "[" exp "]"
    struct_access     -> identifier "." identifier
    pointer_op        -> "*" exp
    type_cast         -> "(" type ")" exp
    boolean_exp       ->  "&&" exp | "||" exp |  relational_exp exp
    boolean_exp_      -> exp boolean_exp  
    relational_exp    -> "==" | "!=" | ">" | "<" | ">=" | "<="
    string_op         -> exp "+" exp
    bitwise_op        -> "&" exp |  "|" exp | "^" exp |  "<<" exp | ">>" exp
    bitwise_op__      -> exp bitwise_op 
    unary_op          -> "+"exp | "-"exp | "!"exp | sign_exp exp | exp sign_exp | "~"exp
    sign_exp          ->  "++"| "--"
    number            -> digit digit_ 
    digit_            -> number | "#"
    digit             -> "0"| "1"| "2"| "3"| "4"| "5"| "6"| "7"| "8"| "9"
    identifier        -> start_char following_chars
    start_char        -> letter | "_"
    following_chars   -> letter following_chars | digit following_chars| "_" following_chars 
    letter            -> "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" | "j" | "k" | "l" | "m" | "n" | "o" | "p" | "q" | "r" | "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z" | "A" | "B" 
                       | "C" | "D" | "E" | "F" | "G" | "H" | "I" | "J" | "K" | "L" | "M" | "N" | "O" | "P" | "Q" | "R" | "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z"

  










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





