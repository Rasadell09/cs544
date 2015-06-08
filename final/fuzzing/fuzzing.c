#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/random.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <asm/uaccess.h>

//fuzzing loop times
#define _NUM 50

//Store the 0xc17882c0 in this pointer as the sys_call_table
void **sys_call_table;
mm_segment_t old_fs;

void fuzzing_test_umask(void);
void fuzzing_test_chdir(void);
void fuzzing_test_mkdir(void);
void fuzzing_test_rmdir(void);
void fuzzing_test_open(void);
void fuzzing_test_create(void);
void fuzzing_test_getcwd(void);
int rand_num(int s, int e, int flag);
void rand_string(char *des, int size, int flag);

//Define syscalls
asmlinkage int (*umask)(int);
asmlinkage int (*chdir)(char *);
asmlinkage int (*mkdir)(char *, int);
asmlinkage int (*rmdir)(char *);
asmlinkage int (*open)(char *, int, int);
asmlinkage int (*create)(char *, int);
asmlinkage int (*getcwd)(char *, int);

static int fuzzing_init(void)
{
	printk(KERN_ALERT "####################Fuzzing is running!####################\nPlease wait a moment.\n\n\n");

	//Get sys_call_table from c17882c0 which is live in System.map
	sys_call_table = (void *)0xc17882c0;

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	
	//Call every fuzzing functon
	fuzzing_test_umask();
	fuzzing_test_chdir();
	fuzzing_test_mkdir();
	fuzzing_test_rmdir();
	fuzzing_test_open();
	fuzzing_test_create();
	fuzzing_test_getcwd();
	
	return 0;
}

static void fuzzing_exit(void)
{
	set_fs(old_fs);
	printk(KERN_ALERT "####################Fuzzing has done!####################\n");
}

void fuzzing_test_umask(void)
{
	int i = 0;

	//Get umask from sys_call_table
	umask = (void *)sys_call_table[__NR_umask];

       	printk(KERN_ALERT "--------------------Start fuzzing test: sys_umask.--------------------\n");

	//Negative numbers
	printk(KERN_ALERT "Test with negative numbers\n");
	for(i = 0; i < _NUM; i++) {
		int m = rand_num(-32000, -1, 0);
		int v = 0;
		v = umask(m);
		printk(KERN_ALERT "%d's m is %d\t v is %d.\n", i, m, v);
	}

	//The numbers which bigger than o777
	printk(KERN_ALERT "\nTest with numbers bigger than o777\n");
	for(i = 0; i < _NUM; i++) {
		int m = rand_num(512, 32000, 1);
		int v = 0;
		v = umask(m);
		printk(KERN_ALERT "%d's m is %d\t v is %d.\n", i, m, v);
	}
	
	printk(KERN_ALERT "---------------------Done fuzzing test: sys_umask.--------------------\n\n\n");
}

void fuzzing_test_chdir(void)
{
	int i = 0;

	//Get chdir from sys_call_table
	chdir = (void *)sys_call_table[__NR_chdir];

	printk(KERN_ALERT "---------------------Start fuzzing test: sys_chdir.--------------------\n");

	//Pure random string
	printk(KERN_ALERT "Test with pure random string\n");
	for(i = 0; i < _NUM; i++) {
		char dir[50] = {'\0'};
		int v = 0;
		rand_string(dir, 50, 0);
		v = chdir(dir);
		printk(KERN_ALERT "%d's dir is: %s\nsys_chdir return value is %d\n", i, dir, v);
	}

	//Generate string looks like a legal directory string
	printk(KERN_ALERT "\nTest with a string looks like a legal directory\n");
	for(i = 0; i < _NUM; i++) {
		char dir[50] = {'\0'};
		int v = 0;
		rand_string(dir, 50, rand_num(1,7,1));
		v = chdir(dir);
		printk(KERN_ALERT "%d's dir is: %s\nsys_chdir return value is %d\n", i, dir, v);
	}
	
	printk(KERN_ALERT "---------------------Done fuzzint test: sys_chdir.--------------------\n\n\n");
}

void fuzzing_test_mkdir(void)
{
	int i = 0;
	char *testdir = "mkdir_test";
	
	//Get mkdir from sys_call_table
       	mkdir = (void *)sys_call_table[__NR_mkdir];

	//Generate testdir and change into it
	umask(0);
	mkdir(testdir, 0644);
	chdir(testdir);
	
	printk(KERN_ALERT "---------------------Start fuzzing test: sys_mkdir.--------------------\n");

	//Pure random string
	printk(KERN_ALERT "Test with pure random string\n");
	for(i = 0; i < _NUM; i++) {
		char dir[50] = {'\0'};
		int v = 0;
		int mode = rand_num(-32000,-1,0);
		rand_string(dir, 50, 0);
		v = mkdir(dir,mode);
		printk(KERN_ALERT "%d's dir is: %s, mode is: %d\nsys_mkdir return value is %d\n", i, dir, mode, v);
	}
	
	//Generate string looks like a legal directory string
	printk(KERN_ALERT "\nTest with a string looks like a legal directory\n");
	for(i = 0; i < _NUM; i++) {
		char dir[50] = {'\0'};
		int v = 0;
		int mode = rand_num(1,32000,1);
		rand_string(dir, 50, rand_num(1,7,1));
		v = mkdir(dir,mode);
		printk(KERN_ALERT "%d's dir is: %s, mode is: %d\nsys_mkdir return value is %d\n", i, dir, mode, v);
	}

	chdir("..");
	
	printk(KERN_ALERT "---------------------Done fuzzint test: sys_mkdir.--------------------\n\n\n");
}

void fuzzing_test_rmdir(void)
{
	int i = 0;

	//Get rmdir from sys_call_table
       	rmdir = (void *)sys_call_table[__NR_rmdir];

	printk(KERN_ALERT "---------------------Start fuzzing test: sys_rmdir.--------------------\n");

	//Pure random string
	printk(KERN_ALERT "Test with pure random string\n");
	for(i = 0; i < _NUM; i++) {
		char dir[50] = {'\0'};
		int v = 0;
		rand_string(dir, 50, 0);
		v = rmdir(dir);
		printk(KERN_ALERT "%d's dir is: %s\nsys_rmdir return value is %d\n", i, dir, v);
	}

	//Generate string looks like a legal directory string
	printk(KERN_ALERT "\nTest with a string looks like a legal directory\n");
	for(i = 0; i < _NUM; i++) {
		char dir[50] = {'\0'};
		int v = 0;
		rand_string(dir, 50, rand_num(1,7,1));
		v = rmdir(dir);
		printk(KERN_ALERT "%d's dir is: %s\nsys_rmdir return value is %d\n", i, dir, v);
	}
	
	printk(KERN_ALERT "---------------------Done fuzzint test: sys_rmdir.--------------------\n\n\n");
}

void fuzzing_test_open(void)
{
	int i = 0;
	char *testdir = "open_test";
	
	//Get open from sys_call_table
       	open = (void *)sys_call_table[__NR_open];

	//Generate testdir and change into it
	umask(0);
	mkdir(testdir, 0644);
	chdir(testdir);
	
	printk(KERN_ALERT "---------------------Start fuzzing test: sys_open.--------------------\n");

	//Pure random string, negative flags and negative mode
	printk(KERN_ALERT "Test with random filename, negative flags and negative mode\n");
	for(i = 0; i < _NUM; i++) {
		char filename[50] = {'\0'};
		int flags = rand_num(-32000,-1,0);
		int mode = rand_num(-32000,-1,0);
		int v = 0;
		rand_string(filename, 50, 0);
		v = open(filename, flags, mode);
		printk(KERN_ALERT "%d's filename is: %s\tflags is: %d\tmode is: %d\nsys_open return value is %d\n", i, filename, flags, mode, v);
	}

	//Pure random string, flags and mode look like legal
	printk(KERN_ALERT "\nTest with random filename, flags and mode look like normal number but semantically incorrect\n");
	for(i = 0; i < _NUM; i++) {
		char filename[50] = {'\0'};
		int flags = rand_num(1,32000,0);
		int mode = rand_num(1,32000,0);
		int v = 0;
		rand_string(filename, 50, 0);
		v = open(filename, flags, mode);
		printk(KERN_ALERT "%d's filename is: %s\tflags is: %d\tmode is: %d\nsys_open return value is %d\n", i, filename, flags, mode, v);
	}

	chdir("..");

	printk(KERN_ALERT "---------------------Done fuzzint test: sys_open.--------------------\n\n\n");
}

void fuzzing_test_create(void)
{
	int i = 0;
	char *testdir = "creat_test";

	//Generate testdir and change into it
	umask(0);
	mkdir(testdir, 0644);
	chdir(testdir);
	
	//Get create from sys_call_table
       	create = (void *)sys_call_table[__NR_creat];

	printk(KERN_ALERT "---------------------Start fuzzing test: sys_create.--------------------\n");

	//Pure random string, negative mode
	printk(KERN_ALERT "Test with random filename and negative mode\n");
	for(i = 0; i < _NUM; i++) {
		char filename[50] = {'\0'};
		int mode = rand_num(-32000, -1, 0);
		int v = 0;
		rand_string(filename, 50, 0);
		v = create(filename, mode);
		printk(KERN_ALERT "%d's filename is: %s\tmode is: %d\nsys_creat return value is %d\n", i, filename, mode, v);
	}

	//Pure random string, mode looks like legal
	printk(KERN_ALERT "\nTest with random filename and some maybe legal mode\n");
	for(i = 0; i < _NUM; i++) {
		char filename[50] = {'\0'};
		int mode = rand_num(1, 32000, 1);
		int v = 0;
		rand_string(filename, 50, 0);
		printk(KERN_ALERT "%d's filename is: %s \t mode is %d\nsys_creat return value is %d\n", i, filename, mode, v);
	}

	chdir("..");
	
	printk(KERN_ALERT "---------------------Done fuzzint test: sys_create.--------------------\n\n\n");
}

void fuzzing_test_getcwd(void)
{
       	int i = 0;

	//Get chdir from sys_call_table
	getcwd = (void *)sys_call_table[__NR_getcwd];

	printk(KERN_ALERT "---------------------Start fuzzing test: sys_getcwd.--------------------\n");

	//Pure random string, negative size
	printk(KERN_ALERT "Test with random string and negative size\n");
	for(i = 0; i < _NUM; i++) {
		char dir[50] = {'\0'};
		int size = rand_num(-32000,-1,0);
		int v = 0;
		rand_string(dir, 50, 0);
		v = getcwd(dir, size);
		printk(KERN_ALERT "%d's dir is %s\tsize is %d\nsys_getcwd return value is %d\n", i, dir, size, v);
	}

	//NULL pointer, positive size
	printk(KERN_ALERT "\nTest with NULL pointer and positive size\n");
	for(i = 0; i < _NUM; i++) {
		char *dir = NULL;
		int size = rand_num(1,32767,1);
		int v = getcwd(dir, size);
		printk(KERN_ALERT "%d's dir is %s\tsize is %d\nsys_getcwd return value is %d\n", i, dir, size, v);
	}
	
	printk(KERN_ALERT "---------------------Done fuzzint test: sys_getcwd.--------------------\n\n\n");
}

int rand_num(int s, int e, int flag)
{
	int result = 0, i = 0;

	get_random_bytes(&i, 2);
	result = (i%(e-s+1))+s; 

	if(flag) {
		if(result < 0) {
			result = 0 - result;
		}
	}
	
	return result;
}

void rand_string(char *des, int size, int flag)
{
	int i = 0;
	
	static const char text[] = "_.abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	for(i = 0; i < size; i++) {
		des[i] = text[rand_num(0,54,1)];
	}	
	des[i] = '\0';

	for(i = 0; i < flag; i++) {
		des[rand_num(0,size-1,1)] = '/';
	}
}

module_init(fuzzing_init);
module_exit(fuzzing_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YUNFAN LI");
MODULE_DESCRIPTION("HELLO WORLD TEST MODULE");

