; ModuleID = 'size.cpp'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [18 x i8] c"struct sizeof=%d\0A\00", align 1

define i32 @main(i32 %argc, i8** %argv) uwtable {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i8**, align 8
  %str = alloca [128 x i32], align 16
  store i32 0, i32* %1
  store i32 %argc, i32* %2, align 4
  store i8** %argv, i8*** %3, align 8
  call void @_Z4testIA128_wEvRT_([128 x i32]* %str)
  ret i32 0
}

define linkonce_odr void @_Z4testIA128_wEvRT_([128 x i32]* %t) uwtable {
  %1 = alloca [128 x i32]*, align 8
  store [128 x i32]* %t, [128 x i32]** %1, align 8
  %2 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([18 x i8]* @.str, i32 0, i32 0), i64 512)
  ret void
}

declare i32 @printf(i8*, ...)
