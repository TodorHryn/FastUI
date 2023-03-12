#include "Util.h"
#include "resource.h"
#include <vector>

/*
https://stackoverflow.com/questions/13397571/precise-thread-sleep-needed-max-1ms-error
*/
#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>

#else
#  include <time.h>
#  include <errno.h>

#  ifdef __APPLE__
#    include <mach/clock.h>
#    include <mach/mach.h>
#  endif
#endif // _WIN32

/**********************************=> unix ************************************/
#ifndef _WIN32
void SleepInMs(uint32 ms) {
	struct timespec ts;
	ts.tv_sec = ms / 1000;
	ts.tv_nsec = ms % 1000 * 1000000;

	while (nanosleep(&ts, &ts) == -1 && errno == EINTR);
}

void SleepInUs(uint32 us) {
	struct timespec ts;
	ts.tv_sec = us / 1000000;
	ts.tv_nsec = us % 1000000 * 1000;

	while (nanosleep(&ts, &ts) == -1 && errno == EINTR);
}

#ifndef __APPLE__
uint64 NowInUs() {
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return static_cast<uint64>(now.tv_sec) * 1000000 + now.tv_nsec / 1000;
}

#else // mac
uint64 NowInUs() {
	clock_serv_t cs;
	mach_timespec_t ts;

	host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cs);
	clock_get_time(cs, &ts);
	mach_port_deallocate(mach_task_self(), cs);

	return static_cast<uint64>(ts.tv_sec) * 1000000 + ts.tv_nsec / 1000;
}
#endif // __APPLE__
#endif // _WIN32
/************************************ unix <=**********************************/

/**********************************=> win *************************************/
#ifdef _WIN32
void SleepInMs(uint32_t ms) {
	::Sleep(ms);
}

void SleepInUs(uint32_t us) {
	::LARGE_INTEGER ft;
	ft.QuadPart = -static_cast<int64_t>(us * 10);  // '-' using relative time

	::HANDLE timer = ::CreateWaitableTimer(NULL, TRUE, NULL);
	::SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	::WaitForSingleObject(timer, INFINITE);
	::CloseHandle(timer);
}

static inline uint64_t GetPerfFrequency() {
	::LARGE_INTEGER freq;
	::QueryPerformanceFrequency(&freq);
	return freq.QuadPart;
}

static inline uint64_t PerfFrequency() {
	static uint64_t xFreq = GetPerfFrequency();
	return xFreq;
}

static inline uint64_t PerfCounter() {
	::LARGE_INTEGER counter;
	::QueryPerformanceCounter(&counter);
	return counter.QuadPart;
}

uint64_t NowInUs() {
	return static_cast<uint64_t>(
		static_cast<double>(PerfCounter()) * 1000000 / PerfFrequency());
}
#endif // _WIN32






/*
	https://stackoverflow.com/questions/25255899/reading-an-embedded-text-file-resource-visual-studio-c
*/


void LoadResource(int name, std::vector<uint8_t>& data)
{
	HMODULE handle = ::GetModuleHandleW(NULL);
	HRSRC rc = ::FindResourceW(handle, MAKEINTRESOURCEW(name), MAKEINTRESOURCEW(TEXTFILE));
	HGLOBAL rcData = ::LoadResource(handle, rc);
	data.resize(::SizeofResource(handle, rc));
	LPVOID res = ::LockResource(rcData);
	memcpy(data.data(), res, data.size());
}

std::string LoadResourceAsString(int name)
{
	std::vector<uint8_t> data;
	LoadResource(name, data);
	return std::string(data.begin(), data.end());
}
