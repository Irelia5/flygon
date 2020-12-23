#ifndef FLYGON_BASE_MUTEX_H
#define FLYGON_BASE_MUTEX_H

#include "flygon/base/noncopyable.h"
#include "flygon/base/CurrentThread.h"
#include <pthread.h>
#include <assert.h>

#ifdef CHECK_PTHREAD_RETURN_VALUE

#ifdef NDEBUG
__BEGIN_DECLS
extern void __assert_perror_fail (int errnum,
                                  const char *file,
                                  unsigned int line,
                                  const char *function)
    noexcept __attribute__ ((__noreturn__));
__END_DECLS
#endif

#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       if (__builtin_expect(errnum != 0, 0))    \
                         __assert_perror_fail (errnum, __FILE__, __LINE__, __func__);})

#else  // CHECK_PTHREAD_RETURN_VALUE

#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       assert(errnum == 0); (void) errnum;})

#endif // CHECK_PTHREAD_RETURN_VALUE

namespace flygon 
{

class MutexLock : noncopyable
{
public:
    MutexLock()
		: holder_(0)
	{
		MCHECK(pthread_mutex_init(&mutex_, NULL));
	}

	~MutexLock()
	{
		assert(holder_ == 0);
		MCHECK(pthread_mutex_destroy(&mutex_));
	}

	bool isLockedByThisThread()
	{
		return holder_ == CurrentThread::tid();
	}

	void assertLocked()
	{
		assert(isLockedByThisThread());
	}

	void lock() // 仅供MutexLockGuard调用，严禁用户代码使用
	{
		MCHECK(pthread_mutex_lock(&mutex_));		//这两行顺序不能反
		assignHolder();
	}

	void unlock()
	{
		unassignHolder();					//这两行顺序不能反
		MCHECK(pthread_mutex_unlock(&mutex_));
	}

	pthread_mutex_t* getPthreadMutex() 		//仅供 Condition 调用，严禁用户代码调用
	{
		return &mutex_;
	}

	private:
	friend class Condition;

	class UnassignGuard : noncopyable
	{
	public:
		explicit UnassignGuard(MutexLock& owner)
			: owner_(owner)
		{
			owner_.unassignHolder();
		}

		~UnassignGuard()
		{
			owner_.assignHolder();
		}
	private:
		MutexLock& owner_;
	};

	//这两个接口不给外部用，只给Mutex和Condition(friend)
	void unassignHolder()
	{
		holder_ = 0;
	}

	void assignHolder()
	{
		holder_ = CurrentThread::tid();
	}


private:
  pthread_mutex_t mutex_;
  pid_t holder_;
};

class MutexLockGuard : noncopyable
{
public:
	explicit MutexLockGuard(MutexLock& mutex)
		: mutex_(mutex)
	{
		mutex_.lock();
	}

	~MutexLockGuard()
	{
		mutex_.unlock();
	}
private:
	MutexLock& mutex_; 
};

#define MutexLockGuard(x) error "missing mutex guard var name" 


} // end of namespace flygon


#endif //FLYGON_BASE_MUTEX_H