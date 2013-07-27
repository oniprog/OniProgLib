///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	スレッド用 スマートポインタ
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "boost/shared_ptr.hpp"
#include "boost/atomic.hpp"

namespace ThreadLib {

template<class T>
class thread_shared_ptr {
public:
    typedef boost::atomic<bool>                         ZObsoleteFlag;
    typedef boost::shared_ptr<boost::atomic<bool> >     ZSharedObsoleteFlag;
public:

    thread_shared_ptr() : m_pObsolete(new ZObsoleteFlag()) {}
    thread_shared_ptr(boost::detail::sp_nullptr_t ) : m_pObsolete(new ZObsoleteFlag()) {}

    // 最初の作成時以外は、使ってはまずい。
    explicit thread_shared_ptr( T * p ) : m_pP(boost::shared_ptr<T>(p)), m_pObsolete(new ZObsoleteFlag()) {}
    explicit thread_shared_ptr( const T * p ) : m_pP(boost::shared_ptr<T>(p)), m_pObsolete(new ZObsoleteFlag()) {}

    // 作成用。ここから保護が始まる。
    thread_shared_ptr( boost::shared_ptr<T> &p ) : m_pP(p), m_pObsolete( new ZObsoleteFlag() ) {}
    thread_shared_ptr( const boost::shared_ptr<T> &p ) : m_pP(p), m_pObsolete( new ZObsoleteFlag() ) {}

    thread_shared_ptr( const thread_shared_ptr<T> &m ) : m_pP(m.m_pP), m_pObsolete(m.m_pObsolete) {
    }

    thread_shared_ptr &operator = ( const thread_shared_ptr &m ) {

        // Obsolete属性を付ける
        SetObsolete();

        // コピーする
        m_pP = m.m_pP;
        m_pObsolete = m.m_pObsolete;
        return *this;
    }

    // このスマートポインタが無効になっているかをチェックする
    bool    IsObsolete() const {
        return m_pObsolete->load();
    }

    // このスマートポインタが無効であることをセットする(セットだけ)
    void    SetObsolete() {
        m_pObsolete->store(true);
    }

    // const参照しか用意しない
    const T * operator->() {
        return &*m_pP;
    } 
    const T & operator*() {
        return *m_pP;
    }

    // 書き込み用に作成する
    boost::shared_ptr<T>   Write() {
        // CopyInsideで中身をコピーしてそのポインターを返すこと
        return boost::shared_ptr<T>( ThreadLib::CopyInside( m_pP.get() ) );
    }

private:
    ZSharedObsoleteFlag             m_pObsolete;        ///< 破棄済みフラグ
    boost::shared_ptr<T>            m_pP;               ///< ポインタを持つ
};

// 内部コピー用の関数を定義する
template<class T>
T* CopyInside( const T* );


};	// namespace ThreadLib
