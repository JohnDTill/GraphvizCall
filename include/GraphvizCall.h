#ifndef GRAPHVIZ_CALL_H
#define GRAPHVIZ_CALL_H

#include <stdio.h>
#include <stdlib.h>

namespace GraphvizCall {

enum Result{
    SUCCESS,
    WRITE_ERROR,
    DOT_CODE_ERROR,
	DOT_NOT_ON_PATH
};

#ifndef GRAPHVIZ_CALL_EXTENSION_TYPE
#define GRAPHVIZ_CALL_EXTENSION_TYPE pdf
#endif

#define GRAPHVIZ_CALL_STRING_INNER(x) #x
#define GRAPHVIZ_CALL_STRING(x) GRAPHVIZ_CALL_STRING_INNER(x)

inline Result show(const char* source){
    FILE* out = fopen("./temp.dot", "w");
    if(!out) return WRITE_ERROR;

    fprintf(out, "%s", source);
    fclose(out);
	
	#ifndef NDEBUG
	if(system("dot -V")) return DOT_NOT_ON_PATH;
	#endif

    system("dot -T" GRAPHVIZ_CALL_STRING(GRAPHVIZ_CALL_EXTENSION_TYPE)
           " temp.dot -o temp." GRAPHVIZ_CALL_STRING(GRAPHVIZ_CALL_EXTENSION_TYPE));
    remove("temp.dot");

    FILE* file = fopen("temp." GRAPHVIZ_CALL_STRING(GRAPHVIZ_CALL_EXTENSION_TYPE), "r");
    if(!file) return DOT_CODE_ERROR;
    fclose(file);

    #ifdef __unix__
    system("temp." GRAPHVIZ_CALL_STRING(GRAPHVIZ_CALL_EXTENSION_TYPE)
           #ifndef GRAPHVIZ_CALL_LEAVE_TEMP
           " ; rm temp." GRAPHVIZ_CALL_STRING(GRAPHVIZ_CALL_EXTENSION_TYPE)
           #endif
           " &");
    #else
    system("start cmd /c \"temp." GRAPHVIZ_CALL_STRING(GRAPHVIZ_CALL_EXTENSION_TYPE)
           #ifndef GRAPHVIZ_CALL_LEAVE_TEMP
           " && del temp." GRAPHVIZ_CALL_STRING(GRAPHVIZ_CALL_EXTENSION_TYPE)
           #endif
           " \"");
    #endif

    return SUCCESS;
}

#undef GRAPHVIZ_CALL_STRING_INNER
#undef GRAPHVIZ_CALL_STRING

}

#endif