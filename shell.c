/*
TOS shell implementation by Kriti Upadhyaya
Fall 2019
*/

#include <kernel.h>
#define BACKSPACE 8
#define SPACE 20
#define ENTER 13
#define MAX_HISTORY 50
	

/**
* The function match the user command with the 
* supported TOS command.
* Function is called from execute_command.
*/

int match_command(char *tos_command, char *user_command)
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

/*
	The function ifhistory is executed when 
*/
void ifhistory(int windowID, int command_num, char (*history) [20], int ifhistory_check)
{
	if(ifhistory_check == 0)
	{
	  for(int j =0;j<=command_num;j++)
		wm_print(windowID, "\n%2d %s", j+1, history[j]);
	}
	else
	{
	  for(int j =0;j<command_num-1;j++)
		wm_print(windowID, "\n%2d %s", j+1, history[j]);
	}
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
		"READY          ",
		"ZOMBIE         ",
		"SEND_BLOCKED   ",
		"REPLY_BLOCKED  ",
		"RECEIVE_BLOCKED",
		"MESSAGE_BLOCKED",
		"INTR_BLOCKED   "
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
			
		if(c>='1' && c<='50')
		{
			number = c-'0';
			history_index = history_index*10+number;
		}
		if(history_index>50)
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
	int check =0, match = 0;
	wm_print(windowID, "\n");
	while(*msg!='\0')
	{
		while(*tos_command!='\0')
		{
			if(*msg==' ')
				msg++;	
		
			else if(*tos_command == *msg)
			{	
				tos_command++;
				msg++;
				match++;
			}
			else if(*tos_command!=*msg)
			
				return 0;
		}
	
		if(match == 4)
		{
			
			wm_print(windowID, "%c", *msg);
			msg++;
			check = 1;
		}
		else
			msg++;
	}	
	
	return check;
}

/**
* The function checks if the command is a !<number> command.
*/

int check_number(char *tos_command, char *user_command)
{
	if(*tos_command=='!' && *user_command=='!')
		return 1;
	else
		return 0;
}


/**
* The function implement !<number> command by finding the command from history buffer and then implementing it.		
*/


void find_command(int windowID, int num, char (*history) [20], int current_command_num)
{
	int i, size =0, multi=0;
	char *command, *cmd_size;
	command = history[num-1];
	cmd_size = command;
	if(*command =='\0')
		wm_print(windowID, "Invalid index. Check history to find correct index.");
	else
	{
		while(*cmd_size!='\0')
		{
			size++;
			cmd_size++;
		}
		multi = check_multiple_commands(windowID, command, size, current_command_num, history);
		if(multi ==0)
		
		   i = execute_command(windowID, command, current_command_num, history);
	}
	if(i==0)
		wm_print(windowID, "Enter a valid command. Check help for more information.");
}

/**
*The function execute_command has implementation of all the commands supported by TOS shell.
*/

int execute_command(int windowID, char *this_command, int command_num, char (*history) [20])
{
	char *command, *run_command;
	run_command = this_command;
	int i=0, j=0, result = 0, ifhistory_check = 0;
	while(run_command[i]!='\0')
	{
		if(run_command[i]!=' ')
			command[j++]=run_command[i];		
		i++;
	}
	command[j] = '\0';
	if(match_command("help", command)==1)
	{
		wm_print(windowID, "\nFollowing commands are supported by TOS:");
		wm_print(windowID, "\nhelp         -Help on commands supported by TOS.");
		wm_print(windowID, "\nclear        -Clear TOS Shell Window.");
		wm_print(windowID, "\nshell        -Starts default terminal for TOS.");
		wm_print(windowID, "\npong         -Starts pong game.");
		wm_print(windowID, "\necho <msg>   -Displays message.");
		wm_print(windowID, "\nps           -Displays all the processes and their states.");
		wm_print(windowID, "\nhistory      -Displays history.");
		wm_print(windowID, "\n!<command>   -Execute the command on given number from history");
		wm_print(windowID, "\nabout        -Information about TOS.");
		result = 1;
	}
	else if(match_command("clear", command)==1)
	{
		wm_clear(windowID);
		result = 1;
	}
	else if(match_command("shell", command)==1)
	{
		start_shell();
		result = 1;
	}
	else if(match_command("pong", command)==1)
	{
		start_pong();
		result = 1;
	}
	
	
	else if(match_command("ps", command)==1)
	{
		ifps(windowID);
		result = 1;
	}	
	
	else if(match_command("history", command)==1)
	{
		ifhistory(windowID, command_num, history, ifhistory_check);
		result = 1;
	}
	
	else if(check_number("!", command)==1)
	{
				
		//wm_print(windowID, "\ncommand num %d", command_num);
		if(command_num==0)
			wm_print(windowID, "\nThis is first command of this shell. Create a command history then run this command.");
		else
		{
			ifhistory_check = 1;
			int num = ifnumber(windowID, command);
			if (num<1 || num >10)
				wm_print(windowID, "\nEnter valid command");
			else if(num>command_num)
				wm_print(windowID, "\nCommand not found. Enter right index");
			else
				find_command(windowID, num, history, command_num);
		}
		return 1;
	}
	else if(match_command("about",command)==1)
	{
		wm_print(windowID, "\nThis is TOS shell.");
		wm_print(windowID, "\nCreated by Kriti Upadhyaya for CSC720.");
		wm_print(windowID, "\nFall 2019");
		result = 1;
	}
	else if(echo(windowID, "echo", run_command)==1)
	{	
		result = 1;
	}
	return result;
	
}


int check_multiple_commands(int windowID, char* command, int command_size, int command_num, char (*history) [20])
{
	char *cmd, *multi;
	multi = malloc(command_size);
	cmd = multi;
	int multiple=0, index = 0,size = 0;

	while(*command!='\0')
	{
		if(*command == ';' && size!=0)
		{
			int result_multi = 0;
			multiple = 1;
			*multi = '\0';
			
			result_multi = execute_command(windowID, cmd, command_num, history);
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
		int result = execute_command(windowID, cmd, command_num, history);
		if (result == 0)
				wm_print(windowID, "Invalid command! Please check help for more information.");
	}	
	return multiple;
}


void get_input(int windowID)
{
  char *command_buffer;
  char *input_char, *traverse;
  char history[MAX_HISTORY][20];
	
	//hist_ptr = history[MAX_HISTORY];
  traverse = malloc(21);
  int command_size = 20, entered_size = 0, command_num=0;
  command_buffer = malloc(21);
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
	 int multiple_commands;
	
	 			
	 if(entered_size>0)
	 {
	     
	     int i=0;
	     
	     /* Maintaining history of the commands that are
		 entered by the user.
	     */

	     
	     if(command_num<MAX_HISTORY)
	     {	
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
		   for(int k =0;k<20;k++)
		     history[j][k]=history[j+1][k];
		}
		for(i=0;i<entered_size;i++)
		{	
		   history[MAX_HISTORY-1][i] = *traverse;	
		   traverse++;
		}
		command_num=MAX_HISTORY-1;
	     }
			
	    /* The TOS shell supports multiple commands as input.
		Checking for multiple commnads seperated by ';'
	    */

	     multiple_commands = check_multiple_commands(windowID, command_buffer, entered_size, command_num, history);
	
	    /*After checking for multiple commands, if the user 
	      input is a single command it will run.	
	    */
	     
	     if (multiple_commands == 0)
	     {
		executed = execute_command(windowID, command_buffer, command_num, history);	
		
		if(executed==0)
		   wm_print(windowID, "\nPlease enter a valid command. Check help for more information");
	      }

	     command_num++;
	entered_size = 0;
	free(command_buffer);
	command_buffer=malloc(21);
	traverse = command_buffer;
	input_char = command_buffer;
	}
   wm_print(windowID, "%s" , "\ntos>>");
   continue;
	
   }
  wm_print(windowID, "%c", *input_char);
  entered_size++;
  input_char++;
			
 }
 else
 {
   wm_print(windowID, "\nBuffer overflow: TOS supports command size less than 20 char");
   free(command_buffer);
   command_buffer=malloc(21);
   traverse = command_buffer;
   input_char = command_buffer;
   entered_size=0;
   wm_print(windowID, "%s" , "\ntos>>");
   continue;
 }
}

}

void shell_process()
{
	int windowID = wm_create(0,5,60,20);
	wm_clear(windowID);
	wm_print(windowID, "Welcome to TOS!\n");
	wm_print(windowID, "tos>>");	
	get_input(windowID);
	
}

void start_shell()
{
	create_process(shell_process, 5, 0, "Shell Process");
	resign();
}
