#include <kernel.h>
#define BACKSPACE 8
#define SPACE 20
#define ENTER 13
#define MAX_HISTORY 4

//char* start_history = history[0][0];
	

int match_command(char *tos_command, char *user_command, int windowID)
{
	
	while(*tos_command==*user_command)
	{
		if(*tos_command=='\0' || *user_command=='\0')
		{
			break;
		}	
			
		tos_command++;
		user_command++;
		
	}
	
	if(*tos_command=='\0' && *user_command=='\0')
		return 1;
	else
		return 0;
}

void ifhistory(int windowID, int command_num, char* history)
{
	for(int j =0;j<=command_num;j++)
		wm_print(windowID, "\n%2d %s", j+1, history[j]);
}

void ifps(int windowID)
{
	int i;
	PCB *p = pcb; 
	wm_print(windowID, "\nState           Active     Prio        Name\n");
	wm_print(windowID, "--------------------------------------------\n");
	for(i=0;i<MAX_PROCS;i++, p++)
	{
		if(!p->used)
			continue;
		static const char *process_state[]={
		"READY		",
		"ZOMBIE		",
		"SEND_BLOCKED	",
		"REPLY_BLOCKED	",
		"RECEIVE_BLOCKED",
		"MESSAGE_BLOCKED",
		"INTR_BLOCKED	"
		};
		
		if(!p->used)
		{	
			wm_print(windowID, "No Process");
			return;
		}
		wm_print(windowID, process_state[p->state]);
		if(p==active_proc)
			wm_print(windowID,"     *   ");
		else
			wm_print(windowID,"	    ");
		wm_print(windowID, "          %2d  ",p->priority);
		wm_print(windowID, " %s\n", p->name);
	}
}

int ifnumber(int windowID, char* command_buffer)
{
	char* user_command;
	user_command = command_buffer;
	//wm_print(windowID, "\n%s", user_command);
	int number = 0, history_index=0;
	//user_command++;
	char c;
	
	while(*user_command!='\0')
	{		
		c = *user_command;
		//wm_print(windowID, "\n%c", c);	
		if(c>='1' && c<='10')
		{
			number = c-'0';
			history_index = history_index*10+number;
		}
		else
			wm_print(windowID, "Enter a valid command index.");
		user_command++;
	}
	//wm_print(windowID, "command num: %d", history_index);
	return history_index;
}

int echo(int windowID, char *tos_command, char *cmd_echo)
{
	char* msg;
	msg = cmd_echo;	
	int check =0;
	while(*msg!='\0' && check==0)
	{
	//wm_print(windowID, "\n%c", *msg);
		if(*msg==' ')
			msg++;
				
		else if(*tos_command=='\0')
		{
			check = 1;
		}	
		
		else if(*tos_command == *msg)
		{	
			tos_command++;
			msg++;
		}
	}

	if(check == 1)
	{
		wm_print(windowID, "\n");
		while(*msg!='\0')
		{
			wm_print(windowID, "%c", *msg);
			msg++;
		}
		return 1;
	}	
	else
		return 0;
}

int check_number(char *tos_command, char *user_command)
{
	if(*tos_command=='!' && *user_command=='!')
		return 1;
	else
		return 0;
}

void find_command(int windowID, int num)
{
	/*char *p;
	char *selected_command = history[num-1];
	p=traverse;
	wm_print(windowID, "\n%s", traverse);
	while(*p!='!')	
	{
		*processed_ptr = *p;
		wm_print(windowID, "loop1:%c", processed_ptr);
		p++;
		processed_ptr++;	
	}
	while(*selected_command!='\0')
	{
		wm_print(windowID, "loop2:%c", processed_ptr);
		*processed_ptr = *selected_command;
		processed_ptr++;
		selected_command++;
	}*/
	//int i = check_multiple_commands(windowID, number_command, size);
						
	//if (i == 0)
	//	ifexecuted = execute_command(windowID, number_command, size);	
	//execute_command(windowID, number_command, size);
	/*while(*number_command!='\0')
	{
		size++;
		number_command++;	
	}
	maintain_history(number_command, size, windowID);*/
	//*processed_ptr = '\0';
	//wm_print(windowID, "\nprocessed: \n%s", processed_ptr);
	
}
int execute_command(int windowID, char *this_command, int size, int command_num, char* history)
{
	wm_print(windowID, "\nhistor exey: %s\n", history);
	char *command, *cmd, *run_command;
	cmd = this_command;
	run_command = cmd;
	int i=0, j=0, result = 0;
	while(run_command[i]!='\0')
	{
		if(run_command[i]!=' ')
			command[j++]=run_command[i];		
		i++;
	}
	command[j] = '\0';
	if(match_command("help", command, windowID)==1)
	{
		wm_print(windowID, "\nFollowing commands are supported by TOS:");
		wm_print(windowID, "\nhelp		-Help on commands supported by TOS.");
		wm_print(windowID, "\nclear		-Clear TOS Shell Window.");
		wm_print(windowID, "\nshell		-Starts default terminal for TOS.");
		wm_print(windowID, "\npong		-Starts pong game.");
		wm_print(windowID, "\necho <msg>	-Displays message.");
		wm_print(windowID, "\nps		-Displays all the processes and their states.");
		wm_print(windowID, "\nhistory		-Displays history.");
		wm_print(windowID, "\n!<command>	-Execute the command on given number from history");
		wm_print(windowID, "\nabout		-Information about TOS.");
		result = 1;
	}
	else if(match_command("clear", command, windowID)==1)
	{
		wm_clear(windowID);
		result = 1;
	}
	else if(match_command("shell", command, windowID)==1)
	{
		start_shell();
		result = 1;
	}
	else if(match_command("pong", command, windowID)==1)
	{
		start_pong();
		result = 1;
	}
	
	
	else if(match_command("ps", command, windowID)==1)
	{
		ifps(windowID);
		result = 1;
	}	
	
	else if(match_command("history", command, windowID)==1)
	{
		ifhistory(windowID, command_num, history);
		result = 1;
	}
	
	/*else if(check_number("!", command)==1)
	{
		//wm_print(windowID, "yes");
		int num = ifnumber(windowID);
		if (num<1 || num >10)
			wm_print(windowID, "\nEnter valid command");
		else
			find_command(windowID, num);
		return 1;
	}*/
	else if(match_command("about",command, windowID)==1)
	{
		wm_print(windowID, "\nThis is TOS shell.");
		wm_print(windowID, "\nCreated by Kriti Upadhyaya for CSC720.");
		wm_print(windowID, "\nFall 2019");
		result = 1;
	}
	/*else if(echo(windowID, "echo", run_command)==1)
	{	
		//wm_print(windowID, "yes");
		result = 1;
	}*/
	return result;
	
}


int check_multiple_commands(int windowID, char* command, int command_size, int command_num, char *history)
{
	char *cmd;
	char *multi;
	multi = malloc(command_size);
	cmd = multi;
	int multiple=0;
	int index = 0,size = 0;
	while(*command!='\0')
	{
		//wm_print(windowID, "\nmulti: %c", *command);
		if(*command == ';' && size!=0)
		{
			int result_multi = 0;
			multiple = 1;
			*multi = '\0';
			
			result_multi = execute_command(windowID, cmd, size, command_num, history);
			if (result_multi == 0)
				wm_print(windowID, "Invalid command : %s", cmd);
			size = 0;
			multi = malloc(command_size-size);
			cmd = multi;
			command++;
		}
		else
		{
		*multi=*command;

		size++;
		command++;
		multi++;
		index++;
		}
	}
	if(multiple==1)
	{
		int result = execute_command(windowID, cmd, size, command_num, history);
		if (result == 0)
				wm_print(windowID, "Invalid command : %s", cmd);
	}	
	return multiple;
}


void get_input(int windowID)
{
  char *command_buffer, *processed_buffer;
  char *input_char, *traverse;
  char history[MAX_HISTORY][20];
	
	//hist_ptr = history[MAX_HISTORY];
  traverse = malloc(20);
  int command_size = 20, entered_size = 0, if_history=0, command_num=0;
  command_buffer = malloc(20);
  input_char = command_buffer;
  traverse = command_buffer;
  int executed;
  while(1)
  {
    if (entered_size<command_size-1)
    {
      *input_char = keyb_get_keystroke(windowID, TRUE);
      switch(*input_char)
      {
	case '!':
	 wm_print(windowID, "%c", *input_char);
	 if_history = 1;
         continue;				
				
        case BACKSPACE:
	 if(entered_size == 0)
	 {
	   continue;
	 }
	 else
	 {
	   wm_print(windowID, "%c", *input_char);
	   input_char--;
	   entered_size--;
	   continue;
	 }

	case ENTER:
	 *input_char = '\0';
	 int command_number, multiple_commands;
					
	 if(entered_size>0)
	 {
	   if(if_history == 1)
	   {
	     command_number = ifnumber(windowID, command_buffer);
	     find_command(windowID, command_number);
	    //entered_size = maintain_history(processed_buffer,entered_size, windowID);
	   }
	   else
	   {
	     
	     int i=0;
	     if(command_num<MAX_HISTORY)
	     {
		wm_print(windowID, "\nsize:%d",entered_size);
		wm_print(windowID, "\nentry : %s", traverse);
			
		for(i=0;i<entered_size;i++)
		{	
		   history[command_num][i] = *traverse;	
		   traverse++;
		}
				
	     }
	     else
	     {
																																		
		for(int j=0;j<MAX_HISTORY;j++)
		{
		   wm_print(windowID, "\nbeforehistory : %s", history[j]);
	
		   for(int k =0;k<20;k++)
		     history[j][k]=history[j+1][k];
		   wm_print(windowID, "\nafterhistory : %s", history[j]);
		}
		for(i=0;i<entered_size;i++)
		{	
		   history[MAX_HISTORY-1][i] = *traverse;	
		   traverse++;
		   
		}
		command_num=MAX_HISTORY-1;
		wm_print(windowID, "\nfinalhistory : %s", history[command_num]);
	     }
			
	     wm_print(windowID, "\nhistory : %s", history[command_num]);
	
	     multiple_commands = check_multiple_commands(windowID, command_buffer, entered_size, command_num, history);
	    //wm_print(windowID,"%d", multiple_commands);
	     if (multiple_commands == 0)
	     {
		executed = execute_command(windowID, command_buffer, entered_size, command_num, history);	
		wm_print(windowID, "if Executed: %d",executed);
		if(executed==0)
		   wm_print(windowID, "\nPlease enter a valid command. Check help for more information");
	      }
	}
						
						
	command_num++;
	//history->command_num++;
	entered_size = 0;
	free(command_buffer);
	command_buffer=malloc(20);
	traverse = command_buffer;
	input_char = command_buffer;
	}
   wm_print(windowID, "%s" , "\ntos>>");
   continue;

   }
  wm_print(windowID, "%c", *input_char);
  //history->command_history[entered_size] = command_buffer[entered_size];
  entered_size++;
  input_char++;
			
 }
 else
 {
   wm_print(windowID, "Buffer overflow: TOS supports command size less than 20 char");
   break;
 }
}
}

void shell_process()
{
	int windowID = wm_create(0,10,40,20);
	wm_clear(windowID);
	wm_print(windowID, "Welcome to TOS!\n");
	wm_print(windowID, "tos>>");
	//clear_history();	
	get_input(windowID);
	
}

void start_shell()
{
	create_process(shell_process, 5, 0, "Shell Process");
	resign();
}
