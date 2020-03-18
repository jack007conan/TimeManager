#include"Manager.h"
#include<DxLib.h>

Time::Unit Time::Unit::GetGlobalTime() {
#ifndef NDEBUG
	static unsigned int t = (unsigned int)(-1);
	t++;
	return Time::Unit(t * 60 / gFrameRate);
#else
	//ŠÔ‚ªƒ~ƒŠ•b‚Å‹A‚Á‚Ä‚­‚éŠÖ”
	return Time::Unit(GetNowCount());
#endif
}

Time::Unit Time::Unit::GetUnitFromFrame(double frame) {
#ifndef NDEBUG
	return Unit((unsigned int)frame);
#else
	return Unit(GetCountFromFrame(frame));
#endif
}

Time::Unit Time::Unit::GetUnitFromCount(unsigned int count) {
#ifndef NDEBUG
	return Unit(GetFrameFromCount<unsigned int>(count));
#else
	return Unit(count);
#endif
}

double Time::Unit::getFrame() const {
#ifndef NDEBUG
	return unit;
#else
	return GetFrameFromCount(unit);
#endif
}

unsigned int Time::Unit::getCount() const {
#ifndef NDEBUG
	return GetCountFromFrame(unit);
#else
	return unit;
#endif
}

Time::Level::Level(Level *p) : 
	mBase(p),
	mIni(p->mNow),
	mNow(0U),
	mDur(0U),
	mStopFrameSum(0U),
	mStopFrame(0U)
{
}

Time::Level::Level(std::nullptr_t) :
	mBase(nullptr),
	mIni(Unit::GetGlobalTime()),
	mNow(0U),
	mDur(0U),
	mStopFrameSum(0U),
	mStopFrame(0U)
{
}

void Time::Level::update() {
	if( mStopFrame == 0 ) {
		auto t = mNow;
		mNow = mBase->mNow - mIni - Time::Unit::GetUnitFromFrame(mStopFrameSum);
		mDur = mNow - t;
	}
	else {
		if( mBase->mNow - mIni - mNow <= Time::Unit::GetUnitFromFrame(mStopFrame + mStopFrameSum) ) {
			mDur = 0;
		}
		else {
			mStopFrameSum += mStopFrame;
			mStopFrame = 0;
			auto t = mNow;
			mNow = mBase->mNow - mIni - Time::Unit::GetUnitFromFrame(mStopFrameSum);
			mDur = mNow - t;
		}
	}
}

void Time::Level::stop() {
	mStopFrame = (unsigned int)(-1 - mStopFrameSum);
}

void Time::Level::stop(const unsigned int frame) {
	mStopFrame = frame;
}

void Time::Level::start() {
	mStopFrame = 0;
	mStopFrameSum = 0;
	mIni = mBase->mNow - mNow;
}

Time::Time(Time::Level *p) :
	mBase(p),
	mIni(p->mNow)
{
}

void Time::update() {
	//Global‚Ìupdate
	auto t = Level::Global.mNow;
	Level::Global.mNow = Unit::GetGlobalTime() - Level::Global.mIni;
	Level::Global.mDur = Level::Global.mNow - t;
}

Time::Level Time::Level::Global{nullptr};