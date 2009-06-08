/*
 * This file is part of FreeHAL 2009.
 *
 * Copyright(c) 2006, 2007, 2008, 2009 Tobias Schulz and contributors.
 * http://freehal.org
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/


#include "aw.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>

#include "hal2009.h"

#include <getopt.h>


int entered=0,entercode=0;
void *in1, *in2;
void handle_avatar_add (void);
void handle_chat (void);
void handle_enter (int rc);
char *name="FreeHAL";
char last_speaker[10000];

#define OPTION_SIZE 512

char* programming_language;
char* base_dir;
char* language;

int main (int argc, char *argv[])
{
    programming_language = calloc(OPTION_SIZE + 1, 1);
    base_dir = calloc(OPTION_SIZE + 1, 1);
    language = calloc(OPTION_SIZE + 1, 1);
    sql_engine = calloc(OPTION_SIZE + 1, 1);
    char* exe_name = argv[0];
    int c;

    strcpy(programming_language, "perl5");
    strcpy(language, "de");
    strcpy(base_dir, ".");
    strcpy(sql_engine, "sqlite");

    strcpy(last_speaker, "");
    
    {
        char* sqlite_filename = calloc(OPTION_SIZE + 1, 1);
        strcat(sqlite_filename, base_dir);
        strcat(sqlite_filename, "/lang_");
        strcat(sqlite_filename, language);
        strcat(sqlite_filename, "/database.db");
        sql_sqlite_set_filename(sqlite_filename);
    }

    hal2009_clean();
    chdir(base_dir);
    extract();

    fprintf(stdout, "Programming language is %s\n", programming_language);
    fprintf(stdout, "Text language is %s\n", language);
    pthread_t signal_thread = hal2009_start_signal_handler(strdup(programming_language), strdup(language), MULTI);
    fprintf(stdout, "Shell started...\nThis is %s.\n\n", FULL_NAME);

    int rc,i;
    void *inst1;
    /* check command line */
    if (argc < 4) {
        printf ("Usage: %s citnumber password world\n", argv[0]);
        exit (1);
    }

    /* initialize Active Worlds API */
    if (rc = aw_init (AW_BUILD)) {
        printf ("Unable to initialize API (reason %d)\n", rc);
        exit (1);
    }

    /* install handler for avatar_add event */
    aw_event_set (AW_EVENT_AVATAR_ADD, handle_avatar_add);
    aw_event_set (AW_EVENT_CHAT, handle_chat);
    aw_callback_set(AW_CALLBACK_ENTER, handle_enter);
    /* create bot instances */
    if (rc = aw_create ("server.nightshift.de", 5670,&inst1)) {
        printf ("Unable to create bot instance (reason %d)\n", rc);
        exit (1);
    }
    /* log bot into the universe */
    aw_int_set (AW_LOGIN_OWNER, atoi (argv[1]));
    aw_string_set (AW_LOGIN_PRIVILEGE_PASSWORD, argv[2]);
    aw_string_set (AW_LOGIN_APPLICATION, "SDK Sample Application #1");
    aw_string_set (AW_LOGIN_NAME, name);
    if (rc = aw_login ()) {
        printf ("Unable to login (reason %d)\n", rc);
        exit (1);
    }
    printf("Logged in\n");

    /* log bot into the world  */
    printf ("Entering to world \n");
    if (rc = aw_enter (argv[3])) {
        printf ("Unable to enter world (reason %d)\n", rc);
        exit (1);
    }
    while (1)
    {
        if (entered)
            break;
        aw_wait(10);
    }
    if (entercode)
    {
        printf("Unable to enter world - entercode=%d\n",entercode);
        exit(1);
    }
    entered=0;
    entercode=0;
    printf ("Entered to world \n");
    /* announce our position in the world */
    aw_int_set (AW_MY_X, 1000); /* 1W */
    aw_int_set (AW_MY_Y, 1000); /* 1a */
    aw_int_set (AW_MY_Z, 1000); /* 1N */
    aw_int_set (AW_MY_YAW, 1800); /* face towards GZ */
    if (rc = aw_state_change ()) {
        printf ("Unable to change state (reason %d)\n", rc);
        aw_destroy();
        aw_term();
        exit (1);
    }
    printf ("State changed \n");

    /* main event loop */
    printf("type q <cr> for quit\nsxxxxx for message xxxx\n");
    time_t starttime = time(NULL);
    while (1)
    {
        fd_set rfds;
        struct timeval tv;
        int retval;
    
        /* Watch stdin (fd 0) to see when it has input. */
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        /* Wait up to 100 mseconds. */
        tv.tv_sec = 0;
        tv.tv_usec = 100000;
    
        retval = select(1, &rfds, NULL, NULL, &tv);
        /* Don't rely on the value of tv now! */
    
        if (retval)
        {
            char c;
            c=getchar();
            if (c == 'q')
            {
                aw_destroy();
                aw_term();
                return 0;
            }
            if (c=='s')
            {
                char buff[2048];
                fgets(buff,1024,stdin);
                aw_say(buff);
            }
        };
        aw_wait(10);

        if (time(NULL) > starttime + 30*60) {
            aw_destroy();
            aw_term();
            execv(argv[0], argv);
        }
    }

    /* close everything down */
    aw_destroy ();
    aw_term ();
    return 0;

}

void handle_avatar_add (void)
{
    char message[100];

    sprintf (message, "Hello %s", aw_string (AW_AVATAR_NAME));
    aw_say (message);
    /* log the event to the console */
    printf ("avatar_add: %s\n", aw_string (AW_AVATAR_NAME));
}
void handle_enter(int rc)
{
	entered=1;
	printf("Enter code=%d\n",rc);
	entercode=rc;
}
static char* chat_type[] = {"Chat ", "Broadcast ", "Whisper "};

void handle_chat(void)
{
	if (aw_string(AW_AVATAR_NAME)[0]!='[')
    	printf("%s %s: %s\n",chat_type[aw_int (AW_CHAT_TYPE)], aw_string(AW_AVATAR_NAME),aw_string(AW_CHAT_MESSAGE));
    while (strlen(last_speaker)) {
        usleep(1000);
    }
    strncpy(last_speaker, aw_string(AW_AVATAR_NAME), 9999);
    ask(aw_string(AW_CHAT_MESSAGE));    
}

void ask(char* message) {
    fprintf(stdout, "Human: ");
    fflush(stdout);
    char* input = message;
    if (0 == input || 0 == strlen(input))
        return;
    char* copy_of_programming_language = (char*)halmalloc(16, "hal2009_server_statement");
    strcpy(copy_of_programming_language, programming_language);
    char* copy_of_language             = (char*)halmalloc(16, "hal2009_server_statement");
    strcpy(copy_of_language,             language);
    char* copy_of_base_dir             = (char*)halmalloc(16, "hal2009_server_statement");
    strcpy(copy_of_base_dir,             base_dir);
    char* copy_of_input                = (char*)halmalloc(1025, "hal2009_server_statement");
    strncpy(copy_of_input,               input, 1024);
    pthread_t answer_thread = hal2009_answer(copy_of_input, copy_of_programming_language, copy_of_language, copy_of_base_dir, NOT_JOIN, MULTI);
    pthread_join(answer_thread, NULL);
}

void hal2009_handle_signal(void* arg) {
    char* type       = ((void**)arg)[0];
    char* text       = ((void**)arg)[1];
    char* start_type = ((void**)arg)[2];
    
    if (0 == strcmp(type, "_output__pos")) {
        fprintf(output(), "\nUnknown part of speech: %s\n\nPlease type it in: ", text);
        scanf("%s", text);
        FILE* target = fopen("_input__pos", "w+b");
        halwrite(text, 1, strlen(text), target);
        halclose(target);
    }
    else if (0 == strcmp(type, "_output__link")) {
        if (strlen(text) < 99) {
            char link[99] = {0};
            int f1 = 0;
            int f2 = 0;
            sscanf(text, "%98[a-zA-Z]%d\n%d", &link, &f1, &f2);
            fprintf(output(), "--%s--\n%s\n%i\n%i", text, link, f1, f2);
            hal2009_add_link(link, f1, f2);
        }
    }
    else if (0 == strcmp(type, "_output__add_pro_file")) {
        hal2009_add_pro_file(text);
        FILE* target = fopen("_input__add_pro_file", "w+b");
        halclose(target);
    }
    else if (0 == strcmp(type, "_output__get_csv")) {
        struct DATASET set = hal2009_get_csv(text);
        const char* csv_data = hal2009_make_csv(&set);
        FILE* target = fopen("_input__get_csv", "w+b");
        halwrite(csv_data, 1, strlen(csv_data), target);
        halclose(target);
        fprintf(output(), "Release memory now.\n");
        free(csv_data);
        fprintf(output(), "Memory is released.\n");
    }
    else if (0 == strcmp(type, "_output")) {
        fprintf(output(), "\nFreeHAL: %s\n", text);
        char* message = calloc(100000, 1);
        sprintf (message, "%s: %s", last_speaker, text);
        strcpy(last_speaker, "");
        aw_say (message);
        free(message);
    }
    else if (0 == strcmp(type, "_exit") && start_type == SINGLE) {
        exit(0);
    }
}

