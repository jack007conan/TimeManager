//Debug時のフレームレート
constexpr unsigned int gFrameRate = 60;

class Time {
public:
	//グローバル時間の更新
	//※必ずフレームの最初に更新
	static void update();

private:
	//時間単位クラス
	class Unit {
	public:
		Unit(unsigned int unit = 0U) : 
			unit(unit)
		{
		}
		template<typename T>
		const Unit &operator= (const T &o) {
			unit = (unsigned int)(o);
			return(*this);
		}
	private:
		unsigned int unit;
	public:
		//グローバル時間の取得
		static Unit GetGlobalTime();

		//フレーム数をカウント(ms)に変換
		static unsigned int GetCountFromFrame(double frame) {
			return (unsigned int)(frame * ((double)1000 / 60));
		}

		//カウント(ms)をフレーム数に変換
		template<typename T = double>
		static T GetFrameFromCount(unsigned int count) {
			return ((T)(count) * 60 / 1000);
		}

		//フレーム数をUnitに変換
		static Unit GetUnitFromFrame(double);

		//カウント(ms)をUnitに変換
		static Unit GetUnitFromCount(unsigned int);

		//フレーム数を取得
		double getFrame() const;

		//カウント(ms)を取得
		unsigned int getCount() const;

	public:
		//operator
		operator unsigned int() const {
			return unit;
		}
		template<typename T>
		Unit operator+(const T &o) const {
			return Unit(unit + (unsigned int)(o));
		}
		template<typename T>
		Unit operator-(const T &o) const {
			return Unit(unit - (unsigned int)(o));
		}
		unsigned int operator()() const {
			return unit;
		}
		template<typename T>
		bool operator<(const T &o) const {
			return (unit < (unsigned int)(o));
		}
		template<typename T>
		bool operator>(const T &o) const {
			return (unit > (unsigned int)(o));
		}
		template<typename T>
		bool operator<=(const T &o) const {
			return !((*this) > o);
		}
		template<typename T>
		bool operator>=(const T &o) const {
			return !((*this) < o);
		}
	};
public:
	class Level {
	public:
		Level(Level *p = &Level::Global);

	private:
		//Global専用コンストラクタ
		Level(std::nullptr_t);

		static Level Global;

	private:
		Level *mBase;
		Unit mIni;
		Unit mNow;
		Unit mDur;
		unsigned int mStopFrameSum;
		unsigned int mStopFrame;

	public:

		//階層更新
		//基底になる階層の更新の後に実行
		void update();

		//startされるまで時間停止
		void stop();

		//指定フレーム数時間停止
		void stop(const unsigned int frame);

		//強制的に時間計測を再開
		void start();

		friend Time;
	};
	
	Time(Level *p = &Level::Global);
private:
	Level const *const mBase;
	Unit mIni;

public:
	//フレーム数取得
	double getNow() const {
		return (mBase->mNow - mIni).getFrame();
	}
	
	//カウント(ms)取得
	unsigned int getNowCount() const {
		return (mBase->mNow - mIni).getCount();
	}


};