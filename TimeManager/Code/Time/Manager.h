//Debug���̃t���[�����[�g
constexpr unsigned int gFrameRate = 60;

class Time {
public:
	//�O���[�o�����Ԃ̍X�V
	//���K���t���[���̍ŏ��ɍX�V
	static void update();

private:
	//���ԒP�ʃN���X
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
		//�O���[�o�����Ԃ̎擾
		static Unit GetGlobalTime();

		//�t���[�������J�E���g(ms)�ɕϊ�
		static unsigned int GetCountFromFrame(double frame) {
			return (unsigned int)(frame * ((double)1000 / 60));
		}

		//�J�E���g(ms)���t���[�����ɕϊ�
		template<typename T = double>
		static T GetFrameFromCount(unsigned int count) {
			return ((T)(count) * 60 / 1000);
		}

		//�t���[������Unit�ɕϊ�
		static Unit GetUnitFromFrame(double);

		//�J�E���g(ms)��Unit�ɕϊ�
		static Unit GetUnitFromCount(unsigned int);

		//�t���[�������擾
		double getFrame() const;

		//�J�E���g(ms)���擾
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
		//Global��p�R���X�g���N�^
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

		//�K�w�X�V
		//���ɂȂ�K�w�̍X�V�̌�Ɏ��s
		void update();

		//start�����܂Ŏ��Ԓ�~
		void stop();

		//�w��t���[�������Ԓ�~
		void stop(const unsigned int frame);

		//�����I�Ɏ��Ԍv�����ĊJ
		void start();

		friend Time;
	};
	
	Time(Level *p = &Level::Global);
private:
	Level const *const mBase;
	Unit mIni;

public:
	//�t���[�����擾
	double getNow() const {
		return (mBase->mNow - mIni).getFrame();
	}
	
	//�J�E���g(ms)�擾
	unsigned int getNowCount() const {
		return (mBase->mNow - mIni).getCount();
	}


};