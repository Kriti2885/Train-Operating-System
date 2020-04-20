/*
*Name :	 	Kriti Upadhyaya
*SFSU ID:	918822299
*/

#include <kernel.h>
#define sleep_time 3
#define zamboni_time 30
// **************************
// run the train application
// **************************


void str_concat(char* str1, char* str2)
{
	int i = strlen(str1);
	int j = 0;
	int total_length = strlen(str1)+strlen(str2);
	while(i<total_length)
	{
		str1[i]=str2[j];
		i++;
		j++;
	}
}

/*
*The function sends command to COM port to change the switch.
*/

void set_switch(int train_window, char* color, char* switch_num)
{
	COM_Message message;
	char command[20]="\0";
	str_concat(command,"M");
	str_concat(command, switch_num);
	str_concat(command, color);
	str_concat(command, "\015");
	wm_print(train_window, "Changing switch: %s\n", command);
	//sleep(sleep_time);
	message.output_buffer=command;
	message.len_input_buffer = 0;
	message.input_buffer = NULL;
	send(com_port, &message);
	sleep(sleep_time);
}

/* 
* Function initializes the outer loop switches to ensure that the zamboni runs in 
* the outer loop. The switches are initialized by calling set_switch function.
*/

void initialize_switch(int train_window)
{
	wm_print(train_window, "Initializing the outer switches...\n");
	set_switch(train_window, "G","4");
	set_switch(train_window, "G","5");
	set_switch(train_window, "G","8");
	set_switch(train_window, "R","9");
	set_switch(train_window, "G","1");
	wm_print(train_window, "Switches Initialized for Zamboni to run in the outer loop!\n");	
}

/*
* The function clears memory buffer before every probing command.
*/

void clear_memory_buffer(int train_window)
{
	COM_Message message;
	char cmd[20] = "\0";
	str_concat(cmd, "R");
	str_concat(cmd, "\015");
	message.output_buffer=cmd;
	message.len_input_buffer = 0;
	message.input_buffer = NULL;
	send(com_port, &message);
	sleep(sleep_time);
	wm_print(train_window, "Cleared buffer\n");	
}

/*
*The function probes the segment given in command for presence of train.
*/

int probe_track(int train_window, char* probe_cmd)
{
	char received_msg[3];
	char input_cmd[20] ="\0";
	clear_memory_buffer(train_window);
	str_concat(input_cmd, probe_cmd);
	str_concat(input_cmd, "\015");
	
	wm_print(train_window, "Probing track.. %s\n", probe_cmd);
		
	COM_Message probe_msg;
	probe_msg.output_buffer = input_cmd;
	probe_msg.input_buffer= received_msg;
	probe_msg.len_input_buffer=3;
	send(com_port, &probe_msg);
	sleep(sleep_time);
	//wm_print(train_window, "Received: %d\n", received_msg[1]);
	if (received_msg[1]=='1')
		return 1;
	else
		return 0;
}

/*
* The function is called to find which configuration has been selected by the user.
*/

int find_configuration(int train_window)
{
	int config = 0;
	wm_print(train_window, "Probing for configuration...\n");

	//Probing segments for train.
	if((probe_track(train_window, "C12")==1)&&(probe_track(train_window,"C5")==1))
			config = 1;
	
	else if((probe_track(train_window,"C11")==1) &&(probe_track(train_window,"C2") == 1))
			config=2;
	
	else if((probe_track(train_window,"C16")==1)&&(probe_track(train_window,"C2")==1))
			config=3;
	
	else if((probe_track(train_window, "C16")==1) &&(probe_track(train_window,"C8")==1))
			config=4;
	
	wm_print(train_window, "The selected configuration is : %d\n", config);
	return config;	
}

/* 
* Probing for presence of zamboni in segment C4.
*/

int find_zamboni(int train_window)
{
	int time = 0, zamboni =0;
	for(time=0;time<zamboni_time;time++)
	{
		zamboni = probe_track(train_window, "C4");
		if(zamboni==1)
			return zamboni;
	}
	return zamboni;
}

/*
* Function to start the train. Train speed is 5.
*/

void start_train(int train_window)
{
	COM_Message msg;
	char command[20]="\0";
	str_concat(command, "L20S5");
	str_concat(command, "\015");
	msg.output_buffer=command;
	msg.input_buffer=NULL;
	msg.len_input_buffer=0;
	send(com_port, &msg);
	sleep(sleep_time);
	wm_print(train_window, "Train is starting...\n");		
}

/*
*Function to stop the train.
*/

void stop_train(int train_window, char* cmd)
{
	COM_Message msg;
	char command[20]="\0";
	str_concat(command, cmd);
	str_concat(command, "\015");
	msg.output_buffer=command;
	msg.input_buffer=NULL;
	msg.len_input_buffer=0;
	send(com_port, &msg);
	//sleep(sleep_time);
	wm_print(train_window, "Train has stopped.\n");				
}

/* 
* Function changes the direction of train. First the train is stopped and then 
* the direction is changed.
*/

void change_train_direction(int train_window)
{
	wm_print(train_window, "Changing the direction of train\n");
	stop_train(train_window, "L20S0");				// stops the train.
	
	COM_Message msg;
	char command[20]="L20D";
	
	str_concat(command, "\015");
	msg.output_buffer=command;
	msg.input_buffer=NULL;
	msg.len_input_buffer=0;
	send(com_port, &msg);						// command to change the direction.
	sleep(sleep_time);
}

/*
* Implementation of Configuration1 for both with and without zamboni.
*/

void configuration1(int train_window, int zamboni)
{
	if (zamboni == 0)
	{
		wm_print(train_window, "Positioning the switches\n");
		//set_switch(train_window, "R", "3");
		//set_switch(train_window, "R","4");
		set_switch(train_window, "R", "5");
		set_switch(train_window, "G", "6");
		set_switch(train_window, "G","7");		
		start_train(train_window);				// the train started.
		while(probe_track(train_window, "C9")==0);		// checking entering and exiting ...
		while(probe_track(train_window,"C9")==1);		// ...of train in segmet C9.
		
		sleep(sleep_time);					// the train reach the wagon.
		sleep(sleep_time);
			
		change_train_direction(train_window);			// the direction of train the changed.
		
		start_train(train_window);				//train started after changing direction.
		set_switch(train_window, "R", "4");
		set_switch(train_window, "R", "3");
		while(probe_track(train_window,"C5")==0);
		stop_train(train_window, "L20S0");			// train reach from where it started.
		sleep(sleep_time);
		wm_print(train_window, "Train successfully reached back.\n");		
		
	}
	else if(zamboni==1)
	{
		set_switch(train_window, "G", "4");
		set_switch(train_window, "G", "5");
		set_switch(train_window, "G", "8");
		set_switch(train_window, "R", "9");
		set_switch(train_window, "G","6");
		set_switch(train_window, "G","7");
		set_switch(train_window, "R", "3");
		while(probe_track(train_window, "C10")==0);		//waiting for zamboni to reach segment 10.
		//set_switch(train_window, "R","4");
		set_switch(train_window, "R", "5");
		start_train(train_window);				//train started
		
		while(probe_track(train_window, "C9")==0);
		set_switch(train_window, "G","5");
		while(probe_track(train_window,"C9")==1);		// checking for the train to exit segment 9.
		sleep(sleep_time);
		sleep(sleep_time);					// train reach the wagon. 
		change_train_direction(train_window);			//changing the direction of train.
		
		while(probe_track(train_window,"C10")==0);		//checking for zamboni to reach segment 10.
		set_switch(train_window, "R", "5");
		start_train(train_window);				// starting train for journey back.
		
		set_switch(train_window, "R","4");
		while(probe_track(train_window,"C6")==0);
		set_switch(train_window,"G", "5");
		while(probe_track(train_window, "C6")==1);		//changing train to reach segment 6.
		stop_train(train_window, "L20S0");		
		sleep(sleep_time);			
		wm_print(train_window, "Train successfully reached back.\n");		
	}
}

/* 
* Implementation fo config 2.
*/

void configuration2(int train_window, int zamboni)
{
	if (zamboni == 0)
	{
		wm_print(train_window, "Positioning the switches");
		
		
		set_switch(train_window, "R", "8");			//Configure all switches
		set_switch(train_window, "R", "9");
		set_switch(train_window, "R", "1");
		set_switch(train_window, "G","2");
		
		change_train_direction(train_window);			//Starting the train
		start_train(train_window);
		
		while(probe_track(train_window, "C1")==0);
		while(probe_track(train_window,"C1")==1);		//Probing track to check train movement.

		//sleep(sleep_time);
		
		change_train_direction(train_window);			//Train got wagon..
		start_train(train_window);
		
		while(probe_track(train_window,"C13")==0);		
		while(probe_track(train_window, "C13")==1);		//Probing segment C13 for train movement.
		stop_train(train_window, "L20S0");
		wm_print(train_window, "Train successfully reached back...\n");		
	}
	else if(zamboni==1)
	{
		set_switch(train_window, "G", "2");
		while(probe_track(train_window,"C10")==0);		// Probing segment C10 for zamboni to reach.
		set_switch(train_window, "R","8");
		set_switch(train_window, "G", "2");
		set_switch(train_window,"R", "1");
		change_train_direction(train_window);			//changing train direction ...
		start_train(train_window);				//...and then starting the train.
		
		while(probe_track(train_window,"C1")==0);
		set_switch(train_window, "G","1");
		while(probe_track(train_window, "C1")==1);
		stop_train(train_window, "L20S0");			//train reach wagon.
		set_switch(train_window, "G", "8");
		
		change_train_direction(train_window);			// train direction changed.
		while(probe_track(train_window, "C10")==0);		// probing segment C10 for zamboni presence.
		set_switch(train_window, "R", "1");
		start_train(train_window);				// train started on journey back.
		set_switch(train_window, "R","8");
		while(probe_track(train_window, "C13")==0);
		set_switch(train_window, "G", "1");
		while(probe_track(train_window, "C13")==1);		// checking train movement 
		stop_train(train_window, "L20S0");			// train reached
		set_switch(train_window, "G","8");			// changing switches to ensure zamboni movement in outer loop.
		wm_print(train_window, "Train successfully reached back.");
	}
}

/*
* Implementation of config 3.
*/

void configuration3(int train_window, int zamboni)
{
	if(zamboni ==0)
	{
		wm_print(train_window, "Positioning the switches");
		set_switch(train_window, "G", "9");
		set_switch(train_window, "G", "8");
		set_switch(train_window, "G","5");
		set_switch(train_window, "R", "4");
		set_switch(train_window, "G","3");
		start_train(train_window);				//train started
		while(probe_track(train_window,"C6")==0);
		while(probe_track(train_window,"C6")==1);
		sleep(sleep_time);
		
		//set_switch(train_window, "R", "9");
		change_train_direction(train_window);			//train reached wagon
		start_train(train_window);				//train started after changing the direction.
		while(probe_track(train_window,"C14")==0);
		while(probe_track(train_window,"C14")==1);		// checking for train to reach C14
		stop_train(train_window, "L20S0");			//train reached back.
		wm_print(train_window, "Train successfully reached back!");
	}
	else if(zamboni==1)
	{
		while(probe_track(train_window, "C10")==0);		// waiting for zamboni to reach C10
		//while(probe_track(train_window,"C10")==1);
		set_switch(train_window, "G", "9");
		start_train(train_window);				// train started
		while(probe_track(train_window,"C4")==0);
		while(probe_track(train_window, "C4")==1);		//waiting for zamboni to cross C4.
		set_switch(train_window, "R", "9");
		set_switch(train_window, "R","4");			// changing switches for train.
		set_switch(train_window, "G","3");
		while(probe_track(train_window, "C6")==0);		// checking train reach C6
		while(probe_track(train_window, "C6")==1);
		sleep(sleep_time);
		//sleep(sleep_time);
		change_train_direction(train_window);			// train reached wagon.
		set_switch(train_window,"G", "4");			// changing switches to ensure zamboni is on outer loop.
		while(probe_track(train_window, "C4")==0);		//checking for zamboni to cross C4.
		set_switch(train_window,"R","4");			// changing switch for train.
		start_train(train_window);				// train started
		set_switch(train_window, "R", "5");
		set_switch(train_window, "G", "6");
		set_switch(train_window, "G", "7");
		set_switch(train_window, "R", "2");
		set_switch(train_window, "R", "1");
		while(probe_track(train_window, "C13")==0);
		set_switch(train_window, "G", "5");
		set_switch(train_window, "G", "4");
		while(probe_track(train_window, "C14")==0);		//checking train to reach C14 so that it can be reversed.
		change_train_direction(train_window);
		set_switch(train_window,"G","9");
		start_train(train_window);
		while(probe_track(train_window, "C16")==0);		// checking train to reach C16
		stop_train(train_window, "L20S0");			// train reached station.
		set_switch(train_window,"R", "9");			
		wm_print(train_window, "Train successfully reached back!");	
	}
}

/* 
* Implementation of config 4
*/

void configuration4(int train_window, int zamboni)
{
	if(zamboni==0)
	{
		wm_print(train_window, "Positioning the switches");
		set_switch(train_window, "G", "9");
		set_switch(train_window, "G", "8");
		set_switch(train_window, "G", "5");
		start_train(train_window);
		while(probe_track(train_window, "C7")==0);
		change_train_direction(train_window);
		set_switch(train_window, "R", "5");
		set_switch(train_window, "R", "6");
		start_train(train_window);
		while(probe_track(train_window,"C7")==1);
		sleep(sleep_time);
		sleep(sleep_time);
		//stop_train(train_window, "L20S0");

		change_train_direction(train_window);
		start_train(train_window);
		//change_train_direction(train_window);
		set_switch(train_window, "G", "5");
		set_switch(train_window, "G", "8");
		set_switch(train_window, "G", "9");
		while(probe_track(train_window,"C14")==0);
		change_train_direction(train_window);
		start_train(train_window);
		while(probe_track(train_window,"C14")==1);
		stop_train(train_window, "L20S0");
		wm_print(train_window, "Train successfully reached back!");
	}
	else if(zamboni==1)
	{
		while(probe_track(train_window, "C10")==0);		// waiting for zamboni to reach C10
		start_train(train_window);				
		while(probe_track(train_window, "C7")==0);		// checking for zamboni presence
		while(probe_track(train_window, "C6")==0);
		while(probe_track(train_window,"C7")==0);		// checking for train presence
		change_train_direction(train_window);			// train reached wagon.
		set_switch(train_window,"R", "5");
		set_switch(train_window, "R","6");
		start_train(train_window);
		while(probe_track(train_window, "C7")==1);		// check train presence in C7 to reverse train
		sleep(sleep_time);
		//sleep(sleep_time);
		change_train_direction(train_window);			
		set_switch(train_window, "G", "5");

		while(probe_track(train_window, "C6")==0);
		set_switch(train_window, "R","5");
		start_train(train_window);
		while(probe_track(train_window, "C7")==0);
		set_switch(train_window, "G","5");
		while(probe_track(train_window,"C14")==0);
		while(probe_track(train_window,"C14")==1);
		
		while(probe_track(train_window, "C14")==0);		// waiting for train to reach C14 to reverse it.
		change_train_direction(train_window);
		set_switch(train_window, "G","9");
		start_train(train_window);
		while(probe_track(train_window, "C14")==1);		// waiting for train to come out of C14
		stop_train(train_window, "L20S0");			// train reached.
		set_switch(train_window, "R","9");	
		wm_print(train_window, "Train successfully reached back!");
	}
}

/*
* Implementation of train process. The process starts with initializing the outer switches
* to ensure that if there is zamboni, it runs in the outer loop. The process then check
* the configuration. Followed by looking for zamboni's presence.
* The the process makes necessary call to the particular configuration implementation.
*/

void train_process(PROCESS self, PARAM param)
{
	int train_window = wm_create(10,3,50,17);
	initialize_switch(train_window);
	int configuration, zamboni;
	configuration = find_configuration(train_window);
	zamboni = find_zamboni(train_window);
	if(zamboni==1)
	
		wm_print(train_window, "Found Zamboni\n");
	else
		wm_print(train_window, "Zamboni not found!\n");
	switch(configuration)
	{
		case 1:
			configuration1(train_window, zamboni);
			break;
		case 2:
			configuration2(train_window, zamboni);
			break;
		case 3:
			configuration3(train_window, zamboni);
			break;
		case 4:
			configuration4(train_window, zamboni);
			break;
	}
	wm_print(train_window, "Voila! Config %d Successfull!\n Time to become zombie!!\n", configuration);
	become_zombie();	
	while(1);
}

/*
*Initializing train
*/

void init_train()
{
	create_process(train_process, 5, 0, "Train Process");
	resign();
}
