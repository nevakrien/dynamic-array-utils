#ifndef ARRAY_H
#define ARRAY_H

#include <assert.h>

#if defined(__GNUC__) || defined(__clang__)
    #define COMPILER_UNREACHABLE() __builtin_unreachable()
#elif defined(_MSC_VER)
    #define COMPILER_UNREACHABLE() __assume(0)
#else
    #define COMPILER_UNREACHABLE() ((void)0)
#endif


#ifndef NDEBUG

    #define ASSERT(x) assert(x)
    #define UNREACHABLE() assert(!"UNREACHABLE")

#else

    #define ASSERT(x) \
        do { if (!(x)) COMPILER_UNREACHABLE(); } while (0)

    #define UNREACHABLE() COMPILER_UNREACHABLE()

#endif

typedef struct {
	char* data;
	size_t len;
	size_t cap;
} StringBuilder;

typedef struct {
	const char* data;
	size_t len;
} StringView;

typedef struct {
	char* data;
	size_t len;
} StringSlice;


#define POP(arr) ((arr).data[(ASSERT(arr.len),--(arr).len)])
#define PEEK(arr) ((arr).data[(ASSERT(arr.len),(arr).len-1)])
#define AT(arr,i) ((arr).data[(ASSERT(i<arr.len),i)])
#define REMOVE_UNORDERED(arr,i) (AT(arr,i)=PEEK(arr),arr.data[--(arr).len])

#define ENSURE_CAP(arr) \
((arr).len >= (arr).cap ? ( \
    (arr).cap = (arr).cap ? (arr).cap * 2 : 8, \
    (arr).data = realloc((arr).data, (arr).cap * sizeof(*(arr).data)), \
    assert((arr).data != NULL && "went OOM") \
) : (void)0, \
(arr))

#define PUSH(arr,x) (ENSURE_CAP(arr).data[(arr).len++] = (x))

#define SET_FROM_CSTR(tgt,cstr) ((tgt).data=cstr,(tgt).len=strlen(cstr),tgt)

#endif // ARRAY_H

