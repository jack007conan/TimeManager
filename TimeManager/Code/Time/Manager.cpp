#include"Manager.h"
#include<Windows.h>
#pragma comment(lib, "winmm.lib")

using namespace Time;

unsigned int GetNowCount() {
	return (unsigned int)(timeGetTime());
}

void Manager::m_update() {
	unsigned int t = b_now - m_ini - m_stopUnit.getCount();
	m_dur = t - m_now;
	if (m_leftStop.elem == 0) {
		m_now = t;
	}
	else if (m_leftStop.getCount() >= m_dur) {
		m_dur = 0;
	}
	else {
		m_dur -= m_leftStop.getCount();
		m_now += m_dur;
		m_stopUnit.elem += m_leftStop.elem;
		m_leftStop.elem = 0;
	}
}

Manager::Manager() :
	b_now(g_now),
	m_ini(b_now),
	m_now(0),
	m_dur(0),
	m_stopUnit(0),
	m_leftStop(0)
{
	mList.emplace_back(this);
	mItr = --(mList.end());
}

Manager::Manager(Manager *manager) :
	b_now(manager->getNow()),
	m_ini(b_now),
	m_now(0),
	m_dur(0),
	m_stopUnit(0),
	m_leftStop(0)
{
	mList.emplace_back(this);
	mItr = --(mList.end());
}



Manager::~Manager() {
	mList.erase(mItr);
}

void Manager::stop(const unsigned int &stopFrame) {
	m_leftStop = Unit(stopFrame);
}

void Manager::update() {
#ifndef NDEBUG
	g_dur = 1;
	g_now++;
#else
	unsigned int t = (unsigned int)(GetNowCount()) - g_ini;
	g_dur = t - g_now;
	g_now = t;
#endif

	for (auto &i : mList) {
		i->m_update();
	}
}

std::list<Time::Manager*> Manager::mList;
#ifndef NDEBUG
const unsigned int Time::Manager::g_ini = 0;
#else
const unsigned int Time::Manager::g_ini = (unsigned int)(GetNowCount());
#endif
unsigned int Time::Manager::g_now = 0;
unsigned int Time::Manager::g_dur = 0;

bool Time::Manager::operator==(const unsigned int &o) {
	return (m_now - (unsigned int)(o * ((double)(50) / 3)) < m_dur);
}