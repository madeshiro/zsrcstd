#ifndef _Z_Thread_S
#define _Z_Thread_S
#include "zsr_std.h"

#define KILL_SIGTERM 15
#define KILL_SIGKILL 9

#define CSIGNAL                 0x000000ff
#define CLONE_VM                0x00000100
#define CLONE_FS                0x00000200
#define CLONE_FILES             0x00000400
#define CLONE_SIGHAND           0x00000800
#define CLONE_PTRACE            0x00002000
#define CLONE_VFORK             0x00004000
#define CLONE_PARENT            0x00008000
#define CLONE_THREAD            0x00010000
#define CLONE_NEWNS             0x00020000
#define CLONE_SYSVSEM           0x00040000
#define CLONE_SETTLS            0x00080000
#define CLONE_PARENT_SETTID     0x00100000
#define CLONE_CHILD_CLEARTID    0x00200000
#define CLONE_DETACHED          0x00400000
#define CLONE_UNTRACED          0x00800000
#define CLONE_CHILD_SETTID      0x01000000
#define CLONE_NEWCGROUP         0x02000000
#define CLONE_NEWUTS            0x04000000
#define CLONE_NEWIPC            0x08000000
#define CLONE_NEWUSER           0x10000000
#define CLONE_NEWPID            0x20000000
#define CLONE_NEWNET            0x40000000
#define CLONE_IO                0x80000000

#define SIGCHLD 20

#define WNOHANG		0x00000001
#define WUNTRACED	0x00000002
#define WSTOPPED	WUNTRACED
#define WEXITED		0x00000004
#define WCONTINUED	0x00000008
#define WNOWAIT		0x01000000

#define __WNOTHREAD	0x20000000	// Don't wait on children of other threads in this group
#define __WALL		0x40000000	// Wait on all children, regardless of type
#define __WCLONE	0x80000000	// Wait only on non-SIGCHLD children

// First argument to waitid:
#define P_ALL	    0
#define P_PID 	1
#define P_PGID    2

#define FUTEX_PRIVATE_FLAG      128
#define FUTEX_CLOCK_REALTIME	256

#define FUTEX_WAIT              0x0
#define FUTEX_WAKE              0x1
#define FUTEX_FD                0x2
#define FUTEX_REQUEUE           0x3
#define FUTEX_CMP_REQUEUE       0x4
#define FUTEX_WAKE_OP           0x5
#define FUTEX_LOCK_PI           0x6
#define FUTEX_UNLOCK_PI         0x7
#define FUTEX_TRYLOCK_PI        0x8
#define FUTEX_WAIT_BITSET       0x9
#define FUTEX_WAKE_BITSET       0xa
#define FUTEX_WAIT_REQUEUE_PI	0xb
#define FUTEX_CMP_REQUEUE_PI	0xc
#define SIGCHLD 20

#include <sys/types.h>
#include <sys/wait.h>



typedef struct timespec_t timespec_t;
struct timespec_t {
    long int millis;
    long nano;
} in_timespec, out_timespec;

int zsrcall _Z_sys_futex(int *uaddr, int futex_op, int val,
                        const timespec_t* timeout /* or uint32 val2 */,
                        int *uaddr2, int val3);

int zsrcall _Z_sys_clone(unsigned int flags, void* stacks, void* p_tid, void* c_tid,
                         unsigned int tid);

int zsrcall _Z_sys_tkill(unsigned int tid, int sig);
int _Z_sys_nanosleep(timespec_t *rqtp, timespec_t *rmtp);
int _Z_sys_waitid(idtype_t idtype, id_t id, siginfo_t* infop, int options);

#endif // _Z_Thread_S
