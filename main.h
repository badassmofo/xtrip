/*
 * main.h - xtrip
 * Created by Rusty Shackleford on 24/01/2013.
 * Copyright (c) 2012, Rusty Shackleford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. All advertising materials mentioning features or use of this software
 *     must display the following acknowledgement:
 *     This product includes software developed by the <organization>.
 *  4. Neither the name of the <organization> nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY RUSTY SHACKLEFORD ''AS IS'' AND AN
 *  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL RUSTY SHACKLEFORD BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>

#include <openssl/des.h>
#define _MULTI_THREADED
#include <pthread.h>
#include <errno.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#define ESCAPE_KEY  27

static char char_pool[] = { '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}' };
#define CHAR_POOL_LEN (ARRAY_SIZE(char_pool))

void   print_usage(void);
long   get_time(void);
char*  gen_trip(const char*, size_t);
void   gen_rand_str(char*, size_t);
bool   strcmp_fast(const char*, const char*);
int    str_contains_char(const char*, char);
void   str_to_lower(char*, const char*, size_t);

bool str_contains_normal(const char*, const char*);
bool str_contains_ignore_case(const char*, const char*);
static bool (*str_contains)(const char*, const char*);

void* gen_thread(void*);
void* nstop_gen_thread(void*);
void* search_thread(void*);
bool  thread_quit(pthread_mutex_t*);
void  signal_handle(int signal);

#define DEF_MIN_RND_LEN 3
#define DEF_MAX_RND_LEN 15
#define RAND_STR_LEN(a, b) ((rand() % ((b + 1) - a)) + a)

typedef struct {
	int total, min, max;
} gen_thread_arg;

typedef struct {
	int min, max;
	pthread_mutex_t* mtx;
} nstop_gen_thread_arg;

typedef struct {
	char* needle;
	int min, max;
	pthread_mutex_t* mtx;
} search_thread_arg;

static volatile sig_atomic_t exit_loops = 0;

/* build: clang -Weverything -O3 main.c -o ~/bin/xtrip -pthread -lcrypto  */
