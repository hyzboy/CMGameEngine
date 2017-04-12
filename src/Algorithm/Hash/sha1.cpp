#include<hgl/algorithm/Hash.h>
#include<hgl/endian/Endian.h>

namespace hgl
{
    namespace algorithm
    {
        /* The SHA f()-functions */
        #define f1(x,y,z)   ( ( x & y ) | ( ~x & z ) )              /* Rounds  0-19 */
        #define f2(x,y,z)   ( x ^ y ^ z )                           /* Rounds 20-39 */
        #define f3(x,y,z)   ( ( x & y ) | ( x & z ) | ( y & z ) )   /* Rounds 40-59 */
        #define f4(x,y,z)   ( x ^ y ^ z )                           /* Rounds 60-79 */

        /* The SHA Mysterious Constants */
        #define K1  0x5A827999L     /* Rounds  0-19 */
        #define K2  0x6ED9EBA1L     /* Rounds 20-39 */
        #define K3  0x8F1BBCDCL     /* Rounds 40-59 */
        #define K4  0xCA62C1D6L     /* Rounds 60-79 */

        #define S(n,X)  ( ( ((uint32)X) << n ) | ( ((uint32)X) >> ( 32 - n ) ) )

        /* The initial expanding function */
        #define expand(count)   W[ count ] = S(1,(W[ count - 3 ] ^ W[ count - 8 ] ^ W[ count - 14 ] ^ W[ count - 16 ]))    /* to make this SHA-1 */

        /* The four SHA sub-rounds */
        #define subRound1(count)    \
        { \
            temp = S( 5, A ) + f1( B, C, D ) + E + W[ count ] + K1; \
            E = D; \
            D = C; \
            C = S( 30, B ); \
            B = A; \
            A = temp; \
        }

        #define subRound2(count)    \
        { \
            temp = S( 5, A ) + f2( B, C, D ) + E + W[ count ] + K2; \
            E = D; \
            D = C; \
            C = S( 30, B ); \
            B = A; \
            A = temp; \
        }

        #define subRound3(count)    \
        { \
            temp = S( 5, A ) + f3( B, C, D ) + E + W[ count ] + K3; \
            E = D; \
            D = C; \
            C = S( 30, B ); \
            B = A; \
            A = temp; \
        }

        #define subRound4(count)    \
        { \
            temp = S( 5, A ) + f4( B, C, D ) + E + W[ count ] + K4; \
            E = D; \
            D = C; \
            C = S( 30, B ); \
            B = A; \
            A = temp; \
        }

        class SHA1:public Hash
        {
            enum
            {
                BLOCK_SIZE  = 64,
                DIGEST_SIZE = 20
            };

            uint32 digest[5]; // Message digest
            uint32 countLo;   // 64-bit bit count
            uint32 countHi;
            uint32 data[16];  // SHA data buffer
            uint32 slop;      // # of bytes saved in data[]

        private:

            void sha1_transform()
            {
                uint32 W[80];
                uint32 temp;
                uint32 A, B, C, D, E;
                int i;

                /* Step A.  Copy the data buffer into the local work buffer */
                for( i = 0; i < 16; i++ )
                W[ i ] = data[ i ];

                /* Step B.  Expand the 16 words into 64 temporary data words */
                expand( 16 ); expand( 17 ); expand( 18 ); expand( 19 ); expand( 20 );
                expand( 21 ); expand( 22 ); expand( 23 ); expand( 24 ); expand( 25 );
                expand( 26 ); expand( 27 ); expand( 28 ); expand( 29 ); expand( 30 );
                expand( 31 ); expand( 32 ); expand( 33 ); expand( 34 ); expand( 35 );
                expand( 36 ); expand( 37 ); expand( 38 ); expand( 39 ); expand( 40 );
                expand( 41 ); expand( 42 ); expand( 43 ); expand( 44 ); expand( 45 );
                expand( 46 ); expand( 47 ); expand( 48 ); expand( 49 ); expand( 50 );
                expand( 51 ); expand( 52 ); expand( 53 ); expand( 54 ); expand( 55 );
                expand( 56 ); expand( 57 ); expand( 58 ); expand( 59 ); expand( 60 );
                expand( 61 ); expand( 62 ); expand( 63 ); expand( 64 ); expand( 65 );
                expand( 66 ); expand( 67 ); expand( 68 ); expand( 69 ); expand( 70 );
                expand( 71 ); expand( 72 ); expand( 73 ); expand( 74 ); expand( 75 );
                expand( 76 ); expand( 77 ); expand( 78 ); expand( 79 );

                /* Step C.  Set up first buffer */
                A = digest[ 0 ];
                B = digest[ 1 ];
                C = digest[ 2 ];
                D = digest[ 3 ];
                E = digest[ 4 ];

                /* Step D.  Serious mangling, divided into four sub-rounds */
                subRound1( 0 ); subRound1( 1 ); subRound1( 2 ); subRound1( 3 );
                subRound1( 4 ); subRound1( 5 ); subRound1( 6 ); subRound1( 7 );
                subRound1( 8 ); subRound1( 9 ); subRound1( 10 ); subRound1( 11 );
                subRound1( 12 ); subRound1( 13 ); subRound1( 14 ); subRound1( 15 );
                subRound1( 16 ); subRound1( 17 ); subRound1( 18 ); subRound1( 19 );
                subRound2( 20 ); subRound2( 21 ); subRound2( 22 ); subRound2( 23 );
                subRound2( 24 ); subRound2( 25 ); subRound2( 26 ); subRound2( 27 );
                subRound2( 28 ); subRound2( 29 ); subRound2( 30 ); subRound2( 31 );
                subRound2( 32 ); subRound2( 33 ); subRound2( 34 ); subRound2( 35 );
                subRound2( 36 ); subRound2( 37 ); subRound2( 38 ); subRound2( 39 );
                subRound3( 40 ); subRound3( 41 ); subRound3( 42 ); subRound3( 43 );
                subRound3( 44 ); subRound3( 45 ); subRound3( 46 ); subRound3( 47 );
                subRound3( 48 ); subRound3( 49 ); subRound3( 50 ); subRound3( 51 );
                subRound3( 52 ); subRound3( 53 ); subRound3( 54 ); subRound3( 55 );
                subRound3( 56 ); subRound3( 57 ); subRound3( 58 ); subRound3( 59 );
                subRound4( 60 ); subRound4( 61 ); subRound4( 62 ); subRound4( 63 );
                subRound4( 64 ); subRound4( 65 ); subRound4( 66 ); subRound4( 67 );
                subRound4( 68 ); subRound4( 69 ); subRound4( 70 ); subRound4( 71 );
                subRound4( 72 ); subRound4( 73 ); subRound4( 74 ); subRound4( 75 );
                subRound4( 76 ); subRound4( 77 ); subRound4( 78 ); subRound4( 79 );

                /* Step E.  Build message digest */
                digest[ 0 ] += A;
                digest[ 1 ] += B;
                digest[ 2 ] += C;
                digest[ 3 ] += D;
                digest[ 4 ] += E;
            }

        public:

            void GetName(UTF8String &str)const override{str="SHA1";}
            void GetName(UTF16String &str)const override{str=U16_TEXT("SHA1");}

            const int GetHashBytes()const override{return DIGEST_SIZE;}

            void Init()override
            {
                digest[0] = 0x67452301L;
                digest[1] = 0xEFCDAB89L;
                digest[2] = 0x98BADCFEL;
                digest[3] = 0x10325476L;
                digest[4] = 0xC3D2E1F0L;
                countLo   = 0;
                countHi   = 0;
                slop      = 0;
                memset(data, 0, sizeof(data));
            }

            void Update(const void *input,uint count)override
            {
                const uint8 *buffer=(const uint8 *)input;
                uint8* db = (uint8*) &data[0];

                /* Update bitcount */
                if ((countLo + ((uint32) count << 3)) < countLo)
                    countHi++; /* Carry from low to high bitCount */
                countLo += ((uint32) count << 3);
                countHi += ((uint32 ) count >> 29);

                /* Process data in BLOCK_SIZE chunks */
                while (count-- > 0)
                {
                    db[slop++] = *(buffer++);
                    if (slop == BLOCK_SIZE)
                    {
                        /* transform this one block */
                        ToBigEndian<uint32>(data, BLOCK_SIZE>>2);
                        sha1_transform();
                        slop = 0 ;    /* no slop left */
                    }
                }
            }

            void Final(void *result)override
            {
                int count;
                uint32 lowBitcount  = countLo;
                uint32 highBitcount = countHi;

                /* Compute number of bytes mod 64 */
                count = (int) ((countLo >> 3) & 0x3F);

                /* Set the first char of padding to 0x80.  This is safe since there is
                    always at least one byte free */
                ((uint8*) data)[count++] = 0x80;

                /* Pad out to 56 mod 64 */
                if (count > 56)
                {
                    /* Two lots of padding:  Pad the first block to 64 bytes */
                    memset((uint8*) &data + count, 0, 64 - count);
                    ToBigEndian<uint32>(data, BLOCK_SIZE>>2);
                    sha1_transform();

                    /* Now fill the next block with 56 bytes */
                    memset(&data, 0, 56);
                }
                else
                {
                    /* Pad block to 56 bytes */
                    memset((uint8*) &data + count, 0, 56 - count);
                }
                ToBigEndian<uint32>(data, BLOCK_SIZE>>2);

                /* Append length in bits and transform */
                data[14] = highBitcount;
                data[15] = lowBitcount;

                sha1_transform();
                ToBigEndian<uint32>(data, BLOCK_SIZE>>2);

                uint8 *hash=(uint8 *)result;
                for (count = 0; count < DIGEST_SIZE; count++)
                    hash[count] = (uint8) ((digest[count>>2]) >> (8*(3-(count & 0x3)))) & 0xff;
            }
        };//class SHA1

        Hash *CreateSHA1Hash(){return(new SHA1);}
    }//namespace algorithm
}//namespace hgl
