%{
#include "y.tab.h"

%}

lettre [A-Za-z]
chiffre [0-9]
espace [ \t]
finligne [\n]

apientry (APIENTRY|GLAPIENTRY)
glapi (GLAPI|WINGDIAPI)
paramspe "const GLvoid* const *"

mot  {lettre}(_|{lettre}|{chiffre})*
entier {chiffre}+

%x COMMENT TYPEDEF

%%

"/*"                BEGIN COMMENT;
<COMMENT>.|"\n"       ;
<COMMENT>"*/"       BEGIN 0;           
"("                 return(PO);
")"                 return(PF);
","                 return(V);
"*"                 return(ETOILE);
"const"             return(CONST);
{glapi}             return(GLAPI2); 
{apientry}          return(APIENTRY2); 
{mot}               return(MOT);
{paramspe}          return(PARAMSPE);
"["{entier}"]"      return(TAB);
"typedef".          BEGIN TYPEDEF;
<TYPEDEF>[^;]           ;
<TYPEDEF>";"        BEGIN 0;
"extern".*              ;
"#".*                   ;
{finligne}              ;  
{espace}                ;        
.                       ;

%%
