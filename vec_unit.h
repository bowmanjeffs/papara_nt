/*
 * Copyright (C) 2010 Simon A. Berger
 *
 *  This program is free software; you may redistribute it and/or modify its
 *  under the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 */


#ifndef __vec_unit_h
#define __vec_unit_h
#include <iostream>
#include <stdexcept>
#include <immintrin.h>

#ifdef __AVX__
#define HAVE_AVX
//#include <immintrin.h>
#endif

template<class T, size_t W> 
struct vector_unit {
   
};

// vector unit specialization: SSE 8x16bit integer 

template<>
struct vector_unit<short, 8> {

    const static bool do_checks = false;
    
    typedef __m128i vec_t;
    typedef short T;
    
    const static T SMALL_VALUE = -32000;
    const static T BIAS = 0;
    const static size_t W = 8;
    
    static inline vec_t setzero() {
        return set1(0);
    }
    
    static inline vec_t set1( T val ) {
        return _mm_set1_epi16( val );
    }
    
    static inline void store( const vec_t &v, T *addr ) {

        if( do_checks && addr == 0 ) {
            throw std::runtime_error( "store: addr == 0" );
        }
        //std::cout << "store to: " << addr << "\n";
        
        _mm_store_si128( (vec_t*)addr, v );
    }
    
    static inline const vec_t load( T* addr ) {
        return _mm_load_si128( (vec_t*)addr );
    }
    
    static inline const vec_t bit_and( const vec_t &a, const vec_t &b ) {
        return _mm_and_si128( a, b );
    }
    
    static inline const vec_t bit_andnot( const vec_t &a, const vec_t &b ) {
        return _mm_andnot_si128( a, b );
    }
    
//     static inline const vec_t bit_invert( const vec_t &a ) {
//         //return _mm_andnot_pd(a, set1(0xffff));
//     }
    
    static inline const vec_t add( const vec_t &a, const vec_t &b ) {
        return _mm_add_epi16( a, b );
    }
    static inline const vec_t sub( const vec_t &a, const vec_t &b ) {
        return _mm_sub_epi16( a, b );
    }
    static inline const vec_t cmp_zero( const vec_t &a ) {
        return _mm_cmpeq_epi16( a, setzero() );
    }
    
    static inline const vec_t cmp_eq( const vec_t &a, const vec_t &b ) {
        return _mm_cmpeq_epi16( a, b );
    }
    
    static inline const vec_t cmp_lt( const vec_t &a, const vec_t &b ) {
     
        return _mm_cmplt_epi16( a, b );
    }
    
    static inline const vec_t min( const vec_t &a, const vec_t &b ) {
        return _mm_min_epi16( a, b );
    }
    
    static inline const vec_t max( const vec_t &a, const vec_t &b ) {
        return _mm_max_epi16( a, b );
    }
};


#ifdef HAVE_AVX
// AVX 16x16bit vector unit
template<>
struct vector_unit<short, 16> {

    const static bool do_checks = false;
    
    typedef __m256i vec_t;
    typedef short T;
    
    const static T SMALL_VALUE = -32000;
    const static T BIAS = 0;
    const static size_t W = 8;
    
    static inline vec_t setzero() {
        return _mm256_setzero_si256();
    }
    
    static inline vec_t set1( T val ) {
        return _mm256_set1_epi16( val );
    }
    
    static inline void store( const vec_t &v, T *addr ) {

        if( do_checks && addr == 0 ) {
            throw std::runtime_error( "store: addr == 0" );
        }
        //std::cout << "store to: " << addr << "\n";
        
        _mm256_store_si256( (vec_t*)addr, v );
    }
    
    static inline const vec_t load( T* addr ) {
        return _mm256_load_si256( (vec_t*)addr );
    }
    
    static inline const vec_t bit_and( const vec_t &a, const vec_t &b ) {
        __m128i lowa = _mm256_castsi256_si128(a);
        const __m128i lowb = _mm256_castsi256_si128(b);
        
        lowa = _mm_and_si128( lowa, lowb );
        
        
        __m128i higha = _mm256_extractf128_si256(a,1);
        const __m128i highb = _mm256_extractf128_si256(b,1);
        
        higha = _mm_and_si128( higha, highb );
        
        
        return _mm256_insertf128_si256( _mm256_castsi128_si256(lowa), higha, 1 );
    }
    
    static inline const vec_t bit_andnot( const vec_t &a, const vec_t &b ) {
        __m128i lowa = _mm256_castsi256_si128(a);
        const __m128i lowb = _mm256_castsi256_si128(b);
        
        lowa = _mm_andnot_si128( lowa, lowb );
        
        
        __m128i higha = _mm256_extractf128_si256(a,1);
        const __m128i highb = _mm256_extractf128_si256(b,1);
        
        higha = _mm_andnot_si128( higha, highb );
        
        
        return _mm256_insertf128_si256( _mm256_castsi128_si256(lowa), higha, 1 );
    }
    
//     static inline const vec_t bit_invert( const vec_t &a ) {
//         //return _mm_andnot_pd(a, set1(0xffff));
//     }
    
    static inline const vec_t add( const vec_t &a, const vec_t &b ) {
        const __m128i lowa = _mm256_extractf128_si256(a,0);
        const __m128i lowb = _mm256_extractf128_si256(b,0);
        
        //lowa = _mm_add_epi16( lowa, lowb );
        
        
        const __m128i higha = _mm256_extractf128_si256(a,1);
        const __m128i highb = _mm256_extractf128_si256(b,1);
        
        //higha = _mm_add_epi16( higha, highb );
        
        
        return _mm256_insertf128_si256( _mm256_castsi128_si256(_mm_add_epi16( lowa, lowb )),  _mm_add_epi16( higha, highb ), 1 );
    }
    static inline const vec_t sub( const vec_t &a, const vec_t &b ) {
        __m128i lowa = _mm256_castsi256_si128(a);
        const __m128i lowb = _mm256_castsi256_si128(b);
        
        lowa = _mm_sub_epi16( lowa, lowb );
        
        
        __m128i higha = _mm256_extractf128_si256(a,1);
        const __m128i highb = _mm256_extractf128_si256(b,1);
        
        higha = _mm_sub_epi16( higha, highb );
        
        
        return _mm256_insertf128_si256( _mm256_castsi128_si256(lowa), higha, 1 );
    }
    static inline const vec_t cmp_zero( const vec_t &a ) {
        return cmp_eq( a, setzero() );
    }
    
    static inline const vec_t cmp_eq( const vec_t &a, const vec_t &b ) {
        __m128i lowa = _mm256_castsi256_si128(a);
        const __m128i lowb = _mm256_castsi256_si128(b);
        
        lowa = _mm_cmpeq_epi16( lowa, lowb );
        
        
        __m128i higha = _mm256_extractf128_si256(a,1);
        const __m128i highb = _mm256_extractf128_si256(b,1);
        
        higha = _mm_cmpeq_epi16( higha, highb );
        
        
        return _mm256_insertf128_si256( _mm256_castsi128_si256(lowa), higha, 1 );
    }
    
    static inline const vec_t cmp_lt( const vec_t &a, const vec_t &b ) {
     
        __m128i lowa = _mm256_castsi256_si128(a);
        const __m128i lowb = _mm256_castsi256_si128(b);
        
        lowa = _mm_cmplt_epi16( lowa, lowb );
        
        
        __m128i higha = _mm256_extractf128_si256(a,1);
        const __m128i highb = _mm256_extractf128_si256(b,1);
        
        higha = _mm_cmplt_epi16( higha, highb );
        
        
        return _mm256_insertf128_si256( _mm256_castsi128_si256(lowa), higha, 1 );
    }
    
    static inline const vec_t min( const vec_t &a, const vec_t &b ) {
        __m128i lowa = _mm256_castsi256_si128(a);
        const __m128i lowb = _mm256_castsi256_si128(b);
        
        lowa = _mm_min_epi16( lowa, lowb );
        
        
        __m128i higha = _mm256_extractf128_si256(a,1);
        const __m128i highb = _mm256_extractf128_si256(b,1);
        
        higha = _mm_min_epi16( higha, highb );
        
        
        return _mm256_insertf128_si256( _mm256_castsi128_si256(lowa), higha, 1 );
    }
    
    static inline const vec_t max( const vec_t &a, const vec_t &b ) {
        __m128i lowa = _mm256_castsi256_si128(a);
        const __m128i lowb = _mm256_castsi256_si128(b);
        
        lowa = _mm_max_epi16( lowa, lowb );
        
        
        __m128i higha = _mm256_extractf128_si256(a,1);
        const __m128i highb = _mm256_extractf128_si256(b,1);
        
        higha = _mm_max_epi16( higha, highb );
        
        
        return _mm256_insertf128_si256( _mm256_castsi128_si256(lowa), higha, 1 );
    }
};

#endif


// vector unit specialization: SSE 16x8bit integer 

template<>
struct vector_unit<unsigned char, 16> {

    const static bool do_checks = false;
    
    typedef __m128i vec_t;
    typedef unsigned char T;
    
    const static size_t W = 16;
    const static T SMALL_VALUE = 32;
    const static T BIAS = 127;
    
    
    static inline vec_t setzero() {
        return set1(0);
    }
    
    static inline vec_t set1( T val ) {
        return _mm_set1_epi8( val );
    }
    
    static inline void store( const vec_t &v, T *addr ) {

        if( do_checks && addr == 0 ) {
            throw std::runtime_error( "store: addr == 0" );
        }
        //std::cout << "store to: " << addr << "\n";
        
        _mm_store_si128( (__m128i*)addr, v );
    }
    
    static inline const vec_t load( T* addr ) {
        return (vec_t)_mm_load_si128( (__m128i *) addr );
    }
    
    static inline const vec_t bit_and( const vec_t &a, const vec_t &b ) {
        return _mm_and_si128( a, b );
    }
    
    static inline const vec_t add( const vec_t &a, const vec_t &b ) {
        return _mm_add_epi8( a, b );
    }
    
    static inline const vec_t sub( const vec_t &a, const vec_t &b ) {
        return _mm_sub_epi8( a, b );
    }
    
    static inline const vec_t cmp_zero( const vec_t &a ) {
        return _mm_cmpeq_epi8( a, setzero() );
    }
    
    static inline const vec_t min( const vec_t &a, const vec_t &b ) {
        return _mm_min_epu8( a, b );
    }
    static inline const vec_t max( const vec_t &a, const vec_t &b ) {
        return _mm_max_epu8( a, b );
    }
    
    static inline void println( const vec_t &v, T *tmp ) {
        store(v, tmp);
        std::cout << "(";
        for( size_t i = 0; i < W; i++ ) {
         
            std::cout << int(tmp[i]) << ((i < W-1) ? "," : ")");
        }
        
        std::cout << std::endl;
    }
};


template<>
struct vector_unit<short, 1> {

    const static bool do_checks = false;
    
    typedef __m128i vec_t;
    typedef short T;
    
    const static size_t W = 1;
    
    static inline vec_t setzero() {
        return set1(0);
    }
    
    static inline vec_t set1( T val ) {
        return _mm_set1_epi16( val );
    }
    
    static inline void store( const vec_t &v, T *addr ) {

        if( do_checks && addr == 0 ) {
            throw std::runtime_error( "store: addr == 0" );
        }
        //std::cout << "store to: " << addr << "\n";
        
        //_mm_store_ps( (float*)addr, (__m128)v );
        
        *addr = _mm_extract_epi16( v, 0 );
    }
    
    static inline const vec_t load( T* addr ) {
        //return (vec_t)_mm_load_ps( (float *) addr );
        return set1(*addr);
    }
    
    static inline const vec_t bit_and( const vec_t &a, const vec_t &b ) {
        return _mm_and_si128( a, b );
    }
    
    static inline const vec_t add( const vec_t &a, const vec_t &b ) {
        return _mm_add_epi16( a, b );
    }
    
    static inline const vec_t cmp_zero( const vec_t &a ) {
        return _mm_cmpeq_epi16( a, setzero() );
    }
    
    static inline const vec_t min( const vec_t &a, const vec_t &b ) {
        return _mm_min_epi16( a, b );
    }
};


// vector unit specialization: SSE scalar 16bit integer 

// template<>
// struct vector_unit<short, 1> {
// 
//     const static bool do_checks = false;
//     
//     typedef short vec_t;
//     typedef short T;
//     
//     const static size_t W = 1;
//     
//     static inline vec_t setzero() {
//         return 0;
//     }
//     
//     static inline vec_t set1( T val ) {
//         return val;
//     }
//     
//     static inline void store( const vec_t &v, T *addr ) {
// 
//         if( do_checks && addr == 0 ) {
//             throw std::runtime_error( "store: addr == 0" );
//         }
//         
//         *addr = v;
//         
//     }
//     
//     static inline const vec_t load( T* addr ) {
//         return *addr;
//     }
//     
//     static inline const vec_t bit_and( const vec_t &a, const vec_t &b ) {
//         return  a & b;
//     }
//     
//     static inline const vec_t add( const vec_t &a, const vec_t &b ) {
//         return a + b;
//     }
//     
//     static inline const vec_t cmp_zero( const vec_t &a ) {
//         //return _mm_cmpeq_epi16( a, setzero() );
//         return a == 0 ? 0xffff : 0x0;
//     }
//     
//     static inline const vec_t min( const vec_t &a, const vec_t &b ) {
//         return std::min( a, b );
//     }
// };
#endif