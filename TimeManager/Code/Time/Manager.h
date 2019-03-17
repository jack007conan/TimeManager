#pragma once
#include<list>

namespace Time {

	class Unit {
	public:
		Unit(const unsigned int &elem, bool absoluteFlag = false) :
			elem(elem),
			absoluteFlag(absoluteFlag)
		{}
		unsigned int elem;
		bool absoluteFlag;
		unsigned int getCount() {
#ifndef NDEBUG
			return elem;
#else
			if (absoluteFlag) return elem;
			else return (unsigned int)(elem * ((double)50 / 3));
#endif
		}
	};

	class Manager {
	private:
		const unsigned int &b_now;
		const unsigned int m_ini;
		unsigned int m_now;
		unsigned int m_dur;

		Unit m_stopUnit; //ã‚ÌŠK‘w‚Æ‚ÌƒYƒŒ
		Unit m_leftStop; //stopŠÖ”‚Åw’è‚³‚ê‚½’l‚ğ•Û‘¶‚·‚é‚¾‚¯
		void m_update();
	public:
		const auto &getNow() {
			return m_now;
		}

		Manager();
		Manager(Manager *);
		Manager(const Manager &) = delete;
		Manager &operator= (const Manager &) = delete;
		~Manager();

		void stop(const unsigned int &stopFrame);

		static void update();
		static unsigned int g_now;
		static const unsigned int g_ini;
		static unsigned int g_dur;
	private:
		static std::list<Time::Manager*> mList;
		std::list<Time::Manager*>::iterator mItr;
	public:
		bool operator==(const unsigned int &o);
		bool operator!=(const unsigned int &o) {
			return !((*this) == o);
		}
	};
}
