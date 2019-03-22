#include"Manager.h"
#include<DxLib.h>

namespace {
	inline unsigned int GetCountfromFrame(const unsigned int &frame) {
#ifndef NDEBUG
		return frame;
#else
		return (unsigned int)(frame * ((double)1000 / Time::FrameRate));
#endif
	}
	inline unsigned int GetCount(const unsigned int &count) {
#ifndef NDEBUG
		return GetCountfromFrame(count);
#else
		return count;
#endif 
	}

	inline unsigned int GetGrobalCount() {
#ifndef NDEBUG
		static unsigned int frame = (unsigned int)(-1);
		frame++;
		return frame;
#else
		return (unsigned int)(GetNowCount()); //OS‹N“®‚©‚ç‚ÌŠÔ‚ğƒ~ƒŠ•b‚Å•Ô‚·ŠÖ”
#endif
	}

	inline float GetFramefromCountf(const unsigned int &count) {
#ifndef NDEBUG
		return (float)(count);
#else
		return ((float)(count * 3) / 50);
#endif
	}
	inline double GetFrameformCountd(const unsigned int &count) {
#ifndef NDEBUG
		return (double)(count);
#else
		return ((double)(count) * 3 / 50);
#endif
	}
}

using namespace Time;

Manager::Manager() :
	m_baseNow(m_g_now),
	m_ini(m_baseNow),
	m_now(0),
	m_dur(0),
	m_stopFrame(0),
	m_leftStopFrame(0)
{
	mList.emplace_back(this);
	mItr = --(mList.end());
}

Manager::Manager(Manager *manager) :
	m_baseNow(manager->m_now),
	m_ini(m_baseNow),
	m_now(0),
	m_dur(0),
	m_stopFrame(0),
	m_leftStopFrame(0)
{
	mList.emplace_back(this);
	mItr = --(mList.end());
}

Manager::~Manager() {
	mList.erase(mItr);
}

void Manager::m_update() {
	unsigned int t = m_baseNow - m_ini - GetCountfromFrame(m_stopFrame);
	m_dur = t - m_now;
	if (m_leftStopFrame == 0) {
		m_now = t;
	}
	else if (GetCountfromFrame(m_leftStopFrame) >= m_dur) {
		m_dur = 0;
	}
	else {
		m_dur -= GetCountfromFrame(m_leftStopFrame);
		m_now += m_dur;
		m_stopFrame += m_leftStopFrame;
		m_leftStopFrame = 0;
	}
}

void Manager::stop(const unsigned int &stopFrame) {
	m_leftStopFrame = stopFrame;
}

void Manager::stop() {
	m_leftStopFrame = (unsigned int)(-1);
}
void Manager::start() {
	m_leftStopFrame = 0;
	m_stopFrame += m_baseNow - m_ini - GetCountfromFrame(m_stopFrame) - m_now;
}

void Manager::init() {
	m_ini = m_baseNow;
	m_now = 0;
	m_dur = 0;
	m_stopFrame = 0;
	m_leftStopFrame = 0;
}

float Manager::getDur() const {
	return (GetFramefromCountf(m_dur));
}
double Manager::getNow() const {
	return (GetFrameformCountd(m_now));
}
unsigned int Manager::getNowCount() const {
	return (GetCount(m_now));
}

void Manager::update() {
	unsigned int t = GetGrobalCount() - m_g_ini;
	m_g_dur = t - m_g_now;
	m_g_now = t;

	for (auto &i : mList) {
		i->m_update();
	}
}

std::list<Time::Manager*> Manager::mList;
const unsigned int Time::Manager::m_g_ini = GetGrobalCount();
unsigned int Time::Manager::m_g_now = 0;
unsigned int Time::Manager::m_g_dur = 0;

bool Time::Manager::operator==(const unsigned int &o) const {
	return (m_now - GetCountfromFrame(o) < m_dur);
}

bool Time::Manager::operator<(const unsigned int &o) const {
	return ((int)(GetCountfromFrame(o) - m_now) > 0);
}
bool Time::Manager::operator>(const unsigned int &o) const {
	return ((int)(m_now - m_dur - GetCountfromFrame(o)) > 0);
}