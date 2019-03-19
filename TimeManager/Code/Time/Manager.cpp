#include"Manager.h"
#include<DxLib.h>

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
	m_baseNow(manager->getNow()),
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
	unsigned int t = m_baseNow - m_ini - mGetCount(m_stopFrame);
	m_dur = t - m_now;
	if (m_leftStopFrame == 0) {
		m_now = t;
	}
	else if (mGetCount(m_leftStopFrame) >= m_dur) {
		m_dur = 0;
	}
	else {
		m_dur -= mGetCount(m_leftStopFrame);
		m_now += m_dur;
		m_stopFrame += m_leftStopFrame;
		m_leftStopFrame = 0;
	}
}

void Manager::stop(const unsigned int &stopFrame) {
	m_leftStopFrame = stopFrame;
}

void Manager::stop() { //挙動未確認
	m_leftStopFrame = (unsigned int)(-1);
}
void Manager::start() { //挙動未確認
	m_leftStopFrame = 0;
	m_stopFrame += m_baseNow - m_ini - mGetCount(m_stopFrame) - m_now;
}

void Manager::init() { //未完成
	m_ini = m_baseNow;

}

void Manager::update() {
#ifndef NDEBUG
	m_g_dur = 1;
	m_g_now++;
#else
	unsigned int t = (unsigned int)(GetNowCount()) - m_g_ini;
	m_g_dur = t - m_g_now;
	m_g_now = t;
#endif

	for (auto &i : mList) {
		i->m_update();
	}
}

std::list<Time::Manager*> Manager::mList;
#ifndef NDEBUG
const unsigned int Time::Manager::m_g_ini = 0;
#else
const unsigned int Time::Manager::m_g_ini = (unsigned int)(GetNowCount());
#endif
unsigned int Time::Manager::m_g_now = 0;
unsigned int Time::Manager::m_g_dur = 0;

bool Time::Manager::operator==(const unsigned int &o) {
	return (m_now - (unsigned int)(o * ((double)(50) / 3)) < m_dur);
}