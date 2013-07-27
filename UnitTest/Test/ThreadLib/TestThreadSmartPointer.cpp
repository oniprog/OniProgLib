///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	スレッド用スマートポインタのテスト
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#include "gtest/gtest.h"

#include <windows.h>

#include "boost/thread.hpp"
#include "ThreadLib/ThreadSmartPointer.h"

///////////////////////////////////////////////////////////////////////////////////
struct Dummy {

    Dummy()  { IncAtomic(nCntConstructor); };
    ~Dummy() { IncAtomic(nCntDestroy); };
    Dummy( const Dummy &dm) {
        IncAtomic(nCntConstructor);
    }

    template<class T>
    void IncAtomic( T & v ) {
        while(true) {
            int val = v.load();
            if ( v.compare_exchange_strong( val, val+1 ) )
                break;
        }
    }

    static void    Reset() {
        nCntConstructor.store(0);
        nCntDestroy.store(0);
    }

    static void GetResult( int &nCnt1, int &nCnt2 ) {
        nCnt1 = nCntConstructor.load();
        nCnt2 = nCntDestroy.load();
    }

    int nDummy;

    static boost::atomic<int>      nCntConstructor;
    static boost::atomic<int>      nCntDestroy;
};

boost::atomic<int> Dummy::nCntConstructor;
boost::atomic<int> Dummy::nCntDestroy;

///////////////////////////////////////////////////////////////////////////////////
namespace ThreadLib {

// Dummyをコピーして返す
template<>
Dummy *CopyInside(const Dummy *pP) {
    return new Dummy( *pP );
}
}

///////////////////////////////////////////////////////////////////////////////////
/*
    何らかの処理をするスレッド。しかし、pA2がObsoleteになったら処理を終了する。
    本来は、Obsoleteになる前に処理が完了したら、そのまま継続するようにする。
*/
static void Thread2( ThreadLib::thread_shared_ptr<Dummy> pA2 ) {

    while(true) {

        if ( pA2.IsObsolete() )
            break;

        // pA2の内容を読みだして、いろいろと処理をすることを想定している。
        // pA2は読み出し専用

        pA2->nDummy;
        //pA2->nDummy++;        // 変更はできない

        ::Sleep(100);
    }
}

///////////////////////////////////////////////////////////////////////////////////
static void Thread1( ThreadLib::thread_shared_ptr<Dummy> pA1 ) {

    // pA1を使って何らかの作業をおこなうスレッド
    std::vector<boost::thread> listThread;
    for(int it=0; it<10; ++it) {

        // スレッド2にて、pA1を使って処理をさせる
        boost::thread thread2( &Thread2, pA1 );
        listThread.push_back(std::move(thread2));

        // しかし、このスレッドで何か処理をして上書きがしたくなった
        auto pA2 = pA1.Write(); 

        Sleep(100);

        // 上書きする。これでpA1はObsoleteになる。
        pA1 = pA2;
    }

    // 終了待ち
    std::for_each( listThread.begin(), listThread.end(), [&](boost::thread &th) {th.join(); } );
}

///////////////////////////////////////////////////////////////////////////////////
TEST(ThreadSmartPointer, Test1) {

    {
        Dummy::Reset();

        ThreadLib::thread_shared_ptr<Dummy> pA1( new Dummy() );

        boost::thread thread1( &Thread1, pA1 );
        thread1.join();
    }

    // コンストラクタとデストラクタが呼ばれた回数をチェックする
    int nCnt1, nCnt2;
    Dummy::GetResult(nCnt1, nCnt2);
    EXPECT_EQ( 11, nCnt1 );
    EXPECT_EQ( 11, nCnt2 );
}
