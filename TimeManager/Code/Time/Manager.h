#pragma once
#include<list>

namespace Time {
	constexpr unsigned int FrameRate = 60;
	class Manager {
	public:
		//コンストラクタ等
		Manager();
		Manager(Manager *);
		Manager(const Manager &) = delete; //コピー禁止
		Manager &operator= (const Manager &) = delete; //コピー禁止
		~Manager();

	private:
		//privateメンバ変数
		const unsigned int &m_baseNow; //上の階層の時間
		unsigned int m_ini; //初期化時の上の階層の時間
		unsigned int m_now; //初期化時からの時間(ms)
		unsigned int m_dur; //前のフレームとの時間差(ms)
		unsigned int m_stopFrame; //stop関数による上の階層とのズレ(F)
		unsigned int m_leftStopFrame; //stop関数で指定された値(F)を保存するだけ。再度時間を進め始めたら0になる
		
		//メンバ関数
		void m_update(); //m_now等を変更
	public:
		void stop(const unsigned int &stopFrame); //m_leftStopFrameを書き換え
		void stop(); //startするまでstop
		void start(); //stopされてたものをstopFrameにかかわらずその地点から動かす
		void init(); //上の階層は変えずにその他要素を初期値に戻す

	private: 
		//内部インライン関数
		unsigned int mGetCount(const unsigned int &frame) const { //絶対時間に変換して返す(Release時のみ)
#ifndef NDEBUG
			return frame;
#else
			return (unsigned int)(frame * ((double)1000 / FrameRate));
#endif
		}

		//とりあえずの関数
	public:
		const auto &getNow() {
			return m_now;
		}

	public:
		//staticなメンバ関数
		static void update();

	private:
		//staticなメンバ変数
		static unsigned int m_g_now;
		static const unsigned int m_g_ini;
		static unsigned int m_g_dur;

		//update関数関連
		static std::list<Time::Manager*> mList;
		std::list<Time::Manager*>::iterator mItr;

	public:
		//operator
		bool operator==(const unsigned int &o);
		bool operator!=(const unsigned int &o) {
			return !((*this) == o);
		}
	};
}
