///////////////////////////////////////////////////////////////////////////////////
/** @file
	@brief	無誤差演算高速版
	@author	oniprog
*/
///////////////////////////////////////////////////////////////////////////////////
// Jonathan Richard Shewchuk先生のおかげでかけました
// http://www.cs.berkeley.edu/~jrs/jrspapers.html
// もっと高速なCライブラリがShewchuk先生のページにあります．
// でも，私はC++がすきだ！ぽっ．

#pragma once

#include <tuple>
#include <algorithm>

namespace ExactDouble {

////////////////////////////////////////////////////////////
/// a < b のときに使える無誤差足し算
std::tuple<double,double> FastToSum( double a, double b ) {
    volatile double x = a + b;
    volatile double bv = x - a;
    volatile double y = b - bv;
    return std::make_tuple(x,y);
}

////////////////////////////////////////////////////////////
/// 無誤差足し算．汎用型
std::tuple<double,double> TwoSum( double a, double b ) {

    volatile double x = a + b;
    volatile double bv = x - a;
    volatile double av = x - bv;
    volatile double br = b - bv;
    volatile double ar = a - av;
    volatile double y = ar + br;
    return std::make_tuple(x,y);
}

////////////////////////////////////////////////////////////
/// 無誤差のかけ算用
std::tuple<double, double>  Split( double a) {

    static double SPLIT = std::sqrt( std::numeric_limits<double>::epsilon() ) + 1.0;

    volatile double c = SPLIT * a;
    volatile double abig = c - a;
    volatile double ahi  = c - abig;
    volatile double alo = a - ahi;
    return std::make_tuple( ahi, alo );
}

////////////////////////////////////////////////////////////
/// 無誤差のかけ算
std::tuple<double,double>   TwoProduct(double a, double b) {

    volatile double x = a * b;
    auto aa = Split( a );
    auto bb = Split( b );
    volatile double err1 = x - std::get<0>(aa) * std::get<0>(bb);
    volatile double err2 = err1 - std::get<1>(aa) * std::get<0>(bb);
    volatile double err3 = err2 - std::get<0>(aa) * std::get<1>(bb);
    volatile double y = std::get<1>(aa) * std::get<1>(bb) - err3;
    return std::make_tuple(x,y);
}

//////////////////////
template<int KETA>
class ExactDoubleFast {
    double  m_data[KETA];       /// スタック上に確保することがポイント
public:

    ExactDoubleFast() { Initialize(); }
    ExactDoubleFast(double dV) { Initialize(); m_data[0] = dV; }

    double  *begin() {
        return &m_data[0];
    }
    double  *end() {
        return &m_data[KETA];
    }

    const double  *cbegin() const {
        return &m_data[0];
    }
    const double  *cend() const {
        return &m_data[KETA];
    }

    void    Initialize() {
        std::fill( begin(), end(), 0.0 );
    }

    ExactDoubleFast<KETA>   operator-() const {
        ExactDoubleFast<KETA> ret(*this);
        std::for_each( ret.begin(), ret.end(), [](double &v) { v=-v; } );
        return ret;
    }

    int         size() const {
        return KETA;
    }

private:
};

////////////////////////////////////////////////////////////
// 足し算を定義
template<int K1, int K2>
ExactDoubleFast<K1+K2> operator + ( const ExactDoubleFast<K1> & d1, const ExactDoubleFast<K2> & d2 ) {
    ExactDoubleFast<K1+K2> ret;

    double tmp[K1+K2];
    auto ii = std::copy( d1.cbegin(), d1.cend(), &tmp[0] );
    std::copy( d2.cbegin(), d2.cend(), ii );
    std::sort( &tmp[0], &tmp[K1+K2], []( double a, double b) { return std::fabs(a) < std::fabs(b); } );

    auto Q2q2 = FastToSum(tmp[1], tmp[0]);
    double Qi_1 = std::get<0>( Q2q2 );

    auto ans = ret.begin();

    if( K1+K2 == 2 ) {

        *ans++ = std::get<1>( Q2q2 );
        *ans = std::get<0>( Q2q2 );
        return ret;
    }
    else {

        for( int i=2; i<K1+K2; ++i) {

            auto Ri_hi_2 = FastToSum( tmp[i], tmp[i-1] );
            *ans = std::get<1>(Ri_hi_2);    
            ++ans;
            auto Qiqi = TwoSum( Qi_1, std::get<0>( Ri_hi_2 ) );
            Qi_1 = std::get<0>( Qiqi );

            if ( i == K1+K2-1 ) {
                *ans++ = std::get<1>( Qiqi );
                *ans = std::get<0>( Qiqi );
            }
        }
    }

    return ret;
}

////////////////////////////////////////////////////////////
// 足し算を定義
template<int K1, int K2>
ExactDoubleFast<K1+K2> operator - ( const ExactDoubleFast<K1> & d1, const ExactDoubleFast<K2> & d2 ) {
    return d1 + (-d2);  // 手抜きです．．．
}

////////////////////////////////////////////////////////////
// かけ算を定義
template<int K1>
ExactDoubleFast<2*K1> operator * ( const ExactDoubleFast<K1> &e, double b ) {

    ExactDoubleFast<2*K1> ret;

    auto ans = ret.begin();
    auto ii = e.cbegin();
    auto Q2h1 = TwoProduct( *ii++, b );
    *ans++ = std::get<1>(Q2h1);
    double Q2_1 = std::get<0>(Q2h1);

    for( int i=1; i<K1; ++i ) {

        auto Titi = TwoProduct(*ii++, b );
        auto Qh1 = TwoSum( Q2_1, std::get<1>(Titi) );
        auto Qh2 = FastToSum( std::get<0>(Titi), std::get<0>(Qh1) );
        Q2_1 = std::get<0>(Qh2);
        *ans++ = std::get<1>(Qh1);
        *ans++ = std::get<1>(Qh2);
    }
    *ans = Q2_1;
    return ret;
}


typedef ExactDoubleFast<1> EF;

} // namespace ExactDouble

