#include <sys/mman.h>
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

static bool* ok = nullptr;

int main(int argc, char* argv[]) {
    cgroup app_cg;
    int shm_fd;
    struct clone_args cl_args {
	.flags = CLONE_INTO_CGROUP | CLONE_NEWPID | CLONE_NEWUSER,
	.cgroup = app_cg.get_fd()
    };
    int app_pid;

    shm_fd = shm_open("/ok", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (shm_fd < 0) {
	perror("Error");
	return -1;
    }

    if (ftruncate(shm_fd, sizeof(bool)) < 0) {
	perror("Error");
	shm_unlink("/ok");
	return -1;
    }

    ok = reinterpret_cast<bool*>(mmap(nullptr, sizeof(bool), PROT_READ | PROT_WRITE, MAP_SHARED_VALIDATE, shm_fd, 0));
    *ok = false;

    app_pid = syscall(SYS_clone3, &cl_args, sizeof(struct clone_args));

    if (app_pid < 0) {
	std::perror("Error");
    } else if (app_pid == 0) {
	shm_fd = shm_open("/ok", O_RDWR, 0);
	if (shm_fd < 0) {
	    perror("Error (0)");
	    return -1;
	}

	ok = reinterpret_cast<bool*>(mmap(nullptr, sizeof(bool), PROT_READ | PROT_WRITE, MAP_SHARED_VALIDATE, shm_fd, 0));

	while (!*ok);

	shm_unlink("/ok");

	execve("_build/app", nullptr, nullptr);
    } else {
	std::stringstream ssu;
	std::stringstream ssg;
	std::stringstream stg;
	std::ofstream ofs;

	ssu << "/proc/" << app_pid << "/uid_map";
	ssg << "/proc/" << app_pid << "/gid_map";
	stg << "/proc/" << app_pid << "/setgroups";

	ofs.open(ssu.str());
	ofs << "0 " << getuid() << " 1";
	ofs.close();

	ofs.open(stg.str());
	ofs << "deny";
	ofs.close();

	ofs.open(ssg.str());
	ofs << "0 " << getgid() << " 1";
	ofs.close();

	*ok = true;

	std::cout << "========== It ==========\n";
	std::cout << "PID: " << app_pid << '\n';
	std::cout << "========================\n";
	sleep(60);

	shm_unlink("/ok");
    }

    return 0;
}
