/*************************************************************************
	> File Name: moduleswitch.c
	> Author: 
	> Mail: 
	> Created Time: 2016年04月11日 星期一 09时30分04秒
 ************************************************************************/

#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
#include "dboperate.h"
	#include "assist.h"
#include "moduleswitch.h"
int loadmodule(void)
{
	int rts=0;
	int index=0;
	char command[1024]= {0};
    sprintf(command, "SELECT * FROM syssuperuser_modulestatus");
    FetchRtePtr sqlval = sql_query(command);
    for(index=0; index<sqlval->row; index++)
    {
    	if((sqlval->dataPtr)[index][2]!=NULL)/*status not null*/
    	{
    		if(*((sqlval->dataPtr)[index][2])=='1')
    		{
    			if((sqlval->dataPtr)[index][1]!=NULL)
    			{
    				if(strcmp((sqlval->dataPtr)[index][1],"keywords")==0)
    					rts|=KEYWORDS_SWITCH;
    				else if(strcmp((sqlval->dataPtr)[index][1],"keywordclass")==0)
    					rts|=KEYCLASS_SWITCH;
    				else if(strcmp((sqlval->dataPtr)[index][1],"url")==0)
    					rts|=URL_SWITCH;
    				else if(strcmp((sqlval->dataPtr)[index][1],"anti_spam")==0)
    					rts|=SPAM_SWITCH;
    				else if(strcmp((sqlval->dataPtr)[index][1],"anti_virus")==0)
    					rts|=VIRUS_SWITCH;
    			}
    			
    		}
    	}
    }
    free_memory(sqlval);
    return rts;
}
