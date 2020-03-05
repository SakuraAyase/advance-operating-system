#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>

static pid_t pid=1;

module_param(pid,int,0644);



struct process
{
	char* cur;
	int pid;
	char* name;
	int state;
};
 
struct process a[512];
int counter = 0;

static int hello_init(void)
{
    
    struct task_struct *p;
    struct list_head *pp;
    struct task_struct *psibling;

    // 当前进程的 PID
/*if(psibling->state == 0)
       statee = "running";
    else if(psibling->state == 1)
       statee = "interruptable";
    else if(psibling->state == 2)
       statee = "uninterruptable";
    else
       statee = "dead or others";*/
    p = pid_task(find_vpid(pid), PIDTYPE_PID);
   
    a[counter].cur = "me";
    a[counter].pid = p->pid;
    a[counter].name = p->comm;
    a[counter].state = p->state;
    counter++;
    
    // 父进程
    if(p->parent != NULL) {
        
   	a[counter].cur = "parent";
    	a[counter].pid = p->parent->pid;
    	a[counter].name = p->parent->comm;
    	a[counter].state = p->parent->state;
        counter++;
    }

  
    // 子进程
    list_for_each(pp, &p->children)
    {
        psibling = list_entry(pp, struct task_struct, sibling);
        
	a[counter].cur = "children";
    	a[counter].pid = psibling->pid;
    	a[counter].name = psibling->comm;
    	a[counter].state = psibling->state;
        counter++;
        if(counter==512)
           break;
        
    }

    return 0;
}

static void hello_exit(void)
{
    int i=0;
    while(i<counter)
    {
	char* statee;
        if(a[i].state == 0)
           statee = "running";
    	else if(a[i].state == 1)
      	   statee = "interruptable";
    	else if(a[i].state == 2)
           statee = "uninterruptable";
    	else
           statee = "dead or others";
        printk("%s %d %s %s \n", a[i].cur, a[i].pid, a[i].name, statee);
	i++;
    }
   
    printk(KERN_ALERT"exit final!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");

