%{
    #include <stdio.h>
    #include <string.h>
    #include "json.h"
    extern FILE* yyin;
    int yylex(void);
    int yyerror(char*);

    json_t json;
    void *current;
%}

%union {
    char *sval;
    int ival;
    double dval;
}

%token FALSE TRUE NUL


// STRING
%token <sval> STRING

// NUMBER
%token <ival> INTEGER
%token <dval> NUMBER

%%

json
: value
;


value
: FALSE     { 
    char boolean;
    boolean = 0;
}
| TRUE      { 
    char boolean; 
    boolean = 1;
}
| NUL       {
    void *nul;
    nul = NULL;
} 
| object    { 
}
| array     {

}
| NUMBER    { 
    double number;
    number = $1;
    json.value = &number;
    json.type = VNUMBER;
}
| INTEGER   { 
    int integer; 
    integer = $1;
    json.value = &integer;
    json.type = VINTEGER;

}
| STRING    { 
    char *string;
    string = calloc(strlen($1), sizeof(string));
    strcpy(string, $1);
    json.value = string;
    json.type = VSTRING;
}
;

object
: begin_object member_list end_object
;

begin_object
: '{'    
;

end_object
: '}'
;

member_list
: member
| member_list ',' member 
;

member 
: STRING ':' value  {
    printf("%s\n", $1);
}
;

array 
: begin_array value_list end_array 
;

begin_array
: '['
;

end_array
: ']'
;

value_list 
: value 
| value_list ',' value 
;


%%

int yyerror(char *s)
{
    fflush(stdout);
    return 1;
}

int main(int argc, char *argv[])
{
    int local_error = 1;
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        initJSON(json);
        /* current = &json; */
        if (!yyparse()) {
            local_error = 1;
            printf("success !!\n");
            printJSON(json);
        } else {
            local_error = 0;
        }
    }
    return local_error;
}