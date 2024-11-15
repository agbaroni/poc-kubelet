#include <sys/capability.h>
#include <linux/prctl.h>
#include <sys/prctl.h>
#include <linux/securebits.h>
#include <sys/stat.h>
#include <cerrno>
#include <sys/syscall.h>
#include <fstream>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sstream>
#include <linux/sched.h>
#include <sched.h>
#include <unistd.h>

class cgroup final {
    public:
	static std::string root;

	cgroup();
	~cgroup();

	unsigned long long get_fd() {
	    return m_fd;
	}

    private:
	unsigned long long m_fd;
};

std::string cgroup::root = "/sys/fs/cgroup/user.slice/user-1000.slice/user@1000.service/user.slice/app";

cgroup::cgroup() {
    mkdir(root.data(), S_IRWXU | S_IRWXG);

    m_fd = open(root.data(), 0);
}

cgroup::~cgroup() {
    close(m_fd);

    rmdir(root.data());
}

int main(int argc, char* argv[]) {
    prctl(PR_SET_SECUREBITS, SECBIT_KEEP_CAPS);

    if (setuid(1000) < 0) {
	perror("Error");
	return -1;
    }

    cgroup app_cg;
    struct clone_args cl_args {
	.flags = CLONE_INTO_CGROUP | CLONE_NEWPID,
	.cgroup = app_cg.get_fd()
    };
    auto app_pid = syscall(SYS_clone3, &cl_args, sizeof(struct clone_args));

    if (app_pid < 0) {
	std::perror("Error");
    } else if (app_pid == 0) {
	/*std::stringstream ss;
	std::ofstream of;

	ss << cgroup::root << "/cgroup.procs";

	of.open(ss.str(), std::ios::app);
	of << 0;
	of.close();*/

	execve("_build/app", nullptr, nullptr);
    } else {
	std::cout << "app PID: " << app_pid << '\n';
	sleep(3);
    }

    return 0;
}
