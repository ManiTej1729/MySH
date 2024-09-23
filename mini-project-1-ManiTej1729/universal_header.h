#ifndef UNIVERSAL_HEADER
#define UNIVERSAL_HEADER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/stat.h>
#include <ctype.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <termios.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <netdb.h>

#include "main/main.h"
#include "spec1/spec1.h"
#include "spec2/spec2.h"
#include "spec3/spec3.h"
#include "spec4/spec4.h"
#include "spec5/spec5.h"
#include "spec6/spec6.h"
#include "spec7/spec7.h"
#include "spec8/spec8.h"
#include "spec9/spec9.h"
#include "spec10_11_12/spec10_11_12.h"
#include "spec13/spec13.h"
#include "spec14/spec14.h"
#include "spec16/spec16.h"
#include "spec17/spec17.h"
#include "utils/utils.h"

#define MAX_STR_LEN 4097

extern int curr_fg_proc;

extern char home_path[MAX_STR_LEN];

#endif
